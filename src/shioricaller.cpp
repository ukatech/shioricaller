// shioricaller.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "my-gists/ukagaka/shiori_loader.hpp"
#include "my-gists/windows/GenerateDump.hpp"
#include "my-gists/windows/EnableVirtualTerminal.hpp"
#include "my-gists/ansi_color.hpp"
#include <iostream>
#include <string>

int wmain(int argc, wchar_t* argv[]){
	InstallExceptionFilter();
	EnableVirtualTerminal();//for color output

	if(argc != 2) {
		std::cout << "Usage 1: \t" GREEN_TEXT("shioricaller shiori.dll < request.txt > response.txt") << std::endl
				  << "Usage 2: \t" GREEN_TEXT("shioricaller shiori.dll") << std::endl
				  << "(In Usage 2, you need to:" << std::endl
				  << "\t1. input request manually" << std::endl
				  << "\t2. press Enter*2 to send request" << std::endl
				  << "\t3. when you want to quit, press Ctrl+Z or something to send EOF" << std::endl
				  << ")" << std::endl;
		return 0;
	}

	Cshiori shiori{argv[1]};
	if(not shiori.All_OK()) {
		std::cerr << RED_TEXT("Error: something fucked up.") << std::endl;
		return 1;
	}

	std::string req_buf,req_line;
	while(1) {
		std::getline(std::cin, req_line);
		if(std::cin.eof())break;
		req_buf += req_line + "\r\n";
		if(req_line.empty()) {
			std::cout << LIGHT_YELLOW_OUTPUT(shiori(req_buf));
			req_buf.clear();
		}
	}
}
