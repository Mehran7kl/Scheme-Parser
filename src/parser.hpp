#pragma once 
#include <string_view>
#include<vector>
#include "scanner.hpp"

namespace  mr{

struct AstNode
{
	enum Type
	{
		ADD, ASSIGN, EXPR,
		STATEMENT,FUNCTION,
		VALUE, VAR, CALL, CONTEXT
		
	};
	Type type;
	AstNode* next{};
	union{
	AstNode* child;
	std::string_view str{};
	};
	
};
	
struct Parser
{
	std::vector<AstNode> heap{};
	Token token{};
	Scanner scanner;
	
	Parser(char* const src):scanner{src}
	{}
	inline Token next_token()
	{
		token=scanner.next_token();
		return token; 
	}
	[[noreturn]] inline void panic(std::string_view msg)
	{
		std::cout<<msg<<std::endl;
		abort();
	}
	//parses a token stream and returns token tree
	//will be a context node;
	AstNode* parse();
	//returns a pointer to a new node;
	AstNode* alloc_node();
	//returns null when expr is void
	AstNode* parse_expr();
	//returns null on end
	AstNode* parse_statemnt();
	//assumes that <name> have been parsed
	AstNode* parse_call();
	//assumes that <name (> have been parsed
	AstNode* parse_fn_call();
	//assumes that <name op> have been parsed
	AstNode* parse_op_chain();
	AstNode* parse_function();
	
};

}
