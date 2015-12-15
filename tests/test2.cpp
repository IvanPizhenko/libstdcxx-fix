#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <memory>

int main()
{
	std::unique_ptr<std::string> pstr = std::make_unique<std::string>("Hello, World!");
	std::cout << *pstr << std::endl;
	
	std::tm tmb1;
	time_t t = std::time(NULL);
	tmb1 = *std::localtime(&t);
	
	const char* format = "%Y-%m-%d %H:%M:%S";
	
	std::ostringstream oss;
	oss << std::put_time<char>(&tmb1, format);
	std::string stm = oss.str();
	
	std::cout << stm << std::endl;

	std::tm tmb2;
	std::istringstream iss(stm);
        std::cout << "Reading date from string: " << stm << " ..." << std::endl;
	iss >> std::get_time<char>(&tmb2, format);
	
	bool match = memcmp(&tmb1, &tmb1, sizeof(tmb1)) == 0;
	
	std::cout << "obtained and expected " << (match ? "match" : "don't match") << std::endl;
	
	return 0;
}

