#pragma once

#include "arguments.hpp"
static constexpr int max_args=10;
static std::array<std::string_view,max_args> args;

namespace mr
{
void Arguments::init(int argc, char* const* argv)
{
	if(argc>max_args)argc=max_args;
	
	for(int i=0;i<argc;i++)
		args[i]=argv[i];
}

std::string_view Arguments::get(int index)
{
	return args[index];
}

const std::array<std::string_view,max_args> Arguments::array()
{
	return args;
}

}