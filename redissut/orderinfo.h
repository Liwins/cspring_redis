#pragma once
#include <cinttypes>
#include <string>
#include <json/json.h>
#include <json/writer.h>
#include <json/reader.h>
#include <sstream>

static const std::string ZD = std::string("zd:order:");
struct ZDOrder
{
	uint64_t Number;
	uint64_t OrderID;
	int64_t  Quantity;
	double limit_premium;
	uint64_t Deal_Quantity;
	unsigned int order_property; //单子属性
	double price;
	std::string customer_id;
	std::string system_id;
	std::string local_system;
	std::string buy_or_ask;
	std::string order_type;
	std::string user; //下单用户
	std::string order_date; //下单时间

	ZDOrder()
	{
		order_property = 0;
		Number = 0;
		OrderID = 0;
		Quantity = 0;
		Deal_Quantity = 0;
		limit_premium = 0;
		price = 0;
	}
	
	static std::string marshal(const ZDOrder& order) {
		Json::Value root;
		
		root["Number"] = order.Number;
		root["OrderID"] = order.OrderID;
		root["Quantity"] = order.Quantity;
		root["limit_premium"] = order.limit_premium;
		root["Deal_Quantity"] = order.Deal_Quantity;
		root["order_property"] = order.order_property;
		root["price"] = order.price;
		root["customer_id"] = order.customer_id;
		root["system_id"] = order.system_id;
		root["local_system"] = order.local_system;
		root["buy_or_ask"] = order.buy_or_ask;
		root["order_type"] = order.order_type;
		root["user"] = order.user;
		root["order_date"] = order.order_date;
		
		
		Json::StreamWriterBuilder wbuilder;
		std::string result = Json::writeString(wbuilder, root);
		return result;
	}
	static ZDOrder unMarshal(const std::string& order) {
		Json::Value root;
		std::stringstream   ss;
		ss <<order;
		ss >> root;
		ZDOrder z;
		
		z.Number = root["Number"].asUInt64();
		z.OrderID = root["OrderID"].asUInt64();
		z.Quantity = root["Quantity"].asInt64();
		z.limit_premium = root["limit_premium"].asDouble();
		z.Deal_Quantity = root["Deal_Quantity"].asUInt64();
		z.order_property = root["order_property"].asUInt();
		z.price = root["price"].asDouble();
		z.customer_id = root["customer_id"].asString();
		z.system_id = root["system_id"].asString();
		z.local_system = root["local_system"].asString();
		z.buy_or_ask = root["buy_or_ask"].asString();
		z.order_type = root["order_type"].asString();
		z.user = root["user"].asString();
		z.order_date = root["order_date"].asString();
		return z;
	}
};
/**
 *test 
 *Date :[9/26/2018 ]
 *Author :[RS]
 */
/*
ZDOrder o;
o.Number = 99646981;
o.OrderID = 7040875824048225353;
o.customer_id = "K000111";
o.Quantity = 1;
o.limit_premium =0.0;
o.Deal_Quantity = 0;
o.order_property = 1111;
o.price = 1118750.0;
o.system_id = "K00001111E000101";
o.local_system = "20180914000198";
o.buy_or_ask = "1";
o.order_type = "1";
o.user = "T_OFDA899_01";
o.order_date = "20180914";
std::string s(ZDOrder::marshal(o));
logger->info("marshal:{}", s);
ZDOrder de = ZDOrder::unMarshal(s);

logger->info("{},{},{},{},{},{},{},{}.{},{},{},{},{},{}", de.Number, de.OrderID, de.Quantity, de.limit_premium, de.Deal_Quantity, de.order_property, de.price, de.customer_id, de.system_id, de.local_system,
de.buy_or_ask, de.order_type, de.user, de.order_date);
*/