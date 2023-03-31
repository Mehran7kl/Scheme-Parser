#include <parser.hpp>
#include <iostream>
using std::cout;
using std::endl;

namespace mr
{

AstNode* Parser::alloc_node()
{
	return node_heap.allocate(1);
}

void Parser::parse()
{
	AstNode* current_node=nullptr;
	
	while((current_node=parse_statemnt())!=nullptr)
	{
		statements.push_back(*current_node);
	}
}
AstNode* Parser::parse_statemnt()
{
	next_token();
	AstNode* node{};
	if(token==Token::LP)
		node=parse_expr();
	else if(token==Token::END) return nullptr;
	else panic("unexpected token");
	return node;
}

AstNode* Parser::parse_expr()
{
	
	AstNode* root=alloc_node();
	auto node=root;
	for(next_token();token!=Token::END;next_token())
	{
	 	if(token==Token::RP) break;
	 	parse_token(*node);
	 	node->next=alloc_node();
	 	node=node->next;
	}
	if(token==Token::END) panic("Unterminated expr");
	 return root;
}
void Parser::parse_token(AstNode& root)
{
	//root.next=alloc_node();
	switch(token)
	{
		case Token::NAME:
			root.type=AstNode::SYMBOL;
			
			
			root.data=(void*)alloc_str(scanner.token_str);
			break;
		case Token::INT:
			root.type=AstNode::INT;
			{
			std::string str{scanner.token_str};
			int val=atoi(str.c_str()); 
			root.data=(void*)alloc_int(val);
			}
			break;
		case Token::FLOAT:
			root.type=AstNode::FLOAT;
			{
			std::string str{scanner.token_str};
			float val=(float)atof(str.c_str()); 
			root.data=(void*)alloc_float(val);
			}
			break;
		case Token::STRING:
			root.type=AstNode::STRING;
			
			root.data=(void*)alloc_str(scanner.token_str);
			break;
		case Token::LP:
			root.type=AstNode::NODE;
			root.data=parse_expr();
			break;
		default:
		panic("unreacjeable");
	}
}
} // namespace mr