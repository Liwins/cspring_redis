#pragma once
#include "redisclient.h"
template<typename K, typename V>
class ListOperations {
	virtual	std::vector<V> Range(K var1, int64_t var2, int64_t var4);
	virtual	void Trim(K var1, int64_t var2, int64_t var4);
	virtual	int64_t Size(K var1);
	virtual	int64_t LeftPush(K var1, V var2);
	virtual	int64_t LeftPushAll(K var1, V... var2);
	virtual	int64_t LeftPushAll(K var1, std::vector<V> var2);
	virtual	int64_t LeftPushIfPresent(K var1, V var2);
	virtual	int64_t LeftPush(K var1, V var2, V var3);
	virtual	int64_t RightPush(K var1, V var2);
	virtual	int64_t RightPushAll(K var1, V... var2);
	virtual	int64_t RightPushAll(K var1, std::vector<V> var2);
	virtual	int64_t RightPushIfPresent(K var1, V var2);
	virtual	int64_t RightPush(K var1, V var2, V var3);
	virtual    void Set(K var1, int64_t var2, V var4);
	virtual	int64_t Remove(K var1, int64_t var2, Object var4);
	virtual	V Index(K var1, int64_t var2);
	virtual	V LeftPop(K var1);
	virtual	V LeftPop(K var1, int64_t var2, TimeUnit var4);
	virtual	V RightPop(K var1);
	virtual	V RightPop(K var1, int64_t var2, TimeUnit var4);
	virtual	V RightPopAndLeftPush(K var1, K var2);
	virtual	V RightPopAndLeftPush(K var1, K var2, int64_t var3, TimeUnit var5);
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
		try{
			auto rep = client->lrange(var1, var2, var4);
			client->commit();
			auto res = rep.get();

		}catch (const std::exception & e){
			
		}
		
	}
	void Trim(K var1, int64_t var2, int64_t var4)override {

	}
	int64_t Size(K var1)override {

	}
	int64_t LeftPush(K var1, V var2)override {

	}
	int64_t LeftPushAll(K var1, V... var2)override {

	}
	int64_t LeftPushAll(K var1, std::vector<V> var2) override{

	}
	int64_t LeftPushIfPresent(K var1, V var2)override {

	}
	int64_t LeftPush(K var1, V var2, V var3)override {

	}
	int64_t RightPush(K var1, V var2)override {

	}
	int64_t RightPushAll(K var1, V... var2)override {

	}
	int64_t RightPushAll(K var1, std::vector<V> var2)override {

	}
	int64_t RightPushIfPresent(K var1, V var2)override {

	}
	int64_t RightPush(K var1, V var2, V var3)override {

	}
	void Set(K var1, int64_t var2, V var4)override {

	}
	int64_t Remove(K var1, int64_t var2, Object var4)override {

	}
	V Index(K var1, int64_t var2)override {

	}
	V LeftPop(K var1)override {

	}
	V LeftPop(K var1, int64_t var2, TimeUnit var4)override {

	}
	V RightPop(K var1)override {

	}
	V RightPop(K var1, int64_t var2, TimeUnit var4)override {

	}
	V RightPopAndLeftPush(K var1, K var2)override {

	}
	V RightPopAndLeftPush(K var1, K var2, int64_t var3, TimeUnit var5)override {

	}
private:
	cpp_redis::client* client;
	const 	LOGGER logger = ConsoleUtils::get_mutable_instance().getConsoleLogger("ListOperations");
};