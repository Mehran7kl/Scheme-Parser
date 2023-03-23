#include "scanner.hpp"
#include <string_view>
#include <iostream>

namespace mr
{
void Scanner::print_all()
{
	for(auto tt=next_token(); tt!=Token::END ;tt=next_token())
	{
		
		switch(tt)
		{
			case Token::IDEN:
				std::cout<<"IDEN "<<token_str;
				break;
			case Token::INT:
				std::cout<<"INT "<<token_str;
				break;
			case Token::STRING:
				std::cout<<"IDEN "<<token_str;
				break;
			case Token::ASSIGN:
				std::cout<<"=";
				break;
			case Token::ADD:
				std::cout<<"+";
				break;
			case Token::SUB:
				std::cout<<"-";
				break;
			case Token::MUL:
				std::cout<<"*";
				break;
			case Token::DIV:
				std::cout<<"/";
				break;
			case Token::LC:
				std::cout<<"{";
				break;
			case Token::RC: 
				std::cout<<"}";
				break;
			case Token::LB:
				std::cout<<"[";
				break;
			case Token::RB:
				std::cout<<"]";
				break;
			case Token::LP:
				std::cout<<"(";
				break;
			case Token::RP:
				std::cout<<")";
				break;
			case Token::LT:
				std::cout<<"<";
				break;
			case Token::GT:
				std::cout<<">";
				break;
			case Token::SEMICOLON:
				std::cout<<";";
				break;
			case Token::COMMA:
				std::cout<<",";
				break;
			default: 
				report_error("unhandled token");		
		}
		std::cout<<std::endl;
	}
}
void Scanner::report_error(std::string_view msg)
{
	std::cout << msg << std::endl;
	std::abort();
}
Scanner::Scanner(char const *src)
	: src_ptr{src}, buf_cursor{src},c{*buf_cursor}
{
}
Token Scanner::next_token()
{
	
	Token tt{};
	if (is_space(c))
	{
		skip_space();
		advance();
	}
	if (is_alpha(c))
	{
		scan_word();
		tt = Token::IDEN;
	}
	else if (is_digit(c))
	{
		scan_num();
	//	advance();
		tt= Token::INT;
	}
	else switch (c)
	{
	case '(':
		tt= Token::LP;
		break;
	case ')':
		tt= Token::RP;
		break;
	case '<':
		tt= Token::LT;
		break;
	case '>':
		tt= Token::GT;
		break;
	case '[':
		tt= Token::LB;
		break;
	case ']':
		tt= Token::RB;
		break;
	case '{':
		tt= Token::LC;
		break;
	case '}':
		tt= Token::RC;
		break;
	case '*':
		tt= Token::MUL;
		break;
	case '/':
		if (distinguish_slash())
			tt = Token::DIV;
		else 
		{
			advance();
			tt= next_token();
			return tt;
		}
			break;
	case '+':
		tt= Token::ADD;
		break;
	case '-':
		tt= Token::SUB;
		break;
	case ';':
		tt= Token::SEMICOLON;
		break;
	case ',':
		tt= Token::COMMA;
		break;
	case '=':
		tt= Token::ASSIGN;
		break;
	case '\'':
	case '"':
		scan_string();
		
		tt= Token::STRING;
		break;
	case 0:
		tt= Token::END;
		break;
	default:
		report_error("unrecognized input");
		tt= Token::END;
	}
	advance();
	return tt;
}

void Scanner::skip_space()
{
	advance(
		[this]() {
			return !is_space(c);
		});
}

void Scanner::scan_word()
{
	char const *offset = buf_cursor;
	advance(
		[this]() {
			
			return !is_alpha_num(c);
		});
	
	token_str = {offset, (size_t)(buf_cursor - offset + 1)};
}
void Scanner::scan_num()
{
	char const *offset = buf_cursor;

	advance(
		[this]() {
			return !is_digit(c);
		});

	token_str = {offset, (size_t)(buf_cursor - offset + 1)};
}
void Scanner::scan_string()
{
	char terminator = c;
	char const *offset = buf_cursor + 1;

	advance(
		[this, &terminator]() {
			return c == terminator;
		},
		[this]() { report_error("unterminated string"); });
	//skip terminator char
	advance();
	token_str = {offset, (size_t)(buf_cursor - offset)};
}
bool Scanner::distinguish_slash()
{
	advance();
	switch (c)
	{
	case '/':
		skip_line_comment();
		break;
	case '*':
		skip_multy_comment();
		break;
	default:
		return true;
	}
	return false;
}

void Scanner::skip_line_comment()
{
	advance(
		[this]() {
			return c == '\n';
		});
}

void Scanner::skip_multy_comment()
{
	advance(
		[this]() {
			if (c == '*')
			{
				advance();
				if (c == '/')
					return true;
			}
			return false;
		},
		[this]() { report_error("unterminated comment"); });
}
} // namespace mr