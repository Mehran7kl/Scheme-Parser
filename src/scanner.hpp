#pragma once
#include <type_traits>
#include <cctype>
#include <iostream>
namespace mr
{
enum class Token
{
	END,
	LP,RP, LC,RC, LB,RB,
	MUL,DIV,
	ADD,SUB,
	LT,GT,EQUAL,
	ASSIGN,
	SEMICOLON,COMMA,COLON,
	STRING,
	INT,
	FLOAT,
	IDEN,
	//keywords
	FUNCTION, FOR,WHILE,
	
};
struct Scanner
{
	std::string_view token_str;
	
	Scanner(char const* src);
	
	char const* src_ptr{};
	char const* buf_cursor{};
	//its value is the current pointed char
	char c{};
	inline void advance()
	{
		if (c)
			buf_cursor++;
		c = *buf_cursor;
	}
	//on_fail will be called when condition has'nt meeten till the end
	template <typename CheckF, typename FailF>
	inline void advance(CheckF check, FailF on_fail)
	{
		static_assert(std::is_invocable_v<CheckF>, "expected invocable object");
		static_assert(std::is_invocable_v<FailF>, "expected invocable object");

		for (advance(); c; advance())
			if (check())
			{
				back();
				return;
			}
		on_fail();
	}
	template <typename CheckF>
	inline void advance(CheckF check)
	{
		advance(check, []() {});
	}
	inline void back()
	{
		if (buf_cursor != src_ptr)
			buf_cursor--;
		c = *buf_cursor;
	}
	inline void back_unchecked()
	{
		buf_cursor--;
		c = *buf_cursor;
	}

	inline bool is_alpha(char c)
	{
		return std::isalpha(c);
	}
	inline bool is_digit(char c)
	{
		return std::isdigit(c);
	}
	inline bool is_space(char c)
	{
		return std::isspace(c);
	}
	inline bool is_alpha_num(char c)
	{
		return std::isalnum(c);
	}
	void report_error(std::string_view msg);
	void print_all();
	Token next_token();
	void skip_space();
	void skip_line_comment();
	void skip_multy_comment();
	void scan_word();
	void scan_num();
	void scan_string();
	//returns true if slah is div op
	bool distinguish_slash();
};

} // namespace mr