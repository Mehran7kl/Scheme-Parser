#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>
#include "arguments.hpp"
#include <type_traits>

#include "parser.hpp"


using namespace mr;
int main(int argc, char *argv[])
{
	Parser parser{"(test omg \"wow\" 7(8)) (lol)"};
	//parser.scanner.print_all();
	parser.parse();
	parser.print_tree();
	
	
}