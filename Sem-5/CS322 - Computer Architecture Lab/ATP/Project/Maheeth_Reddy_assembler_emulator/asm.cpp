// Name of Student: M. Maheeth Reddy
//        Roll No.: 1801CS31
// I hereby declare that the source code submitted is my own unaided work. It is part of the assignment of CS321 at the department of Computer Science and Engg, IIT Patna
// To compile this source file: g++ asm.cpp -o asm
#include <bits/stdc++.h>
#include <fstream>
#include <iomanip>
#define INVALID -1
#define NIL -2
using namespace std;

// for removing leading and trailing spaces
const string ALL_SPACES = " \n\r\t\f\v";
string rtrim(const string& s);
string ltrim(const string& s);
string trim(const string& s);

// base verification functions;
int checkOctal(string s);
int checkDecimal(string s);
int checkHexadecimal(string s);

int checkParamType(string s);                                       // check whether parameter is number or symbol
int isValidLabel(string label);                                     // check whether label is valid or not
int symbolToParamValue(int opcode, string param_name, int pc);      // find machine code equivalent of symbol
int isValidMnemParam(int opcode, string param_name);                // does the mnemonic support this parameter
int checkUnusedSymbol(string sym);                                  // check for unused symbol
int symbolLine(string sym);                                         // find the line number of a symbol
string getFileName(string fname);                                   // exclude extension from given input file


// base conversion functions
int strtoi(string s, int base=10);
int tonum(string s);
string Dec2Hex(int num);

// mnemonic related functions
int getOpcode(string mnem);
string getMnem(int opcode);
int isParam(int opcode);
uint32_t getMCode(int opcode, int param_value=0);

// data structure to store an instruction info
typedef struct mnemonic {
    string mnem;
    int opcode,isParam,number,set,data,label;

    mnemonic(string mnem,int opcode,int isParam,int number,int set,int data,int label) {
        this->mnem = mnem;          // name
        this->opcode = opcode;      // opcode
        this->isParam = isParam;    // does it take an operand/parameter
        this->number = number;      // does it take number as operand
        this->set = set;            // does it take SET as operand
        this->data = data;          // does it take data as operand
        this->label = label;        // does it take label as operand
    }
} mnemonic;

// data structure to store instruction set
vector<mnemonic> mnemonics = {
    mnemonic("ldc",0,1,      1,1,1,1),
    mnemonic("adc",1,1,      1,1,1,1),
    mnemonic("ldl",2,1,      1,1,0,0),
    mnemonic("stl",3,1,      1,1,0,0),
    mnemonic("ldnl",4,1,     1,1,0,0),
    mnemonic("stnl",5,1,     1,1,0,0),
    mnemonic("add",6,0,      0,0,0,0),
    mnemonic("sub",7,0,      0,0,0,0),
    mnemonic("shl",8,0,      0,0,0,0),
    mnemonic("shr",9,0,      0,0,0,0),
    mnemonic("adj",10,1,     1,1,0,0),
    mnemonic("a2sp",11,0,    0,0,0,0),
    mnemonic("sp2a",12,0,    0,0,0,0),
    mnemonic("call",13,1,    1,1,0,1),
    mnemonic("return",14,0,  0,0,0,0),
    mnemonic("brz",15,1,     1,1,0,1),
    mnemonic("brlz",16,1,    1,1,0,1),
    mnemonic("br", 17,1,     1,1,0,1),
    mnemonic("HALT",18,0,    0,0,0,0),
    mnemonic("SET",19,1,     1,0,0,0),
    mnemonic("data",20,1,    1,0,0,0)
};

// data structure to store an instruction in an intermediate form
typedef struct inter {
    string line;            // store the line from input file
    uint32_t pcval;         // store program counter
    string label;           // store label (if any)
    int opcode;             // store opcode
    string param_t;         // type of parameter
    string param_name;
    int param_value;

    inter(string line, uint32_t pcval, string label, int opcode, string param_t, string param_name, int param_value) {
        this->line = line;
        this->pcval = pcval;
        this->label = label;
        this->opcode = opcode;
        this->param_t = param_t;
        this->param_name = param_name;
        this->param_value = param_value;
    }
} inter;

// symbol table -> information of each symbol (its name, address or program counter, type) is stored
// a symbol can be of type label or data or SET
map < string, pair<uint32_t,int> > psymbols;
// all instructions are stored as intermediates here
vector < inter > intermediate;

int main(int argc, char **argv) {
    ifstream source;
    
    string usage = "Usage: ./asm filename.asm";
    // if input file is not provided, give error
    if(argc < 2) {
        cout << "No input file specified.\n";
        cout << "Specify atmost one file\n";
        cout << usage << "\n";
        return 0;
    }

    string filename = argv[1];

    // if file does not exist, give error
    source.open(argv[1]);
    if(!source) {
        cout << "File \"" << argv[1] << "\" doesn\'t exist\n";
        cout << usage << "\n";
        return 0;
    }
    
    string lineo;
    uint32_t pc = 0;

    // Pass 1 - Store all instructions in source file as intermediates
    // Assign Address to each instruction
    // Read the source file line by line
    while( source ) {
        string line,label,mnem,comm,tparam;
        size_t found;
        int param=0;
        if(source.eof()) break;     // stop reading if end of file is reached
        getline(source,lineo);
        
        line = trim(lineo);         // remove leading and trailing spaces

        mnem = line;

        // seperate the comment
        found = mnem.find(";");
        if(found != string::npos) {
            comm = mnem.substr(found);
            mnem = mnem.substr(0,found);
        }
        mnem = trim(mnem);

        // seperate the label
        found = mnem.find(":");
        if(found != string::npos) {
            label = mnem.substr(0,found);
            mnem = mnem.substr(found+1);
        }
        mnem = trim(mnem);

        // seperate the mnemonic and parameter
        // if mnem requires the param, seperate the param (this is not error handling)
        found = mnem.find(" ");
        if(found != string::npos) {
            tparam = trim(mnem.substr(found));
            mnem = mnem.substr(0,found);
        }
        
        // store the intermediate form
        if(mnem != "") {
            intermediate.push_back( inter(lineo,pc,label,getOpcode(mnem),"",tparam,NIL) );
            pc++;
        } else {
            intermediate.push_back( inter(lineo,pc,label,NIL,"","",NIL) );
        }
        
    }
    source.close();
    // End of Pass 1
    
    // Pass 2 - Assign addresses to symbols, print errors and conversion to machine code
    int errorCount = 0;     // store number of errors
    ofstream log;           // print errors to log file
    log.open(getFileName(filename)+".log");
    for(int i = 0; i < intermediate.size(); i++) {
        if(intermediate[i].line == "") continue;
        // Validate label here
        if( intermediate[i].label != "" && !isValidLabel(intermediate[i].label) ) {
            log << "ERROR:line " << (i+1) << "\tLabel doesn\'t begin with alphabet: \"" << intermediate[i].line << "\"\n";
            cout << "ERROR:line " << (i+1) << "\tLabel doesn\'t begin with alphabet: \"" << intermediate[i].line << "\"\n";
            errorCount++;
            continue;
        } else {
            // if label is declared first time or was used as parameter previously, update address and type info in symbol table
            if(intermediate[i].label != "" && (psymbols.find(intermediate[i].label) == psymbols.end() || psymbols[intermediate[i].label] == make_pair<uint32_t,int>(-1,-1))) {
                // check whether parameters are valid for the instruction in this line
                if(intermediate[i].opcode == getOpcode("SET")) {
                    int type = checkParamType(intermediate[i].param_name);
                    if(type == 1) {
                        log << "ERROR:line " << (i+1) << "\tInvalid operand for SET instruction, only number is allowed: \"" << intermediate[i].line << "\"\n";
                        cout << "ERROR:line " << (i+1) << "\tInvalid operand for SET instruction, only number is allowed: \"" << intermediate[i].line << "\"\n";
                        errorCount++;
                        continue;
                    } else {
                        int temp = tonum(intermediate[i].param_name);//,type);
                        if(intermediate[i].label != "") {
                            psymbols[intermediate[i].label].first = temp;
                            psymbols[intermediate[i].label].second = 2;
                        } else {
                            // Label is mandatory for SET instruction
                            log << "ERROR:line " << (i+1) << "\tNo label specified for SET instruction: \"" << intermediate[i].line << "\"\n";
                            cout << "ERROR:line " << (i+1) << "\tNo label specified for SET instruction: \"" << intermediate[i].line << "\"\n";
                            errorCount++;
                            continue;
                        }
                    }
                } else if (intermediate[i].opcode == getOpcode("data")) {
                    if(intermediate[i].label != "") {
                        psymbols[intermediate[i].label].first = intermediate[i].pcval;
                        psymbols[intermediate[i].label].second = 1;
                    } else {
                        // Label is mandatory for data instruction
                        log << "ERROR:line " << (i+1) << "\tNo label specified for data instruction: \"" << intermediate[i].line << "\"\n";
                        cout << "ERROR:line " << (i+1) << "\tNo label specified for data instruction: \"" << intermediate[i].line << "\"\n";
                        errorCount++;
                        continue;
                    }
                } else {
                    if(intermediate[i].label != "") {
                        psymbols[intermediate[i].label].first = intermediate[i].pcval;
                        psymbols[intermediate[i].label].second = 0;
                    }
                }
                // if a symbol was used as parameter before its declaration, update the equivalent value in the instructions it occurs
                for(int j = 0; j < intermediate.size(); j++) {
                    if(intermediate[j].param_name == intermediate[i].label && (intermediate[j].param_value == -1 || intermediate[j].param_value == NIL)) {
                        intermediate[j].param_value = symbolToParamValue(intermediate[j].opcode,intermediate[j].param_name,intermediate[j].pcval);//psymbols[intermediate[i].label].first;
                    }
                }
            } else {
                // show duplicate label error
                if(intermediate[i].label != "") {
                    log << "ERROR:line " << (i+1) << "\tDuplicate Label found: \"" << intermediate[i].line << "\"\n";
                    cout << "ERROR:line " << (i+1) << "\tDuplicate Label found: \"" << intermediate[i].line << "\"\n";
                    errorCount++;
                    continue;
                }
            }
        }

        // if mnemonic is not in instruction set, show error
        if(intermediate[i].opcode == INVALID) {
            log << "ERROR:line " << (i+1) << "\tInvalid mnemonic: \"" << intermediate[i].line << "\"\n";
            cout << "ERROR:line " << (i+1) << "\tInvalid mnemonic: \"" << intermediate[i].line << "\"\n";
            errorCount++;
            continue;
        }

        // if no mnemonic is there, go to next line
        if(intermediate[i].opcode == NIL) {
            continue;
        }
        
        // if an instruction doesn't need a parameter, but parameter is specified, give error
        if(isParam(intermediate[i].opcode) == 0) {
            if(intermediate[i].param_name != "") {
                log << "ERROR:line " << (i+1) << "\tThis mnemonic doesn\'t require a parameter: \"" << intermediate[i].line << "\"\n";
                cout << "ERROR:line " << (i+1) << "\tThis mnemonic doesn\'t require a parameter: \"" << intermediate[i].line << "\"\n";
                errorCount++;
                continue;
            }
        }

        if(isParam(intermediate[i].opcode) == 1) {
            if(intermediate[i].param_name == "") {
                // if an instruction needs a parameter, but parameter is not specified, give error
                log << "ERROR:line " << (i+1) << "\tThis mnemonic requires a parameter: \"" << intermediate[i].line << "\"\n";
                cout << "ERROR:line " << (i+1) << "\tThis mnemonic requires a parameter: \"" << intermediate[i].line << "\"\n";
                errorCount++;
                continue;
            } else {
                // check whether parameter is label/number
                int type = checkParamType(intermediate[i].param_name);
                if(type == 1) {
                    // if label is invalid, throw error
                    if(!isValidLabel(intermediate[i].param_name)) {
                        log << "ERROR:line " << (i+1) << "\tLabel doesn\'t begin with alphabet: \"" << intermediate[i].line << "\"\n";
                        cout << "ERROR:line " << (i+1) << "\tLabel doesn\'t begin with alphabet: \"" << intermediate[i].line << "\"\n";
                        errorCount++;
                        continue;
                    }
                    intermediate[i].param_t = "symbol";
                    // if this label doesn't exist in symbol table, add it
                    if( psymbols.find(intermediate[i].param_name) == psymbols.end() ) {
                        psymbols[intermediate[i].param_name] = make_pair<uint32_t,int>(-1,-1);
                    } else {
                        // first validate whether the mnemonic can get the symbol of this type
                        // then check whether offset is required or absolute value
                        if( isValidMnemParam(intermediate[i].opcode,intermediate[i].param_name) ) {
                            intermediate[i].param_value = symbolToParamValue(intermediate[i].opcode,intermediate[i].param_name,intermediate[i].pcval);
                        } else {
                            // otherwise give error
                            log << "ERROR:line " << (i+1) << "\tInvalid operand type for the instruction: \"" << intermediate[i].line << "\"\n";
                            cout << "ERROR:line " << (i+1) << "\tInvalid operand type for the instruction: \"" << intermediate[i].line << "\"\n";
                            errorCount++;
                            continue;
                        }
                    }
                } else {
                    // if instruction doesn't accept number as parameter, show error
                    if(!mnemonics[intermediate[i].opcode].number) {
                        log << "ERROR:line " << (i+1) << "\tInvalid operand type for the instruction: \"" << intermediate[i].line << "\"\n";
                        cout << "ERROR:line " << (i+1) << "\tInvalid operand type for the instruction: \"" << intermediate[i].line << "\"\n";
                        errorCount++;
                        continue;
                    }
                    intermediate[i].param_t = "literal";
                    int temp = tonum(intermediate[i].param_name);
                    intermediate[i].param_value = temp;
                    intermediate[i].param_name = "";
                }
            }
        }
    }

    // show errors for all undefined labels used in the program
    // for these labels address will be -1
    for(int i = 0; i < intermediate.size(); i++) {
        string param_name = intermediate[i].param_name;
        if(param_name != "") {
            if(psymbols[param_name].first == -1) {
                log << "ERROR:line " << (i+1) << "\tUndefined Label found: \"" << intermediate[i].line << "\"\n";
                cout << "ERROR:line " << (i+1) << "\tUndefined Label found: \"" << intermediate[i].line << "\"\n";
                errorCount++;
            }
        }
    }

    // show warnings for all unused labels used in the program
    // such labels will not be present as parameters in any line of the source file
    for(auto it = psymbols.begin(); it != psymbols.end(); it++) {
        if(checkUnusedSymbol(it->first)) {
            int linenum = symbolLine(it->first);
            log << "WARNING:line " << (linenum+1) << "\tUnused Label found: \"" << intermediate[linenum].line << "\"\n";
            cout << "WARNING:line " << (linenum+1) << "\tUnused Label found: \"" << intermediate[linenum].line << "\"\n";
        }
    }
    log.close();

    // generate listing file
    ofstream listing;
    listing.open(getFileName(filename)+".l");
    for(auto it = intermediate.begin(); it != intermediate.end(); it++) {
        // if line in source file is empty, go to next line
        if(trim(it->line) == "") continue;

        // if opcode is valid, print the program counter and machine code
        if(it->opcode >= 0) {
            // print program counter
            listing << Dec2Hex(it->pcval) << " ";
            if(isParam(it->opcode)==1) {
                // if instruction requires parameter, but it is not provided
                // no machine code is printed
                if(it->param_value == NIL) {
                    listing << "         ";
                } else {
                    // machine code is printed
                    string paramV = Dec2Hex(it->param_value);
                    listing << paramV.substr(2);
                    listing << Dec2Hex(it->opcode).substr(6) << " ";
                }
            } else {
                // if instruction doesn't require parameter and also parameter is not provided
                // machine code is printed and operand value is shown as zero
                if(it->param_value == NIL && it->param_name == "") {
                    listing << "000000";
                    listing << Dec2Hex(it->opcode).substr(6) << " ";
                } else {
                    // if instruction doesn't require parameter, but it is provided
                    // no machine code is printed
                    listing << "         ";
                }
            }
        } else {
            listing << "         ";
        }
        // if line in source file is not empty, print the line as it is
        listing << it->line << "\n";
    }
    listing.close();

    // generate machine code if and only if there are no errors
    if(errorCount == 0) {
        ofstream object;
        object.open(getFileName(filename)+".o", ios::out | ios::binary);
        if(!object.is_open()) {
            cout << "Unable to generate object file\n";
            return 0;
        }

        // generate machine code
        for(auto it = intermediate.begin(); it != intermediate.end(); it++) {
            if(it->opcode >= 0) {
                uint32_t mcode;
                if(isParam(it->opcode)==1) {
                    if(it->param_value != NIL) {
                        mcode = getMCode(it->opcode,it->param_value);
                        object.write((char *) &mcode, sizeof(mcode));
                    }
                } else {
                    if(it->param_value == NIL) {
                        mcode = getMCode(it->opcode);
                        object.write((char *) &mcode, sizeof(mcode));
                    }
                }
            } 
        }
        object.close();
    }
    // End of Pass 2

    return 0;
}

int checkOctal(string s) {
    if (s[0] != '0') return false;

    for (int i = 1; i < s.length(); i++) {
        if( !isdigit(s[i]) || s[i] > '7') {
            return false;
        }
    }
    return true;
}

int checkDecimal(string s) {
    int start = (s[0] == '-' || s[0] == '+') ? 1 : 0;

    for(int i = start; i < s.length(); i++) {
        if(!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

int checkHexadecimal(string s) {
    if (s.substr(0,2) != "0x") return false;

    string hexdig = "0123456789abcdefABCDEF";
    for (int i = 2; i < s.length(); i++) {
        if( hexdig.find(s[i]) == string::npos ) {
            return false;
        }
    }
    return true;
}

int checkParamType(string s) {
    if( checkOctal(s) ) {
        return 8;
    }

    if( checkDecimal(s) ) {
        return 10;
    }

    if( checkHexadecimal(s) ) {
        return 16;
    }
    return 1;
}

int isValidLabel(string label) {
    if( !((label[0] >= 'a' && label[0] <= 'z') || (label[0] >= 'A' && label[0] <= 'Z') || (label[0] == '_')) ) return false;
    
    for(int i = 0; i < label.length(); i++) {
        if( !( isdigit(label[i]) || (label[0] >= 'a' && label[0] <= 'z') || (label[0] >= 'A' && label[0] <= 'Z') || (label[0] == '_') ) ) {
            return false;
        }
    }

    return true;
}

int symbolToParamValue(int opcode, string param_name, int pc) {
    switch (opcode) {
        case 13:    // call
        case 15:    // brz
        case 16:    // brlz
        case 17:    // br
            return psymbols[param_name].first-pc-1;
        
        case 0:     // ldc
        case 1:     // adc
        case 2:     // ldl
        case 3:     // stl
        case 4:     // ldnl
        case 5:     // stnl
        case 10:    // adj
            return psymbols[param_name].first;

        default:
            break;
    }
    return 0;
}

int isValidMnemParam(int opcode, string param_name) {
    if(psymbols[param_name].second == 2) {
        return mnemonics[opcode].set;
    }
    
    if(psymbols[param_name].second == 1) {
        return mnemonics[opcode].data;
    }

    return mnemonics[opcode].label;
}

int checkUnusedSymbol(string sym) {
    for(auto it = intermediate.begin(); it != intermediate.end(); it++) {
        if(sym == it->param_name || psymbols[sym].first == -1) {
            return false;
        }
    }
    return true;
}

int symbolLine(string sym) {
    for(int i = 0; i < intermediate.size(); i++) {
        if(sym == intermediate[i].label) {
            return i;
        }
    }
    return NIL;
}

string getFileName(string fname) {
    size_t dotpos = fname.find(".");
    if(dotpos == string::npos) return fname;
    return fname.substr(0,dotpos);
}

uint32_t getMCode(int opcode, int param_value) {
    return (param_value << 8) | opcode;
}

int getOpcode(string mnem) {
    int len = mnemonics.size();
    for(int i = 0; i < len; i++) {
        if(mnemonics[i].mnem == mnem) {
            return i;
        }
    }
    if(mnem != "") return INVALID;
    return NIL;
}

string getMnem(int opcode) {
    int len = mnemonics.size();
    if(opcode >= 0 && opcode < len) {
        return mnemonics[opcode].mnem;
    }
    return "NIL";
}

int isParam(int opcode) {
    int len = mnemonics.size();
    if(opcode >= 0 && opcode < len) {
        return mnemonics[opcode].isParam;
    }
    return NIL;
}

string ltrim(const string& s) {
	size_t start = s.find_first_not_of(ALL_SPACES);
    if(start != string::npos) {
        return s.substr(start);
    }
	return "";
}

string rtrim(const string& s) {
	size_t end = s.find_last_not_of(ALL_SPACES);
    if(end != string::npos) {
        return s.substr(0,end+1);
    }
	return "";
}

string trim(const string& s) {
	return rtrim(ltrim(s));
}

int tonum(string s) {
    s = trim(s);
    if(s.find("0x") == 0) {
        return strtoi(s.substr(2),16);
    } else if(s.find("0") == 0) {
        return strtoi(s.substr(1),8);
    } else if(s.find("-") == 0) {
        return -strtoi(s.substr(1));
    } else if(s.find("+") == 0) {
        return strtoi(s.substr(1));
    } else {
        return strtoi(s);
    }
}

int strtoi(string s, int base) {
    //if(base == 16) { s = s.substr(2); }
    int len = s.length();
    int exp = 1, res = 0;
    for(int i = len-1; i >= 0; i--) {
        int d = (s[i] >= 'a') ? (s[i]-'a'+10) : (s[i]-'0');
        res += (d * exp);
        exp *= base;
    }
    return res;
}

string Dec2Hex(int num) { 
    string digits = "0123456789ABCDEF";
    string res = ""; 

    if(num == 0) res = "0";
    
    if(num > 0) { 
        while(num) { 
            res = digits[num % 16] + res; 
            num /= 16; 
        } 
    } else { 
        u_int n = num; 
        while(n) { 
            res = digits[n % 16] + res; 
            n /= 16; 
        } 
    } 

    if(res.length() < 8) {
        for(int i = res.length(); i < 8; i++) {
            res = "0"+res;
        }
    }
    return res; 
}