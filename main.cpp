#include<iostream>
#include "KJson.h"
#include<fstream>
#include<sstream>


KJson input_json() {
	std::string json_path;
	std::cout << "Please give the json file path:" << std::endl;
	std::cin >> json_path;
	// 读入json文件
	std::ifstream fin("./input_json.json");
	std::stringstream ss;
	ss << fin.rdbuf();
	const std::string& str = ss.str();

	std::string inj = str;

	// 删去注释
	int index = 0;
	while (index < inj.length() - 1) {
		if (inj[index] == '/' && inj[index + 1] == '/') {
			while (inj[index] != '\n' && index < inj.length()) {
				inj[index] = ' ';
				index++;
			}
		}
		else index++;
	}

	KJson res;
	res.parse(inj);
	return res;
}

void query_json(KJson jf) {
	std::string query;

}


int main() {
	KJson jf = input_json();
	std::cout << "success!" << std::endl;
	// std::cout << jf.str() << std::endl;
	// test();

	int order;
	std::string save_pth;
	std::cout << "Please give instructions" << std::endl;
	std::cout << "Enter 1 to make a query" << std::endl;
	std::cout << "Enter 2 to update json" << std::endl;
	std::cout << "Input 3 to output json file" << std::endl;
	std::cout << "Enter 0 to exit" << std::endl;

	while (std::cin >> order) {
		switch (order) {
		case 0:
			exit(0);
			break;
		case 1:
			std::cout << jf.query() << std::endl;
			std::cout << "complete!" << std::endl;
			break;
		case 2:
			jf.update();
			break;
		case 3:
			std::cout << "Please enter the save path" << std::endl;
			std::cin >> save_pth;
			jf.save_json(save_pth);
			std::cout << "complete" << std::endl;
			break;
		default:
			break;
		}
		std::cout << "Please give instructions" << std::endl;
		std::cout << "Enter 1 to make a query" << std::endl;
		std::cout << "Enter 2 to update json" << std::endl;
		std::cout << "Input 3 to output json file" << std::endl;
		std::cout << "Enter 0 to exit" << std::endl;
	}

	return 0;
}

