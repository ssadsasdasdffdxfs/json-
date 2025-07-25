#include <iostream>
#include "trace.h"
using namespace trace;
int main()
{
	json j1;
	json j2(true);
	json j3(42); 
	json j4(3.14);
	json j5("Hello, World!");
	auto a1 = j1;
	auto a2 = j2;
	auto a3 = j3;
	auto a4 = j4;
	auto a5 = j5;

	json j6(json::json_array);
	j6.append(j1);
	j6.append(j2);
	j6.append(j3);
	j6.append(j4);
	j6.append(j5);
	int c = j6[2];
	std::cout << c << std::endl;
	std::cout << j6.str() << std::endl;
	json obj;
	obj["bool"] = true;
	obj["int"] = 42;
	std::cout << obj.str() << std::endl;
	for (auto it = j6.begin(); it != j6.end(); it++)
	{
		std::cout << it->str() << std::endl;
	}
	std::cout << j6.has(0) << std::endl;
	j6.remove(2);
	std::cout << j6.str() << std::endl;

	return 0;
}