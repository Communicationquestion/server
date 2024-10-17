#pragma once
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
class HttpRequest {
public:
	std::string method;
	std::string path;
	std::string version;
	std::map<std::string, std::string> headers;
	std::string body;

	void parse(const std::string& request) {
		std::istringstream requestStream(request);
		std::string line;


		if(std::getline(requestStream, line)) {
			std::istringstream requestLine(line);
			requestLine >> method >> path >> version;
		}

		while(std::getline(requestStream, line) && line != "\r") {
			size_t pos = line.find(":");
			if(pos != std::string::npos) {
				std::string key = line.substr(0, pos);
				std::string value = line.substr(pos + 1);

				value.erase(0, value.find_first_not_of(" \t"));
				headers[key] = value;
			}
		}


		if(requestStream.peek() != std::ifstream::traits_type::eof()) {
			body.assign((std::istreambuf_iterator<char>(requestStream)), std::istreambuf_iterator<char>());
		}
	}

	void print() const {
		std::cout << "Method: " << method << std::endl;
		std::cout << "Path: " << path << std::endl;
		std::cout << "Version: " << version << std::endl;
		std::cout << "Headers: " << std::endl;
		for(const auto& header : headers) {
			std::cout << "  " << header.first << ": " << header.second << std::endl;
		}
		std::cout << "Body: " << body << std::endl;
	}
	std::tuple<std::string, std::string> get_req_pars() {

		return {method,path};
	}
};


class HttpResponse {
public:
	HttpResponse(int status_code)
		: status_code_(status_code) {

		//set_header_ctype("text/plain");
	}

	void set_header_ctype(const std::string& value) {
		headers_ += value;
	}
	void set_header_cLength(const std::string& value) {

		headers_ += "Content-Length: " + std::to_string(value.size()) + "\r\n";
	}
	void send_header(int client_socket) {
		std::string response = "HTTP/1.1 " + std::to_string(status_code_) + " " + get_status_message(status_code_) + "\r\n";
		response += "Cache-Control: max-age=31536000, immutable\r\n";
		response += headers_;
		//response += "X-Content-Type-Options: nosniff\r\n";
		response += "\r\n";
		std::cout << "http_header\n" << response << std::endl;
		int sfd = send(client_socket, response.c_str(), response.size(), MSG_NOSIGNAL);
		if(sfd < 0) {
			perror("send");
			return;
		}
	}

private:
	int status_code_;
	std::string headers_;
	std::string get_status_message(int code) {
		switch(code) {
			case 200: return "OK";
			case 404: return "Not Found";
			case 500: return "Internal Server Error";
			default: return "Unknown";
		}
	}
};
