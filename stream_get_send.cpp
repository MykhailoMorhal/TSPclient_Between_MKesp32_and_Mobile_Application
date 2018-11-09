#include <fstream>
#include <iostream>
#include <string>

using namespace std;

extern string value_sensor1_level1;
extern string value_sensor1_level2;
extern string value_sensor1_level3;
extern string value_sensor1_level4;
 						   
extern string value_sensor2_level1;
extern string value_sensor2_level2;
extern string value_sensor2_level3;
extern string value_sensor2_level4;
						   
extern string value_sensor3_level1;
extern string value_sensor3_level2;
extern string value_sensor3_level3;
extern string value_sensor3_level4;

void read_data()
{
	setlocale(LC_ALL, "rus"); // корректное отображение Кириллицы
	char buff[50]; // буфер промежуточного хранения считываемого из файла текста
	ifstream fin("cppstudio.txt"); // открыли файл для чтения

	//fin >> buff; // считали первое слово из файла
	//cout << buff << endl; // напечатали это слово

	fin.getline(buff, 50); // считали строку из файла
	fin.close(); // закрываем файл
	cout << buff << endl; // напечатали эту строку
}

void write_data_to_txt(char *arg,string path_of_sensor)
{
	ofstream fout;
	fout.open(path_of_sensor);
	 
	if (!fout.is_open()) {
		cout << "Not open file!\n"; 
	}
	fout << arg; // writte to txt
	fout.close(); // close stream
}

string get_data_from_txt(std::string arg) {

	ifstream fout;
	fout.open(arg);
	if (!fout.is_open()) {
		cout << "Not open file!\n"; // сообщить об этом
	}
	char buff[10]; // буфер промежуточного хранения считываемого из файла текста
	fout.getline(buff, 10);
	string str = buff;
	fout.close();
	return str;
}