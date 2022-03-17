// Name : Chandrawanshi Mangesh Shivaji
// Roll Number : 1801CS16
// Declaration of authorship : I hereby certify that the source code I am submitting is entirely my own original work except, where otherwise indicated.

#include <bits/stdc++.h>
using namespace std;

// ALL GLOBAL VARIABLES AND DATA STRUCTURES USED
string fileName;
map < string,char > bin_to_hex;
map < string,int > instructions_without_operand;
map < string,int > instructions_with_one_operand;
map < string,int > instructions_with_pcoffset;
map < string,int> labels;
set < string > used_labels;
set < pair<int,string> > errors;
set < pair<int,string> > warnings;
vector <string> source_code[100000];
vector <string> machine_code(100000);
int machine_code_int[100000];

// ALL FUNCTIONS USED
void initialize();
bool isValidLabel(string s);
int isValidOperand(string s);
void first_pass(string s,int line_num,int* p);
string decimal_to_2scomplement(int data,int no_of_bits);
int string_to_decimal(string s,int no_of_bits);
void modify_labels_using_SET();
void second_pass();
void calculate_machine_code(int instruction_type,string mnemonic,string operand,int itr,string label_to_be_used);
void identify_unused_lables();
void print_warnings();
void print_errors();
void write_log();
void write_list();
void write_obj();

int main(int argc,char** argv)
{
	// Store all instructions with their format specifications
	initialize();

	// Check if proper arguments are provided or not 
	if(argc != 2)
	{
		cout << "Invalid Command, Check Format : ./asm filename.asm";
	}

	// Open source file for reading
	fstream input_fptr;
	input_fptr.open(argv[1],ios::in);
	fileName = "";

	// Extract FileName from given arguments
	for(int i=0;i<strlen(argv[1]);i++)
	{
		if(argv[1][i] == '.')
			break;
		fileName += argv[1][i];
	}

	// Check if it is a valid file pointer or not
	if(input_fptr.is_open())
	{
		int line_num = 1;
		int program_counter = 0;
		string curr_line;

		// Read Line by Line and extract useful data
		while(getline(input_fptr,curr_line))
		{
			string s;
			for(int i=0;i<curr_line.length();i++)
			{
				if(curr_line[i] == ';')
					break;
				s += curr_line[i];
			}

			// Used to Identify Labels and Common assembly errors
			first_pass(s,line_num,&program_counter);
			line_num++;
		}

		modify_labels_using_SET();

		// Used to convert given assmebly to machine code and along with that identify more errors 
		second_pass();
		identify_unused_lables();
	}
	else
	{
		// Invalid file argument 
		errors.insert({0,"Unable to open given file, please check format of the file"});
	}	
	
	// Write lables info, errors and warnings to a log file
	write_log();
	
	// generate listing file
	write_list();

	// Check if given assembly has errors or not 
	if(errors.empty())
	{		
		// Print warnings, if present
		if(!warnings.empty())
			print_warnings();

		// generate object file	
		write_obj();
	}
	else
	{
		// print all errors along with existing warnings
		print_errors();
		if(!warnings.empty())
			print_warnings();
	}
	// Close source file 
	input_fptr.close();
	return 0;
}

// Insert instructions along with their opcode and also categorize instructions according to their format
void initialize()
{
	instructions_without_operand.insert({"add",6});
	instructions_without_operand.insert({"sub",7});
	instructions_without_operand.insert({"shl",8});
	instructions_without_operand.insert({"shr",9});
	instructions_without_operand.insert({"a2sp",11});
	instructions_without_operand.insert({"sp2a",12});
	instructions_without_operand.insert({"return",14});
	instructions_without_operand.insert({"HALT",18});

	instructions_with_one_operand.insert({"ldc",0});
	instructions_with_one_operand.insert({"adc",1});
	instructions_with_one_operand.insert({"ldl",2});
	instructions_with_one_operand.insert({"stl",3});
	instructions_with_one_operand.insert({"ldnl",4});
	instructions_with_one_operand.insert({"stnl",5});
	instructions_with_one_operand.insert({"adj",10});
	instructions_with_one_operand.insert({"call",13});
	instructions_with_one_operand.insert({"brz",15});
	instructions_with_one_operand.insert({"brlz",16});
	instructions_with_one_operand.insert({"br",17});

	instructions_with_one_operand.insert({"SET",19});
	instructions_with_one_operand.insert({"data",20});

	instructions_with_pcoffset.insert({"call",13});
	instructions_with_pcoffset.insert({"brz",15});
	instructions_with_pcoffset.insert({"brlz",16});
	instructions_with_pcoffset.insert({"br",17});

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

// For Checking Valid Label name - should be alphanumerix, not starting with a number
bool isValidLabel(string s)
{
	if(s[0] >= '0' and s[0] <= '9')
		return false;	

	for(int i=0;i<s.length();i++)
	{
		if( !((s[i] >= 'a' and s[i] <= 'z') or (s[i] >= 'A' and s[i] <= 'Z') or (s[i] >= '0' and s[i] <= '9') or (s[i] == '_')) )
			return false;
	}
	return true;
}

// For Checking Valid Operand returns 0 - if invalid number,1 - if non-existing label,
// 2 - if valid decimal,3 - if valid octal,4 - if valid hexadecimal,5 - if existing label
int isValidOperand(string s)
{
	if(isValidLabel(s) and labels.find(s) != labels.end())
		return 5;

	if(isValidLabel(s) and labels.find(s) == labels.end())
		return 1;
	
	int len = s.length();
	int flag = 3;

	for(int i=0;i<len;i++)
	{
		if(!(s[i] >= '0' and s[i] <= '7'))
			flag = 2;
	}

	if(flag == 3)
		return 3;

	for(int i=0;i<len;i++)
	{
		if(!(s[i] >= '0' and s[i] <= '9'))
			flag = 4; 
	}

	if(flag == 2)
		return 2;

	for(int i=0;i<len;i++)
	{
		if( !((s[i] >= 'a' and s[i] <= 'f') or (s[i] >= 'A' and s[i] <= 'F') or (s[i] >= '0' and s[i] <= '9')) )
			flag = 0;
	}

	if(flag == 4)
		return 4;

	return 0;
}

// Used to Identify Labels and Common assembly errors
void first_pass(string s,int line_num,int* p)
{
	if(s != "")
	{
		int len = s.length();
		string s1;
		for(int i=0;i<len;i++)
		{
			s1 += s[i];
			if(s[i] == ':')
				s1 += ' ';
		}

		// Separate line into different contiguous words
		string words[5]={"","","","",""};
		int len_words[5] = {0,0,0,0,0};
		stringstream ss(s1);

		int i=0;
		while(ss >> words[i])
		{
			len_words[i] = words[i].length();
			i++;
		}

		for(int i=0;i<5;i++)
		{
			if(words[i]!= "")
				source_code[(*p)].push_back(words[i]);
		}

		// Check if label is there or not 
		if(words[0][len_words[0]-1] == ':')
		{
			string labelName = words[0].substr(0,len_words[0]-1);

			// Check if labelName is valid or not
			if(!isValidLabel(labelName))
			{
				errors.insert({line_num,"Invalid Label Name, Only include alphanumeric, start with alphabets only"});
			}
			else
			{	
				// Check if label with same labelName already exists or not 
				if(labels.find(labelName) != labels.end())
				{
					errors.insert({line_num,"Duplicate Label Definition"});
				}
				else
				{
					// Insert labelName into labels table
					labels.insert({labelName,*p});
					if(!(len_words[1] == 0 and len_words[2] == 0))
						(*p)++;
				}
			}

			// Check if some instruction is there on the same line or not
			if(words[1] != "" and words[1][len_words[1]-1] != ':')
			{
				// Store mnemonic of the instruction 
				string mnemonic = words[1];

				// Check if instruction should have operand or not 
				if(instructions_without_operand.find(mnemonic) != instructions_without_operand.end())
				{
					// Print Error if there is a operand 
					if(words[2] != "")
						errors.insert({line_num,"Unexpected Operand, No Operand is allowed"});
				}
				else if(instructions_with_one_operand.find(mnemonic) != instructions_with_one_operand.end())
				{
					// Print Error if there are more than one operands
					if(words[3] != "")
						errors.insert({line_num,"Unexpected Operand, Only One Operand is allowed"});

					// Print Error if there is no operand
					if(words[2] == "")
						errors.insert({line_num,"No Operand found"});
				}
				else
				{
					// Print Error if instuction is invalid
					errors.insert({line_num,"No such insruction"});
				}
			}
		}
		else
		{
			// No Label, only instruction is there, get mnemonic of the instruction 
			string mnemonic = words[0];

			// Check if instruction should have operand or not 
			if(instructions_without_operand.find(mnemonic) != instructions_without_operand.end())
			{
				// Print Error if there is a operand 
				if(words[1] != "")
					errors.insert({line_num,"Unexpected Operand, No Operand is allowed"});
			}
			else if(instructions_with_one_operand.find(mnemonic) != instructions_with_one_operand.end())
			{
				// Print Error if there are more than one operands
				if(words[2] != "")
					errors.insert({line_num,"Unexpected Operand, Only One Operand is allowed"});

				// Print Error if there is no operand
				if(words[1] == "")
					errors.insert({line_num,"No Operand found"});
			}
			else
			{
				// Print Error if instuction is invalid
				errors.insert({line_num,"No such insruction"});
			}	
			(*p)++;
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
	if(no_of_bits == 32 or no_of_bits == 24)
	{
		int value = 0;
		int len = s.length();
		if(s == "0")
			return 0;
		// if string is a existing label
		if(isValidOperand(s) == 5)
		{
			return value = labels[s];
		}
		else if(isValidOperand(s) == 1)
		{
			// string is a non-existing label
			errors.insert({0,"No such Label"});
			return 0;
		}

		// string starts with 'negative' sign
		if(s[0] == '-')
		{
			// string is octal
			if(s[1] == '0')
			{
				if(isValidOperand(s.substr(2,len-2)) == 3)
				{				
					int multiplier = 1;
					for(int i=len-1;i>=2;i--)
					{
						value += ((s[i]-'0')*multiplier); 
						multiplier *= 8;
					}
					value *= -1;
				}
				else
					errors.insert({0,"Not a Valid Number Format, Use Suitable Prefix For hex = '0x', oct = '0'"});
			}
			else
			{
				// string is decimal
				if(isValidOperand(s.substr(1,len-1)) == 3 or isValidOperand(s.substr(1,len-1)) == 2)
				{
					value = stoi(s.substr(1,len-1));
					value *= -1;
				}
				else
					errors.insert({0,"Not a Valid Number Format,Use Suitable Prefix For hex = '0x', oct = '0'"});
			}
			return value;
		}

		// string starts with 'positive' sign
		if(s[0] == '+')
		{
			// string is octal
			if(s[1] == '0')
			{
				if(isValidOperand(s.substr(2,len-2)) == 3)
				{				
					int multiplier = 1;
					for(int i=len-1;i>=2;i--)
					{
						value += ((s[i]-'0')*multiplier); 
						multiplier *= 8;
					}
				}
				else
					errors.insert({0,"Not a Valid Number Format, Use Suitable Prefix For hex = '0x', oct = '0'"});
			}
			else
			{
				// string is decimal
				if(isValidOperand(s.substr(1,len-1)) == 3 or isValidOperand(s.substr(1,len-1)) == 2)
					value = stoi(s.substr(1,len-1));
				else
					errors.insert({0,"Not a Valid Number Format,Use Suitable Prefix For hex = '0x', oct = '0'"});
			}
			return value;
		}

		// string is hexadecimal
		if(s[0] == '0' and s[1] == 'x')
		{ 
			if(isValidOperand(s.substr(2,len-2)) == 3 or isValidOperand(s.substr(2,len-2)) == 2 or isValidOperand(s.substr(2,len-2)) == 4)
			{
				unsigned int x;
				string s1 = s.substr(2,len-2);
				while(s1.length() < (no_of_bits)/4)
				{
					s1.insert(0,"0");
				}
				stringstream ss;
				ss << std::hex << s1;
				ss >> x;
				value = static_cast<int>(x);
			}
			else
				errors.insert({0,"Not a Valid Number Format,Use Suitable Prefix For hex = '0x', oct = '0'"});				
		}
		else if(s[0] == '0')
		{
			// string is octal
			if(isValidOperand(s.substr(1,len-1)) == 3)
			{			
				value = 0;
				int multiplier = 1;
				for(int i=len-1;i>=1;i--)
				{
					value += ((s[i]-'0')*multiplier); 
					multiplier *= 8;
				}
			}	
			else
				errors.insert({0,"Not a Valid Number Format,Use Suitable Prefix For hex = '0x', oct = '0'"});
		}
		else
		{
			// string is decimal 
			if(isValidOperand(s) == 3 or isValidOperand(s) == 2)
				value = stoi(s);
			else
				errors.insert({0,"Not a Valid Number Format,Use Suitable Prefix For hex = '0x', oct = '0'"});
			
		}
		return value;
	}
	else
	{
		// string is mnemonic
		int value = 0;
		if(instructions_without_operand.find(s) != instructions_without_operand.end())
		{
			value = instructions_without_operand[s];
		}
		else if(instructions_with_one_operand.find(s) != instructions_with_one_operand.end())
		{
			value = instructions_with_one_operand[s];
		}
		return value;
	}
	return 0;
}

void modify_labels_using_SET()
{
	for(int i=0;i<100000;i++)
	{
		if(source_code[i].empty())
			break;
		
		if(source_code[i].size() == 3 and source_code[i][1] == "SET" and labels.find(source_code[i][0].substr(0,source_code[i][0].length()-1)) != labels.end() )
		{
			labels[source_code[i][0].substr(0,source_code[i][0].length()-1)] = string_to_decimal(source_code[i][2],32);
		}
		else if(source_code[i].size() == 4 and source_code[i][2] == "SET" and labels.find(source_code[i][1].substr(0,source_code[i][1].length()-1)) != labels.end())
		{
			labels[source_code[i][1].substr(0,source_code[i][1].length()-1)] = string_to_decimal(source_code[i][3],32);
		}
	}
}

// instruction type (0-without operand) (1-with operand)
void calculate_machine_code(int instruction_type,string mnemonic,string operand,int itr,string label_to_be_used)
{
	int value = 0;
	if(instruction_type == 0)
	{
		machine_code_int[itr] = instructions_without_operand[mnemonic];
		machine_code[itr] = decimal_to_2scomplement(instructions_without_operand[mnemonic],32);
	}
	else if(instruction_type == 1)
	{	
		if(mnemonic == "data")
		{
			used_labels.insert(label_to_be_used);
			value = string_to_decimal(operand,32);
			machine_code_int[itr] = value;
			machine_code[itr] = decimal_to_2scomplement(value,32);
		}
		else if(mnemonic == "SET")
		{
			used_labels.insert(label_to_be_used);
			value = string_to_decimal(operand,32);
			machine_code_int[itr] = value;
			machine_code[itr] = decimal_to_2scomplement(value,32);
		}
		else
		{
			int value2 = string_to_decimal(mnemonic,8);
			int value1 = string_to_decimal(operand,24);
			if( instructions_with_pcoffset.find(mnemonic) != instructions_with_pcoffset.end() )
			{
				if(isValidLabel(operand))
					value1 = (value1 - itr) - 1;								
			}
			machine_code_int[itr] = value1<<8 | value2;
			machine_code[itr] = decimal_to_2scomplement(value1,24) + decimal_to_2scomplement(value2,8);
		}
	}
}

// Used to convert given assmebly to machine code and along with that identify more errors 
void second_pass()
{
	for(int i=0;i<100000;i++)
	{
		if(source_code[i].empty())
			break;

		// instructions with only mnemonic (not followed by a label)
		if(source_code[i].size() == 1)
		{
			calculate_machine_code(0,source_code[i][0],"",i,"");
			continue;
		}

		string mnemonic,operand;
		int value = 0;
		string s1,s2;
		s1 = source_code[i][0];
		s2 = source_code[i][1];
		int flag1 = 0,flag2 = 0;

		if(s1[s1.length()-1] == ':') 
			flag1 = 1;		

		if(s2[s2.length()-1] == ':') 
			flag2 = 1;		
		
		// instruction after two labels
		if(flag1 == 1 and flag2 == 1)
		{
			mnemonic = source_code[i][2];

			if( instructions_without_operand.find(mnemonic) != instructions_without_operand.end() )
			{
				calculate_machine_code(0,mnemonic,"",i,"");
			}
			else
			{
				if(source_code[i].size() >= 4)
					operand = source_code[i][3];

				if(isValidLabel(operand) and labels.find(operand) != labels.end())
					used_labels.insert(operand);	

				calculate_machine_code(1,mnemonic,operand,i,s2.substr(0,s2.length()-1));		
			}
		}
		else if(flag1 == 1 and flag2 == 0)
		{
			// instruction after a single label 
			mnemonic = source_code[i][1];

			if( instructions_without_operand.find(mnemonic) != instructions_without_operand.end() )
			{
				calculate_machine_code(0,mnemonic,"",i,"");
			}
			else
			{
				if(source_code[i].size() >= 3)
					operand = source_code[i][2];

				if(isValidLabel(operand) and labels.find(operand) != labels.end())
					used_labels.insert(operand);

				calculate_machine_code(1,mnemonic,operand,i,s1.substr(0,s1.length()-1));
			}
		}
		else
		{
			// instruction without label 
			mnemonic = source_code[i][0];

			if( instructions_without_operand.find(mnemonic) != instructions_without_operand.end() )
			{
				calculate_machine_code(0,mnemonic,"",i,"");
			}
			else
			{
				if(source_code[i].size() >= 2)
					operand = source_code[i][1];

				if(isValidLabel(operand) and labels.find(operand) != labels.end())
					used_labels.insert(operand);

				calculate_machine_code(1,mnemonic,operand,i,"");
			}
		}
	}
}

void identify_unused_lables()
{
	for(auto it=labels.begin();it!=labels.end();it++)
	{
		string s = it->first;
		if(used_labels.find(s) == used_labels.end())
			warnings.insert({0,"Unused Label"});	
	}
}

void print_warnings()
{
	cout << "Warnings : (Line 0 refers to unidentified location) \n";
	for(auto it=warnings.begin();it!=warnings.end();it++)
	{
		cout << "Line " << it->first << " : " << it->second << "\n";
	}
	cout << "\n";
}

void print_errors()
{
	cout << "Errors : (Line 0 refers to unidentified location) \n";
	for(auto it=errors.begin();it!=errors.end();it++)
	{
		cout << "Line " << it->first << " : " << it->second << "\n";
	}
	cout << "\n";
}

void write_log()
{
	string s = fileName;
	s += ".log";
	fstream log_fptr;
	log_fptr.open(s,ios::out);

	if(log_fptr.is_open())
	{
		log_fptr << "LabelName => Value\n"; 
		for(auto it=labels.begin();it!=labels.end();it++)
		{
			log_fptr << it->first << " => " << it->second << "\n";
		}

		log_fptr << "\nUsedLabelName\n"; 
		for(auto it=used_labels.begin();it!=used_labels.end();it++)
		{
			if(*it != "")
				log_fptr << *it << "\n";
		}

		log_fptr << "\nWarnings \n";
		for(auto it=warnings.begin();it!=warnings.end();it++)
		{
			log_fptr << "Line " << it->first << " : " << it->second << "\n";
		}

		log_fptr << "\nErrors \n";
		for(auto it=errors.begin();it!=errors.end();it++)
		{
			log_fptr << "Line " << it->first << " : " << it->second << "\n";
		}
	}
	log_fptr.close();
	return;
}


void write_list()
{
	fstream list_fptr;
	string s = fileName;
	s += ".lst";
	list_fptr.open(s,ios::out);

	if(list_fptr.is_open())
	{
		for(int i=0;i<100000;i++)
		{
			if(source_code[i].empty())
				break;

			list_fptr << decimal_to_2scomplement(i,32) << " ";
			list_fptr << machine_code[i] << " ";

			for(int j=0;j<source_code[i].size();j++)
			{
				list_fptr << source_code[i][j] << " ";
			}
			list_fptr << "\n";
		}
	}
	list_fptr.close();
	return;
}

void write_obj()
{
	FILE* obj_fptr;
	string s = fileName;
	s += ".o";
	obj_fptr = fopen(s.c_str(),"wb");
	int sz = 0;
	for(int i=0;i<100000;i++)
	{
 		if(source_code[i].empty())
			break;
		sz++;
	}
	fwrite(machine_code_int,sizeof(int),sz,obj_fptr);
	fclose(obj_fptr);
	return;
}