#include "orderinfo.h"
#include "RedisTemplate.h"
#include "RedisUtils.hpp"
#include <chrono>
#include <regex>
#include "templatetest.hpp"
template<typename T>
void print22(T p, LOGGER logger) {
	std::for_each(p.begin(), p.end(), [logger](auto pi) {logger->info(pi); });
}
template<typename T>
void printMap(T p, LOGGER logger) {
	std::for_each(p.begin(), p.end(), [logger](auto pi) {logger->info("{}:{}", pi.first, pi.second); });
}
int main() {

	auto logger = ConsoleUtils::get_mutable_instance().getConsoleLogger("main");
	StringRedisTemplate templatesf;


	auto zset = templatesf.GetZSetOps();
	logger->info(zset.Add("test.zset.ha", "lilei", 195));
	std::vector<std::pair<double, std::string>> v{ {12,"zshagn"},{174,"baidu"},{23,"as"},{45,"wwef"} };
	logger->info(zset.Add("test.zset.ha5", v));
	logger->info(zset.Count("test.zset.ha", 14, 50));
	logger->info(zset.IncrementScore("test.zset.ha", "zhagn", 5.5));
	auto re = zset.Range("test.zset.ha", 0, 2);
	auto re = zset.RangeByScore("test.zset.ha", 14, 50);
	auto re = zset.RangeByScoreWithScores("test.zset.ha", 14, 50);
	auto re = zset.RangeWithScores("test.zset.ha", 0, 3);
	std::vector<std::string> vtemp{ "test.zset.h1a" };
	logger->info(zset.IntersectAndStore("test.zset.ha3", "test.zset.ha1", vtemp));
	logger->info(zset.Rank("test.zset.ha", "baidu"));
	std::vector<std::string> vai{ "baidu" };
	logger->info(zset.Remove("test.zset.ha", vai));
	logger->info(zset.RemoveRangeByRank("test.zset.ha", 0, 2));
	logger->info(zset.RemoveRangeByScore("test.zset.ha1", 12, 14));
	auto re = zset.ReverseRange("test.zset.ha5", 0, 1);
	auto re = zset.ReverseRangeByScore("test.zset.ha5", 50.1, 20);
	auto re = zset.ReverseRangeByScoreWithScores("test.zset.ha5", 50.1, 20);
	auto re = zset.ReverseRangeWithScores("test.zset.ha5", 0, 1);
	logger->info(zset.Score("test.zset.ha5", "ba3idu"));
	logger->info(zset.UnionAndStore("test.zset.hei1", vtemp, "test.zset.ha5"));
	logger->info(zset.ZCard("test.zset.hei1"));
	logger->info(zset.ReverseRank("test.zset.ha", "baidu"));
	printMap(re, logger);
	print22(re, logger);


	logger->info(templatesf.ConnectCount());
	system("pause");
	return 0;



}
