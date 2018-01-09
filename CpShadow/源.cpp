#include<iostream>
#include <string>
#include <ParserDom.h>

#include <winsock2.h>  
#pragma comment(lib, "ws2_32.lib")  

using namespace std;
using namespace htmlcxx;



int main()

{
	//����һ��Html����

	string html = "<html><body><p>hey</p><a href=\"nihao\">sss</a></body></html>";

	HTML::ParserDom parser;

	tree<HTML::Node> dom = parser.parseTree(html);

	//�������DOM��

	cout << dom << endl;

	//����������еĳ����ӽڵ�

	tree<HTML::Node>::iterator it = dom.begin();

	tree<HTML::Node>::iterator end = dom.end();

	for (; it != end; ++it)

	{

		if (_stricmp(it->tagName().c_str(), "A") == 0)

		{

			it->parseAttributes();

			cout << it->attribute("href").second << endl;

		}

	}

	//������е��ı��ڵ�

	it = dom.begin();

	end = dom.end();

	for (; it != end; ++it)

	{

		if ((!it->isTag()) && (!it->isComment()))

		{

			cout << it->text();

		}

	}

	cout << endl;
	system("pause");
}