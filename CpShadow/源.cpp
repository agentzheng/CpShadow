#include <windows.h>
#include <Wininet.h>
#include <iostream> 
using namespace std; 
#pragma comment(lib, "Wininet.lib") 

void Download(char url[]); 
int main(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0; char url[1000];//存放URL地址 //用户键入quit时退出 
	while(cin >> url && strcmp(url, "quit") != 0) 
	{ 
		try {
			cout << "目标URL：" << url << endl; Download(url); 
		} catch (char* e)//显示出错信息 
		{
			cout << e << endl << endl; 
		} 
	} return nRetCode; 
} 
void Download(char url[]) 
{
	char buffer[100000];//下载文件的缓冲区 
	DWORD bytes_read;//下载的字节数 //打开一个internet连接 
	HINTERNET internet=InternetOpen("HTTP Downloader", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL); 
	if(!internet) 
		throw "InternetOpen error!"; 
	if(!internet) throw "InternetOpen error!"; //打开一个http url地址 
	HINTERNET file_handle = InternetOpenUrl(internet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0); 
	if(!file_handle) 
		throw "InternetOpenUrl error! - Maybe you should add Http:// or Ftp://"; //从url地址中读取文件内容到缓冲区buffer 
	BOOL b = InternetReadFile(file_handle, buffer, 100000, &bytes_read); 
	if(!b) 
		throw "InternetReadFile error!"; 
	buffer[bytes_read] = 0; 
	cout << buffer << endl << endl; //关闭连接 
	InternetCloseHandle(internet); 
}