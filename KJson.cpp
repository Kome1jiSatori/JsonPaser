#include "KJson.h"
#include <stdexcept>
#include<sstream>
#include "Parse.h"
#include<iostream>
#include<fstream>

KJson::KJson() : m_type(json_null){

}

KJson::KJson(bool value) : m_type(json_bool) {
	m_value.m_bool = value;
}

KJson::KJson(int value) : m_type(json_int) {
	m_value.m_int = value;
}

KJson::KJson(double value) : m_type(json_double) {
	m_value.m_double = value;
}

KJson::KJson(const char* value) : m_type(json_string) {
	m_value.m_string = new std::string(value);
}

KJson::KJson(const std::string& value) : m_type(json_string) {
	m_value.m_string = new std::string(value);
}
KJson::KJson(Type type) : m_type(type) {
	switch (m_type) {
	case json_null:
		break;
	case json_bool:
		m_value.m_bool = false;
		break;
	case json_int:
		m_value.m_int = 0;
		break;
	case json_double:
		m_value.m_double = 0.0;
		break;
	case json_string:
		m_value.m_string = new std::string("");
		break;
	case json_array:
		m_value.m_array = new std::vector<KJson>();
		break;
	case json_object:
		m_value.m_object = new std::map<std::string, KJson>();
		break;
	default:
		break;
	}
}


KJson& KJson::operator [](int index) {
	if (m_type != json_array) {
		m_type = json_array;
		m_value.m_array = new std::vector<KJson>();
	}
	if (index < 0) {
		throw new std::logic_error("array index < 0");
	}
	int size = m_value.m_array->size();
	if (index >= size) { // 索引大于数组大小则扩容
		for (int i = size; i <= index; i++)
			m_value.m_array->push_back(KJson());
	}
	return m_value.m_array->at(index);
}


void KJson::append(const KJson& other) {
	if (m_type != json_array) { // 调用append要求类型是数组，若不是则转化
		clear();
		m_type = json_array;
		m_value.m_array = new std::vector<KJson>();
	}
	(m_value.m_array)->push_back(other);
}

std::string KJson::str() const {
	std::stringstream ss;
	switch (m_type) {
	case json_null:
		ss << "null";
		break;
	case json_bool:
		if (m_value.m_bool)
			ss << "true";
		else
			ss << "false";
		break;
	case json_int:
		ss << m_value.m_int;
		break;
	case json_double:
		ss << m_value.m_double;
		break;

	case json_string:
		ss << '\"' << *(m_value.m_string) << '\"';
		break;
	case json_array:
		ss << '[';
		for (auto t = m_value.m_array->begin(); t != m_value.m_array->end(); t++) {
			if (t != m_value.m_array->begin())
				ss << ',';
			ss << t->str();
		}
		ss << ']';
		break;

	case json_object:
		ss << '{';
		for (auto t = m_value.m_object->begin(); t != m_value.m_object->end(); t++) {
			if (t != m_value.m_object->begin())
				ss << ',';
			ss << '\"' << t->first << '\"' << ':' << t->second.str();
		}
		ss << '}';
		break;
	default:
		break;
	}
	return ss.str();
}

KJson& KJson::operator [] (const char* key) {
	std::string name(key);
	return (*(this))[name];
}
KJson& KJson::operator [] (const std::string& key) {
	if (m_type != json_object) {
		clear();
		m_type = json_object;
		m_value.m_object = new std::map<std::string, KJson>;
	}
	return (*(m_value.m_object))[key];
}

void KJson::clear() {
	switch (m_type) {
	case json_null:
		break;
	case json_bool:
		m_value.m_bool = false;
		break;
	case json_int:
		m_value.m_int = 0;
		break;
	case json_double:
		m_value.m_double = 0.0;
		break;
	case json_string:
		delete m_value.m_string;
		break;
	case json_array:
		for (auto t = m_value.m_array->begin(); t != m_value.m_array->end(); t++)
			t->clear();
		delete m_value.m_array;
		break;
	case json_object:
		for (auto t = m_value.m_object->begin(); t != m_value.m_object->end(); t++)
			t->second.clear();
		delete m_value.m_array;
		break;
	default:
		break;
	}
	m_type = json_null;
}

void KJson::remove(int index) {
	if (index < 0 || index >= m_value.m_array -> size()) {
		std::cout << "Out of bounds!" << std::endl;
		return;
	}
	else m_value.m_array->erase(m_value.m_array->begin() + index);
}

void KJson::remove(const char* key) {
	std::string name(key);
	remove(name);
}
void KJson::remove(const std::string& key) {
	auto t = m_value.m_object->find(key);
	if (t == (m_value.m_object)->end()) {
		std::cout << "key error!" << std::endl;
		return;
	}
	(*(m_value.m_object))[key].clear();
	(m_value.m_object)->erase(key);
}

void KJson::parse(const std::string& str) {
	Parse p;
	p.load(str);
	*this = p.parse();
}

std::string KJson::query() const {
	std::stringstream ss;
	std::string qk;
	int x;
	switch (m_type) {
	case json_null:
		ss << "null";
		return ss.str();
		break;
	case json_bool:
		if (m_value.m_bool) {
			ss << "true";
			return ss.str();
		}
		else {
			ss << "false";
			return ss.str();
		}
		break;
	case json_int:
		ss << m_value.m_int;
		return ss.str();
		break;
	case json_double:
		ss << m_value.m_double;
		return ss.str();
		break;
	case json_string:
		ss << '\"' << *(m_value.m_string) << '\"';
		return ss.str();
		break;
	case json_array:

		std::cout << "The array size is " << m_value.m_array->size() << std::endl;

		while (std::cin >> x) {
			if (x > m_value.m_array->size() || x < 0) std::cout << "Out of bounds" << std::endl;
			else
				return m_value.m_array->at(x - 1).query();
		}
		for (auto t = m_value.m_array->begin(); t != m_value.m_array->end(); t++) {
			if (t != m_value.m_array->begin())
				ss << ',';
			ss << t->str();
		}
		return ss.str();
		break;

	case json_object:
		std::cout << "The keys available for query are" << std::endl;
		for (auto t = m_value.m_object->begin(); t != m_value.m_object->end(); t++) {
			if (t != m_value.m_object->begin())
				std::cout << ',';
			std::cout << '\"' << t->first << '\"' ;
		}
		std::cout << std::endl;
		while (std::cin >> qk) {
			if (m_value.m_object->find(qk) == m_value.m_object->end()) {
				std::cout << "Thers is no key: " << qk << ", please re-enter:" << std::endl;
			}
			else {
				return m_value.m_object->find(qk)->second.query();
			}
		}
		return ss.str();
		break;
	default:
		break;
	}
	return ss.str();
}

void KJson::delete_json() {
	if (m_type == json_array) {
		int index;
		std::cout << "Please enter the index you want to delete:" << std::endl;
		std::cin >> index;
		int arr_order;
		std::cout << "Do you want to delete the current item or get into the array?" << std::endl;
		std::cout << "Enter 1 to delete and Enter 0 to get into the array" << std::endl;
		std::cin >> arr_order;
		switch(arr_order) {
		case 1:
			remove(index);
			break;
		case 0:
			m_value.m_array->at(index).delete_json();
			break;
		default:
			std::cout << "Wrong order!" << std::endl;
		}
	}
	else if (m_type == json_object) {
		std::string key;
		std::cout << "Please enter the key you want to delete:" << std::endl;
		std::cin >> key;
		int obj_order;
		std::cout << "Do you want to delete the current item or get into the object?" << std::endl;
		std::cout << "Enter 1 to delete and Enter 0 to get into the object" << std::endl;
		std::cin >> obj_order;
		switch (obj_order) {
		case 1:
			remove(key);
			break;
		case 0:
			m_value.m_object ->find(key)->second.delete_json();
			break;
		default:
			std::cout << "Wrong order!" << std::endl;
		}
	}
	else {
		std::cout << "type error!" << std::endl;
		return;
	}
}

KJson KJson::create_json() {
	std::cout << "Please choose the type you want to insert" << std::endl;
	std::cout << "Enter 1 to insert null" << std::endl;
	std::cout << "Enter 2 to insert bool" << std::endl;
	std::cout << "Enter 3 to insert number" << std::endl;
	std::cout << "Enter 4 to insert string" << std::endl;
	std::cout << "Enter 5 to insert array" << std::endl;
	std::cout << "Enter 6 to insert object" << std::endl;
	int type;
	std::string content;
	int number;
	std::cin >> type;
	switch (type) {
	case 1:
		return new KJson();
		break;
	case 2:
		std::cout << "Enter false or true" << std::endl;
		std::cin >> content;
		if (content == "false") return new KJson(false);
		else if (content == "true") return new KJson(true);
		else std::cout << "error" << std::endl;
		break;
	case 3:
		std::cout << "Enter the number" << std::endl;
		std::cin >> number;
		return new KJson(number);
		break;
	case 4:
		std::cout << "Enter the content" << std::endl;
		std::cin >> content;
		return new KJson(content);
		break;
	case 5:
		return new KJson(json_array);
		break;
	case 6:
		return new KJson(json_object);
		break;
	default:
		break;
	}
}

void KJson::insert_json() {
	if (m_type == json_array) {
		int arr_order;
		int index;
		std::cout << "Do you want to insert the item at current index or get into the array?" << std::endl;
		std::cout << "Enter 1 to insert and Enter 0 to get into the array" << std::endl;
		std::cin >> arr_order;
		switch (arr_order) {
		case 1:
			append(create_json());
			std::cout << "success!" << std::endl;
			break;
		case 0:
			std::cout << "Please enter the index you want to insert into:" << std::endl;
			std::cin >> index;
			m_value.m_array->at(index).insert_json();
			std::cout << "success!" << std::endl;
			break;
		default:
			std::cout << "Wrong order!" << std::endl;
		}
	}
	else if (m_type == json_object) {
		std::string key;
		int obj_order;
		std::string nk;
		std::cout << "Do you want to insert the item at current index or get into the object?" << std::endl;
		std::cout << "Enter 1 to insert and Enter 0 to get into the object" << std::endl;
		std::cin >> obj_order;
		switch (obj_order) {
		case 1:
			std::cout << "Please enter the key" << std::endl;
			std::cin >> nk;
			(*(m_value.m_object))[nk] = create_json();
			std::cout << "success!" << std::endl;
			break;
		case 0:
			std::cout << "Please enter the key you want to insert into:" << std::endl;
			std::cin >> key;
			m_value.m_object->find(key)->second.insert_json();
			break;
		default:
			std::cout << "Wrong order!" << std::endl;
		}
	}
	else {
		std::cout << "type error!" << std::endl;
		return;
	}
}


void KJson::update() {
	std::cout << "Please give instructions" << std::endl;
	std::cout << "Enter 1 to delete" << std::endl;
	std::cout << "Enter 2 to insert" << std::endl;
	std::cout << "Enter 0 to exit" << std::endl;
	int order;
	while (std::cin >> order) {
		switch (order) {
		case 1:
			delete_json();
			break;
		case 2:
			insert_json();
			break;
		case 0:
			return;
			break;
		default:
			break;
		}
	std::cout << "Please give instructions" << std::endl;
	std::cout << "Enter 1 to delete" << std::endl;
	std::cout << "Enter 2 to insert" << std::endl;
	std::cout << "Enter 0 to exit" << std::endl;
	}
}

void KJson::save_json(std::string save_pth) {
	std::ofstream out_json;
	out_json.open(save_pth);
	out_json << str();
	out_json.close();
}