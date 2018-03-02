/*
	Utility class for D3D functionality
*/

#pragma once
#include <string>

//Release COM-object
#define SAFE_RELEASE(arg) if(*arg != nullptr) { (*arg)->Release(); (*arg) = nullptr; } 
#define WIDTH 1400
#define HEIGHT 900

inline std::wstring ToWChar(const std::string & inputString)
{
	return std::wstring(inputString.begin(), inputString.end());
}
