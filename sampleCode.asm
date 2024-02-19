.text
main:
    # load a0 with 2, load a1 with 3
    addi $a0, $zero, 2
    addi $a1, $zero, 3

    # jump to function (address 7 --> since addressing starts at 0, the address is 6)
    jal function
    addi $t0, $zero, 2
    add $v0, $v0, $t0

function:
    add $v0, $a0, $a1
    jr $ra
