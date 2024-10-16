#pragma once
#include <iostream>
#include <http/header.h>
#include <unordered_map>
#include <stdlib.h>
#include <vector>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <chrono>
#include <thread>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include<mutex>
#include <string>
#include<sys/stat.h>
#include <sys/types.h>
#include <type_traits>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include<string>
#include<map>
#include<vector>
#include <unistd.h>
#include <cstddef>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <iostream>
#include<fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
template <class T, class N>
class HeaderType
{
public:
	HeaderType()
	{
		for (int i = 0; i < s1.size(); i++)
		{
			map_headerType.insert(std::pair<T, N>(s1[i], s2[i]));
		}
	}
	std::string getFilenameFromUrl(auto filename)
	{
		std::string url(filename);
		size_t pos = url.find_last_of('/');
		if (pos != std::string::npos)
		{
			return url.substr(pos + 1);
		}
		return "";
	}
	std::string getSubstringAfterLastDot(const std::string &input)
	{
		// 查找最后一个点的位置
		size_t lastDotIndex = input.rfind('.');
		if (lastDotIndex != std::string::npos)
		{
			// 返回最后一个点到结尾的子字符串
			return input.substr(lastDotIndex);
		}
		return "erro"; // 如果没有找到点，返回空字符串
	}
	std::string findtype(std::string filename)
	{
		std::string sc = getSubstringAfterLastDot(filename);
		if(sc == "erro"){
			std::cout<<"findtype fail\n";
			return "erro";
		}
		try
		{
			for (auto &s : s1)
			{
				if (sc == s)
				{

					return map_headerType.find(sc)->second;
				}
			}
			throw std::out_of_range("erro");
		}
		catch (const std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
		return "erro";
	}
	bool type_binary(auto filename)
	{

		std::string sc = getSubstringAfterLastDot(filename);
		for (auto i : binary_file)
		{
			if (i == sc)
			{
				return 1;
			}
		}

		return 0;
	}

private:
	std::vector<std::string> s1 = {".js", ".avi", ".css", ".html", ".wasm", ".ico", ".jpg", ".png", ".mp3", ".txt"};
	std::vector<std::string> s2 = {"Content-Type: text/javascript; charset=utf-8\r\n", "Content-Type: video/x-msvideo\r\n", "Content-Type: text/css\r\n", "Content-Type: text/html; charset=utf-8\r\n", "Content-Type: application/wasm\r\n", "Content-Type: image/x-icon\r\n", "Content-Type: application/x-jpg\r\n", "Content-Type: image/png\r\n", "Content-Type: audio/mpeg\r\n", "Content-Type: text/plain\r\n"};
	std::map<std::string, std::string> map_headerType;
	std::vector<std::string> binary_file{".mp3", ".wasm", ".jpg", ".png", ".ico"};
};

class Http
{
public:
	Http(int _client, std::string _req) : client(_client), req(_req) {};
	void parse_request();
	int accept_request();
	int response_requests();
	int response_without_parameters();
	int Response_with_parameters();
	bool file_existence();
	int catbinary();
	std::string get_txt_data();
	int send_binary(std::string url);
	std::unordered_map<std::string, std::string> parse_query_params(const std::string &query);

private:
	const int client;
	// req
	std::string req;
	std::string method;
	std::string url;
	std::string http_version;
	HttpRequest request;
	// http header
	// res

	char buffer[2048];
	std::string path;
	std::string res;
};
