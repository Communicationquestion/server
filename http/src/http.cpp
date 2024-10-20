#include <http/http.h>
#include <http/header.h>
#include <fstream>
#include <string.h>
void Http::parse_request() {
	std::cout << "parse_request: \n"
		<< req << std::endl;
	request.parse(req);
	request.print();
	std::tuple<std::string, std::string> pars = request.get_req_pars();
	method = std::get<0>(pars);
	url = std::get<1>(pars);
}
int Http::accept_request() {
	//std::this_thread::sleep_for(std::chrono::seconds(5));
	std::string pwd(getcwd(NULL, 0));
	parse_request();
	std::string fleod = "/dist";
	if(url == "/") {
		url = pwd + fleod + url + "index.html";
	} else {
		url = pwd + fleod + url;
	}

	std::cout << "url\n"
		<< url << std::endl;
	response_requests();
	return 0;
}
int Http::response_requests() {
	if("GET" == method) {
		if(!file_existence()) {
			std::cout << "file not found" << std::endl;
			return -1;
		}
		std::cout << "find file\n";
		HttpResponse resheader(200);
		if(parse_query_params(url).empty()) {
			HeaderType<std::string, std::string> htype;
			std::string headertype = htype.findtype(url);
			std::cout << "headertype \n"
				<< headertype << std::endl;
			if(headertype == "erro") {
				return -1;
			}
			resheader.set_header_ctype(headertype);

			if(htype.type_binary(url)) {

				resheader.send_header(client);

				int sfd = send_binary(url);


				std::cout << "send : " << headertype << std::endl;
				if(sfd == -1) {
					std::cout << headertype << "fail \n";
					return -1;
				}
			} else {
				res = get_txt_data();
				resheader.set_header_cLength(res);

				resheader.send_header(client);
				int sfd = send(client, res.c_str(), res.size(), MSG_NOSIGNAL);

				std::cout << "send : " << headertype << std::endl;
				if(sfd == -1) {
					std::cout << headertype << "fail \n";
					return -1;
				}
			}
		} else {
			 //cgi = 1
			return -1;
		}
		return 1;
	} else if("POST" == method) {
		//cgi= 1



		if(!file_existence()) {
			std::cout << "file not found" << std::endl;
			return -1;
		}
		HttpResponse resheader(200);

		if(parse_query_params(url).empty()) {

		} else {

		}
		return 1;
	} else {
		HttpResponse resheader(404);
		resheader.set_header_ctype("Content-Type: text/plain\r\n");
		resheader.send_header(client);
		std::string e{"erorr method"};
		send(client, e.c_str(), e.size(), MSG_NOSIGNAL);
		// resheader.send_header(client);
		return -1;
	}

	return 0;
}

int Http::response_without_parameters() {


	return 0;
}

int Http::Response_with_parameters(int client, std::string res) {
	
	
	


	return 0;
}

bool Http::file_existence() {

	std::ifstream file(url);
	if(!file) {
		return 0;
	}
	return 1;
}

int Http::catbinary() {
	std::cout << "enter sendfile" << std::endl;

	int fd = open(url.c_str(), O_RDONLY);
	if(fd == -1) {
		printf("not found file\n");
		return -1;
	}

	struct stat stat = {};
	int ret = fstat(fd, &stat);
	if(ret == -1 || stat.st_size < 0) {
		return -1;
	}

	signal(SIGPIPE, SIG_IGN);
	ret = sendfile(client, fd, NULL, stat.st_size);
	if(ret <= 0) {
		printf("errrno is:%d", errno);
		return -1;
	}
	std::cout << "enter sendfile over" << std::endl;
	return 0;
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

int Http::send_binary(std::string url) {
	std::ifstream file(url, std::ios::binary | std::ios::ate);
	if(!file) {
		std::cerr << "无法打开文件: " << url << std::endl;
		return -1;
	}

	std::streamsize file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	const size_t buffer_size = 8192; // 8KB buffer
	char buffer[buffer_size];

	size_t total_sent = 0;
	while(total_sent < file_size) {
		size_t remaining = file_size - total_sent;
		size_t chunk_size = std::min(remaining, buffer_size);
		file.read(buffer, chunk_size);
		if(file.gcount() != chunk_size) {
			std::cerr << "读取文件失败!" << std::endl;
			return 1;
		}
		ssize_t sent_bytes = 0;
		while(sent_bytes < chunk_size) {
			ssize_t result = send(client, buffer + sent_bytes, chunk_size - sent_bytes, MSG_NOSIGNAL);
			if(result == -1) {
				if(errno == EINTR)
					continue; // 被信号中断，重试
				if(errno == EAGAIN || errno == EWOULDBLOCK) {
					// 资源暂时不可用，等待一段时间后重试
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					continue;
				}
				std::cerr << "发送数据失败: " << strerror(errno) << std::endl;
				return 1;
			}
			sent_bytes += result;
		}
		total_sent += sent_bytes;
	}
	std::cout << "成功发送 " << total_sent << " 字节的数据!" << std::endl;
	return 0;
}
std::unordered_map<std::string, std::string> Http::parse_query_params(const std::string& query) {
	std::unordered_map<std::string, std::string> params;
	std::stringstream ss(query);
	std::string key_value;

	while(std::getline(ss, key_value, '&')) {
		auto pos = key_value.find('=');
		if(pos != std::string::npos) {
			std::string key = key_value.substr(0, pos);
			std::string value = key_value.substr(pos + 1);
			params[key] = value;
		}
	}
	return params;
}
