// shioricaller.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "my-gists/ukagaka/shiori_loader.hpp"
#include "my-gists/windows/GenerateDump.hpp"
#include <iostream>
#include <string>

#include <ConsoleApi2.h> //GetConsoleScreenBufferInfo & SetConsoleTextAttribute
#include <ProcessEnv.h> //GetStdHandle

int wmain(int argc, wchar_t* argv[]){
	InstallExceptionFilter();

	//获取标准输出原有颜色
	auto stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO out_csbi;
	GetConsoleScreenBufferInfo(stdout_handle, &out_csbi);

	auto reset_output_color=[&](){
		//恢复标准输出颜色
		SetConsoleTextAttribute(stdout_handle, out_csbi.wAttributes);
	};
	auto set_output_color=[&](WORD color){
		//设置输出颜色
		SetConsoleTextAttribute(stdout_handle, color);
	};

	if(argc != 2) {
		std::cout << "Usage 1: \t";

		//命令部分使用绿色显示
		set_output_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << "shioricaller shiori.dll < request.txt > response.txt" << std::endl;
		//恢复标准输出颜色
		reset_output_color();
		std::cout << "Usage 2: \t";
		//命令部分使用绿色显示
		set_output_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << "shioricaller shiori.dll" << std::endl;
		//恢复标准输出颜色
		reset_output_color();
		std::cout << "(In Usage 2, you need to:" << std::endl
				  << "\t1. input request manually" << std::endl
				  << "\t2. press Enter*2 to send request" << std::endl
				  << "\t3. when you want to quit, press Ctrl+Z or something to send EOF" << std::endl
				  << ")" << std::endl;
		return 0;
	}

	Cshiori shiori{argv[1]};
	if(not shiori.All_OK()) {
		//cerr设置颜色到红色
		auto stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO err_csbi;
		GetConsoleScreenBufferInfo(stderr_handle, &err_csbi);
		SetConsoleTextAttribute(stderr_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cerr << "Error: something fucked up." << std::endl;
		//恢复标准错误输出颜色
		SetConsoleTextAttribute(stderr_handle, err_csbi.wAttributes);
		return 1;
	}

	std::string req_buf,req_line;
	while(not std::cin.eof()) {
		//恢复标准输出颜色
		reset_output_color();
		std::getline(std::cin, req_line);
		req_buf += req_line + "\r\n";
		if(req_line.empty()) {
			//设置输出颜色到淡黄色
			set_output_color(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << shiori(req_buf);
			req_buf.clear();
		}
	}
}
