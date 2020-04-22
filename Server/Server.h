#pragma once
#include <string>
#include <unordered_map>
#include <functional>

//TODO: Forward declare or wrap classes
#include "../external/rapidjson/rapidjson/document.h"
#include "../external/rapidjson/rapidjson/stringbuffer.h"
#include "../external/rapidjson/rapidjson/writer.h"

//Forward declare winsock objects
struct addrinfo;
class WinSock;

class Server
{
public:
	struct Endpoint
	{
	public:
		enum class Type
		{
			POST
		};

		Endpoint(Type type, std::string& extension);

		bool operator ==(const Endpoint& rhs);

		Type									type;
		std::string								extension;
		std::function<std::string(std::string)> eventRequest;
	};

	Server(const std::string& ipAddress, const std::string& port);
	~Server();

	void listenSocket	();		//!< Connects to the underlying socket
	void closeSocket	();		//!< Disconnects from the underlying socket

private:
	void initResources();
	void exitResources();

	std::string parseSlackChallenge(const rapidjson::Document& doc);

private:
	std::unique_ptr<WinSock>	sock;
	addrinfo*					info;

	std::unordered_map<Endpoint, bool> endPoints;

	std::string ipAddress;
	std::string port;
};