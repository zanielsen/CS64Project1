#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <map>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Type { 
    int op_code;
    void print() {
        cout << op_code << endl;
    }

    struct R {
        int32_t rs, rt, rd, shamt, func;

        void print() {
            cout << rd << " " << rs << " " << rt << " " << shamt << " " <<  func << endl;
        }
    }; 

    struct I {
        int32_t rs, rt;
        int16_t imm;

        void print() {
            cout << rt << " " << rs << " " << imm << endl;
        }
    };

    struct J {
        int32_t address;

        void print() {
            cout << address << endl;
        }
    };
    
    struct I i;
    struct R r;
    struct J j;
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
    fin.op_code = ((input >> 26) & 63);
    fin.i.rt = (input >> 21) & 31;
    fin.i.rs = (input >> 16) & 31;
    fin.i.imm = input & 65535;
    return fin;
}

Type buildJ(int32_t& input) {
    struct Type fin;
    fin.op_code = ((input >> 26) & 63);
    fin.i.address = input & 67108863;
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
    uint32_t zeroExtImm = imm;
    uint32_t oneImm = 4294901760;
    int firstBit = imm >> 15;
    uint32_t signExtImm;

    if (firstBit == 1) {
        signExtImm = oneImm || zeroExtImm;
    } else {
        signExtImm = zeroExtImm;
    }

    rt = rs + signExtImm;
}

void sra(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    rd = rt >> shamt;
}

void lui(int32_t& rt, int32_t rs, int16_t imm) {
    rt = imm << 16;
}

void andi(int32_t& rt, int32_t rs, int16_t imm) {
    uint32_t zeroExtImm = imm;
    rt = rs && zeroExtImm;
}

void ori(int32_t& rt, int32_t rs, int16_t imm) {
    uint32_t zeroExtImm = imm;
    rt = rs || zeroExtImm;
}

void j() {

}

void jal() {

}

int main() {

    vector<Type> commands(0);

    string temp;
    typedef void (*rfunctions)(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt);
    typedef void (*ifunctions)(int32_t& rt, int32_t rs, int16_t imm);
    typedef void (*jfunctions)(int32_t& address);

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
    iInstructions[9] = &addiu;
    iInstructions[15] = &lui;
    iInstructions[12] = &andi;
    iInstructions[13] = &ori;

    map<int32_t, jfunctions> jInstructions;
    jInstructions[2] = &j;
    jInstructions[3] = &jal;
    
    int32_t registers[26];
    registers[0] = 0;

    string line;
    int32_t num;
    ifstream instructionsFile("instructions.txt");
    int counter = 0;
    int numInstructionsToRun;

    cout << "Number of Instructions to Run: ";
    cin >> numInstructionsToRun;

    if (instructionsFile.is_open()) {
        while (counter < numInstructionsToRun && getline(instructionsFile, line)) {
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

            // J Type
            else if ((num & 4227858432) == 2 || (num & 4227858432) == 3) {
                tempCommand.op_code = ((num >> 26) & 63);
                tempCommand = buildJ(num);
            }
            
            // I Type
            else {
                tempCommand.op_code = ((num >> 26) & 63);
                tempCommand = buildI(num);
            }

            commands.push_back(tempCommand);
            counter++;
        }

        instructionsFile.close();
    }

    else 
        cout << "Unable to open file." << endl;

    int index = 0;

    while (index < commands.size()) {
        struct Type tempCom = commands.at(index);

        if (tempCom.op_code == 0) {
            rInstructions[tempCom.r.func](registers[tempCom.r.rd], registers[tempCom.r.rs], registers[tempCom.r.rt], tempCom.r.shamt);
        } 

        else if (tempCom.op_code == 2 || tempCom.op_code == 3) {
            jInstructions[tempCom.op_code](registers[tempCom.j.address]);
        }
        
        else {
            iInstructions[tempCom.op_code](registers[tempCom.i.rt], registers[tempCom.i.rs], tempCom.i.imm);
        }

        index++;
    }

    // current status: works with hex code "221820" and "20220003"

    cout << "Register 1: " << registers[1] << endl;
    cout << "Register 2: " << registers[2] << endl;
    cout << "Register 3: " << registers[3] << endl;
    cout << "Register 4: " << registers[4] << endl;
    cout << "Register 5: " << registers[5] << endl;

    return 0;
}

// CODE USING COMMANDS QUEUE
/*
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
    iInstructions[9] = &addiu;
    iInstructions[15] = &lui;
    iInstructions[12] = &andi;
    iInstructions[13] = &ori;
    
    int32_t registers[26];
    registers[0] = 0;

    string line;
    int32_t num;
    ifstream instructionsFile("instructions.txt");
    int counter = 0;
    int numInstructionsToRun;

    cout << "Number of Instructions to Run: ";
    cin >> numInstructionsToRun;

    if (instructionsFile.is_open()) {
        while (counter < numInstructionsToRun && getline(instructionsFile, line)) {
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
                tempCommand.op_code = ((num >> 26) & 63);
                tempCommand = buildI(num);
            }

            commands.push(tempCommand);
            counter++;
        }

        instructionsFile.close();
    }

    else 
        cout << "Unable to open file." << endl;

    while (!commands.empty()) {
        struct Type tempCom = commands.front();
        commands.pop();

        if (tempCom.op_code == 0) {
            rInstructions[tempCom.r.func](registers[tempCom.r.rd], registers[tempCom.r.rs], registers[tempCom.r.rt], tempCom.r.shamt);
        } else {
            iInstructions[tempCom.op_code](registers[tempCom.i.rt], registers[tempCom.i.rs], tempCom.i.imm);
        }
    }
    */