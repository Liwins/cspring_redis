#pragma once
#include <boost/serialization/singleton.hpp>
#include <cpp_redis/cpp_redis.hpp>
#ifdef _WIN32
#include <Winsock2.h>
#endif /* _WIN32 */
#include <mutex>
#include <utils/ConsoleUtils.hpp>
#include <memory>
class RedisClient :public boost::serialization::singleton<RedisClient> {
private:
	const LOGGER logger = ConsoleUtils::get_mutable_instance().getConsoleLogger("RedisClient");
protected:
	RedisClient();
	~RedisClient();
public:
	cpp_redis::client* GetClient();
	void ReliseClient(cpp_redis::client* client);
	bool init();
	/**
	 * ’Àı 
	 *Date :[10/17/2018 ]
	 *Author :[RS]
	 */
	void shrink();
	bool expand();
private:
	std::string url="127.0.0.1";
	int port=6379;
	int mStepPool = 5;
	std::vector<cpp_redis::client*> clients;
	std::mutex mClientsLock;
};
