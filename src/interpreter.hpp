#pragma once
#include "parser.hpp"
#include "str_hash.hpp"
#include "zone.hpp"
#include "node.hpp"
#include "var_context.hpp"
#include <variant>


namespace  mr{

struct Interpreter
{
	
	Interpreter();
	//API
	AstNode::Ptr eval_root(AstNode::Ptr node);
	//Follwing functions wont modify result->next
	
	// gets head of an expr and evaluates it. puts the result to @result
	// @head usualy points to function name
	void eval(AstNode::Ptr result, AstNode::Ptr head);
	//resolves a raw value. and alternates it by computed value.
	void resolve(AstNode::Ptr raw_value);
	// resolves the computed value of raw_value and puts it on result.
	void resolve_to(AstNode::Ptr result, AstNode::Ptr raw_value);
	void def_var(char const* str, AstNode::Ptr val);
	void def_func(char const* str, EvalFunc func);
	//lisp basic rules
	void lisp_define(AstNode::Ptr result, AstNode::Ptr rest);
	void lisp_lambda(AstNode::Ptr result, AstNode::Ptr rest);
	//lisp basic functions
	void lisp_add(AstNode::Ptr result, AstNode::Ptr rest);
	void lisp_mul(AstNode::Ptr result, AstNode::Ptr rest);
	void lisp_div(AstNode::Ptr result, AstNode::Ptr rest);
	template <typename T>
	void print(T t)
	{
		std::cout<<t;
		std::cout<<std::endl;
	}
	template <typename T, typename ...Args>
	void print(T t, Args... args)
	{
		std::cout<<t<<" ";
		print(args...);
	}
	template <typename ...Args>
	[[noreturn]] void panic(Args... args)
	{
		print(args...);
		abort();
	}
	
	private:
	
	
	//states
	VarContext::Ptr local_context{};
	
	VarContext::Ptr global_context{};
};



}