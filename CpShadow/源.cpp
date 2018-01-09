// rapidjson/example/simpledom/simpledom.cpp`
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <cassert>
#include <iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

#define CONFIG_FILE_NAME "gui-config.json"
#define CONFIG_TEMPLATE_FILE_NAME "gui-config-template.json"

using namespace rapidjson;
int main() {
	// 1. 把 JSON 解析至 DOM。
	ifstream ifs(CONFIG_TEMPLATE_FILE_NAME,ios::in);
	if (!ifs)
		cerr << "ERROR:Can't open config file,make sure there are" <<CONFIG_TEMPLATE_FILE_NAME<<"in the same directory."<< endl;
	std::stringstream string_buffer;
	string_buffer << ifs.rdbuf();
	ifs.close();
	std::string contents(string_buffer.str());
	cout << contents << endl;

	Document document;
	document.Parse(contents.c_str());
	 
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	try 
	{
		rapidjson::Value& configs = document["configs"];
		assert(configs.IsArray());
		configs.Clear();
		for (int i = 0; i < 10; i++)
		{
			Value item(Type::kObjectType);
			string str = "c.isxb.bid";
			str += to_string(i);
			Value server(Type::kStringType);
			server.SetString(StringRef(str.c_str()));
			item.AddMember("sever", server, document.GetAllocator());
			item.AddMember("server_port", "18008", document.GetAllocator());
			item.AddMember("password", "12121691", document.GetAllocator());
			item.AddMember("method","aes-256-cfb", document.GetAllocator());
			item.AddMember("remarks","", document.GetAllocator());	
			item.AddMember("auth","false", document.GetAllocator());		
			item.AddMember("timeout","5", document.GetAllocator());		
			configs.PushBack(item, document.GetAllocator());
		}
		document.Accept(writer);
		// Output {"project":"rapidjson","stars":11}
		ofstream ofs(CONFIG_FILE_NAME, ios::trunc);
		if(!ofs)
			cerr<<"Generate config file fail"<<endl;
		ofs << buffer.GetString();
		ofs.close();
	}catch (exception e)
	{
	
	}
	


	// 2. 利用 DOM 作出修改。
	/*
	Value& s = document["stars"];
	s.SetInt(s.GetInt() + 1);
	
	// 3. 把 DOM 转换（stringify）成 JSON。
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	document.Accept(writer);
	// Output {"project":"rapidjson","stars":11}
	std::cout << buffer.GetString() << std::endl;
	*/
	ifs.close();
	system("pause");
	return 0;
}