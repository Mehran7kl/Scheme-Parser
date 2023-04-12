#include "interpreter.hpp"
namespace  mr{
	

void Interpreter::lisp_add(AstNode::Ptr result, AstNode::Ptr rest)
{
	
	if(rest==nullptr) panic(" needs argument");
	resolve(rest);
	bool is_float;
	if(rest->index()==AstNode::FLOAT)
		is_float=true;
	else if(rest->index()==AstNode::INT)
		is_float=false;
	else panic("Can't add this type");
	if(is_float) result->emplace<AstNode::FLOAT>(0.0f);
	else result->emplace<AstNode::INT>(0);
	
	rest->foreach_next([this](AstNode::Ptr ptr)
	{
		resolve(ptr);
		if(is_float) std::get<AstNode::FLOAT>(*ptr);
		return false;
	});
	
}
void Interpreter::lisp_mul(AstNode* result, AstNode* rest)
{
	
	if(rest==nullptr) panic(" needs argument");
	
	resolve(rest);
	result->type=rest->type;
	switch(result->type)
		{
			case AstNode::INT:
				result->value.int_value=1;
				break;
			case AstNode::FLOAT:
				result->value.float_value=1;
				break;
			default:
				panic("dont support this type for multiplication");
		}
	for(; rest;  rest=rest->next)
	{
		resolve(rest);
		if(result->type!=rest->type)
			panic("All args should be same type");
		
		switch(result->type)
		{
			case AstNode::INT:
				result->value.int_value*=rest->value.int_value;
				break;
			case AstNode::FLOAT:
				result->value.float_value*=rest->value.float_value;
				break;
			default:
				panic("dont support this type for addition");
				
		}
	}
	
}


Interpreter::Interpreter()
{
	global_context=VarContext::make_shared(nullptr);
	local_context=global_context;
	def_func("add", &Interpreter::lisp_add); 
	def_func("mul", &Interpreter::lisp_mul); 
	def_func("define", &Interpreter::lisp_define);
	def_func("lambda", &Interpreter::lisp_lambda);
	
}


AstNode* Interpreter::alloc_node()
{
	//return zone.allocate<AstNode>(1);
	return new AstNode{};
}


void Interpreter::def_var(char const* str, AstNode* val)
{
	global_context->set_var(str,val);
}

void Interpreter::def_func(char const* str,EvalFunc func )
{
	AstNode* node=zone.allocate<AstNode>(1);
	node->type=AstNode::NATIVE_EVAL;
	node->value.native_eval=func;
	global_context->set_var(str, node);
}

AstNode Interpreter::eval_root(AstNode* expr)
{
	auto result=alloc_node();
	
	eval(result, expr);
	return *result;
}
void Interpreter::lisp_define(AstNode* result, AstNode* rest)
{
	result->type=AstNode::NIL;
	if(!rest)panic("need name");
	if(rest->type!=AstNode::SYMBOL) panic("need name for var");
	auto val=rest->next;
	if(!val)panic("need value");
	
	resolve(val);
	
	global_context->set_var(rest->value.str, rest->next);
}
void Interpreter::lisp_lambda(AstNode* result, AstNode* rest)
{
	result->type=AstNode::LAMBDA;
	if(!rest)panic("need args");
	if(!rest->next)panic("needs body");
	result->value.node=rest;
}

void Interpreter::eval(AstNode* result, AstNode* rest)
{
		if(!rest){
			result->type=AstNode::NIL;
			return;
		}
	
		
		AstNode* val{};
		if(rest->type==AstNode::SYMBOL){
			
			val=local_context->get_var(rest->value.str);
			if(!val) panic("this var is not defined and not isnt a function:" ,*rest);
		}
		else if(rest->type==AstNode::NODE)
		{
			resolve(rest);
			val=rest;
			if(val->type==AstNode::NIL) 
				panic("returned null");
		}
		
		
		if(val->type==AstNode::NATIVE_EVAL)
		{
			EvalFunc func=val->value.native_eval;
			(this->*func)(result, rest->next);
			return;
		}
		else if(val->type==AstNode::LAMBDA)
		{
			
			local_context=VarContext::make_shared(local_context);
			auto args=val->value.node;
			auto body=args->next;
			auto param_ptr=rest->next;
			
			for(auto arg_ptr=args->value.node;
					arg_ptr;
					arg_ptr=arg_ptr->next)
			{
				if(arg_ptr->type!=AstNode::SYMBOL)
					panic("invalid argument name");
				if(!param_ptr) panic("need more parameters");
				resolve(param_ptr);
				local_context->set_var(arg_ptr->value.str, param_ptr);
				
				param_ptr=param_ptr->next;
			}
			
			resolve_to(result,body);
			local_context=local_context->outer;
			return;
		}
		else panic("dont support other types to be first:", *val);
		panic("unreachable");
	
}
void Interpreter::resolve_to(AstNode* result, AstNode* raw)
{
	assert(result&& raw && "require nonull args");
	switch(raw->type){
		case AstNode::NODE:
			eval(result, raw->value.node);
			break;
		case AstNode::SYMBOL:
			{
				auto val=local_context->get_var(raw->value.str);
				if(!val) panic("undefined symbol:", *raw);
				result->type=val->type;
				result->value=val->value;
			}
			break;
		default:
			if(raw==result)break;
			result->type=raw->type;
			result->value=raw->value;
			break;
	}
	
}
//make sure to not resolve wrongly a node that is used more than once
void Interpreter::resolve(AstNode* raw)
{
	resolve_to(raw,raw);
}

}