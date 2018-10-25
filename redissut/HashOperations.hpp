#pragma once
#include "redisclient.h"
#include <set>
#include <vector>
#include <map>
template<typename H, typename HK, typename HV>
class HashOperations {
public:
	virtual	int64_t Delete(H var1, std::vector<HK>keys) = 0;
	virtual	bool HasKey(H var1, HK var2) = 0;
	virtual	HV Get(H var1, HK var2) = 0;
	virtual	std::vector<HV> MultiGet(H var1, std::vector<HK> var2) = 0;
	virtual	int64_t Increment(H var1, HK var2, int64_t var3) = 0;
	virtual	double Increment(H var1, HK var2, double var3) = 0;
	virtual	std::set<HK> Keys(H var1) = 0;
	virtual	int64_t Size(H var1) = 0;
	virtual	void PutAll(H var1, std::map< HK, HV>& var2) = 0;
	virtual	void Put(H var1, HK var2, HV var3) = 0;
	virtual	bool PutIfAbsent(H var1, HK var2, HV var3) = 0;
	virtual	std::vector<HV> Values(H var1) = 0;
	virtual	std::map<HK, HV> Entries(H var1) = 0;

};
template<typename H, typename HK, typename HV>
class DefaultHashOperations :public HashOperations<H, HK, HV> {
public:
	DefaultHashOperations() {
		client = RedisClient::get_mutable_instance().GetClient();
	}
	~DefaultHashOperations() {
		RedisClient::get_mutable_instance().ReliseClient(client);
		client = nullptr;
	}
	DefaultHashOperations(const DefaultHashOperations& rlt) {
		this.client = RedisClient::get_mutable_instance().GetClient();
	}
	int64_t Delete(H var1, std::vector<HK>keys)override {
		try {
			auto rep = client->hdel(var1, keys);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception& e) {
			logger->error(e.what());
			return 0;
		}
	}
	bool HasKey(H var1, HK var2)override {
		try {
			auto rep = client->hexists(var1, var2);
			client->commit();
			auto res = rep.get();
			if (res.as_integer() == 1) {
				return true;
			}
			else {
				return false;
			}
		}
		catch (const std::exception& e) {
			logger->error(e.what());
			return false;
		}
	}
	HV Get(H var1, HK var2)override {
		HV v;
		try {
			auto rep = client->hget(var1, var2);
			client->commit();
			auto res = rep.get();

			if (res.is_null()) {
				v = "";
			}
			else {
				v = res.as_string();
			}
		}
		catch (const std::exception& e) {
			logger->error(e.what());
			v = "";
			return v;
		}
		return v;
	}
	std::vector<HV> MultiGet(H var1, std::vector<HK> var2)override {
		std::vector<HV> v;
		try {
			auto rep = client->hmget(var1, var2);
			client->commit();
			auto res = rep.get();
			if (res.is_null()) {
				return v;
			}
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					if (re.is_null()) {
						v.push_back("");
					}
					else {
						v.push_back(re.as_string());
					}
				}
			}
		}
		catch (const std::exception & e) {
			logger->error(e.what());
			return v;
		}
	}
	int64_t Increment(H var1, HK var2, int64_t var3)override {
		try {
			auto rep = client->hincrby(var1, var2, var3);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->error(e.what());
			return 0;
		}
	}
	double Increment(H var1, HK var2, double var3) override {
		try {
			auto rep = client->hincrbyfloat(var1, var2, var3);
			client->commit();
			auto res = rep.get();
			return std::stod(res.as_string());
		}
		catch (const std::exception & e) {
			logger->error(e.what());
			return 0;
		}
	}
	std::set<HK> Keys(H var1)override {
		std::set<HK> values;
		try {
			auto rep = client->hkeys(var1);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					values.insert(re.as_string());
				}
			}
		}
		catch (const std::exception & e) {
			logger->error(e.what());
			return values;
		}
		return values;
	}
	int64_t Size(H var1)override {
		try {
			auto rep = client->hlen(var1);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->error(e.what());
			return 0;
		}
	}
	void PutAll(H var1, std::map< HK, HV>& var2)override {
		std::vector<std::pair<HK, HV>> v;
		for (auto var : var2) {
			v.push_back(var);
		}
		try {
			client->hmset(var1, v, [](auto & reply) {});
			client->commit();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
		}
	}
	void Put(H var1, HK var2, HV var3) override {

	}
	bool PutIfAbsent(H var1, HK var2, HV var3) override {
		try {
			auto rep = client->hsetnx(var1, var2, var3);
			client->commit();
			if (rep.get().as_integer() == 1) {
				return true;
			}
			else {
				return false;
			}
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return false;
		}
	}
	std::vector<HV> Values(H var1)override {
		std::vector<HV> v;
		try {
			auto rep = client->hvals(var1);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					v.push_back(re.as_string());
				}
			}
		}
		catch (const std::exception & e) {
			logger->error(e.what());
			return v;
		}
		return v;
	}
	std::map<HK, HV> Entries(H var1)override {
		std::map<HK, HV> m;
		try {
			auto rep = client->hgetall(var1);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				int i = 1;
				std::pair<std::string, std::string> temp;
				for (auto re : res.as_array()) {
					if (i % 2 == 1) {
						temp.first = re.as_string();
					}if (i % 2 == 0) {
						temp.second = re.as_string();
						m.insert(temp);
					}
					i++;
				}
			}
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return m;
		}
		return m;
	}
private:
	cpp_redis::client* client;
	const 	LOGGER logger = ConsoleUtils::get_mutable_instance().getConsoleLogger("HashOperations");
};


