#pragma once
#include "redisclient.h"
template<typename K, typename V>
class SetOperations {
	virtual	int64_t Add(K var1, std::vector<V>& var2) = 0;
	virtual	int64_t Remove(K var1, std::vector<V>& var2) = 0;
	virtual	V Pop(K var1) = 0;
	virtual	std::vector<V> Pop(K var1, int64_t var2) = 0;
	virtual	bool Move(K var1, K var2, V var3) = 0;
	virtual	int64_t Size(K var1) = 0;
	virtual	bool IsMember(K var1, V var2) = 0;
	virtual	std::set<V> Intersect(K var1, K var2) = 0;
	virtual	std::set<V> Intersect(K var1, std::vector<K> var2) = 0;
	virtual	int64_t IntersectAndStore(K var1, K var2, K var3) = 0;
	virtual	int64_t IntersectAndStore(K var1, std::vector<K> var2, K var3) = 0;
	virtual	std::set<V> Union(K var1, K var2) = 0;
	virtual	std::set<V> Union(K var1, std::vector<K> var2) = 0;
	virtual	int64_t UnionAndStore(K var1, K var2, K var3) = 0;
	virtual	int64_t UnionAndStore(K var1, std::vector<K> var2, K var3) = 0;
	virtual	std::set<V> Difference(K var1, K var2) = 0;
	virtual	std::set<V> Difference(K var1, std::vector<K> var2) = 0;
	virtual	int64_t DifferenceAndStore(K var1, K var2, K var3) = 0;
	virtual	int64_t DifferenceAndStore(K var1, K var3, std::vector<K> var2) = 0;
	virtual	std::set<V> Members(K var1) = 0;
	virtual	V RandomMember(K var1) = 0;
	virtual	std::vector<V> DistinctRandomMembers(K var1, int64_t var2) = 0;
	virtual	std::vector<V> RandomMembers(K var1, int64_t var2) = 0;
	virtual	std::vector<V> Scan(K var1, V patterns) = 0;
	virtual	std::vector<V> Scan(K var1, V patterns, int64_t var2) = 0;
};
template<typename K, typename V>
class DefaultSetOperations :public SetOperations<K, V> {
public:
	DefaultSetOperations() {
		client = RedisClient::get_mutable_instance().GetClient();
	}
	~DefaultSetOperations() {
		RedisClient::get_mutable_instance().ReliseClient(client);
		client = nullptr;
	}
	DefaultSetOperations(const DefaultSetOperations& rlt) {
		this.client = RedisClient::get_mutable_instance().GetClient();
	}
	int64_t Add(K var1, std::vector<V>& var2) override {
		try {
			auto rep = client->sadd(var1, var2);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception& e) {
			logger->info(e.what());
			return -1;
		}
	}
	int64_t Remove(K var1, std::vector<V>& var2)override {
		try {
			auto rep = client->srem(var1, var2);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception& e) {
			logger->info(e.what());
			return -1;
		}
	}
	V Pop(K var1)override {
		return Pop(var1, 1).front();
	}
	std::vector<V> Pop(K var1, int64_t var2)override {
		std::vector<V> v;
		try {
			auto rep = client->spop(var1, var2);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					v.push_back(re.as_string());
				}
			}
		}
		catch (const std::exception& e) {
			logger->info(e.what());
			return v;
		}
		return v;
	}
	bool Move(K src, K dst, V var3) override {
		try {
			auto rep = client->smove(src, dst, var3);
			client->commit();
			auto res = rep.get();
			if (res.is_integer()) {
				if (res.as_integer() == 1) {
					return true;
				}
			}
			return false;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return false;
		}

	}
	int64_t Size(K var1)override {
		try {
			auto rep = client->scard(var1);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0;
		}
	}
	bool IsMember(K var1, V var2)override {
		try {
			auto rep = client->sismember(var1, var2);
			client->commit();
			auto res = rep.get();
			return res.as_integer() == 1;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return false;
		}
	}
	std::set<V> Intersect(K var1, K var2)override {
		std::vector<K> v;
		v.push_back(var1);
		v.push_back(var2);
		std::set<V> s;
		try {
			auto rep = client->sinter(v);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					s.insert(re.as_string());
				}
			}
			return s;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return s;
		}
	}
	std::set<V> Intersect(K var1, std::vector<K> var2)override {
		var2.push_back(var1);
		std::set<V> s;
		try {
			auto rep = client->sinter(var2);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					s.insert(re.as_string());
				}
			}
			return s;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return s;
		}
	}
	int64_t IntersectAndStore(K var1, K var2, K var3)override {
		try {
			auto rep = client->sinterstore(var1, std::vector<V>{var2, var3});
			client->commit();
			auto res = rep.get();
			if (res.is_integer()) {
				return res.as_integer();
			}
			return -1;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return -1;
		}
	}
	int64_t IntersectAndStore(K var1, std::vector<K> var2, K var3)override {
		var2.push_back(var3);
		try {
			auto rep = client->sinterstore(var1, var2);
			client->commit();
			auto res = rep.get();
			if (res.is_integer()) {
				return res.as_integer();
			}
			return -1;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return -1;
		}
	}
	std::set<V> Union(K var1, K var2)override {
		std::set<V> s;
		std::vector<K> v;
		v.push_back(var1);
		v.push_back(var2);
		try {
			auto rep = client->sunion(v);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					s.insert(re.as_string());
				}
			}
			return s;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return s;
		}
	}
	std::set<V> Union(K var1, std::vector<K> var2)override {
		std::set<V> s;
		var2.push_back(var1);
		try {
			auto rep = client->sunion(var2);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					s.insert(re.as_string());
				}
			}
			return s;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return s;
		}
	}
	int64_t UnionAndStore(K var1, K var2, K var3)override {
		try {
			auto rep = client->sunionstore(var1, std::vector<V>{var2, var3});
			client->commit();
			auto res = rep.get();
			if (res.is_integer()) {
				return res.as_integer();
			}
			return -1;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return -1;
		}
	}
	int64_t UnionAndStore(K var1, std::vector<K> var2, K var3)override {
		var2.push_back(var3);
		try {
			auto rep = client->sunionstore(var1, var2);
			client->commit();
			auto res = rep.get();
			if (res.is_integer()) {
				return res.as_integer();
			}
			return -1;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return -1;
		}
	}
	std::set<V> Difference(K var1, K var2)override {
		std::set<V> s;
		std::vector<V> v{ var1,var2 };
		try {
			auto rep = client->sdiff(v);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					s.insert(re.as_string());
				}
			}
			return s;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return s;
		}
	}
	std::set<V> Difference(K var1, std::vector<K> var2)override {
		std::set<V> s;
		std::vector<K> v;
		v.push_back(var1);
		std::for_each(var2.begin(), var2.end(), [&v](auto str) {v.push_back(str); });
		try {
			auto rep = client->sunion(v);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					s.insert(re.as_string());
				}
			}
			return s;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return s;
		}
	}
	int64_t DifferenceAndStore(K var1, K var2, K var3)override {
		try {
			auto rep = client->sdiffstore(var1, std::vector<K>{var2, var3});
			client->commit();
			auto res = rep.get();
			if (res.is_integer()) {
				return res.as_integer();
			}
			return -1;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return -1;
		}
	}
	int64_t DifferenceAndStore(K var1, K var3, std::vector<K> var2)override {
		std::vector<K> v;
		v.push_back(var3);
		std::for_each(var2.begin(), var2.end(), [&v](auto str) {v.push_back(str); });
		try {
			auto rep = client->sunionstore(var1, v);
			client->commit();
			auto res = rep.get();
			if (res.is_integer()) {
				return res.as_integer();
			}
			return -1;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return -1;
		}
	}
	std::set<V> Members(K var1)override {
		std::set<V> s;
		try {
			auto rep = client->smembers(var1);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					s.insert(re.as_string());
				}
			}
			return s;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return s;
		}

	}
	V RandomMember(K var1)override {
		return RandomMembers(var1, 1).front();
	}
	std::vector<V> DistinctRandomMembers(K var1, int64_t var2)override {
		return RandomMembers(var1, -var2);
	}
	std::vector<V> RandomMembers(K var1, int64_t var2)override {
		std::vector<V> s;
		try {
			auto rep = client->srandmember(var1, -var2);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					s.push_back(re.as_string());
				}
			}
			return s;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return s;
		}
	}
	std::vector<V> Scan(K var1, V patterns)override {
		std::vector<V> v;
		try {
			for (std::string flag = "-1"; flag != "0";) {
				if (flag == "-1")flag = "0";
				auto rep = client->sscan(var1, std::stol(flag), patterns);
				client->commit();
				auto res = rep.get();
				if (res.is_array()) {
					flag = res.as_array().front().as_string();
					if (res.as_array().back().is_array()) {
						for (auto re : res.as_array()[1].as_array()) {
							v.push_back(re.as_string());
						}
					}
				}
			}
			return v;

		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return v;
		}

	}
	std::vector<V> Scan(K var1, V patterns, int64_t var2)override {
		std::vector<V> v;
		try {
			auto rep = client->sscan(var1, 0, patterns, var2);
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				if (res.as_array()[1].is_array()) {
					for (auto re : res.as_array()[1].as_array()) {
						v.push_back(re.as_string());
					}
				}
			}
			return v;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return v;
		}
	}
private:
	cpp_redis::client* client;
	const 	LOGGER logger = ConsoleUtils::get_mutable_instance().getConsoleLogger("std::vectorOperations");
};

