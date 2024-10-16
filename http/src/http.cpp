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

			//�����ļ�����
			HeaderType<std::string, std::string> htype;
			std::string heaadertype = htype.findtype(url);

			//����header.����
			resheader.set_header_ctype(heaadertype); //����
													 //����
			if(htype.type_binary(url)) {
				// get binary data
				// ����Ҫ���ó���

			} else {
			    // get txt data
				//��ȡbody
				res = get_txt_data();
				//���ó���
				resheader.set_header_cLength(res);
			}


			// ����header��body
			try {
				resheader.send_header(client);
				send(client, res.c_str(), res.size(), 0);
			} catch(const std::exception& e) {
				e.what();
			}
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
		HttpResponse resheader(200);

		if(parse_query_params(url).empty()) {
			//����Ҫִ�г���
		} else {
			//��Ҫִ�г���
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
