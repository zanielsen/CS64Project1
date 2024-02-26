# MIPS Assembly Simulator

This program simulates the execution of MIPS assembly instructions. It reads instructions from a file named `instructions.txt` and executes them sequentially. The simulator supports R-type, I-type, and J-type instructions. The file containing the final code is `spimulator.cpp`.

## Instructions

- R-Type Instructions:
  - Supported instructions: `add`, `sub`, `addu`, `and`, `or`, `nor`, `sll`, `srl`, `sra`, `jr`
  - Syntax: `OP rd, rs, rt, shamt`
- I-Type Instructions:
  - Supported instructions: `addi`, `addiu`, `andi`, `ori`, `lui` (`beq`, `bne` untested but implemented)
  - Syntax: `OP rt, rs, immediate`
- J-Type Instructions:
  - Supported instructions: `j`, `jal`
  - Syntax: `OP address`

## Explanation

After reading in the instructions from the `instructions.txt` files, the program parses each of these to determine their OP code. After this, they are further parsed depending on their instruction type, stored in their respective type, and then inputted into our vector storing the instructions. It should be noted that labels are inputted with `0x04` at the start to reflect that they are a label. The address is then a 16-bit value which serves as a key in our memory map. With the instructions properly inputted and stored, the program begins executing at the start of the vector (we have not yet implemented what the `main` key should be, but this will be easily amendable to change the starting location). It then executes each instruction per specification until the stack pointer has a negative value, which is what we use to exit our program.

## Example `instructions.txt`

```
0x20040002
0x20050003
0x0C000006
0x20080002
0x00481020
0x201DFF9C
0x04000006
0x00851020
0x03E00008
```

## Output

The program will display the values of several registers (`$zero`, `$a0`, `$a1`, `$v0`, `$t0`) after executing the instructions, just to see what has been modified for the sake of testing.

## Note

- Ensure that the instructions in `instructions.txt` are valid MIPS assembly instructions in hexadecimal format.
- This simulator does not cover all MIPS instructions and features.
