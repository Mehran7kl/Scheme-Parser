#include "interpreter.hpp"
namespace  mr{
	

void Interpreter::lisp_add(AstNode::Ptr result, AstNode::Ptr rest)
{
	
	if(!rest) panic(" needs argument");
	
	int sum=0;
	rest->foreach([this, &sum](AstNode::Ptr ptr)
	{
		
		resolve(ptr);
		
		if(ptr->index()!=AstNode::INT) panic("add only work for int");
		sum+=std::get<AstNode::INT>(*ptr);
		return false;
	});
	
	result->emplace<AstNode::INT>(sum);
}
void Interpreter::lisp_mul(AstNode::Ptr result, AstNode::Ptr rest)
{
	
	if(!rest) panic(" needs argument");
	
	int ret=1;
	rest->foreach([this, &ret](AstNode::Ptr ptr)
	{
		resolve(ptr);
		if(ptr->index()!=AstNode::INT) panic("mul only work for int");
		ret*=std::get<AstNode::INT>(*ptr);
		return false;
	});
	result->emplace<AstNode::INT>(ret);
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




void Interpreter::def_var(char const* cstr, AstNode::Ptr val)
{
	std::string str{cstr};
	global_context->set_var(str,val);
}

void Interpreter::def_func(char const* str,EvalFunc func )
{
	AstNode::Ptr node= AstNode::make();
	node->emplace<AstNode::NATIVE_EVAL>(func);
	global_context->set_var(str, node);
}

AstNode::Ptr Interpreter::eval_root(AstNode::Ptr expr)
{
	auto result=AstNode::make();
	eval(result,expr);
	return result;
}
void Interpreter::lisp_define(AstNode::Ptr result, AstNode::Ptr rest)
{
	result->emplace<AstNode::NIL>();
	if(!rest)panic("need name");
	if(rest->index()!=AstNode::SYMBOL) panic("need name for var");
	auto val=rest->next_node();
	if(!val)panic("need value");
	
	resolve(val);
	
	global_context->set_var(std::get<AstNode::SYMBOL>(*rest).c_str(), rest->next_node());
}
void Interpreter::lisp_lambda(AstNode::Ptr result, AstNode::Ptr rest)
{
	if(!rest)panic("need args");
	if(!rest->next_node())panic("needs body");
	
	result->emplace<AstNode::LAMBDA>(rest);
	
}

void Interpreter::eval(AstNode::Ptr result, AstNode::Ptr rest)
{
		if(!rest){
			
			result->emplace<AstNode::NIL>();
			return;
		}
	
		
		AstNode::Ptr val{};
		if(rest->index()==AstNode::SYMBOL){
			
			
			val=local_context->get_var(std::get<AstNode::SYMBOL>(*rest));
			
			
			
			if(!val) panic("this var is not defined and not isnt a function:" ,*rest);
		}
		else if(rest->index()==AstNode::NODE)
		{
			resolve(rest);
			val=rest;
			if(val->index()==AstNode::NIL) 
				panic("returned null");
		}
		
		if(val->index()==AstNode::NATIVE_EVAL)
		{
			
			EvalFunc func=std::get<AstNode::NATIVE_EVAL>(*val);
			
			(this->*func)(result, rest->next_node());
			
			return;
		}
		else if(val->index()==AstNode::LAMBDA)
		{
			
			local_context=VarContext::make_shared(local_context);
			auto args=std::get<AstNode::LAMBDA>(*val);
			auto body=args->next_node();
			auto param_ptr=rest->next_node();
			
			for(auto arg_ptr=std::get<AstNode::NODE>(*args);
					arg_ptr;
					arg_ptr=arg_ptr->next_node())
			{
				if(arg_ptr->index()!=AstNode::SYMBOL)
					panic("invalid argument name");
				if(!param_ptr) panic("need more parameters");
				resolve(param_ptr);
				local_context->set_var(std::get<AstNode::SYMBOL>(*arg_ptr), param_ptr);
				
				param_ptr=param_ptr->next_node();
			}
			
			resolve_to(result,body);
			local_context=local_context->outer;
			return;
		}
		else panic("dont support other types to be first:", *val);
		panic("unreachable");
	
}
void Interpreter::resolve_to(AstNode::Ptr result, AstNode::Ptr raw)
{
	assert(result && raw && "require nonull args");
	
	switch(raw->index()){
		case AstNode::NODE:
			eval(result, std::get<AstNode::NODE>(*raw));
			break;
		case AstNode::SYMBOL:
			{
				
				auto val=local_context->get_var(std::get<AstNode::SYMBOL>(*raw));
				
				if(!val) panic("undefined symbol:", *raw);
				result->content_of(val);
				
			}
			break;
		default:
			if(raw==result)break;
			
			{
			result->content_of(raw);
			}
			break;
	}
	
}
//make sure to not resolve wrongly a node that is used more than once
void Interpreter::resolve(AstNode::Ptr raw)
{
	resolve_to(raw,raw);
}

}