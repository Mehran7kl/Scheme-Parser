#pragma once
#include "str_hash.hpp"
#include <iostream>
#include <memory>
#include "node.hpp"

namespace mr{

struct VarContext
{
	//Only shared pointers  this class are allowed becouse it's lifetime is ambigus and complex
	//TODO: Find probable memory leak in clousors cause strong refrence
	
	typedef std::shared_ptr<VarContext> Ptr ;
	
	AstNode::Ptr set_var(std::string const& str, AstNode::Ptr value);
	AstNode::Ptr get_var(std::string const& str);
	
	static Ptr make_shared(Ptr const& outer);
	VarContext(Ptr const& outer);
	friend std::ostream& operator << (std::ostream& out, VarContext const& context); 
	Ptr outer{};
	private:
	
	str_hash_map<AstNode::Ptr> context{};
	
}; 

}