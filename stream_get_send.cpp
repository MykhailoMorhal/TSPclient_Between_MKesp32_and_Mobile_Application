/*----------------------------------------------------------------------
                                Include
*----------------------------------------------------------------------*/
#include <fstream>
#include <iostream>
#include <string>

/*----------------------------------------------------------------------
                                Defines
*----------------------------------------------------------------------*/
#define	ESP_BUFF_SIZE							10u

void write_data_to_file(char *arg, const std::string path)
{
	std::ofstream fout;
	fout.open(path);
	 
	if (!fout.is_open()) 
	{
		std::cout << "Don't open file!\n"; 
	}
	else
	{
		fout << arg; 	// writte to txt
		fout.close(); 
	}
}

std::string get_data_from_file(const std::string path)
{
	std::ifstream fout;
	fout.open(path);
	if (!fout.is_open()) 
	{
		std::cout << "Don't open file!\n"; 
	}
	else
	{
		char buff[ESP_BUFF_SIZE];
		fout.getline(buff, ESP_BUFF_SIZE);
		std::string str = buff;
		fout.close();
		return str;
	}
}