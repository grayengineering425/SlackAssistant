#pragma once
#include <string>

enum class ResponseCode
{
	OK
};

class ResponseGenerator
{
public:
	static std::string generateResponse(ResponseCode code, const std::string& json);
};