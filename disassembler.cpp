//Disassembler

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <utility>

#include "disassembler_header.h"

using namespace std;

//Constructor
Dissassembler::Dissassembler() {}

/*
	Breaks down Hex input into binaryand determines if the input line is an
	R Type or I Type input. Pushes an error if hex entered is invalid.
*/
void Dissassembler::disassmebly(int linecount)
{
	//checks input hex size
	if (hexValue.size() == 8)
	{
		string binary = HexToBinary(hexValue);
		char type = Type(binary);

		//checks function type and runs different function depending
		if (type == 'R') RType(binary);
		if (type == 'I') IType(binary, linecount);
	}
	//binary doesnt work
	else invalid.push_back("invalid hex string enter");
}

//Function to convert hex input string into a binary string output
string Dissassembler::HexToBinary(string input)
{
	string binary;
	//sets binary based on each hex character
	for (int i = 0; i < 8; i++)
	{
		switch (input[i])
		{
			case '0':
				binary.append("0000");
				break;
			case '1':
				binary.append("0001");
				break;
			case '2':
				binary.append("0010");
				break;
			case '3':
				binary.append("0011");
				break;
			case '4':
				binary.append("0100");
				break;
			case '5':
				binary.append("0101");
				break;
			case '6':
				binary.append("0110");
				break;
			case '7':
				binary.append("0111");
				break;
			case '8':
				binary.append("1000");
				break;
			case '9':
				binary.append("1001");
				break;
			case 'A':
			case 'a':
				binary.append("1010");
				break;
			case 'B':
			case 'b':
				binary.append("1011");
				break;
			case 'C':
			case 'c':
				binary.append("1100");
				break;
			case 'D':
			case 'd':
				binary.append("1101");
				break;
			case 'E':
			case 'e':
				binary.append("1110");
				break;
			case 'F':
			case 'f':
				binary.append("1111");
				break;
			default:
				cout << "Invalid Hex";
		}
	}
	return binary;
}

//check to see if binary input is an R type or I type
char Dissassembler::Type(string check)
{
	string opcode;
	//sets opcode string
	opcode = check.substr(0, 6);

	if (opcode == "000000") return 'R';
	else return 'I';
}

//If the function is an R Type
void Dissassembler::RType(string input)
{
	string functionReturn, RS, RT, RD, finalOut, shamt;
	string function = input.substr(26, 6);

	//valid function code check
	bool check = true;

	//checks function code of last 6 bits from binary input
	if (function == "100000") functionReturn = "add";
	else if (function == "100001") functionReturn = "addu";
	else if (function == "100100") functionReturn = "and";
	else if (function == "100111") functionReturn = "nor";
	else if (function == "100101") functionReturn = "or";
	else if (function == "101010") functionReturn = "slt";
	else if (function == "101011") functionReturn = "sltu";
	else if (function == "000000") functionReturn = "sll";
	else if (function == "000010") functionReturn = "slr";
	else if (function == "100010") functionReturn = "sub";
	else if (function == "100011") functionReturn = "subu";

	//if the function code doesnt match any function string
	else
	{
		invalid.push_back("unknown function code");
		check = false;
	}

	//if check is true then there is no error
	if (check)
	{
		//if function is sll or slr then a function is needed using shamt
		if (functionReturn == "sll" || functionReturn == "slr")
		{
			string shamt = input.substr(21, 5);
			string rt = input.substr(11, 5);
			string rd = input.substr(16, 5);

			RT = BinaryToRegister(rt);
			RD = BinaryToRegister(rd);
			int number = stoi(shamt, nullptr, 2);
			shamt = to_string(number);

			//pushes final values
			finalOut.append("     " + functionReturn + " " + RD + " " + RT + " " + shamt);
			valid.push_back(finalOut);
		}
		//any other R-type follows normal format
		else
		{
			string rs = input.substr(6, 5);
			string rt = input.substr(11, 5);
			string rd = input.substr(16, 5);

			//Converts the binary to corresponding register value
			RS = BinaryToRegister(rs);
			RT = BinaryToRegister(rt);
			RD = BinaryToRegister(rd);

			//pushes final values
			finalOut.append("     " + functionReturn + " " + RD + " " + RS + " " + RT);
			valid.push_back(finalOut);
		}
	}
}

//If the function type is an I Type
void Dissassembler::IType(string input, int linecount)
{
	string opcode = input.substr(0, 6);
	string functionReturn , RS, RT, finalOut;

	//valid function code check
	bool check = true;

	//checks function code of first 6 bits from binary input
	if (opcode == "001000") functionReturn = "addi";
	else if (opcode == "001001") functionReturn = "addiu";
	else if (opcode == "001100") functionReturn = "andi";
	else if (opcode == "000100") functionReturn = "beq";
	else if (opcode == "000101") functionReturn = "bne";
	else if (opcode == "100100") functionReturn = "ibu";
	else if (opcode == "100101") functionReturn = "ihu";
	else if (opcode == "110000") functionReturn = "ll";
	else if (opcode == "001111") functionReturn = "lui";
	else if (opcode == "100011") functionReturn = "lw";
	else if (opcode == "001101") functionReturn = "ori";
	else if (opcode == "001010") functionReturn = "slti";
	else if (opcode == "001011") functionReturn = "sltui";
	else if (opcode == "101000") functionReturn = "sb";
	else if (opcode == "111000") functionReturn = "sc";
	else if (opcode == "101001") functionReturn = "sh";
	else if (opcode == "101011") functionReturn = "sw";

	//if invalid opcode function
	else
	{
		invalid.push_back("unknown function code");
		check = false;
	}

	//if valid opcode
	if (check)
	{
		//sw and lw require a different format
		if (functionReturn == "sw" || functionReturn == "lw")
		{
			string rs = input.substr(6, 5);
			string rt = input.substr(11, 5);
			string imm = input.substr(16, 16);
			RS = BinaryToRegister(rs);
			RT = BinaryToRegister(rt);

			int immediate = stoi(imm, nullptr, 2);
			imm = IntToHex(immediate);

			//Pushes final values
			finalOut.append("     " + functionReturn + " " + RT + ", " + "0x" + imm + "(" + RS + ")");
			valid.push_back(finalOut);
		}
		//if I-type is branch instruction
		else if (functionReturn == "beq" || functionReturn == "bne")
		{
			string rs = input.substr(6, 5);
			string rt = input.substr(11, 5);
			string imm = input.substr(16, 16);
			RS = BinaryToRegister(rs);
			RT = BinaryToRegister(rt);

			//finds position of branch position
			int position = stoi(signedNumber(imm));
			position = position + linecount + 1;
			int address = position * 4;

			//Branch instruction output
			finalOut.append("     " + functionReturn + " " + RT + ", " + RS + ", ");
			string addressLocation = "Addr_" + IntToHex(address);
			finalOut.append(addressLocation);
			
			//adds address location
			pair <string, int> function(addressLocation, position);
			address_file.insert(function);
			valid.push_back(finalOut);
		}
		//Any other I-type format instruction
		else
		{
			string rs = input.substr(6, 5);
			string rt = input.substr(11, 5);
			string imm = input.substr(16, 16);

			RS = BinaryToRegister(rs);
			RT = BinaryToRegister(rt);
			int immediate = stoi(imm, nullptr, 2);
			imm = IntToHex(immediate);

			//Pushes final output
			finalOut.append("     " + functionReturn + " " + RT + ", " + RS + ", " + "0x" + imm);
			valid.push_back(finalOut);
		}
	}
}

//takes decimal input and outputs into 4 hex digits for branch
string Dissassembler::IntToHex(int integer)
{
	stringstream hexValue;
	hexValue << setfill('0') << setw(4) << hex << integer;

	return hexValue.str();
}

//puts binary into into signed int
string Dissassembler::signedNumber(string binary)
{
	string variable;
	if (binary[0] == '1')
	{
		string comp = complement(binary);
		int Numberint = stoi(comp, nullptr, 2) * -1;
		variable = to_string(Numberint);
	}
	else
	{
		int Numberint = stoi(binary, nullptr, 2);
		variable = to_string(Numberint);
	}
	return variable;
}

//takes 2s complement
string Dissassembler::complement(string binary)
{
	string variable;
	unsigned int i = binary.size() - 1;

	while (binary[i] != '1' && i >= 0)
	{
		i--;
	}

	string first = binary.substr(0, i);
	string second = binary.substr(i, binary.size() - i);

	for (auto a : first)
	{
		if (a == '1')
			variable.push_back('0');
		if (a == '0')
			variable.push_back('1');
	}
	variable += second;
	return variable;
}

//finds register based on binary input
string Dissassembler::BinaryToRegister(string binary)
{
	if (binary == "00000") return "$zero";
	else if (binary == "00001") return "$at";
	else if (binary == "00010") return "$v0";
	else if (binary == "00011") return "$v1";
	else if (binary == "00100") return "$a0";
	else if (binary == "00101") return "$a1";
	else if (binary == "00110") return "$a2";
	else if (binary == "00111") return "$a3";
	else if (binary == "01000") return "$t0";
	else if (binary == "01001") return "$t1";
	else if (binary == "01010") return "$t2";
	else if (binary == "01011") return "$t3";
	else if (binary == "01100") return "$t4";
	else if (binary == "01101") return "$t5";
	else if (binary == "01110") return "$t6";
	else if (binary == "01111") return "$t7";
	else if (binary == "10000") return "$s0";
	else if (binary == "10001") return "$s1";
	else if (binary == "10010") return "$s2";
	else if (binary == "10011") return "$s3";
	else if (binary == "10100") return "$s4";
	else if (binary == "10101") return "$s5";
	else if (binary == "10110") return "$s6";
	else if (binary == "10111") return "$s7";
	else if (binary == "11000") return "$t8";
	else if (binary == "11001") return "$t9";
	else if (binary == "11101") return "$sp";
	else if (binary == "11110") return "$fp";
	else if (binary == "11111") return "$ra";
	else
	{
		invalid.push_back("unknown register");
	}
}