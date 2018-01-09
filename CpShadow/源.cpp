#include<iostream>
#include <string>
#include <ParserDom.h>

#include <winsock2.h>  
#pragma comment(lib, "ws2_32.lib")  

using namespace std;
using namespace htmlcxx;



int main()

{
	//解析一段Html代码

	string html = "<html><body><p>hey</p><a href=\"nihao\">sss</a></body></html>";

	HTML::ParserDom parser;

	tree<HTML::Node> dom = parser.parseTree(html);

	//输出整棵DOM树

	cout << dom << endl;

	//输出树中所有的超链接节点

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

	//输出所有的文本节点

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