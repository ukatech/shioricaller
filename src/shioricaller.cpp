// shioricaller.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "my-gists/ukagaka/shiori_loader.hpp"
#include "my-gists/ukagaka/protocol_message.hpp"
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

	auto error_handler = [](Cshiori::Error err) {
		std::cerr << RED_OUTPUT("Error: " << to_ansi_colored_string(err)) << std::endl;
	};
	auto	warning_handler = nullptr;//shioricaller does not need to handle warnings as it is only used for basic purposes

	Cshiori shiori{
		argv[1], 
		Cshiori::error_logger_type{
			error_handler,
			warning_handler
		}
	};
	
	if(not shiori.All_OK())
		return 1;

	std::string req_buf,req_line;
	while(1) {
		std::getline(std::cin, req_line);
		if(std::cin.eof())break;
		req_buf += req_line + "\r\n";
		if(req_line.empty()) {
			auto ret = ukagaka::protocol_message{shiori(req_buf)};
			std::cout << to_ansi_colored_string(ret);
			req_buf.clear();
		}
	}
}
