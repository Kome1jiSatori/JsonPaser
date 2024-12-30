#pragma once

#include<string>
#include "KJson.h"

class Parse
{
public:
	Parse();
	void load(const std::string& str);
	KJson parse();

private:

	void skip_white_space();
	char get_next_token();
	KJson parse_null();
	KJson parse_bool();
	KJson parse_number();
	std::string parse_string();
	KJson parse_array();
	KJson parse_object();


	std::string m_str;
	int m_idx;
};

