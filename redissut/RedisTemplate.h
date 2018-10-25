#pragma once
#include <mutex>
#include "redisclient.h"
#include <vector>
#include <string>
#include <chrono>
#include <utils/ConsoleUtils.hpp>
#include "ValueOperations.hpp"
#include "HashOperations.hpp"
#include "ListOperations.hpp"
#include "SetOperations.hpp"
#include "ZSetOperations.hpp"
#include <regex>
template<typename K, typename V>
class RedisTemplate {
public:
	RedisTemplate() {
		client = RedisClient::get_mutable_instance().GetClient();
	}
	~RedisTemplate() {
		RedisClient::get_mutable_instance().ReliseClient(client);
		client = nullptr;
	}
private:
	static const LOGGER logger;
	cpp_redis::client* client;
	std::mutex mMutexClient;
public:
	bool HasKey(const K& key){
	{
		std::lock_guard<std::mutex> lock(mMutexClient);
		try {
			std::vector<std::string> v;
			v.push_back(key);
			auto rs = client->exists(v);
			client->commit();
			auto answer = rs.get();
			if (answer.is_error()) {
				return false;
			}
			else {
				return true;
			}
		}
		catch (...) {
			logger->info("HasKey error");
			return false();
		}
	}
}
	bool DeleteKey(const K& key) {
		{
			std::lock_guard<std::mutex> lock(mMutexClient);
			std::vector<std::string> v;
			v.push_back(key);
			auto rs = client->exists(v);
			client->commit();
			auto answer = rs.get();
			if (answer.as_integer() == 1) {
				return true;
			}
			return false;
		}
	 }
	uint32_t DeleteKey(const std::vector<K> keys) {
		{
			std::lock_guard<std::mutex> lock(mMutexClient);
			auto rs = client->exists(keys);
			client->commit();
			auto answer = rs.get();
			if (answer.is_error()) {
				return 0;
			}
			else {
				return answer.as_integer();
			}
		}
	 }
	bool Expire(const K& key, std::chrono::seconds second) {
		{
			std::lock_guard<std::mutex> lock(mMutexClient);
			if (HasKey(key)) {
				try {
					auto rs = client->expire(key, second.count());
					client->commit();
					auto answer = rs.get();
					if (answer.is_error()) {
						logger->error("Expire error:{}", key.c_str());
						return false;
					}
					else {
						return true;
					}
				}
				catch (...) {
					logger->error("Expire exception:{}", key.c_str());
				}
			}
			else {
				logger->error("{} is not exist", key.c_str());
				return false;
			}
		}
	 }
	bool ExpireAt(const K& key, std::chrono::system_clock::time_point& time_point) {
		auto time = std::chrono::system_clock::to_time_t(time_point);
		return ExpireAt(key, time);
	 }
	bool ExpireAt(const K& key, int timepoint) {
		{
			std::lock_guard<std::mutex> lock(mMutexClient);
			try {
				auto rs = client->expireat(key, timepoint);
				client->commit();
				auto answer = rs.get();
				if (answer.is_integer()) {
					int result = answer.as_integer();
					if (result == 1) {
						return true;
					}
					return false;
				}
				else {
					return false;
				}
			}
			catch (...) {
				return false;
			}
		}
	 }
	int ConnectCount() {
		{
			std::lock_guard<std::mutex> lock(mMutexClient);
			try {
				auto rs = client->client_list();
				client->commit();
				auto answer = rs.get();
				if (answer.is_string()) {
					auto res = answer.as_string();
					//searchËÑµ½¾ÍÍ£Ö¹
					std::regex r("addr=([\\d]+\\.[\\d]+\\.[\\d]+\\.[\\d]+:[\\d]+)");
					const std::sregex_iterator end;
					int result = 0;
					for (std::sregex_iterator iter(std::cbegin(res), std::cend(res), r); iter != end; ++iter) {
						result++;
					}
					return result;
				}
				else {
					return -1;
				}
			}
			catch (...) {
				logger->error("ConnectCount error");
			}
		}
	 }
};
class StringRedisTemplate :public RedisTemplate<std::string, std::string> {
public:
	DefaultValueOperations<std::string, std::string> GetValueOps() {
		return  DefaultValueOperations<std::string, std::string>();
	}
	DefaultHashOperations<std::string, std::string, std::string> GetHashOps() {
		return  DefaultHashOperations<std::string, std::string, std::string>();
	}
	DefaultListOperations<std::string, std::string> GetListOps() {
		return  DefaultListOperations<std::string, std::string>();
	}
	DefaultSetOperations<std::string, std::string> GetSetOps() {
		return  DefaultSetOperations<std::string, std::string>();
	}
	DefaultZSetOperations<std::string, std::string> GetZSetOps() {
		return  DefaultZSetOperations<std::string, std::string>();
	}
};