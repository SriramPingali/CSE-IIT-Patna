// Name of Student: M. Maheeth Reddy
//        Roll No.: 1801CS31
// I hereby declare that the source code submitted is my own unaided work. It is part of the assignment of CS321 at the department of Computer Science and Engg, IIT Patna
// To compile this source file: g++ emu.cpp -o emu
#include <bits/stdc++.h>
#define INVALID -1
#define NIL -2
using namespace std;

// data structure to store an instruction info
typedef struct mnemonic {
    string mnem;
    int opcode,isParam,number,set,data,label;

    mnemonic(string mnem,int opcode,int isParam,int number,int set,int data,int label) {
        this->mnem = mnem;          // name
        this->opcode = opcode;      // opcode
        this->isParam = isParam;    // does it take any parameters
        this->number = number;      // does it take number as parameter
        this->set = set;            // does it take SET as parameter
        this->data = data;          // does it take data as parameter
        this->label = label;        // does it take label as parameter
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

// valid options
vector<string> options = {
    "-trace",
    "-before",
    "-after",
    "-isa"
};

// variables for registers, memory (includes space for stack),
// Program Counter, Stack Pointer
int32_t A,B,memory[10000];
uint32_t PC,SP = sizeof(memory)/sizeof(memory[0])-1;

// helps in printing output corresponding to option provided
int optEnable[4] = { 0 };
// trace - 0    before - 1      after - 2       isa - 3

int isOption(string option);                                            // check whether an option is valid
string getMnemonic(int opcode);                                         // get the mnemonic for an opcode
void getOpdOpr(int32_t mcode, int32_t *opcode, int32_t *operand);       // obtain the opcode and operand from machine code
int getOpcode(string mnem);                                             // get opcode for a mnemonic
void dump(int num, ostream& output);                                    // dump memory
void trace(int opcode, int operand, ofstream& output);                  // generate trace for an instruction given opcode and operand
void usage();                                                           // print the usage message in case of errors
void execute(ofstream& output);                                         // function for program execution
void isa(ofstream& output);                                             // function to print instruction set
void setOptions(string option);                                         // function to check which option was given so that relevant output is only printed
string getFileName(string fname);                                       // function to get output filename from input filename

int main(int argc, char *argv[]) {
    ifstream object;

    // if option or input file are not provided, show usage message
    if(argc < 3) {
        usage();
        return 0;
    }

    string option = string(argv[1]);
    // if invalid option is provided, show usage message
    if(!isOption(option)) {
        cout << "Invalid Option\n";
        usage();
        return 0;
    }

    // set which option is provided
    setOptions(option);
    object.open(argv[2], ios::binary);
    if(!object) {
        cout << "File " << argv[2] << " doesn\'t exist\n";
        return 0;
    }

    int pc = 0;     // determines the location of instruction in memory
    uint32_t temp;
    // read 32 bits at a time from the object file
    while (object.read((char*) &temp, sizeof(temp))) {
        int32_t opcode,operand;
        getOpdOpr(temp,&opcode,&operand);
        
        // if instruction is SET or data, put only operand value in memory
        // for other instruction put machine code in memory
        // for invalid instructions, give error
        if(opcode == getOpcode("SET") || opcode == getOpcode("data")) {
            memory[pc] = operand;
        } else if (opcode >= 0 && opcode < 20) {
            memory[pc] = temp;
        } else {
            cout << "Invalid Opcode found\n";
            return 0;
        }
        
        pc++;
    }
    object.close();

    cout << "\n";
    
    ofstream output;    // to print output to a file
    output.open(getFileName(string(argv[2]))+".txt");
    // if -before option was given, dump memory before execution
    if (optEnable[1]) {
        dump(pc,output);
    } else if(optEnable[3]) {
        // if -isa option was given, show instruction set before execution
        isa(output);
    }

    // execute the file
    execute(output);

    // if -after option was given, dump memory after execution
    if (optEnable[2]) {
        dump(pc,output);
    }
    
    output.close();
    return 0;
}

// check whether an option is valid
int isOption(string option) {
    for(auto it = options.begin(); it != options.end(); it++) {
        if(*it == option) return true;
    }
    return false;
}

// get the mnemonic for an opcode
string getMnemonic(int opcode) {
    if(opcode >= 0 && opcode <= 20)
        return mnemonics[opcode].mnem;
    
    return "NIL";
}

// obtain the opcode and operand from machine code
void getOpdOpr(int32_t mcode, int32_t *opcode, int32_t *operand) {
    *opcode = mcode & 0xff;
    
    int opr = (mcode & 0xffffff00);
    opr >>= 8;
    *operand = opr;
}

// get opcode for a mnemonic
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

// dump memory
void dump(int num, ostream& output) {
    int size = sizeof(memory)/sizeof(memory[0]);
    for(int i = 0; i < num; i++) {
        if(i % 4 == 0) {
            cout << setw(8) << setfill('0') << hex << uppercase << i << " ";
            output << setw(8) << setfill('0') << hex << uppercase << i << " ";
        }
        
        cout << setw(8) << hex << uppercase << setfill('0') << memory[i];
        output << setw(8) << hex << uppercase << setfill('0') << memory[i];

        if( (i+1) % 4 == 0 || i == num-1) {
            cout << "\n";
            output << "\n";
        } else {
            cout << " ";
            output << " ";
        }
    }
}

// generate trace for an instruction given opcode and operand
void trace(int opcode, int operand, ofstream& output) {
    cout << "PC=" << setw(8) << setfill('0') << hex << uppercase << PC << ", ";
    cout << "SP=" << setw(8) << setfill('0') << hex << uppercase << SP << ", ";
    cout << "A=" << setw(8) << setfill('0') << hex << uppercase << A << ", ";
    cout << "B=" << setw(8) << setfill('0') << hex << uppercase << B << "   ";
    cout << getMnemonic(opcode) << " ";
    if(mnemonics[opcode].isParam) cout << setw(8) << setfill('0') << hex << uppercase << operand;
    cout << "\n";

    output << "PC=" << setw(8) << setfill('0') << hex << uppercase << PC << ", ";
    output << "SP=" << setw(8) << setfill('0') << hex << uppercase << SP << ", ";
    output << "A=" << setw(8) << setfill('0') << hex << uppercase << A << ", ";
    output << "B=" << setw(8) << setfill('0') << hex << uppercase << B << "   ";
    output << getMnemonic(opcode) << " ";
    if(mnemonics[opcode].isParam) output << setw(8) << setfill('0') << hex << uppercase << operand;
    output << "\n";
}

// print the usage message in case of errors
void usage() {
    cout << "Usage: ./emu [option] file.o\n";
    cout << "Options:\n";
    cout << "\t-trace\tshow instruction trace\n";
    cout << "\t-before\tshow memory dump before execution\n";
    cout << "\t-after\tshow memory dump after execution\n";
    cout << "\t-isa\tdisplay ISA\n";
}

// function for program execution
void execute(ofstream& output) {
    int noIns = 0;
    PC = 0;
    while(true) {
        int32_t opcode, operand;
        getOpdOpr(memory[PC],&opcode,&operand);

        if(optEnable[0]) trace(opcode,operand,output);
        switch(opcode) {
            case 0:     // ldc
                B = A;
                A = operand;
                break;
            case 1:     // adc
                A = A + operand;
                break;
            case 2:     // ldl
                B = A;
                A = memory[SP+operand];
                break;
            case 3:     // stl
                memory[SP+operand] = A;
                A = B;
                break;
            case 4:     // ldnl
                A = memory[A+operand];
                break;
            case 5:     // stnl
                memory[A+operand] = B;
                break;
            case 6:     // add
                A = B + A;
                break;
            case 7:     // sub
                A = B - A;
                break;
            case 8:     // shl
                A = B << A;
                break;
            case 9:     // shr
                A = B >> A;
                break;
            case 10:    // adj
                SP = SP + operand;
                break;
            case 11:    // a2sp
                SP = A;
                A = B;
                break;
            case 12:    // sp2a
                B = A;
                A = SP;
                break;
            case 13:    // call
                B = A;
                A = PC;
                PC = PC + operand;
                break;
            case 14:    // return
                PC = A;
                A = B;
                break;
            case 15:    // brz
                if(A == 0) PC = PC + operand;
                break;
            case 16:    // brlz
                if(A < 0) PC = PC + operand;
                break;
            case 17:    // br
                PC = PC + operand;
                break;
            case 18:    // HALT
                cout << "\n" << dec << ++noIns << " instructions executed\n\n";
                output << "\n" << dec << noIns << " instructions executed\n\n";
                return;
        }
        PC++;
        noIns++;
    }
}

// function to print instruction set
void isa(ofstream& output) {
    cout << "Opcode Mnemonic Operand\n0      ldc      value\n1      adc      value\n2      ldl      value\n4      ldnl     value\n5      stnl     value\n";
    cout << "6      add\n7      sub\n8      shl\n9      shr\n10     adj      value\n11     a2sp\n12     sp2a\n13     call     offset\n14     return\n";
    cout << "15     brz      offset\n16     brlz     offset\n17     br       offset\n18     HALT\n       SET      value\n";

    output << "Opcode Mnemonic Operand\n0      ldc      value\n1      adc      value\n2      ldl      value\n4      ldnl     value\n5      stnl     value\n";
    output << "6      add\n7      sub\n8      shl\n9      shr\n10     adj      value\n11     a2sp\n12     sp2a\n13     call     offset\n14     return\n";
    output << "15     brz      offset\n16     brlz     offset\n17     br       offset\n18     HALT\n       SET      value\n";
}

// function to check which option was given so that relevant output is only printed
void setOptions(string option) {
    if(option == "-trace") {
        optEnable[0] = 1;
    } else if(option == "-before") {
        optEnable[1] = 1;
    } else if(option == "-after") {
        optEnable[2] = 1;
    } else if(option == "-isa") {
        optEnable[3] = 1;
    }
}

// function to get output filename from input filename
string getFileName(string fname) {
    size_t dotpos = fname.find(".");
    if(dotpos == string::npos) return fname;
    return fname.substr(0,dotpos);
}