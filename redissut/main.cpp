#include "orderinfo.h"
#include "RedisTemplate.h"
#include "RedisUtils.hpp"
#include <chrono>
#include <regex>
#include "templatetest.hpp"
int main() {
	auto logger = ConsoleUtils::get_mutable_instance().getConsoleLogger("main");
	StringRedisTemplate templatesf;
	logger->info(templatesf.ConnectCount());
	system("pause");
	return 0;
}
