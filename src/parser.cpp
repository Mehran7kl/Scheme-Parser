#include <parser.hpp>
#include <iostream>

using std::cout;
using std::endl;

namespace mr
{



void Parser::print_tree()
{
	for (auto st : statements)
	
	{
		std::cout << "(";
		auto& ptr =st;
		ptr->foreach([](AstNode::Ptr p){
			std::cout<<*p;
			return false; 
		});
		std::cout << ")\n";
	}
	std::cout<<endl;
}
Token Parser::next_token()
{
	token = scanner.next_token();
	return token;
}
[[noreturn]] void Parser::panic(std::string_view msg)
{
	std::cout << msg << std::endl;
	abort();
}

void Parser::parse()
{
	if constexpr(LMODE){
		std::cout<<"parsing started"<<std::endl;
	}
	AstNode::Ptr current_node = nullptr;
	
	while ((current_node = parse_statemnt()) != nullptr)
	{
		statements.push_back(current_node);
	}
	if constexpr(LMODE){
		std::cout<<"parsing ended"<<std::endl;
	}
}
AstNode::Ptr Parser::parse_statemnt()
{
	
	next_token();
	AstNode::Ptr node{};
	if (token == Token::LP)
		node = parse_expr();
	else if (token == Token::END)
		return nullptr;
	else
		panic("unexpected token");
	return node;
}

AstNode::Ptr Parser::parse_expr()
{
	
	AstNode::Ptr root = AstNode::make();
	
	auto node = root;
	for (next_token(); token != Token::END;)
	{
		
		parse_token(node);
		
		std::cout<<*node<<std::endl;
		if (next_token() == Token::RP)
		{
			break;
		}
		node =node->append(AstNode::make());
		
	}
	if (token == Token::END)
		panic("Unterminated expr");
	return root;
}
void Parser::parse_token(AstNode::Ptr& root)
{
	
	switch (token)
	{
	case Token::NAME:
		root->emplace<AstNode::SYMBOL>(scanner.token_str) ;
		
		break;
	case Token::INT:
		{
			std::string str{scanner.token_str};
			int val = atoi(str.c_str());
			root->emplace<AstNode::INT>(val) ;
		}
		break;
	case Token::FLOAT:
		{
			std::string str{scanner.token_str};
			float val = atof(str.c_str());
			root->emplace<AstNode::FLOAT>(val) ;
		}
		break;
	case Token::STRING:
		root->emplace<AstNode::STRING>(scanner.token_str) ;
		break;
	case Token::LP:
		root->emplace<AstNode::NODE>(parse_expr());
		break;
	default:
		panic("unreachable");
	}
}
} // namespace mr