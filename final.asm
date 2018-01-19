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
move $s7 $fp
move $fp $sp
subi $fp $fp 4
sw $s7 0($fp)
sw $ra -4($fp)
subi $sp $sp 4152
li $t0 2
sw $t0 -4108($fp)
LABEL0:
li $t0 0
li $t1 1
sub $t0 $t0 $t1
sw $t0 -4144($fp)
lw $t0 -4144($fp)
sw $t0 -4112($fp)
lw $t0 -4108($fp)
sw $t0 -4116($fp)
li $t0 1
sw $t0 -4104($fp)
LABEL1:
lw $t0 -4108($fp)
lw $t1 -4104($fp)
div $t0 $t1
mflo $t0
sw $t0 -4144($fp)
lw $t0 -4144($fp)
lw $t1 -4104($fp)
mul $t0 $t0 $t1
sw $t0 -4148($fp)
lw $t0 -4148($fp)
sw $t0 -4120($fp)
lw $t0 -4120($fp)
lw $t1 -4108($fp)
beq $t0 $t1 LABEL12
sw $zero -4144($fp)
j LABEL13
LABEL12:
li $t2 1
sw $t2 -4144($fp)
LABEL13:
lw $t0 -4144($fp)
beq $t0 $zero LABEL2
lw $t0 -4112($fp)
li $t1 1
add $t0 $t0 $t1
sw $t0 -4144($fp)
lw $t0 -4144($fp)
sw $t0 -4112($fp)
lw $t0 -4116($fp)
lw $t1 -4104($fp)
sub $t0 $t0 $t1
sw $t0 -4144($fp)
lw $t0 -4144($fp)
sw $t0 -4116($fp)
lw $t0 -4112($fp)
li $t1 1024
bge $t0 $t1 LABEL14
sw $zero -4144($fp)
j LABEL15
LABEL14:
li $t2 1
sw $t2 -4144($fp)
LABEL15:
lw $t0 -4144($fp)
beq $t0 $zero LABEL3
la $a0 S31
li $v0 4
syscall
LABEL3:
lw $t0 -4112($fp)
li $t1 1024
blt $t0 $t1 LABEL16
sw $zero -4144($fp)
j LABEL17
LABEL16:
li $t2 1
sw $t2 -4144($fp)
LABEL17:
lw $t0 -4144($fp)
beq $t0 $zero LABEL4
lw $t0 -4104($fp)
lw $t1 -4112($fp)
li $s6 4
mul $s6 $s6 $t1
subi $s7 $fp 4100
add $s7 $s7 $s6
sw $t0 0($s7)
LABEL4:
LABEL2:
lw $t0 -4104($fp)
li $t1 1
add $t0 $t0 $t1
sw $t0 -4144($fp)
lw $t0 -4144($fp)
sw $t0 -4104($fp)
lw $t0 -4104($fp)
lw $t1 -4108($fp)
blt $t0 $t1 LABEL18
sw $zero -4148($fp)
j LABEL19
LABEL18:
li $t2 1
sw $t2 -4148($fp)
LABEL19:
lw $t0 -4148($fp)
bne $t0 $zero LABEL1
lw $t0 -4116($fp)
li $t1 0
beq $t0 $t1 LABEL20
sw $zero -4144($fp)
j LABEL21
LABEL20:
li $t2 1
sw $t2 -4144($fp)
LABEL21:
lw $t0 -4144($fp)
beq $t0 $zero LABEL5
la $a0 S44
li $v0 4
syscall
lw $t0 -4108($fp)
move $a0 $t0
li $v0 1
syscall
li $t0 0
sw $t0 -4104($fp)
LABEL6:
lw $t0 -4104($fp)
li $s6 4
mul $s6 $s6 $t0
subi $s7 $fp 4100
add $s7 $s7 $s6
lw $t1 0($s7)
sw $t1 -4144($fp)
la $a0 S48
li $v0 4
syscall
lw $t0 -4144($fp)
move $a0 $t0
li $v0 1
syscall
lw $t0 -4104($fp)
li $t1 1
add $t0 $t0 $t1
sw $t0 -4144($fp)
lw $t0 -4144($fp)
sw $t0 -4104($fp)
lw $t0 -4104($fp)
lw $t1 -4112($fp)
ble $t0 $t1 LABEL22
sw $zero -4148($fp)
j LABEL23
LABEL22:
li $t2 1
sw $t2 -4148($fp)
LABEL23:
lw $t0 -4148($fp)
bne $t0 $zero LABEL6
la $a0 S53
li $v0 4
syscall
LABEL5:
lw $t0 -4108($fp)
li $t1 1
add $t0 $t0 $t1
sw $t0 -4144($fp)
lw $t0 -4144($fp)
sw $t0 -4108($fp)
lw $t0 -4108($fp)
li $t1 1024
blt $t0 $t1 LABEL24
sw $zero -4148($fp)
j LABEL25
LABEL24:
li $t2 1
sw $t2 -4148($fp)
LABEL25:
lw $t0 -4148($fp)
bne $t0 $zero LABEL0
la $a0 S59
li $v0 4
syscall
li $t0 0
sw $t0 -4132($fp)
li $t0 1
sw $t0 -4136($fp)
li $t0 2
sw $t0 -4124($fp)
LABEL7:
lw $t0 -4124($fp)
li $t1 1
sub $t0 $t0 $t1
sw $t0 -4144($fp)
lw $t0 -4144($fp)
sw $t0 -4128($fp)
li $t0 2
sw $t0 -4104($fp)
LABEL8:
lw $t0 -4124($fp)
lw $t1 -4104($fp)
div $t0 $t1
mflo $t0
sw $t0 -4144($fp)
lw $t0 -4144($fp)
lw $t1 -4104($fp)
mul $t0 $t0 $t1
sw $t0 -4148($fp)
lw $t0 -4148($fp)
sw $t0 -4140($fp)
lw $t0 -4140($fp)
lw $t1 -4124($fp)
beq $t0 $t1 LABEL26
sw $zero -4144($fp)
j LABEL27
LABEL26:
li $t2 1
sw $t2 -4144($fp)
LABEL27:
lw $t0 -4144($fp)
beq $t0 $zero LABEL9
li $t0 0
sw $t0 -4136($fp)
LABEL9:
lw $t0 -4104($fp)
li $t1 1
add $t0 $t0 $t1
sw $t0 -4144($fp)
lw $t0 -4144($fp)
sw $t0 -4104($fp)
lw $t0 -4104($fp)
lw $t1 -4128($fp)
ble $t0 $t1 LABEL28
sw $zero -4148($fp)
j LABEL29
LABEL28:
li $t2 1
sw $t2 -4148($fp)
LABEL29:
lw $t0 -4148($fp)
bne $t0 $zero LABEL8
lw $t0 -4136($fp)
li $t1 1
beq $t0 $t1 LABEL30
sw $zero -4144($fp)
j LABEL31
LABEL30:
li $t2 1
sw $t2 -4144($fp)
LABEL31:
lw $t0 -4144($fp)
beq $t0 $zero LABEL10
la $a0 S81
li $v0 4
syscall
lw $t0 -4124($fp)
move $a0 $t0
li $v0 1
syscall
lw $t0 -4132($fp)
li $t1 1
add $t0 $t0 $t1
sw $t0 -4144($fp)
lw $t0 -4144($fp)
sw $t0 -4132($fp)
lw $t0 -4132($fp)
li $t1 10
div $t0 $t1
mflo $t0
sw $t0 -4144($fp)
lw $t0 -4144($fp)
li $t1 10
mul $t0 $t0 $t1
sw $t0 -4148($fp)
lw $t0 -4148($fp)
sw $t0 -4140($fp)
lw $t0 -4140($fp)
lw $t1 -4132($fp)
beq $t0 $t1 LABEL32
sw $zero -4144($fp)
j LABEL33
LABEL32:
li $t2 1
sw $t2 -4144($fp)
LABEL33:
lw $t0 -4144($fp)
beq $t0 $zero LABEL11
la $a0 S89
li $v0 4
syscall
LABEL11:
LABEL10:
li $t0 1
sw $t0 -4136($fp)
lw $t0 -4124($fp)
li $t1 1
add $t0 $t0 $t1
sw $t0 -4144($fp)
lw $t0 -4144($fp)
sw $t0 -4124($fp)
lw $t0 -4124($fp)
li $t1 1024
ble $t0 $t1 LABEL34
sw $zero -4148($fp)
j LABEL35
LABEL34:
li $t2 1
sw $t2 -4148($fp)
LABEL35:
lw $t0 -4148($fp)
bne $t0 $zero LABEL7
la $a0 S97
li $v0 4
syscall
lw $t0 -4132($fp)
move $a0 $t0
li $v0 1
syscall
lw $ra -4($fp)
addi $sp $fp 4
lw $fp 0($fp)
jr $ra
main:
move $s7 $fp
move $fp $sp
subi $fp $fp 4
sw $s7 0($fp)
sw $ra -4($fp)
subi $sp $sp 8
jal complete_num
li $v0 10
syscall
