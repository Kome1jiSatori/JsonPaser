#include "Parse.h"
#include <stdexcept>

Parse::Parse() :m_str(""), m_idx(0) {

}

void Parse::load(const std::string& str) {
	m_str = str;
	m_idx = 0;
}

void Parse::skip_white_space() {
	while (m_str[m_idx] == ' ' || m_str[m_idx] == '\n' || m_str[m_idx] == '\r' || m_str[m_idx] == '\t')
		m_idx++;
}

char Parse::get_next_token() {
	skip_white_space();
	return m_str[m_idx++];
}

KJson Parse::parse() {
	char ch = get_next_token();
	switch (ch) {
	case 'n':
		m_idx--;
		return parse_null();
	case 't':
	case 'f':
		m_idx--;
		return parse_bool();
	case '-':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		m_idx--;
		return parse_number();
	case '"':
		return KJson(parse_string());
	case '[':
		return parse_array();
	case '{':
		return parse_object();
	default:
		break;
	}
	
}

KJson Parse::parse_null() {
	if (m_str.compare(m_idx, 4, "null") == 0) { // 从m_idx开始匹配4个字符
		m_idx += 4;
		return KJson();
	}
	throw new std::logic_error("parse null error");
}
KJson Parse::parse_bool() {
	if (m_str.compare(m_idx, 4, "true") == 0) {
		m_idx += 4;
		return KJson(true);
	}
	else if (m_str.compare(m_idx, 5, "false") == 0) {
		m_idx += 5;
		return KJson(false);
	}
	throw new std::logic_error("parse bool error");
}
KJson Parse::parse_number() {
	int pos = m_idx;
	if (m_str[m_idx] == '-') m_idx++;

	if (m_str[m_idx] < '0' || m_str[m_idx] > '9')
		throw new std::logic_error("parse number error");
	while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9')
		m_idx++;

	if (m_str[m_idx] != '.') {
		int i = std::atoi(m_str.c_str() + pos);
		return KJson(i);
	}
	m_idx++;
	if (m_str[m_idx] < '0' || m_str[m_idx] > '9')
		throw new std::logic_error("parse number error");
	while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9')
		m_idx++;

	double f = std::atof(m_str.c_str() + pos);
	return KJson(f);
}
std::string Parse::parse_string() {
	std::string out;
	while (true) {
		char ch = m_str[m_idx++];
		if (ch == '"') {
			break;
		}
		if (ch == '\\') {
			ch = m_str[m_idx++];
			switch (ch)
			{
			case '\n':
				out += '\n';
				break;
			case '\r':
				out += '\r';
				break;
			case '\t':
				out += '\t';
				break;
			case '\b':
				out += '\b';
				break;
			case '\f':
				out += '\f';
				break;
			case '"':
				out += "\\\"";
				break;
			case '\\':
				out += "\\\\";
				break;
			case 'u':
				out += "\\u";
				for (int i = 0; i < 4; i++) {
					out += m_str[m_idx++];
				}
				break;
			default:
				break;
			}
		}
		else {
			out += ch;
		}
	}
	return out;
}
KJson Parse::parse_array() {
	KJson arr(KJson::json_array);
	char ch = get_next_token();
	if (ch == ']') {
		return arr;
	}
	m_idx--;
	while (true) {
		arr.append(parse());
		ch = get_next_token();
		if (ch == ']')
			break;
		if (ch != ',') {
			throw new std::logic_error("parse array error");
		}
		m_idx++;
	}
	return arr;
}
KJson Parse::parse_object() {
	KJson obj(KJson::json_object);
	char ch = get_next_token();
	if (ch == '}')
		return obj;
	m_idx--;
	while (true) {
		ch = get_next_token();
		if (ch != '"') {
			throw new std::logic_error("parse object error");
		}
		std::string key = parse_string();
		ch = get_next_token();
		if (ch != ':') {
			throw new std::logic_error("parse object error");
		}
		obj[key] = parse();
		ch = get_next_token();
		if (ch == '}') break;
		if (ch != ',') {
			throw new std::logic_error("parse object error");
		}
		m_idx++;
	}
	return obj;
}