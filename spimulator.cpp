#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

map<int32_t, int> memory;
int32_t registers[32];

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
    fin.i.rs = (input >> 21) & 31;
    fin.i.rt = (input >> 16) & 31;
    fin.i.imm = input & 65535;
    return fin;
}

Type buildJ(int32_t& input, int index) {
    struct Type fin;
    fin.j.address = input & 67108863;
    memory[fin.j.address] = index;
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

void j(int32_t address) {
    registers[29] = memory[address];
}

void jal(int32_t address) {
    registers[31] = registers[29];
    registers[29] = memory[address];
}

void jr(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    registers[29] = rs;
}

void slt(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    
}

void sltu(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    
}

void slti(int32_t& rt, int32_t rs, int16_t imm) {
    
}

void sltiu(int32_t& rt, int32_t rs, int16_t imm) {
    
}

void bne(int32_t& rt, int32_t rs, int16_t imm) {

}

void beq(int32_t& rt, int32_t rs, int16_t imm) {

}

int main() {
    vector<Type> commands;
    string temp;
    typedef void (*rfunctions)(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt);
    typedef void (*ifunctions)(int32_t& rt, int32_t rs, int16_t imm);
    typedef void (*jfunctions)(int32_t imm);
    registers[0] = 0;
    registers[29] = 0; // sp
    registers[31] = 0; // ra


    map<int32_t, rfunctions> rInstructions; 
    rInstructions[32] = &add;
    rInstructions[36] = &andCom;
    rInstructions[37] = &orCom;
    rInstructions[39] = &nor;
    rInstructions[0] = &sll;
    rInstructions[3] = &sra;
    rInstructions[2] = &srl;
    rInstructions[34] = &sub;
    rInstructions[8] = &jr;

    map<int32_t, ifunctions> iInstructions;
    iInstructions[8] = &addi;
    iInstructions[9] = &addiu;
    iInstructions[15] = &lui;
    iInstructions[12] = &andi;
    iInstructions[13] = &ori;

    map<int32_t, jfunctions> jInstructions;
    jInstructions[2] = &j;
    jInstructions[3] = &jal;

    string line;
    int32_t num;
    ifstream instructionsFile("instructions.txt");
    int counter = 0;

    if (instructionsFile.is_open()) {
        while (getline(instructionsFile, line)) {
            
            line = line.substr(2, 8);

            istringstream(line) >> hex >> num;

            struct Type tempCommand;

            if ((num & 0xFC000000) == 0) {
                // R Type
                Type tempCommand = buildR(num);
                tempCommand.op_code = 0;
                commands.push_back(tempCommand);
                counter++;
            } else if (((num & 0xFC000000) >> 26) == 2 || ((num & 0xFC000000) >> 26) == 3) {
                // J Type
                Type tempCommand = buildJ(num, counter);
                tempCommand.op_code = (num & 0xFC000000) >> 26;
                commands.push_back(tempCommand);
                counter++;
            } else if ((num & 0xFC000000) == 0x04000000) {
                // Label
                memory[num & 0x03FFFFFF] = counter-1;
            } else {
                // I Type
                Type tempCommand = buildI(num);
                tempCommand.op_code = (num & 0xFC000000) >> 26;
                commands.push_back(tempCommand);
                counter++;
            }


        }
        instructionsFile.close();
    } else {
        cout << "Unable to open file." << endl;
    }

    counter = 0;
    while (registers[29] >= 0) {
        struct Type tempCom = commands[registers[29]];
        if (tempCom.op_code == 0) {
            rInstructions[tempCom.r.func](registers[tempCom.r.rd], registers[tempCom.r.rs], registers[tempCom.r.rt], tempCom.r.shamt);
        } else if (tempCom.op_code == 2 || tempCom.op_code == 3) {
            jInstructions[tempCom.op_code](tempCom.j.address);
        } else {
            iInstructions[tempCom.op_code](registers[tempCom.i.rt], registers[tempCom.i.rs], tempCom.i.imm);
        }
        registers[29]++;
        counter++;
    }

    cout << "Register 1: " << registers[1] << endl;
    cout << "Register 2: " << registers[2] << endl;
    cout << "Register 3: " << registers[3] << endl;
    cout << "Register 4: " << registers[4] << endl;
    cout << "Register 5: " << registers[5] << endl;

    return 0;
}
