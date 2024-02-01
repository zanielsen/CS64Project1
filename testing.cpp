#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <map>
#include <fstream>
#include <string>

using namespace std;

struct Type { 
    int op_code;
    void print() {
        cout << op_code << endl;
    }

    struct R {
        int32_t rs, rt, rd, shamt, func;

        void print() {
            cout << rs << " " << rt << " " << rd << " " << shamt << " " <<  func << endl;
        }
    }; 

    struct I {
        int32_t rs, rt;
        int16_t imm;

        void print() {
            cout << rs << " " << rt << " " << imm << endl;
        }
    };
    
    struct I i;
    struct R r;
};

Type buildR(int32_t& input) {
    struct Type fin;
    fin.r.rs = (input >> 21) & 31;
    fin.r.rt = (input >> 16) & 31;
    fin.r.rd = (input >> 11) & 31;
    fin.r.shamt = (input >> 6) & 31;
    fin.r.func = input & 63; // reads correctly
    return fin;
}

Type buildI(int32_t& input) {
    struct Type fin;
    fin.i.rs = input & 65011712 >> 21;
    fin.i.rt = input & 2031616 >> 16;
    fin.i.imm = input & 65535;
    return fin;
}

void add(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    rd = rs + rt;
}

void addi(int32_t& rt, int32_t rs, int16_t imm) {
    rt = rs + imm;
}

void addiu(int32_t& rt, int32_t rs, int16_t imm) {
    rt = rs + imm;
}

void sub(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    rd = rs - rt;
}

void addu(uint32_t& rd, uint32_t rs, uint32_t rt, int32_t shamt) {
    rd = rs + rt;
}

void andCom(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    rd = rs & rt;
}

void orCom(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    rd = rs | rt;
}

void nor(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    rd = !(rs | rt);
}

void sll(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    rd = rt << shamt;
}

void srl(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    rd = rt >> shamt;
}

void subu(uint32_t& rd, uint32_t rs, uint32_t rt, int32_t shamt) {
    rd = rs - rt;
}

void addiu(uint32_t& rt, uint32_t rs, uint16_t imm) {
    // uint32_t immPadded = 
    rt = rs + imm;
}

void sra(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    
}

void lui(int32_t& rt, int32_t rs, int16_t imm) {
    rt = imm << 16;
}

void andi(int32_t& rt, int32_t rs, int16_t imm) {
    // rt = rs & ;
}

void ori(int32_t& rt, int32_t rs, int16_t imm) {
    // rt = rs & ;
}


int main() {
    queue<Type> commands;
    string temp;
    typedef void (*rfunctions)(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt);
    typedef void (*ifunctions)(int32_t& rt, int32_t rs, int16_t imm);

    map<int32_t, rfunctions> rInstructions; 
    rInstructions[32] = &add;
    rInstructions[36] = &andCom;
    rInstructions[37] = &orCom;
    rInstructions[39] = &nor;
    rInstructions[0] = &sll;
    rInstructions[3] = &sra;
    rInstructions[2] = &srl;
    rInstructions[34] = &sub;

    map<int32_t, ifunctions> iInstructions;
    iInstructions[8] = &addi;
    iInstructions[8] = &addiu;
    iInstructions[8] = &lui;
    iInstructions[8] = &andi;
    iInstructions[8] = &ori;
    
    int32_t registers[26];
    registers[0] = 0;

    string line;
    int32_t num;
    ifstream instructionsFile("instructions.txt");

    if (instructionsFile.is_open()) {
        while (getline(instructionsFile, line)) {
            cout << "Instruction: " << line << '\n';

            line = line.substr(2, 8);

            istringstream(line) >> hex >> num;

            cout << "Line: " << line << ", Number: " << num << '\n';

            struct Type tempCommand;

            // R Type
            if ((num & 4227858432) == 0) {
                tempCommand.op_code = 0;
                tempCommand = buildR(num);
            } 
            
            // I Type
            else {
                tempCommand.op_code = (num & 4227858432);
                tempCommand = buildI(num);
            }

            commands.push(tempCommand);
        }

        instructionsFile.close();
    }

    else 
        cout << "Unable to open file." << endl;

    while (!commands.empty()) {
        struct Type tempCom = commands.front();
        commands.pop();
        if (tempCom.op_code == 0) {
            rInstructions[tempCom.r.func](registers[tempCom.r.rs], registers[tempCom.r.rt], registers[tempCom.r.rd], tempCom.r.shamt);
        } else {
            iInstructions[tempCom.op_code](registers[tempCom.i.rt], registers[tempCom.i.rs], tempCom.i.imm);
        }
    }
    // current status: works with hex code "221820"
    cout << registers[1] << endl;

    return 0;
}