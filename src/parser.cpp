#include <parser.hpp>
#include <iostream>
using std::cout;
using std::endl;

namespace mr
{

AstNode* Parser::alloc_node()
{
	return &heap.emplace_back();
}

AstNode* Parser::parse()
{
	auto root=alloc_node();
	root->type=AstNode::CONTEXT;
	AstNode* current_node=nullptr;
	auto last_node=current_node;
	while(true)
	{
		current_node=parse_statemnt();
		if(!current_node) break;
		last_node->next=current_node;
		last_node=current_node;
	}
	
	return root;
}
AstNode* Parser::parse_statemnt()
{
	next_token();
	AstNode* node{};
	switch(token)
	{
			case Token::IDEN:
				// op call: name op expr
				// fn call: name (expr)
				node=parse_call();
				break;
			case Token::FUNCTION:
				node=parse_function();
				break;
			
			default:
				panic("unexpected token");
				break;
	 }
}

} // namespace mr