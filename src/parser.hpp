#pragma once 
#include <string_view>
#include<vector>
#include "scanner.hpp"
#include <iostream>
namespace  mr{

static int i=0;
struct AstNode
{
	enum Type
	{
		NIL,
		SYMBOL,
		INT,FLOAT,
		STRING,
		NODE,
		
	};
	
	Type type{NIL};
	AstNode* next{};
	inline void print()
	{
		
		switch(type){
			case SYMBOL:
				std::cout<<( char*)data;
				break;
			case INT:
				std::cout<<*(int*)data;
				break;
			case FLOAT:
				std::cout<<*(float*)data;
				break;
			case STRING: 
				std::cout<<"\""<<(char*)data<<"\"";
				break;
			case NODE:
				std::cout<<"(";
				((AstNode*)data)->print();
				break;
			case NIL:
				std::cout<<")";
				break;
		}
		std::cout<<" ";
		
		if(next) next->print();
	}
	void* data{};
	
};

struct Parser
{
	
	std::vector<AstNode> statements{};
	Token token{};
	Scanner scanner;
	std::allocator<AstNode> node_heap{};
	std::allocator<char> char_heap{};
	std::allocator<int> int_heap{};
	std::allocator<float> float_heap{};
	
	Parser(char const* src):scanner{src}
	{}
	inline void print_tree()
	{
		for(auto& st: statements)
		{
			std::cout<<"(";
			st.print();
		}
	}
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
	inline char* alloc_str(std::string_view view)
	{
		char* ptr=char_heap.allocate(view.length()+1);
		view.copy(ptr,view.length());
		ptr[view.length()]='\0';
		return ptr;
	}
	inline int* alloc_int(int val)
	{
		int* p=int_heap.allocate(1);
		*p=val;
		return p;
	}
	inline float* alloc_float(float val)
	{
		float* p=float_heap.allocate(1);
		*p=val;
		return p;
	}
	AstNode* alloc_node();
	//parses a token stream and gens token tree
	void parse();
	//returns null when expr is void
	AstNode* parse_expr();
	void parse_token(AstNode& node);
	//returns null if no statement exist; at the end
	AstNode* parse_statemnt();
	
};

}
