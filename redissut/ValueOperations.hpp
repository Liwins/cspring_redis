#pragma once
#include <map>
#include<vector>
#include "redisclient.h"
template<typename K, typename V>
class ValueOperations {
	virtual void Set(K var, V var2) = 0;
	virtual	void Set(K var1, V var2, int64_t var3) = 0;
	virtual	bool SetIfAbsent(K var1, V var2) = 0;
	virtual	void MultiSet(std::map< K, V>& var1) = 0;
	virtual	bool MultiSetIfAbsent(std::map< K, V>& var1) = 0;
	virtual	V Get(K var1) = 0;
	virtual	V GetAndSet(K var1, V var2) = 0;
	virtual	std::vector<V> MultiGet(std::vector<K> var1) = 0;
	virtual	int64_t  Increment(K var1, int64_t var2) = 0;
	virtual	double Increment(K var1, double var2) = 0;
	virtual	int Append(K var1, std::string var2) = 0;
	virtual	std::string Get(K var1, int64_t var2, int64_t var4) = 0;
	virtual	int64_t Size(K var1) = 0;
	virtual	bool SetBit(K var1, int64_t var2, bool var4) = 0;
	virtual	bool GetBit(K var1, int64_t var2) = 0;
};
template<typename K, typename V>
class DefaultValueOperations :public ValueOperations<K, V> {
public:
	DefaultValueOperations() {
		client = RedisClient::get_mutable_instance().GetClient();
	}
	~DefaultValueOperations() {
		RedisClient::get_mutable_instance().ReliseClient(client);
		client = nullptr;
	}
	DefaultValueOperations(const DefaultValueOperations& rlt) {
		this.client = RedisClient::get_mutable_instance().GetClient();
	}
	void Set(K var, V var2)override {
		try {
			auto answer = client->set(var, var2);
			client->commit();
		}
		catch (const std::exception & e) {
			logger->info("set {}->{},error", var, var2);
		}
	}
	void Set(K var1, V var2, int64_t var3)override {
		try {
			client->set(var1, var2, [](auto  reply) {

			});
			client->expire(var1, var3, [](auto reply) {

			});
			client->commit();
		}
		catch (const std::exception & e) {
			logger->info("set {}->{}---expire {}  error", var1, var2, var3);
		}

	}
	bool SetIfAbsent(K var1, V var2)override {
		try {
			auto answoer=client->setnx(var1, var2);
			client->commit();
			auto res=answoer.get();
			if (res.as_integer() == 1) {
				return true;
			}
			else {
				return false;
			}
		}
		catch (const std::exception & e) {
			logger->info("set {}->{}  error->{}", var1, var2, e.what());
			return false;
		}
	}
	void MultiSet(std::map< K, V> &var1)override {
		std::vector<std::pair<K, V>> values;
		for (auto var : var1) {
			values.push_back(var);
		}
		try {
			client->mset(values, [](auto reply) {});
			client->commit();

		}
		catch (const std::exception & e) {
			logger->error(e.what());
		}
	}
	bool MultiSetIfAbsent(std::map< K, V>& var1)override {
		std::vector<std::pair<K, V>> values;
		for (auto var : var1) {
			values.push_back(var);
		}
		try {
			auto rep=client->msetnx(values);
			client->commit();
			auto ans=rep.get();
			if (ans.as_integer() == 1) {
				return true;
			}
			else {
				return false;
			}
		}
		catch (const std::exception & e) {
			logger->error(e.what());
		}
	}
	V Get(K var1)override {
		V v1;
		try {
			auto answer = client->get(var1);
			client->commit();
			auto rep = answer.get();
			if (rep.is_null()) {
				v1 = "";
				return v1;
			}
			v1 = rep.as_string();
			return v1;
		}
		catch (const std::exception & e) {
			logger->error(e.what());
		}
	}
	V GetAndSet(K var1, V var2)override {
		V v1;
		try{
			auto answer = client->getset(var1,var2);
			client->commit();
			auto rep = answer.get();
			if (rep.is_null()) {
				v1 = "";
				return v1;
			}
			v1 = rep.as_string();
			return v1;
		}catch (const std::exception & e){
			logger->error(e.what());
		}
	}
	std::vector<V> MultiGet(std::vector<K> var1)override {
		std::vector<V> v;
		try{
			auto rep=client->mget(var1);
			client->commit();
			auto result = rep.get();
			if (result.is_array()) {
				for (auto res : result.as_array()) {
					if (res.is_null()) {
						v.push_back("");
					}
					else {
						v.push_back(res.as_string());
					}
				}
			}
			return v;
		}catch (const std::exception & e){
			logger->error(e.what());
			return v;
		}
	}
	int64_t Increment(K var1, int64_t var2)override {
		try{
			auto rep=client->incrby(var1, var2);
			client->commit();
			auto res = rep.get();
			if (res.is_integer()) {
				return res.as_integer();
			}
			if (res.is_error()) {
				logger->info(res.error().c_str());
				return 0;
			}
		}catch (const std::exception & e){
			logger->info(e.what());
			return 0;
		}
	}
	double Increment(K var1, double var2)override {
		try {
			auto rep = client->incrbyfloat(var1, var2);
			client->commit();
			auto res = rep.get();
			if (res.is_string()) {
				return std::stod(res.as_string());
			}
			if (res.is_error()) {
				logger->info(res.error().c_str());
				return 0;
			}
		}
		catch (const std::exception & e) {
			logger->info(e.what());
			return 0;
		}
	}
	int Append(K var1, std::string var2)override {
		try{
			auto rep = client->append(var1, var2);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}catch (const std::exception & e){
			logger->info(e.what());
			return -1;
		}
	}
	std::string Get(K var1, int64_t var2, int64_t var4)override {
		try{
			auto rep=client->getrange(var1, var2, var4);
			client->commit();
			auto res = rep.get();
			if (res.is_string()) {
				return res.as_string();
			}
			else {
				return "";
			}
		}catch (const std::exception & e){
			logger->error(e.what());
			return "";
		}
		
	}

	int64_t Size(K var1)override {
		try {
			auto rep = client->strlen(var1);
			client->commit();
			auto res = rep.get();
			return res.as_integer();
		}
		catch (const std::exception & e) {
			logger->error(e.what());
			return 0;
		}
	}
	bool SetBit(K var1, int64_t var2, bool var4)override {
		std::string s = var4 ? "1" : "0";
		try{
			auto rep=client->setbit_(var1, var2, s);
			client->commit();
			auto res = rep.get();
			if (res.as_integer() == 1) {
				return true;
			}
			else {
				return false;
			}
		}catch (const std::exception & e){
			logger->error(e.what());
			return false;
		}
	}
	bool GetBit(K var1, int64_t var2)override {
		try {
			auto rep = client->getbit(var1, var2);
			client->commit();
			auto res = rep.get();
			if (res.as_integer() == 1) {
				return true;
			}
			else {
				return false;
			}
		}
		catch (const std::exception & e) {
			logger->error(e.what());
			return false;
		}
	}
private:
	cpp_redis::client* client;
     const 	LOGGER logger=ConsoleUtils::get_mutable_instance().getConsoleLogger("ValueOperations");
};
/**
 *
 /*RedisTemplate::Expire("std", std::chrono::seconds(2300));
 RedisTemplate::Expire("hel", std::chrono::minutes(23));
 RedisTemplate::Expire("hel", std::chrono::hours(23));
 auto tp=std::chrono::system_clock::from_time_t(1532335169);*/
 //auto tp = std::chrono::system_clock::now() + std::chrono::hours(3);
 //templatesf.ExpireAt("hel", tp);
 //RedisTemplate::ExpireAt("hel", 2147483647);
 //auto re = templatesf.GetValueOps();
 /*re.set("hel", "value",1000);
 logger->info("{}",re.setIfAbsent("he2l", "hwe"));*/
 /*std::map<std::string, std::string> a{ {"asf","asdf"},{"asf1","asf1"},{"asf2","asf2"} };
 re.multiSet(a);*/
 //logger->info(re.getAndSet("bala2", "new"));
 /*std::vector<std::string> v{};
 auto ans=re.multiGet(v);
 std::for_each(ans.begin(), ans.end(), [logger](auto asr) {
 logger->info(asr);
 });*/
 //logger->info(re.increment("in1", 12.65));
 //logger->info(re.append("test.ha", "qweasdfasdf"));
 //logger->info(re.get("test.ha",-5, -1));
 //logger->info(re.size("test.h2a"));
 //logger->info(re.getBit("test.bit1", 10086));
 //logger->info(re.setBit("test.bit1", 10086, true));
 /*logger->info(re.setBit("test.bit1", 10086, false));
 logger->info(re.getBit("test.bit1", 10086));*/
