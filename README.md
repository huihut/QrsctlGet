# QrsctlGet

## 下载七牛存储的对象

使用 qrsctl 工具只能单个文件下载，为了避免重复的工作，因此写了个 C++ 程序解决。

### 获取包含所有对象名的 html 文件

首先进入七牛云要下载的 bucket（存储空间）的 web 界面，把内容管理列表中全部对象显示出来（点底部加载更多直到全部显示），在 Chrome 浏览器 `右键` - `检查`，如下图，选中 `tbody` - `右键` - `Copy` - `Copy element`

![QiniuStorageSpaceTbody](https://huihut-img.oss-cn-shenzhen.aliyuncs.com/QiniuStorageSpaceTbody.png)

然后把复制到的内容保存成 `html.html` 文件，放到一个目录下，如我的 `D:\code\test\`（一下均以此目录为工程目录）

### 下载并登录 qrsctl

下载 [命令行辅助工具(qrsctl)](https://developer.qiniu.com/kodo/tools/1300/qrsctl)，保存到 `D:\code\test\` 目录，并重命名为 `qrsctl.exe`

在 `D:\code\test\` 目录的 CMD 或 PowerShell 中输入如下命令登录你的七牛账号

```
.\qrsctl.exe login huihut@outlook.com 123   # 假设密码为 123
```

### 编写批量下载程序

在 `D:\code\test\` 下创建个 `QrsctlGet.cpp`，把下面 C++ 代码保存到刚创建的文件，修改存储空间名为你的存储空间名的名字（本文为 `temp`），用 `VS Code` 或其他工具编译运行，七牛存储的文件即会下载到同目录下。

```cpp
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
```

![QiniuQrsctlDownload](https://huihut-img.oss-cn-shenzhen.aliyuncs.com/QiniuQrsctlDownload.png)