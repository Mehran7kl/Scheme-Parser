#include "var_context.hpp"

namespace mr
{
typedef  VarContext::Ptr Ptr;

VarContext::VarContext(Ptr const& _outer): 
outer{_outer}{}

Ptr VarContext::make_shared(Ptr const& outer)
{
	auto ptr=std::make_shared<VarContext>(outer);
	return ptr;
}

AstNode::Ptr VarContext::get_var(std::string const& str)
{
	auto ptr{this} ;
	
	while(ptr){
		auto val=ptr->context[str];
		if(val) return val;
		ptr=ptr->outer.get();
	}
	return nullptr;
}

AstNode::Ptr VarContext::set_var(std::string const& str, AstNode::Ptr val)
{
	context[str]=val;
	return val;
}
std::ostream& operator << (std::ostream& out, AstNode const& node);
std::ostream& operator << (std::ostream& out, VarContext const& context)
{
	for(auto [key,value]: context.context)
	{
		out<<key<<":"<<*value;
	}
	return out;
} 
	
}