// shioricaller.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "my-gists/ukagaka/shiori_loader.hpp"
#include "my-gists/ukagaka/protocol_message.hpp"
#include "my-gists/windows/EnableVirtualTerminal.hpp"
#include "my-gists/windows/small_io.hpp"
#include "my-gists/ansi_color.hpp"

#include <string>

int wmain(int argc, wchar_t* argv[]){
	EnableVirtualTerminal();//for color output

	if(argc != 2) {
		out << "Usage 1: \t" GREEN_TEXT("shioricaller shiori.dll < request.txt > response.txt") "\n"
			   "Usage 2: \t" GREEN_TEXT("shioricaller shiori.dll") "\n"
			   "(In Usage 2, you need to:" "\n"
			   "\t1. input request manually" "\n"
			   "\t2. press Enter*2 to send request" "\n"
			   "\t3. when you want to quit, press Ctrl+Z or something to send EOF" "\n"
			   ")" "\n";
		return 0;
	}

	auto error_handler = [](Cshiori::Error aerr) {
		err << RED_OUTPUT("Error: " << to_ansi_colored_string(aerr)) << endline;
	};

	Cshiori shiori{
		argv[1], 
		Cshiori::error_logger_type{
			error_handler
		}
	};
	
	if(not shiori.All_OK())
		return 1;
	if(in.is_console()) {
		std::wstring req_buf, req_line;
		while(1) {
			getline(in, req_line);
			if(!in)
				break;
			req_buf += req_line + L"\r\n";
			if(req_line.empty()) {
				const auto request			  = ukagaka::protocol_message{req_buf};
				const auto code_cvted_request = std::string(request);
				const auto ret				  = ukagaka::protocol_message{shiori(code_cvted_request)};
				out << to_ansi_colored_wstring(ret);
				req_buf.clear();
			}
		}
	}
	else {
		std::string req_buf, req_line;
		while(1) {
			getline(in, req_line);
			if(!in)
				break;
			req_buf += req_line + "\r\n";
			if(req_line.empty()) {
				const auto ret = ukagaka::protocol_message{shiori(req_buf)};
				out << to_ansi_colored_string(ret);
				req_buf.clear();
			}
		}
	}
}
