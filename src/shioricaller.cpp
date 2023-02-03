// shioricaller.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "my-gists/ukagaka/shiori_loader.hpp"
#include "my-gists/windows/GenerateDump.hpp"
#include <iostream>
#include <string>

int wmain(int argc, wchar_t* argv[]){
	InstallExceptionFilter();

	if(argc != 2) {
		std::cout << "Usage: shioricaller shiori.dll < request.txt > response.txt" << std::endl;
		return 0;
	}

	Cshiori shiori;
	shiori.SetTo(argv[1]);
	if(not shiori.All_OK()) {
		std::cerr << "Error: something fucked up." << std::endl;
		return 1;
	}

	std::string req;
	std::string req_line;
	while(true) {
		std::getline(std::cin, req_line);
		if(std::cin.eof())
			break;
		req += req_line + "\x0d\x0a";
		if(req_line.empty()) {
			auto res = shiori(req);
			std::cout << res;
			req.clear();
		}
	}

	return 0;
}
