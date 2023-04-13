#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>
#include "arguments.hpp"
#include <type_traits>
#include <fstream>
#include "parser.hpp"
#include "interpreter.hpp"

using namespace mr;
std::string read_file(char const* path)
{
	std::fstream f{path};
	std::string ret{};
	std::string temp{};
	while(std::getline(f, temp)) ret+=temp+='\n';
	return ret;
}
int main(int argc, char *argv[])
{
	
	auto src = read_file(PROJECT_DIR"/lisp_src.txt");
	std::cout<<"Source code:\n"<<src<<std::endl;
	Parser parser{src.c_str()};
	//parser.scanner.print_all();
	std::cout<<"AST:\n";
	parser.parse();
	parser.print_tree();
	Interpreter engine{};
	
	for( auto st : parser.statements)
	{
		
		std::cout<<*engine.eval_root(st)<<std::endl;
	}
	
	
}