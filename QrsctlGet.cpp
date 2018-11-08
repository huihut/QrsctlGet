//==============================================================
//  作者：huihut
//  邮箱：huihut@outlook.com
//  时间：2018-11-08 14:28:00
//  说明：匹配 HTML 的七牛云存储的文件名，并使用 qrsctl 下载文件
//==============================================================

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>
using namespace std;

int main()
{
	// 存储空间名【需要修改为你的存储空间名】
	string bucket = "temp";

	// 打开 html 文件
	ifstream fhtml;
	fhtml.open("html.html");

	// 统计下载的文件对象数
	int num = 0;

	if (fhtml)
	{
		// tbody 里面符合 reg 的奇数为文件对象，偶数是文件类型，因此只取奇数匹配项
		bool isObj = true;
		std::stringstream buffer;
		buffer << fhtml.rdbuf();
		std::string contents(buffer.str());
		std::smatch match;
		std::regex reg("\\b(edit-word ng-binding\">)([^<]*)");
		while (std::regex_search(contents, match, reg)) {
			if (isObj)
			{
				// 匹配到的文件对象
				string objfile = match.format("$2");
				cout << "Matched " + objfile << endl;

				// 合成下载命令
				string command = "qrsctl.exe get " + bucket + " " + objfile + " ./" + objfile;

				// 下载文件
				cout << "Download " + objfile + "..." << endl;
				system(command.c_str());

				// 文件个数加一
				num++;

				// 下一个非文件对象
				isObj = false;
			}
			else
			{
				// 下一个是文件对象
				isObj = true;
			}
			contents = match.suffix().str();
		}
	}
	else
	{
		cerr << "Failed to read html file!" << endl;
		system("pause");
		return -1;
	}

	cout << "Downloaded " + to_string(num) + " files." << endl;
	system("pause");
	return 0;
}
