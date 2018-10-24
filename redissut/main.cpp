#include "RedisUtils.hpp"
#include "orderinfo.h"
#include "RedisTemplate.h"
#include <chrono>
#include <regex>
int main() {
	auto logger = ConsoleUtils::get_mutable_instance().getConsoleLogger("main");
	StringRedisTemplate templatesf;
	logger->info(templatesf.ConnectCount());
	


	auto h=templatesf.GetHashOps();
	auto keys = h.Size("zd.oldhk.ref1324number");
	logger->info(keys);
	//std::map<std::string, std::string> mapuser{ {"username","xiaoli"},{"password","riversky"},{"age","18"} };
	//h.PutAll("test.user.1",mapuser);

	//std::for_each(keys.begin(), keys.end(), [logger](auto key) {logger->info(key); });
	//logger->info(h.PutIfAbsent("test.user.1", "email", "riversky@126.com"));
	//auto values=h.Values("test.user.1");
	//std::for_each(values.begin(), values.end(), [logger](auto val) {logger->info(val); });
	auto mapp = h.Entries("test.us3er.1");
	std::for_each(mapp.begin(), mapp.end(), [logger](auto val) {logger->info("{}-{}",val.first,val.second); });
	system("pause");
}