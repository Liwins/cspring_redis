#include "redisclient.h"
#include <thread>
RedisClient::RedisClient()
{
	if (init()) {
		logger->debug("init redis success");
	}
	else {
		logger->error("init redis fail");
		std::this_thread::sleep_for(std::chrono::seconds(23));
		exit(1);
	}
}

RedisClient::~RedisClient()
{
	std::lock_guard<std::mutex> lock(mClientsLock);
	for (int i = 0; i < clients.size(); ++i) {
		auto c = clients.back();
		c->quit();
		delete c;
		c = nullptr;
		clients.pop_back();
	}
}

cpp_redis::client* RedisClient::GetClient()
{
	if (clients.size()>50){
		shrink();
	}
	if (clients.size() < 5) {
		expand();
	}
	{
		std::lock_guard<std::mutex> lock(mClientsLock);
		auto a = clients.back();
		clients.pop_back();
		return a;
	}
	
}

void RedisClient::ReliseClient(cpp_redis::client* client)
{
	std::lock_guard<std::mutex> lock(mClientsLock);
	clients.push_back(client);
}



bool RedisClient::init()
{
#ifdef _WIN32
	//! Windows netword DLL init
	WORD version = MAKEWORD(2, 2);
	WSADATA data;

	if (WSAStartup(version, &data) != 0) {
		logger->error("WSAStartup() failure");
	}
#endif /* _WIN32 */
	return expand();
}

void RedisClient::shrink()
{
	std::lock_guard<std::mutex> lock(mClientsLock);
	for (int i = 0; i < mStepPool; ++i) {
		auto c=clients.back();
		c->quit();
		delete c;
		c = nullptr;
		clients.pop_back();
	}
}

bool RedisClient::expand()
{
	{
		try {
		std::lock_guard<std::mutex> lock(mClientsLock);
			for (int i = 0; i < mStepPool; ++i) {
				auto client = new cpp_redis::client();
				client->connect(url, port, [=](const std::string& host, std::size_t port, cpp_redis::client::connect_state status) {
				});
				client->auth("riversky", [=](const cpp_redis::reply & reply) {
					if (!reply.is_error()) {
						logger->info("redis auth success ");
					}
					else {
						logger->error("redis auth fail ");
					}
				});
				client->commit();
				clients.push_back(client);
			}
			return true;
		}
		catch (const std::exception & e) {
			logger->error("redis auth exception {}", e.what());
			return false;
		}
		catch (...) {
			logger->info("redis auth exception ");
			return false;
		}
	}
	
}
