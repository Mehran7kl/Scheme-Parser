#pragma once
#include <string_view> 
#include <array>

namespace mr
{
struct Arguments
{
	
static void init(int argc, char* const* argv);
static std::string_view get(int index);

static const std::array<std::string_view,10> array();


};

} // namespace mr