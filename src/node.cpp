#include "node.hpp"
namespace  mr{
std::ostream& operator << (std::ostream& out, AstNode const& node)
{
	switch(node.index())
	{
		case AstNode::SYMBOL:
			
			out<<std::get<AstNode::SYMBOL>(node)<<"N";
			break;
		case AstNode::INT:
			std::cout<<std::get<AstNode::INT>(node)<<"i";
			break;
		case AstNode::FLOAT:
			std::cout<<std::get<AstNode::FLOAT>(node)<<"f";
			break;
		case AstNode::STRING: 
			std::cout<<std::get<AstNode::STRING>(node)<<"S";
			break;
			
		case AstNode::LAMBDA:
			std::cout<<"(";
			{
				auto ptr=std::get<AstNode::LAMBDA>(node);
				ptr->foreach([&out](AstNode::Ptr p){
					out<<*p;
					return false;
				});
			}
			break;
		case AstNode::NODE:
			
			std::cout<<"(";
			{
				auto ptr=std::get<AstNode::NODE>(node);
				ptr->foreach([&out](AstNode::Ptr p){
					out<<*p;
					return false;
				});
			}
			std::cout<<")";
			break;
		case AstNode::NIL:
			std::cout<<"NIL";
			break;
		case AstNode::NATIVE_EVAL:
			std::cout<<"NATIVE";
			break;
		
	}
	out<<" ";
	return out; 
}
AstNode::Ptr AstNode::make()
{
	return std::make_shared<AstNode>();
}

AstNode::Ptr AstNode::append(AstNode::Ptr ptr)
{
	next=ptr;
	return ptr;
}

}