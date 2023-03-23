#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>
#include "arguments.hpp"
#include <type_traits>

#include "scanner.hpp"


using namespace mr;
int main(int argc, char *argv[])
{
	//Parser parser;
	Scanner sc{"testr=9; 1234(78)y"};
	sc.print_all();
	
	
}