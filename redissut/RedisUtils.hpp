#pragma once
#include <cpp_redis/cpp_redis.hpp>
#include <boost/serialization/singleton.hpp>
#include <iostream>
#ifdef _WIN32
#include <WinSock2.h>
#endif /* _WIN32 */
#include <utils/ConsoleUtils.hpp>
#include <mutex>
#include <condition_variable>
#include <signal.h>
#include <tacopie/tacopie>
#include <functional>
static std::string ip = "192.168.33.11";
static int port = 6379;
static std::string password = "riversky";
class RedisUtils :public boost::serialization::singleton<RedisUtils>{
private:
	const LOGGER logger = ConsoleUtils::get_mutable_instance().getConsoleLogger("RedisUtils");

public:
	std::shared_ptr<cpp_redis::client> getClient() {
#ifdef _WIN32
		//! Windows netword DLL init
		WORD version = MAKEWORD(2, 2);
		WSADATA data;

		if (WSAStartup(version, &data) != 0) {
			logger->info("WSAStartup() failure");
		}
#endif /* _WIN32 */
		cli = std::make_shared<cpp_redis::client>();
		cli->connect(ip, port, [=](const std::string& host, std::size_t port, cpp_redis::client::connect_state status) {
			if (status == cpp_redis::client::connect_state::dropped) {
				logger->info("client disconnected ");
			}
		});
		auto res = cli->auth(password);
		cli->sync_commit();
		logger->info("auth:{}",res.get().as_string());
		return cli;
	}
	void printAll() {
		auto res = cli->keys("*");
		cli->sync_commit();
		auto reu = res.get();
		logger->info("----------------keys----------------------");
		if (reu.is_array()) {
			auto a = reu.as_array();
			while (!a.empty()) {
				std::string key = a.back().as_string();
				a.pop_back();
				auto value = cli->get(key);
				cli->sync_commit();
				std::string val = value.get().as_string();
				logger->info("{}:{}",key, val);

			}
		}
		logger->info("----------------keys----------------------");

	}
public:

private:

private:
	 std::shared_ptr<cpp_redis::client> cli;
	 
};

