/***********************************************************************
** Date: 		3/18/22
** Project :	test_driver.cpp
** Programers:	Jiahao Liang, Jianwen QI
** File:		test_driver.cpp
** Purpose:		
** Notes:		
***********************************************************************/

#include "hash_map.h"

#include <iostream>

int main(int argc, char *argv[]) 
{
	HashMap<int, int> test(19);
	test.put(24, 17);
	test.put(24, 19);
	test.put(24, 31);
	test.put(24, 18);
	test.put(34, 12);
	test.put(2, 12);
	test.put(4, 12);
	test.put(924, 12);
	test.erase(924);
	
	for(auto i = test.begin(); i != test.end(); ++i)
	{
		std::cout << i->getKey() << " " << i->getData() << '\n';
	}

//	test.erase(2);
//	test.erase(60);
//	std::cout << "key = 52, value = " << test.find(52)->getData() << '\n';
//	test.find(46)->getData();
//	test.erase(1);
	return 0;
}