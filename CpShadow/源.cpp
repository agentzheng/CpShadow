#include <windows.h>
#include <Wininet.h>
#include <iostream> 
using namespace std; 
#pragma comment(lib, "Wininet.lib") 

void Download(char url[]); 
int main(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0; char url[1000];//���URL��ַ //�û�����quitʱ�˳� 
	while(cin >> url && strcmp(url, "quit") != 0) 
	{ 
		try {
			cout << "Ŀ��URL��" << url << endl; Download(url); 
		} catch (char* e)//��ʾ������Ϣ 
		{
			cout << e << endl << endl; 
		} 
	} return nRetCode; 
} 
void Download(char url[]) 
{
	char buffer[100000];//�����ļ��Ļ����� 
	DWORD bytes_read;//���ص��ֽ��� //��һ��internet���� 
	HINTERNET internet=InternetOpen("HTTP Downloader", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL); 
	if(!internet) 
		throw "InternetOpen error!"; 
	if(!internet) throw "InternetOpen error!"; //��һ��http url��ַ 
	HINTERNET file_handle = InternetOpenUrl(internet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0); 
	if(!file_handle) 
		throw "InternetOpenUrl error! - Maybe you should add Http:// or Ftp://"; //��url��ַ�ж�ȡ�ļ����ݵ�������buffer 
	BOOL b = InternetReadFile(file_handle, buffer, 100000, &bytes_read); 
	if(!b) 
		throw "InternetReadFile error!"; 
	buffer[bytes_read] = 0; 
	cout << buffer << endl << endl; //�ر����� 
	InternetCloseHandle(internet); 
}