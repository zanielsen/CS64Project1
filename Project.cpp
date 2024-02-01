#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <map>


using namespace std;

struct Type { };

struct R : Type {
    int32_t rs, rt, rd, shamt, func;
};

struct I : Type {
    int32_t rs, rt;
    int16_t imm;
};

R buildR(int32_t& input) {
    struct R fin;
    fin.rs = input & 65011712 >> 21;
    fin.rt = input & 2031616 >> 16;
    fin.rd = input & 63488 >> 11;
    fin.shamt = input & 1984 >> 6;
    fin.func = input & 63;
}

I buildI(int32_t& input) {
    struct I fin;
    fin.rs = input & 65011712 >> 21;
    fin.rt = input & 2031616 >> 16;
    fin.imm = input & 65535;
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


int main() {
    queue<Type> commands;
    string temp;
    typedef void (*rfunctions)(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt);
    typedef void (*ifunctions)(int32_t& rt, int32_t rs, int16_t imm);

    map<int32_t, rfunctions> rInstructions {
        {32, &add},
        {33, &addu},
        {36, &and},
        {37, &or},
        {39, &nor},
        {0, &sll},
        {3, &sra},
        {2, &srl},
        {34, &sub},
        {35, &subu}
    };

    map<int32_t, ifunctions> iInstructions {
        {8, &addi},
        {9, &addiu},
        {15, &lui},
        {12, &andi},
        {13, &ori}
    };
    
    int32_t registers[26];

    while (getline(cin, temp)) {
        if (!(temp == "done")) {
            int32_t num;
            istringstream(temp) >> hex >> num;
            struct Type tempCommand;
            if (num & 4227858432 == 0) {
                tempCommand = buildR(num);
            } else {
                tempCommand = buildI(num);
            }
            commands.push(temp                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              Command);
        }   
    }

    while (!commands.empty()) {
        
    }

    return 0;
}

void addu(uint32_t& rd, uint32_t rs, uint32_t rt, int32_t shamt) {
    rd = rs + rt;
}

void and(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
    rd = rs & rt;
}

void or(int32_t& rd, int32_t rs, int32_t rt, int32_t shamt) {
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
    uint32_t immPadded = 
    rt = rs + imm;
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