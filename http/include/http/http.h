#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
class Http {
public:
	Http(int _client) :client(_client) {};

	int accept_request();
	int response_requests();
	int response_get();
	int response_post();
	int response_without_parameters();
	int Response_with_parameters();


	std::unordered_map<std::string, std::string> parse_query_params(const std::string& query);
private:

	const int client;
	//req
	std::string method;
	std::string Target;
	std::string http_version;
	//http header

	//res




};
template <typename Body>
boost::beast::http::response<Body> create_response(boost::beast::http::status status, const std::string& message) {
	boost::beast::http::response<Body> res{status, 11}; // 11 是 HTTP 版本（HTTP/1.1）
	res.set(boost::beast::http::field::content_type, "text/plain");
	res.body() = message; // 设置响应体内容
	res.prepare_payload(); // 准备负载
	return res;
}
template<class T, class N>
class HeaderType {
public:
	HeaderType(auto& x, auto& y) {
		for(int i = 0; i < x.size(); i++) {
			map_headerType.insert(std::pair<T, N>(x[i], y[i]));
		}
	}
	auto findtype(auto& filename, auto& x) {
		char sc[1024] = {};
		auto i = 0;
		for(auto& c : filename) {
			if(c == '.') {
				sc[1024] = {};
				i = 0;
			}
			sc[i] = c;
			++i;
		}
		try {
			for(auto& s : x) {
				if(sc == s) {
					return map_headerType.find(sc)->second;
				}
			}
			throw std::out_of_range("erro");
		} catch(const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
private:
	std::map<std::string, std::string > map_headerType;
};