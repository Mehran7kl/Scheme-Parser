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
			
			case Token::INT:
				std::cout<<"INT "<<token_str;
				break;
			case Token::NAME:
				std::cout<<"NAME "<<token_str;
				break;
			case Token::STRING:
				std::cout<<"STR "<<token_str;
				break;
			case Token::LP:
				std::cout<<"(";
				break;
			case Token::RP:
				std::cout<<")";
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
	
	if (is_digit(c))
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
	case ';':
		skip_line_comment();
		advance();
		tt= next_token();
		break;
	case '"':
		scan_string();
		tt= Token::STRING;
		break;
	
	case 0:
		tt= Token::END;
		break;
	default:
		scan_word();
		tt= Token::NAME;
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
			
			return !(is_alpha(c)||c=='_');
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

void Scanner::skip_line_comment()
{
	advance(
		[this]() {
			return c == '\n';
		});
}


} // namespace mr