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
		tt= scan_num(false) ? Token::FLOAT : Token::INT;
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
	case '-':
		advance();
		bool is_float;
		if(is_digit(c)) {
			is_float=scan_num(true);
			tt= is_float? Token::FLOAT : Token::INT; 
		}else{
			scan_word();
			tt=Token::NAME;
		}
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
			return !(c==' '|| c=='\n'||c=='\r'||c=='\t');
		});
}

void Scanner::scan_word()
{
	char const *offset = buf_cursor;
	advance(
		[this]() {
			
			return !(is_alpha_num(c)||c=='_');
		});
	
	token_str = {offset, (size_t)(buf_cursor - offset + 1)};
}
bool Scanner::scan_num(bool negative)
{
	
	char const *offset = buf_cursor;
	bool saw_dot=false;
	if(negative) offset-=1;
	advance(
		[this, &saw_dot]() {
			
			
			if(c=='.'){
				if(saw_dot) report_error("invalid syntax for number");
				else{
					saw_dot=true;
					return false;
				}
			}
			return !is_digit(c);
		});
	
	token_str = {offset, (size_t)(buf_cursor - offset + 1)};
	return saw_dot;
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