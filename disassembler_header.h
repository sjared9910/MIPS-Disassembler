//Disassembler Header

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <utility>

using namespace std;

class Dissassembler
{
	public:
		//Variables
		string hexValue;
		vector <string> valid;
		vector <string> invalid;
		map <string, int> address_file;
		map <int, string> functionReturn;
	
		//Methods
		Dissassembler();
		string BinaryToRegister(string binary);
		string HexToBinary(string hexValue);
		void disassmebly(int linecount);
		char Type(string check);
		void RType(string input);
		void IType(string input, int linecount);
		string IntToHex(int value);
		string signedNumber(string value);
		string complement(string value);
};


