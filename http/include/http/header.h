#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class HttpResponse {
public:
	HttpResponse(int status_code)
		: status_code_(status_code) {
		// Ĭ����������Ϊ�ı�
		set_header("Content-Type", "text/plain");
	}

	void set_header(const std::string& name, const std::string& value) {
		headers_ += name + ": " + value + "\r\n";
	}

	void set_body(const std::string& body) {
		body_ = body;
		// ���㲢�������ݳ���
		set_header("Content-Length", std::to_string(body.size()));
	}

	void send_header(int client_socket) {
		// ��Ӧ״̬��
		std::string response = "HTTP/1.1 " + std::to_string(status_code_) + " " + get_status_message(status_code_) + "\r\n";
		response += headers_; // ���������Ӧͷ
		response += "\r\n"; // ͷ������
		response += body_; // �����Ӧ��

		// ������Ӧ
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
