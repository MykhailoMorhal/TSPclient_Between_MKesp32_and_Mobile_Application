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
	setlocale(LC_ALL, "rus"); // ���������� ����������� ���������
	char buff[50]; // ����� �������������� �������� ������������ �� ����� ������
	ifstream fin("cppstudio.txt"); // ������� ���� ��� ������

	//fin >> buff; // ������� ������ ����� �� �����
	//cout << buff << endl; // ���������� ��� �����

	fin.getline(buff, 50); // ������� ������ �� �����
	fin.close(); // ��������� ����
	cout << buff << endl; // ���������� ��� ������
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
		cout << "Not open file!\n"; // �������� �� ����
	}
	char buff[10]; // ����� �������������� �������� ������������ �� ����� ������
	fout.getline(buff, 10);
	string str = buff;
	fout.close();
	return str;
}