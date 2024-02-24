.text
main:
    addi $t0, $zero, 3
    addi $t1, $zero, 2
    addi $t2, $zero, 1
    addi $t3, $zero, 4294967296
    addi $t8, $zero, -4294967296

    or $t4, $t3, $zero
    nor $t5, $t3, $zero
    sll $t6, $t1, 2
    srl $t7, $t1, 1
    subu $t9, $t3, $t8
    sra $t9, $t3, 0x2
    lui $s0, 44113
    andi $s1, $t0, 1
    ori $s2, $t0, 0x7