#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<map>
#include<vector>

#define CONFIG_FILE_NAME "gui-config.json"
#define CONFIG_TEMPLATE_FILE_NAME "gui-config-template.json"

class Configer
{
	rapidjson::Document document;
public:
	bool SetTemplate();
	bool SetTemplate(std::string);
	bool GenerateNew(std::vector<std::map<std::string,std::string>>& new_data);
	static void Parse_ID_List(std::string,std::vector<std::map<std::string, std::string>>& new_data);
};

bool Configer::SetTemplate()
{
	 //从硬盘中载入模板
	std::ifstream ifs(CONFIG_TEMPLATE_FILE_NAME, std::ios::in);
	if (!ifs)
		std::cerr << "ERROR:Can't open config file,make sure there are" << CONFIG_TEMPLATE_FILE_NAME << "in the same directory." << std::endl;
	std::stringstream string_buffer;
	string_buffer << ifs.rdbuf();
	ifs.close();
	std::string contents(string_buffer.str());
	return SetTemplate(contents);
}

bool Configer::SetTemplate(std::string contents)
{
	document.Parse(contents.c_str());
	return document.IsObject();
		
}


void Configer::Parse_ID_List(std::string id_list_string,std::vector<std::map<std::string, std::string>>& template_data)
{
	rapidjson::Document id_list;
	id_list.Parse(id_list_string.c_str());
	assert(id_list.IsObject());
	rapidjson::Value& configs = id_list["configs"];
	assert(configs.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = configs.Begin();itr != configs.End(); ++itr)
	{
		std::map<std::string, std::string> temp_map;
		for (rapidjson::Value::ConstMemberIterator mitr = itr->MemberBegin(); mitr != itr->MemberEnd(); mitr++)
		{	
			temp_map.insert({ mitr->name.GetString(),mitr->value.GetString()});
		}
		template_data.push_back(temp_map);
		
	}
}


bool Configer::GenerateNew(std::vector<std::map<std::string, std::string>>& new_data)
{
	
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	if (!document.IsObject())
	{
		std::cerr << "Sorry,an expectable error occured,please try again" << std::endl;
		return false;
	}
	rapidjson::Value& configs = document["configs"];

	//assert(configs.IsArray());
	configs.Clear();
	
		for (std::vector<std::map<std::string, std::string >>::iterator it=new_data.begin(); it!=new_data.end(); it++)
		{
			std::map<std::string, std::string>& info = *it;
			rapidjson::Value item(rapidjson::Type::kObjectType);
			rapidjson::Value temp_value(rapidjson::kStringType);
			
			temp_value.SetString(rapidjson::StringRef(info["ip"].c_str()));
			item.AddMember("server",temp_value, document.GetAllocator());
			
			temp_value.SetString(rapidjson::StringRef(info["port"].c_str()));
			item.AddMember("server_port", temp_value,document.GetAllocator());
			
			temp_value.SetString(rapidjson::StringRef(info["password"].c_str()));
			item.AddMember("password", temp_value, document.GetAllocator());
			
			item.AddMember("method", "aes-256-cfb", document.GetAllocator());
			item.AddMember("remarks", "", document.GetAllocator());
			item.AddMember("auth", "false", document.GetAllocator());
			item.AddMember("timeout", "5", document.GetAllocator());
			configs.PushBack(item, document.GetAllocator());
		
		}
		
		document.Accept(writer);
		// Output {"project":"rapidjson","stars":11}
		std::ofstream ofs(CONFIG_FILE_NAME, std::ios::trunc);
		if (!ofs)
			std::cerr << "Generate config file fail" << std::endl;
		ofs << buffer.GetString();
		ofs.close();
		return true;
}
