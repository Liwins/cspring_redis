# redis 数据操作接口
类java spring的实现方式 c++
## 依赖
[cppredis](https://github.com/Cylix/cpp_redis) 

[spdlog](https://github.com/gabime/spdlog) only header

特点:
1. 线程安全
2. 连接池
3. 多输出sink

## 基本类型操作
1. string 

基本接口
```c++

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
```
基本操作
```c++
 StringRedisTemplate templatesf;
    RedisTemplate::Expire("std", std::chrono::seconds(2300));
	RedisTemplate::Expire("hel", std::chrono::minutes(23));
	RedisTemplate::Expire("hel", std::chrono::hours(23));
	auto tp = std::chrono::system_clock::from_time_t(1532335169);
	auto tp = std::chrono::system_clock::now() + std::chrono::hours(3);
	templatesf.ExpireAt("hel", tp);
	RedisTemplate::ExpireAt("hel", 2147483647);
	auto re = templatesf.GetValueOps();
	re.set("hel", "value", 1000);
	logger->info("{}", re.setIfAbsent("he2l", "hwe"));
	std::map<std::string, std::string> a{ {"asf","asdf"},{"asf1","asf1"},{"asf2","asf2"} };
	re.multiSet(a);
	logger->info(re.getAndSet("bala2", "new"));
	std::vector<std::string> v{};
	auto ans = re.multiGet(v);
	std::for_each(ans.begin(), ans.end(), [logger](auto asr) {
		logger->info(asr);
	});
	logger->info(re.increment("in1", 12.65));
	logger->info(re.append("test.ha", "qweasdfasdf"));
	logger->info(re.get("test.ha", -5, -1));
	logger->info(re.size("test.h2a"));
	logger->info(re.getBit("test.bit1", 10086));
	logger->info(re.setBit("test.bit1", 10086, true));
	logger->info(re.setBit("test.bit1", 10086, false));
	logger->info(re.getBit("test.bit1", 10086));
```
2. list

基本接口
```c++
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
```
test
```c++
    StringRedisTemplate templatesf;   
    auto l = templatesf.GetListOps();
	logger->info(l.LeftPush("test.list.ha", "a2"));
	logger->info(l.Index("test.list.ha", 0));
	logger->info(l.LeftPop("test.list.ha"));
	logger->info(l.LeftPush("test.list.ha", "a3"));
	std::vector<std::string> v{ "a4","a5","a6" };
	logger->info(l.LeftPushAll("test.list.ha", v));
	logger->info(l.LeftPushIfPresent("test.list.ha1", "fsa"));
	logger->info(l.LeftPushIfPresent("test.list.ha", "fsa"));
	auto ved = l.Range("test.list.ha", 0, -1);
	std::for_each(ved.begin(), ved.end(), [logger](auto f) {logger->info(f); });
	logger->info(l.Remove("test.list.ha", 1, "a4"));
	logger->info(l.RightPop("test.list.ha"));
	logger->info(l.RightPopAndLeftPush("test.list.ha", "test.list.ha3"));
	logger->info(l.RightPush("test.list.ha", "右边"));
	logger->info(l.Size("test.list.ha"));
	logger->info(l.RightPushAll("test.list.ha", v));
	l.Trim("test.list.ha", 0, 2);
```

3. hash

接口
```c++
template<typename H,typename HK,typename HV>
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
     virtual	void PutAll(H var1, std::map< HK,  HV>& var2) = 0;
     virtual	void Put(H var1, HK var2, HV var3) = 0;
     virtual	bool PutIfAbsent(H var1, HK var2, HV var3) = 0;
     virtual	std::vector<HV> Values(H var1) = 0;
     virtual	std::map<HK, HV> Entries(H var1) = 0;
};
```
基本操作
```c++
int main() {

	auto logger = ConsoleUtils::get_mutable_instance().getConsoleLogger("main");
	StringRedisTemplate templatesf;

	auto h = templatesf.GetHashOps();

	auto keys = h.Size("zd.oldhk.ref1324number");
	logger->info(keys);
	std::map<std::string, std::string> mapuser{ {"username","xiaoli"},{"password","riversky"},{"age","18"} };
	h.PutAll("test.user.1", mapuser);

	std::for_each(keys.begin(), keys.end(), [logger](auto key) {logger->info(key); });
	logger->info(h.PutIfAbsent("test.user.1", "email", "riversky@126.com"));
	auto values = h.Values("test.user.1");
	std::for_each(values.begin(), values.end(), [logger](auto val) {logger->info(val); });
	auto mapp = h.Entries("test.us3er.1");
	std::for_each(mapp.begin(), mapp.end(), [logger](auto val) {logger->info("{}-{}", val.first, val.second); });

	logger->info(templatesf.ConnectCount());
	system("pause");
	return 0;
}
```
4. set 

接口
```c++
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
  virtual	std::vector<V> Scan(K var1,V patterns) = 0;
  virtual	std::vector<V> Scan(K var1, V patterns, int64_t var2) = 0;
};
```
测试
```c++
int main() {

	auto logger = ConsoleUtils::get_mutable_instance().getConsoleLogger("main");
	StringRedisTemplate templatesf;


	auto s = templatesf.GetSetOps();
	std::vector<std::string> v{ "as", "asfd","fasdf","2" };
	logger->info(s.Add("test.set.ha1", v));
	auto dif = s.Difference("test.set.ha1", "test.set.ha");
	std::for_each(dif.begin(), dif.end(), [logger](const auto  di) {logger->info(di); });
	logger->info(s.DifferenceAndStore("test.set.ha2", "test.set.ha1", "test.set.ha"));
	auto rm = s.DistinctRandomMembers("test.set.ha1", 2);
	std::for_each(rm.begin(), rm.end(), [logger](const auto  di) {logger->info(di); });
	auto ri = s.Intersect("test.set.ha1", "test.set.ha");
	std::for_each(ri.begin(), ri.end(), [logger](const auto  di) {logger->info(di); });
	logger->info(s.IsMember("test.set.ha1", "a3s"));
	logger->info(s.IsMember("test.set.ha1", "as"));
	auto rmemi = s.Members("test.set.ha4");
	std::for_each(rmemi.begin(), rmemi.end(), [logger](const auto  di) {logger->info(di); });
	logger->info(s.Move("test.set.ha1", "test.set.ha4", "as"));
	logger->info(s.Size("test.set.ha1"));
	logger->info(s.Pop("test.set.ha1"));
	logger->info(s.Size("test.set.ha1"));

	logger->info(s.Size("test.set.ha1"));
	auto rpops = s.Pop("test.set.ha1", 2);
	std::for_each(rpops.begin(), rpops.end(), [logger](const auto  di) {logger->info(di); });
	logger->info(s.Size("test.set.ha1"));
	logger->info(s.RandomMember("test.set.ha1"));
	auto srmd = s.RandomMembers("test.set.ha1", 6);
	std::for_each(srmd.begin(), srmd.end(), [logger](const auto  di) {logger->info(di); });
	auto srmd1 = s.DistinctRandomMembers("test.set.ha1", 6);
	std::for_each(srmd1.begin(), srmd1.end(), [logger](const auto  di) {logger->info(di); });
	std::vector<std::string> v3{ "asfd" };
	logger->info(s.Remove("test.set.ha1", v3));
	auto scanas = s.Scan("test.set.ha1", "*as*", 2);
	std::for_each(scanas.begin(), scanas.end(), [logger](const auto  di) {logger->info(di); });


	logger->info(templatesf.ConnectCount());
	system("pause");
	return 0;
}
```
5. zset
接口
```c++
class ZSetOperations {
	virtual	bool Add(K var1, V var2, double var3) = 0;
	virtual	int64_t Add(K var1, std::vector<std::pair<double, V>> var2) = 0;
	virtual	int64_t Remove(K var1, std::vector<V> var2) = 0;
	virtual	double IncrementScore(K var1, V var2, double var3) = 0;
	/**
	 *获取排名
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
```
测试
```c++
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
```

