.data
max_num: .word 1024
S31: .asciiz "OVERFLOW! "
S44: .asciiz "complete number: "
S48: .asciiz "  "
S53: .asciiz "  "
S59: .asciiz "---------------------------------------------------------------\\n"
S81: .asciiz " "
S89: .asciiz "  "
S97: .asciiz "The total is "
.text
move $fp $sp
j main
complete_num:
move $t8 $fp
move $fp $sp
subi $fp $fp 4
sw $t8 0($fp)
sw $ra -4($fp)
subi $sp $sp 4164
lw $s0 -4104($fp)
lw $s1 -4108($fp)
lw $s2 -4112($fp)
lw $s3 -4116($fp)
lw $s4 -4120($fp)
lw $s5 -4124($fp)
lw $s6 -4128($fp)
lw $s7 -4132($fp)
lw $t7 -4136($fp)
li $s1 2
LABEL0:
li $a1 -1
move $s2 $a1
move $s3 $s1
li $s0 1
LABEL1:
div $s1 $s0
mflo $a3
sw $a3 -4144($fp)
lw $a1 -4144($fp)
mul $s4 $a1 $s0
beq $s4 $s1 LABEL36
move $t0 $zero
j LABEL37
LABEL36:
li $t0 1
LABEL37:
beq $t0 $zero LABEL2
li $a2 1
add $s2 $s2 $a2
sub $s3 $s3 $s0
li $a2 1024
bge $s2 $a2 LABEL38
move $t0 $zero
j LABEL39
LABEL38:
li $t0 1
LABEL39:
beq $t0 $zero LABEL3
la $a0 S31
li $v0 4
syscall
LABEL3:
li $a2 1024
blt $s2 $a2 LABEL40
move $t0 $zero
j LABEL41
LABEL40:
li $t0 1
LABEL41:
beq $t0 $zero LABEL4
li $t9 4
mul $t9 $t9 $s2
subi $t8 $fp 4100
add $t8 $t8 $t9
sw $s0 0($t8)
LABEL4:
LABEL2:
li $a2 1
add $s0 $s0 $a2
blt $s0 $s1 LABEL42
move $t1 $zero
j LABEL43
LABEL42:
li $t1 1
LABEL43:
bne $t1 $zero LABEL1
li $a2 0
beq $s3 $a2 LABEL44
move $t0 $zero
j LABEL45
LABEL44:
li $t0 1
LABEL45:
beq $t0 $zero LABEL5
la $a0 S44
li $v0 4
syscall
move $a0 $s1
li $v0 1
syscall
li $s0 0
LABEL6:
li $t9 4
mul $t9 $t9 $s0
subi $t8 $fp 4100
add $t8 $t8 $t9
lw $t0 0($t8)
la $a0 S48
li $v0 4
syscall
move $a0 $t0
li $v0 1
syscall
li $a2 1
add $s0 $s0 $a2
ble $s0 $s2 LABEL46
move $t1 $zero
j LABEL47
LABEL46:
li $t1 1
LABEL47:
bne $t1 $zero LABEL6
la $a0 S53
li $v0 4
syscall
LABEL5:
li $a2 1
add $s1 $s1 $a2
li $a2 1024
blt $s1 $a2 LABEL48
move $t1 $zero
j LABEL49
LABEL48:
li $t1 1
LABEL49:
bne $t1 $zero LABEL0
la $a0 S59
li $v0 4
syscall
li $s7 0
li $t7 1
li $s5 2
LABEL7:
li $a2 1
sub $s6 $s5 $a2
li $s0 2
LABEL8:
div $s5 $s0
mflo $a3
sw $a3 -4156($fp)
lw $a1 -4156($fp)
mul $a3 $a1 $s0
sw $a3 -4140($fp)
lw $a1 -4140($fp)
beq $a1 $s5 LABEL50
move $t0 $zero
j LABEL51
LABEL50:
li $t0 1
LABEL51:
beq $t0 $zero LABEL9
li $t7 0
LABEL9:
li $a2 1
add $s0 $s0 $a2
ble $s0 $s6 LABEL52
move $t1 $zero
j LABEL53
LABEL52:
li $t1 1
LABEL53:
bne $t1 $zero LABEL8
li $a2 1
beq $t7 $a2 LABEL54
move $t0 $zero
j LABEL55
LABEL54:
li $t0 1
LABEL55:
beq $t0 $zero LABEL10
la $a0 S81
li $v0 4
syscall
move $a0 $s5
li $v0 1
syscall
li $a2 1
add $s7 $s7 $a2
li $a2 10
div $s7 $a2
mflo $a3
sw $a3 -4160($fp)
lw $a1 -4160($fp)
li $a2 10
mul $a3 $a1 $a2
sw $a3 -4140($fp)
lw $a1 -4140($fp)
beq $a1 $s7 LABEL56
move $t0 $zero
j LABEL57
LABEL56:
li $t0 1
LABEL57:
beq $t0 $zero LABEL11
la $a0 S89
li $v0 4
syscall
LABEL11:
LABEL10:
li $t7 1
li $a2 1
add $s5 $s5 $a2
li $a2 1024
ble $s5 $a2 LABEL58
move $t1 $zero
j LABEL59
LABEL58:
li $t1 1
LABEL59:
bne $t1 $zero LABEL7
la $a0 S97
li $v0 4
syscall
move $a0 $s7
li $v0 1
syscall
sw $s0 -4104($fp)
sw $s1 -4108($fp)
sw $s2 -4112($fp)
sw $s3 -4116($fp)
sw $s4 -4120($fp)
sw $s5 -4124($fp)
sw $s6 -4128($fp)
sw $s7 -4132($fp)
sw $t7 -4136($fp)
lw $ra -4($fp)
addi $sp $fp 4
lw $fp 0($fp)
jr $ra
main:
move $t8 $fp
move $fp $sp
subi $fp $fp 4
sw $t8 0($fp)
sw $ra -4($fp)
subi $sp $sp 8
jal complete_num
li $v0 10
syscall
