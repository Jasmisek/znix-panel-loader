#pragma once
#include <Windows.h>
#include <intrin.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sddl.h>
#include <algorithm>  //for std::generate_n
#include <locale>
class utilities
{
public:
    static std::string random_string( size_t length);
};