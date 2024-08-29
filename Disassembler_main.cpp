//Disassembler Main

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

int main(int argc, char const* argv[])
{
	//Create dissambler object
	Dissassembler A;
	int linecount = 0;

	//takes two inputs from command console
	if (argc == 2)
	{
		ifstream inputFile(argv[1]);

		//if file is found
		if (!inputFile)
		{
			cout << "File not found";
			return -1;
		}

		//loops through each line of inputted text file
		else
		{
			while (getline(inputFile, A.hexValue))
			{
				A.disassmebly(linecount);
				linecount++;
			}
		}

		//If errors are not found
		if (A.invalid.empty())
		{
			//finds outputs file based on input file
			string filename = argv[1];
			string outputName;
			outputName.append(filename, 0, filename.size() - 4);
			string FileOutput = outputName + ".s";
			ofstream out(FileOutput);

			//Outputs strings for .s file
			out << ".globl     main" << endl;
			out << ".data" << endl;
			out << ".text" << endl << endl;
			out << "main:" << endl << endl;

			//if a branch is found
			if (!A.address_file.empty())
			{
				//puts branch instruction in position address
				for (auto k : A.address_file)
				{
					A.functionReturn[k.second] = k.first;
				}

				//Inserts branch into instruction list
				int check = 0;
				for (auto k : A.functionReturn)
				{
					auto begin = A.valid.begin();
					A.valid.insert(begin + check + k.first, k.second);
					check++;
				}
			}
			//Outs each instruction in output file
			for (int i = 0; i < A.valid.size(); i++)
			{
				out << A.valid[i] << endl;
			}

			//inserts footer
			out << endl << "     syscall" << endl;
		}
		//outputs error messages if any into console
		else
		{
			for (int i = 0; i < A.invalid.size(); i++)
			{
				cout << A.invalid[i] << endl;
			}
		}
	}
}