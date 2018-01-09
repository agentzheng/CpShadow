#include <iostream>
#include<sstream>
#include <ParserDom.h>
#include <windows.h>
#include <Wininet.h>
#include<vld.h>
#include<vector>
#include<map>
#include<windows.h>
#include"Configer.hpp"

#pragma comment(lib, "Wininet.lib")

using namespace htmlcxx;
using namespace std;

#define MY_MAX_BUFFER_SIZE 100000
#define MAX_TRY_TIME 20
#define URL_URL "https://coding.net/u/ElliottZheng/p/elliottzheng.coding.me/git/raw/master/pyshadow_config/ishadowsocks"
#define ID_LIST_URL "https://coding.net/u/ElliottZheng/p/elliottzheng.coding.me/git/raw/master/pyshadow_config/id_list.json"
#define TEMPLATE_URL "https://coding.net/u/ElliottZheng/p/elliottzheng.coding.me/git/raw/master/pyshadow_config/gui-config-template.json"



std::string Fetch(string url,char * buffer)
{
	DWORD bytes_read;//下载的字节数 //打开一个internet连接 
	HINTERNET internet = InternetOpen("HTTP Downloader", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if (!internet)
		throw "InternetOpen error!";
	if (!internet) throw "InternetOpen error!"; //打开一个http url地址 
	HINTERNET file_handle = InternetOpenUrl(internet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (!file_handle)
		throw "InternetOpenUrl error! - Maybe you should add Http:// or Ftp://"; //从url地址中读取文件内容到缓冲区buffer 
	BOOL b = InternetReadFile(file_handle, buffer, MY_MAX_BUFFER_SIZE, &bytes_read);
	if (!b)
		throw "InternetReadFile error!";
	buffer[bytes_read] = 0;
	InternetCloseHandle(internet);
	return buffer;
}

string get_by_id(tree<HTML::Node>& dom,const string& html_content,const string& id)
{
	tree<HTML::Node>::iterator it = dom.begin();
	tree<HTML::Node>::iterator end = dom.end();
	for (; it != end; ++it)
	{
		if (!strcmp(it->tagName().c_str(),"span"))
		{
			it->parseAttributes();
			//cout << it->content(html_content) << endl;
			string str = it->content(html_content);
			if (it->attribute("id").second == id)
				return str;
		}
	}
	return "";
}




int main() 
{
	char buffer[MY_MAX_BUFFER_SIZE];
	int i = 0;
		cout << "Start Fetching resources from Sever..." << endl;
		try {
			string latest_url = Fetch(URL_URL, buffer);//抓取网址文件,获得最新网址
			string html_content = Fetch(latest_url, buffer);//从最新地址获取网页内容
			string id_list_string = Fetch(ID_LIST_URL, buffer);
			Configer configer;
			string template_string = Fetch(TEMPLATE_URL, buffer);
			while (!configer.SetTemplate(template_string) && i < MAX_TRY_TIME)
			{
				cout << "Fetching template error,Retrying....trying tims=" << i << endl;
				template_string = Fetch(TEMPLATE_URL, buffer);
				i++;
			}
			if (i < 20)
				cout << "Done,now we have downloaded all resources" << endl;
			vector<map<string, string>> template_data;//id_list数据

			Configer::Parse_ID_List(id_list_string, template_data);//解析在线json，获得id_list，以map形式存放

			HTML::ParserDom parser;
			tree<HTML::Node> dom = parser.parseTree(html_content);
			for (vector<map<string, string>>::iterator it = template_data.begin(); it != template_data.end(); it++)
			{
				for (map<string, string>::iterator it2 = it->begin(); it2 != it->end(); it2++)
					it2->second = get_by_id(dom, html_content, it2->second);
			}

			if (configer.GenerateNew(template_data))
			{
				for (vector<map<string, string>>::iterator it = template_data.begin(); it != template_data.end(); it++)
				{
					cout << "--------------start------------" << endl;
					//cout << "No." << distance(template_data.begin(),it) << endl;
					for (map<string, string>::iterator it2 = it->begin(); it2 != it->end(); it2++)
					{
						cout << it2->first << '=' << it2->second << endl;
					}
					cout << "-----------------end----------" << endl;
				}

				cout << "Congratulation! Your configuration is up to date" << endl;
				//cout << "Would you like my start the shadowsocks for you?(shadowsocks.exe)\n If not,press ESC to quit" << endl;

			}
		}
		catch (exception e)
		{
			cerr << "Fetching URL or IDLIST error,please try again";
		}
	system("pause");
	return 0;

}