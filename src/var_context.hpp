#pragma once
#include "str_hash.hpp"
#include <iostream>
#include <memory>
namespace mr{
struct AstNode;
	
struct VarContext
{
	//Only shared pointers  this class are allowed becouse it's lifetime is ambigus and complex
	//TODO: Find probable memory leak in clousors cause strong refrence
	
	typedef std::shared_ptr<VarContext> Ptr ;
	
	AstNode* set_var(char const* str, AstNode* value);
	AstNode* get_var(char const* str);
	AstNode const* get_var(char const* str) const;
	static Ptr make_shared(Ptr const& outer);
	VarContext(Ptr const& outer);
	friend std::ostream& operator << (std::ostream& out, VarContext const& context); 
	Ptr outer{};
	private:
	
	str_hash_map<AstNode*> context{};
	
}; 

}