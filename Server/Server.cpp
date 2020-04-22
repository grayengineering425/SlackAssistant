#include "Server.h"

#include "../Utilities/ResponseGenerator.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class WinSock
{
public:
	WinSock(int family, int sockType, int protocol)
	{
		Socket = socket(family, sockType, protocol);
	}

	SOCKET Socket;
};

Server::Server(const std::string& IpAddress, const std::string& Port)
	:	sock		(nullptr)
	,	ipAddress	(IpAddress)
	,	port		(Port)
{
}

Server::~Server() = default;

void Server::listenSocket()
{
	initResources();

	if (!sock || !info) return;

	if (bind(sock->Socket, info->ai_addr, static_cast<int>(info->ai_addrlen)) != 0)	return;
	if (listen(sock->Socket, SOMAXCONN) != 0)										return;

	SOCKET clientSocket = accept(sock->Socket, NULL, NULL);

	//get info from client
	char		buffer[10000];
	
	int			bytesReceived		= 0;
	int			totalBytesReceived	= 0;
	int			contentLength		= -1;
	int			contentStart		= -1;
	std::string response			= "";
	
	while ((bytesReceived = recv(clientSocket, buffer, 10000, 0)) > 0)
	{
		response.resize(totalBytesReceived + bytesReceived);
	
		memcpy(&response[totalBytesReceived], &buffer[0], bytesReceived);
	
		if (contentLength == -1)
		{
			auto lengthFind = response.find("Content-Length: ");
	
			if (lengthFind != std::string::npos)
			{
				std::string cl = "";
	
				for (size_t i=lengthFind + 16; i < response.size(); ++i){ if (response[i] == '\r') break; cl += response[i];}
				
				contentLength = std::atoi(cl.c_str());
			}
		}
	
		totalBytesReceived += bytesReceived;
	
		if (contentStart == -1)
		{
			auto cFind = response.find("\r\n\r\n");
	
			if (cFind != std::string::npos) contentStart = static_cast<int>(cFind) + 4;
		}
	
		if (contentLength != -1 && contentStart != -1 && response.length() == contentStart + contentLength) break;
	}
	
	//check content length & content start, return unsuccessful if either are still undefined
	int i = 0;
	std::string methodText	 = "";
	std::string endPointText = "";
	
	while (i < static_cast<int>(response.size()) && response[i] != ' ') methodText	+= response[i++];
	
	i+=2;
	
	while (i < static_cast<int>(response.size()) && response[i] != ' ') endPointText+= response[i++];
	
	auto bracketIndex = response.find('{', 0);
	
	std::string responseJson = response.substr(bracketIndex, response.size() - bracketIndex);

	//Respond to the message from the server, based on input
	std::string responseMessage = "";

	//Read contents of json
	rapidjson::Document doc;
	doc.Parse(responseJson.c_str());

	if (doc.HasMember("challenge")) responseMessage = parseSlackChallenge(doc);
	else							responseMessage = ResponseGenerator::generateResponse(ResponseCode::OK, "");
	
	send(clientSocket, responseMessage.c_str(), static_cast<int>(strlen(responseMessage.c_str())), 0);
	
	shutdown	(clientSocket, SD_SEND);
	closesocket	(clientSocket);

	exitResources();
}

void Server::initResources()
{
	WSAData wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)						return;
	if (getaddrinfo(ipAddress.c_str(), port.c_str(), 0, &info) != 0)	return;
	if (!info)															return;

	sock = std::make_unique<WinSock>(info->ai_family, info->ai_socktype, info->ai_protocol);
}

void Server::exitResources()
{
	WSACleanup();

	closeSocket();
}

std::string Server::parseSlackChallenge(const rapidjson::Document& doc)
{
	std::string token = doc["challenge"].GetString();
	
	rapidjson::Document rDoc;
	rDoc.Parse("{}");
	
	rapidjson::Value v;
	
	v.SetString(token.c_str(), rDoc.GetAllocator());
	
	rapidjson::Value::StringRefType refName("challenge", static_cast<int>(strlen("challenge")));
	
	rDoc.AddMember(refName, v, rDoc.GetAllocator());
	
	rapidjson::StringBuffer buffer;
	buffer.Clear();
	
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	rDoc.Accept(writer);
	
	std::string bufferString = buffer.GetString();
	
	return ResponseGenerator::generateResponse(ResponseCode::OK, buffer.GetString());
}

void Server::closeSocket()
{
	if (!sock) return;

	if (closesocket(sock->Socket) != 0) return;
}

