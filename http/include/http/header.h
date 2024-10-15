#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class HttpResponse {
public:
	HttpResponse(int status_code)
		: status_code_(status_code) {
		// 默认内容类型为文本
		set_header("Content-Type", "text/plain");
	}

	void set_header(const std::string& name, const std::string& value) {
		headers_ += name + ": " + value + "\r\n";
	}

	void set_body(const std::string& body) {
		body_ = body;
		// 计算并设置内容长度
		set_header("Content-Length", std::to_string(body.size()));
	}

	void send_header(int client_socket) {
		// 响应状态行
		std::string response = "HTTP/1.1 " + std::to_string(status_code_) + " " + get_status_message(status_code_) + "\r\n";
		response += headers_; // 添加所有响应头
		response += "\r\n"; // 头部结束
		response += body_; // 添加响应体

		// 发送响应
		send(client_socket, response.c_str(), response.size(), 0);
	}

private:
	int status_code_;
	std::string headers_;
	std::string body_;

	std::string get_status_message(int code) {
		switch(code) {
			case 200: return "OK";
			case 404: return "Not Found";
			case 500: return "Internal Server Error";
			default: return "Unknown";
		}
	}
};
