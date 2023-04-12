#pragma once
#include <iostream>
#include "var_context.hpp"
#include <variant>

namespace mr{
 //First I adopted raw pointers , raw unions , custom allocators.
 // I faced many bugs that were hard debuging, now i'm migrating to safer methods.
 //So project is incomplete yet.
 struct Interpreter;
 // Need to pass some arguments to use recursion. 
 struct AstNode;
 
 //Abstract syntax tree node
 // Also respresents the dynamic type of the lang
 struct AstNode: std::variant<
		std::monostate, 
		std::string,
		int, 
		float, 
		std::string,
		std::shared_ptr<AstNode>,
		void( Interpreter::*)(std::shared_ptr<AstNode> result, std::shared_ptr<AstNode> rest),
		std::shared_ptr<AstNode>	>,
		std::enable_shared_from_this<AstNode>
{
	//
	typedef  std::shared_ptr<AstNode> Ptr;
	enum Type
	{
		NIL, // empty; no value
		SYMBOL,
		INT,FLOAT,
		STRING,
		NODE,
		//This is not just AST but dynamic type of this lang too
		// Becouse it's useful in LISP
		NATIVE_EVAL,
		// in case of lambda , value.node points to args node and the next pf that will be body node
		LAMBDA,
	};
	
	template <typename FuncT>
	void foreach(FuncT func)
	{
		for(auto ptr=shared_from_this(); ptr; ptr=ptr->next)
		{
			if(func(ptr))return;
		}
	}
	//like above but starts from next
	template <typename FuncT>
	void foreach_next(FuncT func)
	{
		for(auto ptr=next;ptr;ptr=ptr->next)
		{
			if(func(ptr))return;
		}
	}
	AstNode::Ptr append(AstNode::Ptr ptr);
	AstNode operator + (AstNode&& that);
	static AstNode::Ptr make();
	private:
	std::shared_ptr<AstNode> next{};
	
}; 
using EvalFunc= void( Interpreter::*)(AstNode::Ptr result, AstNode::Ptr rest);
		
std::ostream& operator << (std::ostream& out, AstNode const& node);

 
}