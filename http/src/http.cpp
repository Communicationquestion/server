#include<http/http.h>
#include <http/header.h>
#include <fstream>
void Http::parse_request() {
	std::istringstream request_stream(buffer);
	request_stream >> method;        // ��ȡ���󷽷�
	request_stream >> url;           // ��ȡ���� URL
	request_stream >> http_version;  // ��ȡ HTTP �汾
}

int Http::accept_request() {





	return 0;
}

int Http::response_requests() {
	if("GET" == method) {
		if(!file_existence()) {
			std::cout << "file not found" << std::endl;
			return -1;
		}
		if(parse_query_params(url).empty()) {
			//to do
			
			//����Ҫִ�г���
		} else {
			//��Ҫִ�г���
		}
		return 1;
	} else if("POST" == method) {
		if(!file_existence()) {
			std::cout << "file not found" << std::endl;
			return -1;
		}
		if(parse_query_params(url).empty()) {
			//����Ҫִ�г���
		} else {
			//��Ҫִ�г���
			//to do
		
		}
		return 1;
	} else {
		HttpResponse resheader(404);
		resheader.set_body("not the method");
		resheader.send_header(client);
		return -1;
	}

	return 0;
}


int Http::response_without_parameters() {

	return 0;
}

int Http::Response_with_parameters() {
	return 0;
}


bool Http::file_existence() {
	if(url == "/") {
		path = path+"index.html"; 
	}
	std::ifstream file(path);
	if(!file) {
		return 0;
	}
	return 1;
}

std::unordered_map<std::string, std::string> Http::parse_query_params(const std::string& query) {
	std::unordered_map<std::string, std::string> params; // ����һ���յ�unordered_map���ڴ洢����
	std::stringstream ss(query); // ʹ���ַ�������������ѯ�ַ���
	std::string key_value;

	// �� '&' �ָ�ÿ������
	while(std::getline(ss, key_value, '&')) {
		auto pos = key_value.find('='); // ���� '=' �ַ�
		if(pos != std::string::npos) { // ����ҵ��� '='
			std::string key = key_value.substr(0, pos); // ��ȡ��������
			std::string value = key_value.substr(pos + 1); // ��ȡ����ֵ
			params[key] = value; // �������洢��unordered_map��
		}
	}
	return params; // ���ش洢���в�����unordered_map
}
