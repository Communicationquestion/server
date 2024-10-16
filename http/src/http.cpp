#include<http/http.h>
#include <http/header.h>
#include <fstream>
void Http::parse_request() {
	std::cout << "parse_request: " << req << std::endl;
	request.parse(req);
	request.print();
	std::tuple<std::string ,std::string> pars = request.get_req_pars();
	method = std::get<0>(pars);
	url = std::get<1>(pars);
	path = "/htdocs" + url;
	std::cout << "path\n" << path << std::endl;
}

int Http::accept_request() {


	parse_request();
	if (url=="/")
	{
		url = path + "index.html";
	} else {
		url = path + url;
	}
	url = "/home/build/server/index.html";
	std::cout << "url\n" << url << std::endl;

	
	response_requests();

	return 0;
}

int Http::response_requests() {
	if("GET" == method) {
		if(!file_existence()) {
			std::cout << "file not found" << std::endl;
			return -1;
		}
		HttpResponse resheader(200);

		if(parse_query_params(url).empty()) {
			//to do

			//请求文件类型
			HeaderType<std::string, std::string> htype;
			std::string heaadertype = htype.findtype(url);

			//设置header.参数
			resheader.set_header_ctype(heaadertype); //类型
													 //长度
			if(htype.type_binary(url)) {
				// get binary data
				// 不需要设置长度

			} else {
			    // get txt data
				//获取body
				res = get_txt_data();
				//设置长度
				resheader.set_header_cLength(res);
			}


			// 发送header和body
			try {
				resheader.send_header(client);
				send(client, res.c_str(), res.size(), 0);
			} catch(const std::exception& e) {
				e.what();
			}
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
		HttpResponse resheader(200);

		if(parse_query_params(url).empty()) {
			//不需要执行程序
		} else {
			//需要执行程序
			//to do

		}
		return 1;
	} else {
		//HttpResponse resheader(404);
		
		//resheader.send_header(client);
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

	std::ifstream file(url);
	if(!file) {
		return 0;
	}
	return 1;
}

std::string Http::get_txt_data() {
	std::ifstream file(url);
	if(!file) {
		return "Error: could not open file.";
	}

	std::string content((std::istreambuf_iterator<char>(file)),
						std::istreambuf_iterator<char>());
	return content;
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
