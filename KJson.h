#pragma once

#include<string>
#include<vector>
#include<map>

class KJson
{
public:
	enum Type { // json��ö������
		json_null = 0,
		json_bool,
		json_int,
		json_double,
		json_string,
		json_array,
		json_object
	};

	KJson();
	KJson(bool value);
	KJson(int value);
	KJson(double value);
	KJson(const char* value);
	KJson(const std::string &value);
	KJson(Type type);

	// �������
	KJson& operator [](int index);
	void append(const KJson& other);

	KJson& operator [] (const char* key);
	KJson& operator [] (const std::string& key);

	void clear();

	// ���ַ�����ʽ����json����
	std::string str() const;
	std::string query() const;
	
	typedef std::vector<KJson>::iterator iterator;
	iterator begin() {
		return m_value.m_array->begin();
	}
	iterator end() {
		return m_value.m_array->end();
	}

	void remove(int index);
	void remove(const char* key);
	void remove(const std::string& key);

	void parse(const std::string& str);

	void update();
	void delete_json();
	void insert_json();
	KJson create_json();
	void save_json(std::string save_pth);

private:
	union Value { // json��value������
		bool m_bool;
		int m_int;
		double m_double;
		std::string* m_string;
		std::vector<KJson>* m_array;
		std::map<std::string, KJson>* m_object;
	};

	Type m_type; // json����
	Value m_value; // json��ֵ
};

