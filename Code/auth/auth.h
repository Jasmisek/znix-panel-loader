#pragma once
#include <string>

extern bool IsValid;

class authentication
{
public:
	static void login(std::string username, std::string password);
	static void time();
};