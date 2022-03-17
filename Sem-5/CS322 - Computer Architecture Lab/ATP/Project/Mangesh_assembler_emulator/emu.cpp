// Name : Chandrawanshi Mangesh Shivaji
// Roll Number : 1801CS16
// Declaration of authorship : I hereby certify that the source code I am submitting is entirely my own original work except, where otherwise indicated.

#include <bits/stdc++.h>
using namespace std;

// ALL GLOBAL VARIABLES AND DATA STRUCTURES USED
const int MAX_SIZE = 100000;
size_t num;
string fileName;
string options;
string mnemonic,operand;
int opcode,value_or_offset;
map < string,char > bin_to_hex;
map < int,string > instructions_without_operand;
map < int,string > instructions_with_one_operand;
map < int,string > instructions_with_pcoffset;
set < pair<int,pair <string,string> > > isa;
vector <string> machine_code(MAX_SIZE);
int machine_code_int[MAX_SIZE];
int instructions_count = 0;
int reg_A, reg_B, stack_pointer, program_counter;

// ALL FUNCTIONS USED
void initialize();
string decimal_to_2scomplement(int data,int no_of_bits);
int string_to_decimal(string s,int no_of_bits);
int string_to_decimal_24(string s);
void execute();

int main(int argc,char** argv)
{
	// Store all instructions with their format specifications
	initialize();

	// Check if proper arguments are provided or not 
	if(argc != 3)
	{
		cout << "Invalid Command, Check Format : ./emu [options] filename.o\n";
		cout << "-trace  show instruction trace\n";
		cout << "-read   show memory reads\n";
		cout << "-write  show memory writes\n";
		cout << "-before show memory dump before execution\n";
		cout << "-after  show memory dump after execution\n";
		cout << "-wipe   wipe written flags before execution\n"; 
		cout << "-isa    display ISA\n";

		return 0;
	}

	// Extract FileName from given arguments
	for(int i=0;i<strlen(argv[2]);i++)
	{
		if(argv[2][i] == '.')
			break;
		fileName += argv[2][i];
	}

	// Extract options from given arguments
	for(int i=0;i<strlen(argv[1]);i++)
	{
		options += argv[1][i];
	}
	

	// Open object file for reading
	FILE* obj_fptr = NULL;
	string s = fileName;
	s += ".o";
	obj_fptr = fopen(s.c_str(),"rb");
	
	if(obj_fptr != NULL)
	{	
		num = fread(machine_code_int,sizeof(int),MAX_SIZE,obj_fptr);

		for(int i=0;i<num;i++)
		{
			string curr = decimal_to_2scomplement(machine_code_int[i],32);
			machine_code[i] = curr;
		}

		if(options == "-isa")
		{
			cout << "\nOpcode Mnemonic Operand\n";
			
			for(auto it=isa.begin();it!=isa.end();it++)
			{
				mnemonic = (it->second).first;
				operand = (it->second).second;
				cout << it->first << "      " << mnemonic << "       " << operand << "\n"; 
			}
			cout << "        " << "SET       value\n\n";
		}

		if(options == "-wipe")
		{
			cout << "All flags(PC,SP,A,B) are reset\n";
			reg_A = 0;
			reg_B = 0;
			program_counter = 0;
			stack_pointer = 0;
		}

		if(options == "-before")
		{
			cout << "Memory dump before execution \n\n";

			for(int i=0;(i+4)<num;i+=4)
			{
				cout << decimal_to_2scomplement(i,32) << " " << machine_code[i] << " " << machine_code[i+1] << " " << machine_code[i+2] << " " << machine_code[i+3] << "\n";
			}

			cout << "\n\n";
		}

		execute();	
		
		if(options == "-after")
		{
			cout << "Memory dump after execution \n\n";

			for(int i=0;(i+4)<num;i+=4)
			{
				cout << decimal_to_2scomplement(i,32) << " " << machine_code[i] << " " << machine_code[i+1] << " " << machine_code[i+2] << " " << machine_code[i+3] << "\n";
			}

			cout << "\n\n";
		}

		printf(" \n %d instructions executed \n",instructions_count);
		
		// Close object file
		fclose(obj_fptr);
	} 
	else
	{
		cout << "No file with name " << s << " exists \n"; 
	}

		

	return 0;
}

// Insert instructions along with their opcode and also categorize instructions according to their format
void initialize()
{
	instructions_without_operand.insert({6,"add"});
	instructions_without_operand.insert({7,"sub"});
	instructions_without_operand.insert({8,"shl"});
	instructions_without_operand.insert({9,"shr"});
	instructions_without_operand.insert({11,"a2sp"});
	instructions_without_operand.insert({12,"sp2a"});
	instructions_without_operand.insert({14,"return"});
	instructions_without_operand.insert({18,"HALT"});

	instructions_with_one_operand.insert({0,"ldc"});
	instructions_with_one_operand.insert({1,"adc"});
	instructions_with_one_operand.insert({2,"ldl"});
	instructions_with_one_operand.insert({3,"stl"});
	instructions_with_one_operand.insert({4,"ldnl"});
	instructions_with_one_operand.insert({5,"stnl"});
	instructions_with_one_operand.insert({10,"adj"});
	instructions_with_one_operand.insert({13,"call"});
	instructions_with_one_operand.insert({15,"brz"});
	instructions_with_one_operand.insert({16,"brlz"});
	instructions_with_one_operand.insert({17,"br"});

	instructions_with_one_operand.insert({19,"SET"});
	instructions_with_one_operand.insert({20,"data"});

	instructions_with_pcoffset.insert({13,"call"});
	instructions_with_pcoffset.insert({15,"brz"});
	instructions_with_pcoffset.insert({16,"brlz"});
	instructions_with_pcoffset.insert({17,"br"});

	isa.insert({0,{"ldc","value"}});
	isa.insert({1,{"adc","value"}});
	isa.insert({2,{"ldl","value"}});
	isa.insert({3,{"stl","value"}});
	isa.insert({4,{"ldnl","value"}});
	isa.insert({5,{"ldnl","value"}});
	isa.insert({6,{"add",""}});
	isa.insert({7,{"sub",""}});
	isa.insert({8,{"shl",""}});
	isa.insert({9,{"shr",""}});
	isa.insert({10,{"adj","value"}});
	isa.insert({11,{"a2sp",""}});
	isa.insert({12,{"sp2a",""}});
	isa.insert({13,{"call","offset"}});
	isa.insert({14,{"return",""}});
	isa.insert({15,{"brz","offset"}});
	isa.insert({16,{"brlz","offset"}});
	isa.insert({17,{"br","offset"}});
	isa.insert({18,{"HALT",""}});


	bin_to_hex.insert({"0000",'0'});
	bin_to_hex.insert({"0001",'1'});
	bin_to_hex.insert({"0010",'2'});
	bin_to_hex.insert({"0011",'3'});
	bin_to_hex.insert({"0100",'4'});
	bin_to_hex.insert({"0101",'5'});
	bin_to_hex.insert({"0110",'6'});
	bin_to_hex.insert({"0111",'7'});
	bin_to_hex.insert({"1000",'8'});
	bin_to_hex.insert({"1001",'9'});
	bin_to_hex.insert({"1010",'A'});
	bin_to_hex.insert({"1011",'B'});
	bin_to_hex.insert({"1100",'C'});
	bin_to_hex.insert({"1101",'D'});
	bin_to_hex.insert({"1110",'E'});
	bin_to_hex.insert({"1111",'F'});	
}

void execute()
{
	for(program_counter=0;program_counter<num;program_counter++)
	{
		instructions_count++;
		opcode = string_to_decimal(machine_code[program_counter].substr(6,2),8);

		value_or_offset = string_to_decimal_24(machine_code[program_counter].substr(0,6));
		
		if(instructions_without_operand.find(opcode) != instructions_without_operand.end())
		{
			if(options == "-trace")
			printf("PC=%s, SP=%s, A=%s, B=%s, %s\n",decimal_to_2scomplement(program_counter,32).c_str(),decimal_to_2scomplement(stack_pointer,32).c_str(),decimal_to_2scomplement(reg_A,32).c_str(),decimal_to_2scomplement(reg_B,32).c_str(),instructions_without_operand[opcode].c_str());
		
			if(opcode == 6)
			{
				reg_A = reg_B + reg_A;
			}
			else if(opcode == 7)
			{
				reg_A = reg_B - reg_A;
			}
			else if(opcode == 8)
			{
				reg_A = (reg_B << reg_A);
			}
			else if(opcode == 9)
			{
				reg_A = (reg_B >> reg_A);
			}
			else if(opcode == 11)
			{
				stack_pointer = reg_A;
				reg_A = reg_B;
			}
			else if(opcode == 12)
			{
				reg_B = reg_A;
				reg_A = stack_pointer;
			}
			else if(opcode == 14)
			{
				program_counter = reg_A;
				reg_A = reg_B;
			}
			else if(opcode == 18)
			{
				if(options == "-trace")
				cout << "Program Execution Finished\n";
				return;
			}
			else
			{
				cout << "Invalid Instruction, Program Execution Stopped\n";
				return;
			}
		}
		else if(instructions_with_one_operand.find(opcode) != instructions_with_one_operand.end())
		{
			if(options == "-trace")
			printf("PC=%s, SP=%s, A=%s, B=%s, %s %s\n",decimal_to_2scomplement(program_counter,32).c_str(),decimal_to_2scomplement(stack_pointer,32).c_str(),decimal_to_2scomplement(reg_A,32).c_str(),decimal_to_2scomplement(reg_B,32).c_str(),instructions_with_one_operand[opcode].c_str(),decimal_to_2scomplement(value_or_offset,32).c_str());
			if(instructions_with_pcoffset.find(opcode) != instructions_with_pcoffset.end())
			{
				if(opcode == 13)
				{
					reg_B = reg_A;
					reg_A = program_counter;
					program_counter = program_counter + value_or_offset;
				}
				else if(opcode == 15)
				{	
					if(reg_A == 0)
						program_counter = program_counter + value_or_offset;
				}
				else if(opcode == 16)
				{
					if(reg_A < 0)
						program_counter = program_counter + value_or_offset;
				}
				else if(opcode == 17)
				{	
					program_counter = program_counter + value_or_offset;
				}
				else
				{
					cout << "Invalid Instruction, Program Execution Stopped\n";
					return;
				}
			}
			else
			{
				if(opcode == 0)
				{
					reg_B = reg_A;
					reg_A = value_or_offset;
				}
				else if(opcode == 1)
				{
					reg_A = reg_A + value_or_offset;
				}
				else if(opcode == 2)
				{
					reg_B = reg_A;
					if(stack_pointer + value_or_offset < 0 or stack_pointer + value_or_offset > MAX_SIZE)
					{
						if(options == "-trace")
						cout << "ERROR: Invalid Memory Access\n";
						return;
					}
					reg_A = machine_code_int[stack_pointer+value_or_offset];
					if(options == "-read")
					{
						printf("Reading Memory[%s] finds %s\n",decimal_to_2scomplement(stack_pointer+value_or_offset,32).c_str(),decimal_to_2scomplement(reg_A,32).c_str());
					}

				}
				else if(opcode == 3)
				{
					if(stack_pointer + value_or_offset < 0 or stack_pointer + value_or_offset > MAX_SIZE)
					{
						if(options == "-trace")
						cout << "ERROR: Invalid Memory Access\n";
						return;
					}
					if(options == "-write")
					{
						printf("Writing Memory[%s] was %s ",decimal_to_2scomplement(stack_pointer+value_or_offset,32).c_str(),decimal_to_2scomplement(machine_code_int[stack_pointer+value_or_offset],32).c_str());
					}
					machine_code_int[stack_pointer+value_or_offset] = reg_A;
					if(options == "-write")
						printf("now %s\n",decimal_to_2scomplement(reg_A,32).c_str());
					reg_A = reg_B;
				}
				else if(opcode == 4)
				{
					if(reg_A + value_or_offset < 0 or reg_A + value_or_offset > MAX_SIZE)
					{
						if(options == "-trace")
						cout << "ERROR: Invalid Memory Access\n";
						return;
					}
					if(options == "-read")
					{
						printf("Reading Memory[%s] finds %s\n",decimal_to_2scomplement(reg_A+value_or_offset,32).c_str(),decimal_to_2scomplement(machine_code_int[reg_A+value_or_offset],32).c_str());
					}
					reg_A = machine_code_int[reg_A+value_or_offset];				
				}
				else if(opcode == 5)
				{
					if(reg_A + value_or_offset < 0 or reg_A + value_or_offset > MAX_SIZE)
					{
						if(options == "-trace")
						cout << "ERROR: Invalid Memory Access\n";
						return;
					}
					if(options == "-write")
					{
						printf("Writing Memory[%s] was %s ",decimal_to_2scomplement(reg_A+value_or_offset,32).c_str(),decimal_to_2scomplement(machine_code_int[reg_A+value_or_offset],32).c_str());
					}
					machine_code_int[reg_A+value_or_offset] = reg_B;
					if(options == "-write")
						printf("now %s\n",decimal_to_2scomplement(reg_B,32).c_str());
					
				}
				else if(opcode == 10)
				{
					stack_pointer = stack_pointer + value_or_offset;
				}
				else
				{
					cout << "Invalid Instruction, Program Execution Stopped\n";
					return;
				}
			}
		}

		if(program_counter >= num-1)
		{
			if(options == "-trace")
			cout << "ERROR: Invalid Memory Access\n";
			return;
		}
	}
}

// Convert decimal to 2scomplement 32bit,24bit,8bit representation
string decimal_to_2scomplement(int data,int no_of_bits)
{
	const size_t BITS = no_of_bits;
	char bin_str[BITS+1];
	for(unsigned int i=0; i<BITS; i++)
	{		
  		unsigned int mask = 1u << (BITS - 1 - i);
  		bin_str[i] = (data & mask) ? '1' : '0';
	}	
	bin_str[BITS] = '\0';

	string ret="";
	for(int i=0;i<BITS;i+=4)
	{
		string tmp = "";
		tmp += bin_str[i];
		tmp += bin_str[i+1];
		tmp += bin_str[i+2];
		tmp += bin_str[i+3];
		ret += bin_to_hex[tmp];
	}
	return ret;
}


// Convert string to decimal accordingly
int string_to_decimal(string s,int no_of_bits)
{
	int value = 0;
		
	unsigned int x;
	string s1 = s;
	while(s1.length() < (no_of_bits)/4)
	{
		s1.insert(0,"0");
	}
	stringstream ss;
	ss << std::hex << s1;
	ss >> x;
	value = static_cast<int>(x);
		
	return value;	
}

// Convert 24bitstring to decimal appropriately
int string_to_decimal_24(string s)
{
	int value = 0;
		
	unsigned int x;
	string s1 = s;

	if(s[0] == '8' or s[0] == '9' or s[0] == 'A' or s[0] == 'B' or s[0] == 'C' or s[0] == 'D' or s[0] == 'E' or s[0] == 'F')
	{
		while(s1.length() < 8)
		{
			s1.insert(0,"F");
		}
	}
	else
	{
		while(s1.length() < 8)
		{
			s1.insert(0,"0");
		}
	}

	stringstream ss;
	ss << std::hex << s1;
	ss >> x;
	value = static_cast<int>(x);
		
	return value;

}