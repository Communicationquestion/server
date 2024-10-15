#include<http/http.h>
#include <http/header.h>
#include <fstream>
void Http::parse_request() {
	std::istringstream request_stream(buffer);
	request_stream >> method;        // 获取请求方法
	request_stream >> url;           // 获取请求 URL
	request_stream >> http_version;  // 获取 HTTP 版本
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
			
			//不需要执行程序
		} else {
			//需要执行程序
		}
		return 1;
	} else if("POST" == method) {
		if(!file_existence()) {
			std::cout << "file not found" << std::endl;
			return -1;
		}
		if(parse_query_params(url).empty()) {
			//不需要执行程序
		} else {
			//需要执行程序
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
	std::unordered_map<std::string, std::string> params; // 创建一个空的unordered_map用于存储参数
	std::stringstream ss(query); // 使用字符串流来解析查询字符串
	std::string key_value;

	// 按 '&' 分隔每个参数
	while(std::getline(ss, key_value, '&')) {
		auto pos = key_value.find('='); // 查找 '=' 字符
		if(pos != std::string::npos) { // 如果找到了 '='
			std::string key = key_value.substr(0, pos); // 提取参数名称
			std::string value = key_value.substr(pos + 1); // 提取参数值
			params[key] = value; // 将参数存储到unordered_map中
		}
	}
	return params; // 返回存储所有参数的unordered_map
}
