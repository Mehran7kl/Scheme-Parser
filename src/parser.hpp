#pragma once 
#include <string_view>
#include<vector>
#include "scanner.hpp"
#include <iostream>
#include "zone.hpp"
#include <variant>
#include"node.hpp"
namespace  mr{




struct Parser
{
	
	
	
	
	Parser(char const* src):scanner{src}
	{}
	//prints the result AST (Abstract Syntax Tree)
	void print_tree();
	//parses a token stream and gens token tree
	void parse();
	std::vector<AstNode::Ptr> statements{};
	private:
	
	Token token{};
	Scanner scanner;
	
	[[noreturn]] void panic(std::string_view msg);
	
	
	Token next_token();
	//returns null when expr is void
	AstNode::Ptr parse_expr();
	void parse_token(AstNode::Ptr& node);
	//returns null if no statement exist; at the end
	AstNode::Ptr parse_statemnt();
	
};

}
