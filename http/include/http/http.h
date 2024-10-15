#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
inline std::string getFilenameFromUrl(const std::string& url) {

	size_t pos = url.find_last_of('/');
	if(pos != std::string::npos) {
		return url.substr(pos + 1);
	}
	return "";
}
template<class T, class N>
class HeaderType {
public:
	HeaderType() {
		for(int i = 0; i < s1.size(); i++) {
			map_headerType.insert(std::pair<T, N>(s1[i], s2[i]));
		}
	}
	std::string findtype(auto filename) {
		std::string str(filename);
		char sc[1024] = {};
		auto i = 0;
		for(auto c : str) {
			if(c == '.') {
				sc[1024] = {};
				i = 0;
			}
			sc[i] = c;
			++i;
		}
		try {
			for(auto& s : s1) {
				if(sc == s) {
					return map_headerType.find(sc)->second;
				}
			}
			throw std::out_of_range("erro");
		} catch(const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		return "null";
	}
private:
	std::vector<std::string> s1 = {".js",".avi",".css",".html",".wasm",".ico",".jpg",".png",".mp3",".txt"};
	std::vector<std::string> s2 = {"Content-Type: text/javascript; charset=utf-8\r\n","Content-Type: video/x-msvideo\r\n","Content-Type: text/css\r\n","Content-Type: text/html; charset=utf-8\r\n","Content-Type: application/wasm\r\n","Content-Type: image/x-icon\r\n","Content-Type: application/x-jpg\r\n" ,"Content-Type: image/png\r\n","Content-Type: audio/mpeg\r\n","Content-Type: text/plain\r\n"};
	std::map<std::string, std::string > map_headerType;
};
class Http {
public:
	Http(int _client) :client(_client) {
		memset(buffer, 0, sizeof(buffer));
		read(client, buffer, 1024);
		parse_request();
		char* temp;
		sprintf(temp, "htdocs%s", url);
		std::string str(temp);
		path = str;
	};
	void parse_request();
	int accept_request();
	int response_requests();
	int response_without_parameters();
	int Response_with_parameters();
	bool file_existence();
	std::unordered_map<std::string, std::string> parse_query_params(const std::string& query);
private:

	const int client;
	//req
	std::string method;
	std::string url;
	std::string http_version;
	//http header
	//res

	char buffer[1024];
	std::string path;
	std::string res;
};


