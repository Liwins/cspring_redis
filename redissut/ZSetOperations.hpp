#pragma once
#include "redisclient.h"
#include <map>
#include <set>
#include <vector>
template<typename K, typename V>
class ZSetOperations {
	virtual	bool Add(K var1, V var2, double var3) = 0;
	virtual	int64_t Add(K var1, std::vector<std::pair<double, V>> var2) = 0;
	virtual	int64_t Remove(K var1, std::vector<V> var2) = 0;
	virtual	double IncrementScore(K var1, V var2, double var3) = 0;
	/**
	 *ªÒ»°≈≈√˚
	 *Date :[10/24/2018 ]
	 *Author :[RS]
	 */
	virtual	int64_t Rank(K var1, V var2) = 0;
	virtual	int64_t ReverseRank(K var1, V var2) = 0;
	virtual	std::set<V> Range(K var1, int64_t start, int64_t stop) = 0;
	virtual	std::set<std::pair<V, V>> RangeWithScores(K var1, int64_t var2, int64_t var4) = 0;
	virtual	std::set<V> RangeByScore(K var1, double var2, double var4) = 0;
	virtual	std::set<std::pair<V, V>> RangeByScoreWithScores(K var1, double var2, double var4) = 0;
	virtual	std::set<V> RangeByScore(K var1, double var2, double var4, int64_t var6, int64_t var8) = 0;
	virtual	std::set<std::pair<V, V>> RangeByScoreWithScores(K var1, double var2, double var4, int64_t var6, int64_t var8) = 0;
	virtual	std::set<V> ReverseRange(K var1, int64_t var2, int64_t var4) = 0;
	virtual	std::set<std::pair<V, V>> ReverseRangeWithScores(K var1, int64_t var2, int64_t var4) = 0;
	virtual	std::set<V> ReverseRangeByScore(K var1, double var2, double var4) = 0;
	virtual	std::set<std::pair<V, V>> ReverseRangeByScoreWithScores(K var1, double var2, double var4) = 0;
	virtual	std::set<V> ReverseRangeByScore(K var1, double var2, double var4, int64_t var6, int64_t var8) = 0;
	virtual	std::set<std::pair<V, V>> ReverseRangeByScoreWithScores(K var1, double var2, double var4, int64_t var6, int64_t var8) = 0;
	virtual	int64_t Count(K var1, double var2, double var4) = 0;
	virtual	int64_t ZCard(K var1) = 0;
	virtual	double Score(K var1, V var2) = 0;
	virtual	int64_t RemoveRangeByRank(K var1, int64_t var2, int64_t var4) = 0;
	virtual	int64_t RemoveRangeByScore(K var1, double var2, double var4) = 0;
	virtual	int64_t UnionAndStore(K var1, K var2, K var3) = 0;
	virtual	int64_t UnionAndStore(K var1, std::vector<K> var2, K var3) = 0;
	virtual	int64_t IntersectAndStore(K var1, K var2, K var3) = 0;
	virtual	int64_t IntersectAndStore(K var1, K var2, std::vector<K> var3) = 0;
};
template<typename K, typename V>
class DefaultZSetOperations :public ZSetOperations<K, V> {
public:
	DefaultZSetOperations() {
		client = RedisClient::get_mutable_instance().GetClient();
	}
	~DefaultZSetOperations() {
		RedisClient::get_mutable_instance().ReliseClient(client);
		client = nullptr;
	}
	DefaultZSetOperations(const DefaultZSetOperations& rlt) {
		this.client = RedisClient::get_mutable_instance().GetClient();
	}
	bool Add(K var1, V var2, double var3)override {
		std::vector<std::string> options;
		std::multimap < std::string, std::string> scoremember;
		options.push_back("CH");
		scoremember.insert(std::make_pair(std::to_string(var3), var2));
		try {
			auto rep = client->zadd(var1, options, scoremember);
			client->commit();
			auto res = rep.get();
			if (res.is_integer()) {
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
	int64_t Add(K var1, std::vector<std::pair<double, V>> var2) override {
		std::vector<std::string> options;
		std::multimap < std::string, std::string> scoremember;
		options.push_back("CH");
		std::for_each(var2.begin(), var2.end(), [&scoremember, &options](auto const pai) {
			scoremember.insert(std::make_pair(std::to_string(pai.first), pai.second));
			options.push_back("CH");
		});
		try {
			auto rep = client->zadd(var1, options, scoremember);
			client->commit();
			auto res = rep.get();
			if (res.is_integer()) {
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
	int64_t Remove(K var1, std::vector<V> var2)override {
		try {
			auto rep = client->zrem(var1, var2);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0;
		}

	}
	double IncrementScore(K var1, V var2, double var3) override {
		try {
			auto rep = client->zincrby(var1, std::to_string(var3), var2);
			client->commit();
			auto res = rep.get();
			return std::stod(res.as_string());
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0.0;
		}
	}
	int64_t Rank(K var1, V var2) override {
		try {
			auto rep = client->zrank(var1, var2);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0;
		}
	}
	int64_t ReverseRank(K var1, V var2) override {
		try {
			auto rep = client->zrevrank(var1, var2);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0.0;
		}
	}
	std::set<V> Range(K var1, int64_t var2, int64_t var4)override {
		return RangeTempleNoScore< false, false>(var1, var2, var4);
	}
	std::set<std::pair<V, V>> RangeWithScores(K var1, int64_t var2, int64_t var4)override {
		return RangeTempleWithScore< false, false>(var1, var2, var4);
	}
	std::set<V> RangeByScore(K var1, double start, double end)override {
		return RangeTempleNoScore< true, false >(var1, start, end);
	}
	std::set<std::pair<V, V>> RangeByScoreWithScores(K var1, double var2, double var4) override {
		return RangeTempleWithScore< true, false>(var1, var2, var4);
	}
	std::set<V> RangeByScore(K var1, double var2, double var4, int64_t var6, int64_t var8)override {
		return RangeByScoreTempleNoScore<false>(var1, var2, var4, var6, var8);
	}
	std::set<std::pair<V, V>> RangeByScoreWithScores(K var1, double var2, double var4, int64_t var6, int64_t var8) override {
		return RangeByScoreTempleWithScore< false>(var1, var2, var4, var6, var8);
	}
	std::set<V> ReverseRange(K var1, int64_t var2, int64_t var4)override {
		return RangeTempleNoScore< false, true>(var1, var2, var4);
	}
	std::set<std::pair<V, V>> ReverseRangeWithScores(K var1, int64_t var2, int64_t var4) {
		return RangeTempleWithScore< false, true>(var1, var2, var4);
	}
	std::set<V> ReverseRangeByScore(K var1, double var2, double var4) {
		return RangeTempleNoScore< true, true>(var1, var2, var4);
	}
	std::set<std::pair<V, V>> ReverseRangeByScoreWithScores(K var1, double var2, double var4) {
		return RangeTempleWithScore< true, true>(var1, var2, var4);
	}
	std::set<V> ReverseRangeByScore(K var1, double var2, double var4, int64_t var6, int64_t var8) {
		return RangeByScoreTempleNoScore< true>(var1, var2, var4, var6, var8);
	}
	std::set<std::pair<V, V>> ReverseRangeByScoreWithScores(K var1, double var2, double var4, int64_t var6, int64_t var8)override {
		return RangeByScoreTempleWithScore< true>(var1, var2, var4, var6, var8);
	}
	int64_t Count(K var1, double var2, double var4)override {
		try {
			auto rep = client->zcount(var1, var2, var4);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return -1;
		}
	}
	int64_t ZCard(K var1) override {
		try {
			auto rep = client->zcard(var1);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return -1;
		}
	}

	double Score(K var1, V var2)override {
		try {
			auto rep = client->zscore(var1, var2);
			client->commit();
			auto res = rep.get();
			if (res.is_null()) {
				logger->info("nul");
				return -1;
			}
			return std::stod(res.as_string());
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return -1;
		}
	}
	virtual	int64_t RemoveRangeByRank(K var1, int64_t var2, int64_t var4) override {
		try {
			auto rep = client->zremrangebyrank(var1, static_cast<int>(var2), static_cast<int>(var4));
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return -1;
		}
	}
	int64_t RemoveRangeByScore(K var1, double var2, double var4)override {
		try {
			auto rep = client->zremrangebyscore(var1, var2, var4);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return -1;
		}
	}

	int64_t UnionAndStore(K var1, K var2, K var3)override {
		std::vector<K> v;
		v.push_back(var2);
		v.push_back(var3);
		std::vector<std::size_t> weith{ 1,1 };
		return IntersectOrUnionStore<false>(var1, v, weith);
	}
	int64_t UnionAndStore(K var1, std::vector<K> var2, K var3)override {
		var2.push_back(var3);
		int size = var2.size();
		std::vector<std::size_t> weith;
		for (int i = 0; i < size; ++i) {
			weith.push_back(1);
		}
		return IntersectOrUnionStore<false>(var1, var2, weith);
	}
	int64_t IntersectAndStore(K var1, K var2, K var3)override {
		std::vector<K> v;
		v.push_back(var2);
		v.push_back(var3);
		std::vector<std::size_t> weith{ 1,1 };
		return IntersectOrUnionStore<true>(var1, v, weith);
	}
	int64_t IntersectAndStore(K var1, K var2, std::vector<K> var3)override {
		var3.push_back(var2);
		int size = var3.size();
		std::vector<std::size_t> weith;
		for (int i = 0; i < size; ++i) {
			weith.push_back(1);
		}
		return IntersectOrUnionStore<true>(var1, var3, weith);
	}

private:
	template<bool insert>
	int64_t IntersectOrUnionStore(K var1, std::vector<K> &var2, std::vector<std::size_t> &weith) {
		try {
			std::future<cpp_redis::reply> rep;
			if (insert) {
				rep = client->zinterstore(var1, var2.size(), var2, weith, cpp_redis::client::aggregate_method::sum);
			}
			else {
				rep = client->zunionstore(var1, var2.size(), var2, weith, cpp_redis::client::aggregate_method::sum);
			}
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return -1;
		}
	}
	template< bool reverse>
	std::set<V> RangeByScoreTempleNoScore(K var1, double var2, double var4, int64_t var6, int64_t var8) {
		std::set<V> v;
		try {
			std::future<cpp_redis::reply> rep;
			if (reverse) {
				rep = client->zrevrangebyscore(var1, var2, var4, var6, var8 - var6, false);
			}
			else {
				rep = client->zrangebyscore(var1, var2, var4, var6, var8 - var6, false);
			}
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					v.insert(re.as_string());
				}
			}
			return v;
		}
		catch (const std::exception& e) {
			logger->info(e.what());
			return v;
		}
	}
	template< bool reverse>
	std::set<std::pair<V, V>> RangeByScoreTempleWithScore(K var1, double var2, double var4, int64_t var6, int64_t var8) {
		std::set<std::pair<V, V>> v;
		try {
			std::future<cpp_redis::reply> rep;
			if (reverse) {
				rep = client->zrevrangebyscore(var1, var2, var4, var6, var8 - var6, true);
			}
			else {
				rep = client->zrangebyscore(var1, var2, var4, var6, var8 - var6, true);
			}
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				int i = 1;
				std::pair<std::string, std::string> p;
				for (auto re : res.as_array()) {
					if (i % 2 == 1) {
						p.first = re.as_string();
					}
					else {
						p.second = re.as_string();
						v.insert(p);
					}
					i++;
				}
			}
			return v;
		}
		catch (const std::exception& e) {
			logger->info(e.what());
			return v;
		}
	}
	template<  bool byscore, bool reverse>
	std::set<std::pair<V, V>> RangeTempleWithScore(K var1, double var2, double var4) {
		std::set<std::pair<V, V>> v;
		try {
			std::future<cpp_redis::reply> rep;
			if (reverse) {
				if (byscore) {
					rep = client->zrevrangebyscore(var1, var2, var4, true);
				}
				else {
					rep = client->zrevrange(var1, static_cast<int>(var2), static_cast<int>(var4), true);
				}
			}
			else {
				if (byscore) {
					rep = client->zrangebyscore(var1, var2, var4, true);
				}
				else {
					rep = client->zrange(var1, static_cast<int>(var2), static_cast<int>(var4), true);
				}

			}
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				int i = 1;
				std::pair<V, V> p;
				for (auto re : res.as_array()) {
					if (i % 2 == 1) {
						p.first = re.as_string();
					}
					else {
						p.second = re.as_string();
						v.insert(p);
					}
					i++;
				}
			}
			return v;
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return v;
		}
	}
	template<  bool byscore, bool reverse>
	std::set<V> RangeTempleNoScore(K var1, double var2, double var4) {
		std::set<V>  v;
		try {
			std::future<cpp_redis::reply> rep;
			if (reverse) {
				if (byscore) {
					rep = client->zrevrangebyscore(var1, var2, var4);
				}
				else {
					rep = client->zrevrange(var1, static_cast<int>(var2), static_cast<int>(var4));
				}

			}
			else {
				if (byscore) {
					rep = client->zrangebyscore(var1, var2, var4);
				}
				else {
					rep = client->zrange(var1, static_cast<int>(var2), static_cast<int>(var4));
				}
			}
			client->commit();
			auto res = rep.get();
			if (res.is_array()) {
				for (auto re : res.as_array()) {
					v.insert(re.as_string());
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
	const 	LOGGER logger = ConsoleUtils::get_mutable_instance().getConsoleLogger("ZSetOperations");
};



