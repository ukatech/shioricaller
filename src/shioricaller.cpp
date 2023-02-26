// shioricaller.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "my-gists/ukagaka/shiori_loader.hpp"
#include "my-gists/windows/GenerateDump.hpp"
#include "my-gists/windows/EnableVirtualTerminal.hpp"
#include <iostream>
#include <string>

#define SET_GREEN "\033[32m"
#define SET_RED "\033[31m"
#define SET_LIGHT_YELLOW "\033[93m"
#define RESET_COLOR "\033[0m"

int wmain(int argc, wchar_t* argv[]){
	InstallExceptionFilter();
	EnableVirtualTerminal();

	if(argc != 2) {
		std::cout << "Usage 1: \t" SET_GREEN "shioricaller shiori.dll < request.txt > response.txt" RESET_COLOR << std::endl
				  << "Usage 2: \t" SET_GREEN "shioricaller shiori.dll" RESET_COLOR << std::endl
				  << "(In Usage 2, you need to:" << std::endl
				  << "\t1. input request manually" << std::endl
				  << "\t2. press Enter*2 to send request" << std::endl
				  << "\t3. when you want to quit, press Ctrl+Z or something to send EOF" << std::endl
				  << ")" << std::endl;
		return 0;
	}

	Cshiori shiori{argv[1]};
	if(not shiori.All_OK()) {
		std::cerr << SET_RED "Error: something fucked up." RESET_COLOR << std::endl;
		return 1;
	}

	std::string req_buf,req_line;
	while(not std::cin.eof()) {
		std::getline(std::cin, req_line);
		req_buf += req_line + "\r\n";
		if(req_line.empty()) {
			std::cout << SET_LIGHT_YELLOW << shiori(req_buf) << RESET_COLOR;
			req_buf.clear();
		}
	}
}
