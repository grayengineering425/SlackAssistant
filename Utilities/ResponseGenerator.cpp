#include "ResponseGenerator.h"

#include <sstream>

std::string ResponseGenerator::generateResponse(ResponseCode code, const std::string& json)
{
	std::stringstream ss;

	ss << "HTTP/1.1 ";

	switch(code)
	{
		default:
		case ResponseCode::OK:
			ss << "200 OK\r\n";
			break;
	}

	ss << "Content-Type: application/json\r\n";
	ss << "Content-Length: " << std::to_string(json.length()) << "\r\n\r\n";
	ss << json;

	return ss.str();
}
