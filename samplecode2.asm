.text
main:
    addi $t0, $zero, 3
    addi $t1, $zero, 2
    addi $t2, $zero, 1
    addi $t3, $zero, 4294967296

    add $t4, $t0, $t1
    add $t5 $t2 $t0

    addiu $t6 $t2 65536

    sub $t7 $t0 $t2

    addu $t8 $t3 $t0

    and $t9 $zero $t3
