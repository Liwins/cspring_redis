#pragma once
#include <iostream>
void sum(int a, int b, int c) {
	std::cout << a + b + c << std::endl;
}
template<typename T>
void processValuebalabala(T &&arg) {
	std::cout << arg << std::endl;
}

/**
 *一个一个处理的方式
 *Date :[10/25/2018 ]
 *Author :[RS]
 */
template<typename T, typename ...Tn>
void processValuebalabala(T arg1, Tn... args) {
	processValuebalabala(std::forward<T>(arg1));
	std::function<void(int, int, int)> a;
	a = sum;
	a(std::forward<Tn>(args)...);
}
//processValuebalabala(1, 23, 23, 23, "23", 1.5);
