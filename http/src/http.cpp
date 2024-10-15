#include<http/http.h>

int Http::accept_request() {
	boost::beast::flat_buffer buffer;
	boost::asio::io_context ioc;
	boost::asio::ip::tcp::socket socket(ioc);

	try {
		socket.assign(boost::asio::ip::tcp::v4(), client);
		boost::beast::http::request<boost::beast::http::string_body> req;
		boost::beast::http::read(socket, buffer, req);

		std::cout << "Method: " << req.method_string() << std::endl;
		std::cout << "Target: " << req.target() << std::endl;
		std::cout << "HTTP version: " << req.version() << std::endl;
		method = req.method_string().to_string();
		Target = req.target().to_string();
		http_version = req.version();
		response_requests();
	} catch(const std::exception& e) {
		e.what();
	}
	return 0;
}

int Http::response_requests() {



	if("GET" == method) {
		response_get();
		return 1;

	} else if("POST" == method) {
		response_post();

		return 1;
	} else {
		auto res = create_response<boost::beast::http::string_body>(boost::beast::http::status::method_not_allowed, "Error method");
		boost::beast::http::write(socket, res);
		return -1;
	}

	
	return 0;
}

int Http::response_get() {


	std::size_t query_start = Target.find('?');    // 查找 '?' 字符

	if(query_start != std::string::npos) {
		// 提取查询字符串
		std::string query_string = Target.substr(query_start + 1);
		auto params = parse_query_params(query_string);
		std::string body{};
		for(const auto& param : params) {
			body += "Parameter: " + param.first + " = " + param.second + "\n";
		}
		
		return 1;
	} else {
		
		
		return 1;
	}


	return 0;
}

int Http::response_post() {
	return 0;
}

int Http::response_without_parameters() {
	return 0;
}

int Http::Response_with_parameters() {
	return 0;
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
