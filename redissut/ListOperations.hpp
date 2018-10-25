#pragma once
#include "redisclient.h"
template<typename K, typename V>
class ListOperations {
	virtual	std::vector<V> Range(K var1, int64_t var2, int64_t var4) = 0;
	virtual	void Trim(K var1, int64_t var2, int64_t var4) = 0;
	virtual	int64_t Size(K var1) = 0;
	virtual	int64_t LeftPush(K var1, V var2) = 0;
	virtual	int64_t LeftPushAll(K var1, std::vector<V>& var2) = 0;
	virtual	int64_t LeftPushIfPresent(K var1, V var2) = 0;
	virtual	int64_t RightPush(K var1, V var2) = 0;
	virtual	int64_t RightPushAll(K var1, std::vector<V>& var2) = 0;
	virtual	int64_t RightPushIfPresent(K var1, V var2) = 0;
	virtual    void Set(K var1, int64_t var2, V var4) = 0;
	virtual	int64_t Remove(K var1, int64_t var2, V var4) = 0;
	virtual	V Index(K var1, int64_t var2) = 0;
	virtual	V LeftPop(K var1) = 0;
	virtual	V RightPop(K var1) = 0;
	virtual	V RightPopAndLeftPush(K var1, K var2) = 0;
};
template<typename K, typename V>
class DefaultListOperations :public ListOperations<K, V> {
public:
	DefaultListOperations() {
		client = RedisClient::get_mutable_instance().GetClient();
	}
	~DefaultListOperations() {
		RedisClient::get_mutable_instance().ReliseClient(client);
		client = nullptr;
	}
	DefaultListOperations(const DefaultListOperations& rlt) {
		this.client = RedisClient::get_mutable_instance().GetClient();
	}
	std::vector<V> Range(K var1, int64_t var2, int64_t var4)override {
		std::vector<V> v;
		try {
			auto rep = client->lrange(var1, var2, var4);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					v.push_back(re.as_string());
				}
			}
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return v;
		}
		return v;
	}
	/**
	 *根据下标保存数据
	 *Date :[10/24/2018 ]
	 *Author :[RS]
	 */
	void Trim(K var1, int64_t var2, int64_t var4)override {
		try {
			client->ltrim(var1, var2, var4, [](const auto & reply) {});
			client->commit();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
		}

	}
	int64_t Size(K var1)override {
		try {
			auto rep = client->llen(var1);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0;
		}
	}
	int64_t LeftPush(K var1, V var2)override {
		std::vector<V> v;
		v.push_back(var2);
		try {
			auto rep = client->lpush(var1, v);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0;
		}
	}

	int64_t LeftPushAll(K var1, std::vector<V>& var2) override {
		try {
			auto rep = client->lpush(var1, var2);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0;
		}

	}
	int64_t LeftPushIfPresent(K var1, V var2)override {
		try {
			auto rep = client->lpushx(var1, var2);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0;
		}
	}

	int64_t RightPush(K var1, V var2)override {
		std::vector<V> v;
		v.push_back(var2);
		try {
			auto rep = client->rpush(var1, v);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0;
		}
	}

	int64_t RightPushAll(K var1, std::vector<V>& var2)override {
		try {
			auto rep = client->rpush(var1, var2);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0;
		}
	}
	int64_t RightPushIfPresent(K var1, V var2)override {
		try {
			auto rep = client->rpushx(var1, var2);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0;
		}
	}

	void Set(K var1, int64_t var2, V var4)override {
		try {
			client->lset(var1, var2, var4, [](const auto &reply) {});
			client->commit();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
		}
	}
	/**
	 *移除var2 个 var4
	 *Date :[10/24/2018 ]
	 *Author :[RS]
	 */
	int64_t Remove(K var1, int64_t var2, V var4)override {
		try {
			auto rep = client->lrem(var1, var2, var4);
			client->commit();
			auto res = rep.get();
			if (res.is_integer()) {
				return res.as_integer();
			}
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0;
		}
	}
	V Index(K var1, int64_t var2)override {
		try {
			auto rep = client->lindex(var1, var2);
			client->commit();
			auto res = rep.get();
			if (res.is_string()) {
				return res.as_string();
			}
			if (res.is_null()) {
				return "nil";
			}
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return "";
		}
	}
	V LeftPop(K var1)override {
		try {
			auto rep = client->lpop(var1);
			client->commit();
			auto res = rep.get();
			if (res.is_string()) {
				return res.as_string();
			}
			if (res.is_null()) {
				return "nil";
			}
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return "";
		}
	}

	V RightPop(K var1)override {
		try {
			auto rep = client->rpop(var1);
			client->commit();
			auto res = rep.get();
			if (res.is_string()) {
				return res.as_string();
			}
			if (res.is_null()) {
				return "nil";
			}
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return "";
		}
	}

	V RightPopAndLeftPush(K var1, K var2)override {
		try {
			auto rep = client->rpoplpush(var1, var2);
			client->commit();
			auto res = rep.get();
			if (res.is_string()) {
				return res.as_string();
			}
			if (res.is_null()) {
				return "nil";
			}
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return "";
		}
	}
private:
	cpp_redis::client* client;
	const 	LOGGER logger = ConsoleUtils::get_mutable_instance().getConsoleLogger("ListOperations");
};


