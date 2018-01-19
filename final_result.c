#include<stdio.h>
//外部变量
extern struct table Table;
extern struct table new_Table;
extern int var_num;
extern struct mid_code_list CODE_LIST;
extern struct func_blocks FUNC_BLOCKS;
//内部结构体设计，存放各个寄存器对应的变量名字
struct reg_var{
    char reg_name[20];
    char var_name[100];
    int is_var;//0: const 1: just var -1: no data
};
//全局变量
FILE* final_code;                                          //最终结果
//struct reg_var REG_VAR_LIST[17];
//int t_reg_index = 0;
//int s_reg_index = 10;
char now_func_name[100];
int new_const_flag = 0;
int new_var_flag = 0;
int new_para_flag = 0;
int new_global_flag = 0;
//函数声明
void get_new_Table();
void data_area_generate();
void generate_all_final_code();
void generate_text();
int new_index_in_tab(char name[]);
int get_func_para_num(char name[]);
void load_data(char reg_name[],char var_name[]);
void store_data(char reg_name[],char var_name[]);
int get_func_total_size(char name[]);
//函数实现
void get_new_Table(){
    int index,f_index,i;
    new_Table.num_of_func = 0;
    new_Table.list_index = -1;
    for(index=0;index<Table.index_of_func[0];index++){
         new_Table.list_index++;
         new_Table.List[new_Table.list_index].in_address = Table.List[index].in_address;
         new_Table.List[new_Table.list_index].kind = Table.List[index].kind;
         new_Table.List[new_Table.list_index].size = Table.List[index].size;
         new_Table.List[new_Table.list_index].type = Table.List[index].type;
         new_Table.List[new_Table.list_index].value = Table.List[index].value;
         strcpy(new_Table.List[new_Table.list_index].name,Table.List[index].name);
    }
    for(f_index=0;f_index<Table.num_of_func;f_index++){
        new_Table.index_of_func[new_Table.num_of_func] = new_Table.list_index + 1;
        if(f_index != Table.num_of_func-1){
            for(index=Table.index_of_func[f_index];index<Table.index_of_func[f_index+1];index++){
                if(Table.List[index].name[0] != '$'){
                    new_Table.list_index++;
                    new_Table.List[new_Table.list_index].in_address = Table.List[index].in_address;
                    new_Table.List[new_Table.list_index].kind = Table.List[index].kind;
                    new_Table.List[new_Table.list_index].size = Table.List[index].size;
                    new_Table.List[new_Table.list_index].type = Table.List[index].type;
                    new_Table.List[new_Table.list_index].value = Table.List[index].value;
                    strcpy(new_Table.List[new_Table.list_index].name,Table.List[index].name);
                }
            }
            //接下来添加修改后代码的临时变量
            for(index=FUNC_BLOCKS.FUNC_LIST[f_index].start_num;index<FUNC_BLOCKS.FUNC_LIST[f_index+1].start_num;index++){
                if(CODE_LIST.MID_CODE[index].is_effective == 0)
                    continue;
                if(strcmp(CODE_LIST.MID_CODE[index].result,CODE_EMPTY)==0)
                    continue;
                if(CODE_LIST.MID_CODE[index].result[0] == '$'){
                    for(i=new_Table.index_of_func[new_Table.num_of_func];i<=new_Table.list_index;i++){
                        if(strcmp(CODE_LIST.MID_CODE[index].result,new_Table.List[i].name)==0)
                            break;
                    }
                    if(i>new_Table.list_index){
                        new_Table.list_index++;
                        new_Table.List[new_Table.list_index].in_address = new_Table.List[new_Table.list_index-1].in_address + 1;
                        new_Table.List[new_Table.list_index].kind = 1;
                        new_Table.List[new_Table.list_index].size = -1;
                        new_Table.List[new_Table.list_index].type = 1;
                        new_Table.List[new_Table.list_index].value = 0;
                        strcpy(new_Table.List[new_Table.list_index].name,CODE_LIST.MID_CODE[index].result);
                    }
                }
            }
        }
        else{
            for(index=Table.index_of_func[f_index];index<=Table.list_index;index++){
                if(Table.List[index].name[0] != '$'){
                    new_Table.list_index++;
                    new_Table.List[new_Table.list_index].in_address = Table.List[index].in_address;
                    new_Table.List[new_Table.list_index].kind = Table.List[index].kind;
                    new_Table.List[new_Table.list_index].size = Table.List[index].size;
                    new_Table.List[new_Table.list_index].type = Table.List[index].type;
                    new_Table.List[new_Table.list_index].value = Table.List[index].value;
                    strcpy(new_Table.List[new_Table.list_index].name,Table.List[index].name);
                }
            }
            //接下来添加修改后代码的临时变量
            for(index=FUNC_BLOCKS.FUNC_LIST[f_index].start_num;index<CODE_LIST.code_num;index++){
                if(CODE_LIST.MID_CODE[index].is_effective == 0)
                    continue;
                if(strcmp(CODE_LIST.MID_CODE[index].result,CODE_EMPTY)==0)
                    continue;
                if(CODE_LIST.MID_CODE[index].result[0] == '$'){
                    for(i=new_Table.index_of_func[new_Table.num_of_func];i<=new_Table.list_index;i++){
                        if(strcmp(CODE_LIST.MID_CODE[index].result,new_Table.List[i].name)==0)
                            break;
                    }
                    if(i>new_Table.list_index){
                        new_Table.list_index++;
                        new_Table.List[new_Table.list_index].in_address = new_Table.List[new_Table.list_index-1].in_address + 1;
                        new_Table.List[new_Table.list_index].kind = 1;
                        new_Table.List[new_Table.list_index].size = -1;
                        new_Table.List[new_Table.list_index].type = 1;
                        new_Table.List[new_Table.list_index].value = 0;
                        strcpy(new_Table.List[new_Table.list_index].name,CODE_LIST.MID_CODE[index].result);
                    }
                }
            }
        }
        new_Table.num_of_func++;
    }
}

void load_data(char reg_name[],char var_name[]){
    int address;
    if(islower(var_name[0]) || var_name[0] == '_' || var_name[0] == '$'){//要取的是变量或者常量
        address = new_index_in_tab(var_name);
        if(new_global_flag){
            if(new_const_flag){
                fprintf(final_code,"li %s %d\n",reg_name,address);
            }
            else if(new_var_flag){
                fprintf(final_code,"la $s7 %s\n",var_name);
                fprintf(final_code,"lw %s 0($s7)\n",reg_name);
            }
        }
        else{
            if(new_const_flag){
                fprintf(final_code,"li %s %d\n",reg_name,address);
            }
            else if(new_var_flag){
                fprintf(final_code,"lw %s %d($fp)\n",reg_name,-(address+1)*4);
            }
            else if(new_para_flag){
                fprintf(final_code,"lw %s %d($fp)\n",reg_name,-(address+1)*4);
            }
        }
    }
    else{ //载入的是数字
        fprintf(final_code,"li %s %s\n",reg_name,var_name);
    }
}

void store_data(char reg_name[],char var_name[]){
    address = new_index_in_tab(var_name);
    if(new_global_flag){ //全局变量，应该直接存回.data对应的名字中
        fprintf(final_code,"la $s7 %s\n",var_name);
        fprintf(final_code,"sw %s 0($s7)\n",reg_name);
    }
    else{
        fprintf(final_code,"sw %s %d($fp)\n",reg_name,-(address+1)*4);
    }
}
//根据符号表把全局量，根据MIDCODE把字符串和中间变量写入.data
void data_area_generate(){
    int index,first_func_index;
    first_func_index = new_Table.index_of_func[0];
    for(index=0;index<first_func_index;index++){
        switch(new_Table.List[index].kind){
            case 0:{ //const
                fprintf(final_code,"%s: .word %d\n",new_Table.List[index].name,new_Table.List[index].value);
                break;
            }
            case 1:{ //var
                if(new_Table.List[index].size != -1){
                    fprintf(final_code,"%s: .space %d\n",new_Table.List[index].name,new_Table.List[index].size*4);
                }
                else{
                    fprintf(final_code,"%s: .word 0\n",new_Table.List[index].name);
                }
            }
        }
    }
    for(index=0;index<CODE_LIST.code_num;index++){
        if((CODE_LIST.MID_CODE[index].op == 28) && (strcmp(CODE_LIST.MID_CODE[index].src1,CODE_EMPTY)!= 0))
            fprintf(final_code,"S%d: .asciiz %s\n",index,CODE_LIST.MID_CODE[index].src1);
    }
}
//生成.text段
void generate_text(){
    int index,i;
    int address,size,para_num;
    char label1[50];
    char label2[50];
    for(index=0;index<CODE_LIST.code_num;index++){
        if(CODE_LIST.MID_CODE[index].is_effective == 0)
            continue;
        switch(CODE_LIST.MID_CODE[index].op){
            case 7:
            case 8:
            case 9:
            {
                //init_reg_var_list();
                strcpy(now_func_name,CODE_LIST.MID_CODE[index].src1);
                //形成新的运行栈
                size = get_func_total_size(CODE_LIST.MID_CODE[index].src1);
                size = size + 2;
                //size一共对应的地方是所有局部变量常量+返回地址、返回上一个AR的fp地址
                fprintf(final_code,"%s:\n",CODE_LIST.MID_CODE[index].src1);
                para_num = get_func_para_num(now_func_name);
                for(i=0;i<para_num;i++){
                    fprintf(final_code,"lw $s7 0($sp)\n");
                    fprintf(final_code,"subi $sp $sp 8\n");
                    fprintf(final_code,"sw $s7 0($sp)\n");
                    fprintf(final_code,"addi $sp $sp 12\n");
                }
                fprintf(final_code,"move $s7 $fp\n");
                fprintf(final_code,"move $fp $sp\n");
                fprintf(final_code,"subi $fp $fp 4\n");
                fprintf(final_code,"sw $s7 0($fp)\n");
                fprintf(final_code,"sw $ra -4($fp)\n");
                fprintf(final_code,"subi $sp $sp %d\n",size*4);
                break;
            }
            case 11:{
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                store_data("$t0",CODE_LIST.MID_CODE[index].result);
                break;
            }
            case 12:{
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                load_data("$t1",CODE_LIST.MID_CODE[index].src2);
                fprintf(final_code,"li $s6 4\n");
                fprintf(final_code,"mul $s6 $s6 $t1\n");
                address = new_index_in_tab(CODE_LIST.MID_CODE[index].result);
                if(new_global_flag){
                    fprintf(final_code,"la $s7 %s\n",CODE_LIST.MID_CODE[index].result);
                    fprintf(final_code,"add $s7 $s7 $s6\n");
                    fprintf(final_code,"sw $t0 0($s7)\n");
                }
                else{
                    fprintf(final_code,"subi $s7 $fp %d\n",(address+1)*4);
                    fprintf(final_code,"add $s7 $s7 $s6\n");
                    fprintf(final_code,"sw $t0 0($s7)\n");
                }
                break;
            }
            /*case 13:{
                save_scene();
                break;
            }*/
            case 14:{//分为要赋值和不要
                /*1.保存现场
                  2.jal
                  3.可能需要把v0复制给result
                */
                fprintf(final_code,"jal %s\n",CODE_LIST.MID_CODE[index].src1);
                if(strcmp(CODE_EMPTY,CODE_LIST.MID_CODE[index].result)!= 0){
                    fprintf(final_code,"move $t0 $v0\n");
                    store_data("$t0",CODE_LIST.MID_CODE[index].result);
                }
                break;
            }
            case 15:{
                fprintf(final_code,"jal %s\n",CODE_LIST.MID_CODE[index].src1);
                break;
            }
            case 16:{
                generate_label(label1);
                generate_label(label2);
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                load_data("$t1",CODE_LIST.MID_CODE[index].src2);
                fprintf(final_code,"ble $t0 $t1 %s\n",label1);
                store_data("$zero",CODE_LIST.MID_CODE[index].result);
                fprintf(final_code,"j %s\n",label2);
                fprintf(final_code,"%s:\n",label1);
                fprintf(final_code,"li $t2 1\n");
                store_data("$t2",CODE_LIST.MID_CODE[index].result);
                fprintf(final_code,"%s:\n",label2);
                break;
            }
            case 17:{
                generate_label(label1);
                generate_label(label2);
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                load_data("$t1",CODE_LIST.MID_CODE[index].src2);
                fprintf(final_code,"blt $t0 $t1 %s\n",label1);
                store_data("$zero",CODE_LIST.MID_CODE[index].result);
                fprintf(final_code,"j %s\n",label2);
                fprintf(final_code,"%s:\n",label1);
                fprintf(final_code,"li $t2 1\n");
                store_data("$t2",CODE_LIST.MID_CODE[index].result);
                fprintf(final_code,"%s:\n",label2);
                break;
            }
            case 18:{
                generate_label(label1);
                generate_label(label2);
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                load_data("$t1",CODE_LIST.MID_CODE[index].src2);
                fprintf(final_code,"bge $t0 $t1 %s\n",label1);
                store_data("$zero",CODE_LIST.MID_CODE[index].result);
                fprintf(final_code,"j %s\n",label2);
                fprintf(final_code,"%s:\n",label1);
                fprintf(final_code,"li $t2 1\n");
                store_data("$t2",CODE_LIST.MID_CODE[index].result);
                fprintf(final_code,"%s:\n",label2);
                break;
            }
            case 19:{
                generate_label(label1);
                generate_label(label2);
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                load_data("$t1",CODE_LIST.MID_CODE[index].src2);
                fprintf(final_code,"bgt $t0 $t1 %s\n",label1);
                store_data("$zero",CODE_LIST.MID_CODE[index].result);
                fprintf(final_code,"j %s\n",label2);
                fprintf(final_code,"%s:\n",label1);
                fprintf(final_code,"li $t2 1\n");
                store_data("$t2",CODE_LIST.MID_CODE[index].result);
                fprintf(final_code,"%s:\n",label2);
                break;
            }
            case 20:{
                generate_label(label1);
                generate_label(label2);
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                load_data("$t1",CODE_LIST.MID_CODE[index].src2);
                fprintf(final_code,"beq $t0 $t1 %s\n",label1);
                store_data("$zero",CODE_LIST.MID_CODE[index].result);
                fprintf(final_code,"j %s\n",label2);
                fprintf(final_code,"%s:\n",label1);
                fprintf(final_code,"li $t2 1\n");
                store_data("$t2",CODE_LIST.MID_CODE[index].result);
                fprintf(final_code,"%s:\n",label2);
                break;
            }
            case 21:{
                generate_label(label1);
                generate_label(label2);
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                load_data("$t1",CODE_LIST.MID_CODE[index].src2);
                fprintf(final_code,"bne $t0 $t1 %s\n",label1);
                store_data("$zero",CODE_LIST.MID_CODE[index].result);
                fprintf(final_code,"j %s\n",label2);
                fprintf(final_code,"%s:\n",label1);
                fprintf(final_code,"li $t2 1\n");
                store_data("$t2",CODE_LIST.MID_CODE[index].result);
                fprintf(final_code,"%s:\n",label2);
                break;
            }
            case 22:{
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                fprintf(final_code,"beq $t0 $zero %s\n",CODE_LIST.MID_CODE[index].src2);
                break;
            }
            case 23:{
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                fprintf(final_code,"bne $t0 $zero %s\n",CODE_LIST.MID_CODE[index].src2);
                break;
            }
            case 24:{
                fprintf(final_code,"%s:\n",CODE_LIST.MID_CODE[index].src1);
                break;
            }
            //?
            case 25:{
                fprintf(final_code,"lw $ra -4($fp)\n");
                fprintf(final_code,"addi $sp $fp 4\n");
                fprintf(final_code,"lw $fp 0($fp)\n");
                fprintf(final_code,"jr $ra\n");
                break;
            }
            case 26:{
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                fprintf(final_code,"move $v0 $t0\n");
                fprintf(final_code,"lw $ra -4($fp)\n");
                fprintf(final_code,"addi $sp $fp 4\n");
                fprintf(final_code,"lw $fp 0($fp)\n");
                fprintf(final_code,"jr $ra\n");
                break;
            }
            case 27:{
                if(strcmp(CODE_INT,CODE_LIST.MID_CODE[index].src1) == 0){
                    fprintf(final_code,"li $v0 5\n");
                    fprintf(final_code,"syscall\n");
                    fprintf(final_code,"move $t0 $v0\n");
                    store_data("$t0",CODE_LIST.MID_CODE[index].result);
                }
                else{
                    fprintf(final_code,"li $v0 12\n");
                    fprintf(final_code,"syscall\n");
                    fprintf(final_code,"move $t0 $v0\n");
                    store_data("$t0",CODE_LIST.MID_CODE[index].result);
                }
                break;
            }
            case 28:{
                if(strcmp(CODE_LIST.MID_CODE[index].src1,CODE_EMPTY)!=0){
                    fprintf(final_code,"la $a0 S%d\n",index);
                    fprintf(final_code,"li $v0 4\n");
                    fprintf(final_code,"syscall\n");
                }
                if(strcmp(CODE_LIST.MID_CODE[index].src2,CODE_EMPTY)!=0){
                    load_data("$t0",CODE_LIST.MID_CODE[index].src2);
                    if(strcmp(CODE_LIST.MID_CODE[index].result,CODE_CHAR)==0){
                        fprintf(final_code,"move $a0 $t0\n");
                        fprintf(final_code,"li $v0 11\n");
                        fprintf(final_code,"syscall\n");
                    }
                    else{
                        fprintf(final_code,"move $a0 $t0\n");
                        fprintf(final_code,"li $v0 1\n");
                        fprintf(final_code,"syscall\n");
                    }
                }
                break;
            }
            case 29:{
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                load_data("$t1",CODE_LIST.MID_CODE[index].src2);
                fprintf(final_code,"add $t0 $t0 $t1\n");
                store_data("$t0",CODE_LIST.MID_CODE[index].result);
                break;
            }
            case 30:{
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                load_data("$t1",CODE_LIST.MID_CODE[index].src2);
                fprintf(final_code,"sub $t0 $t0 $t1\n");
                store_data("$t0",CODE_LIST.MID_CODE[index].result);
                break;
            }
            case 31:{
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                load_data("$t1",CODE_LIST.MID_CODE[index].src2);
                fprintf(final_code,"mul $t0 $t0 $t1\n");
                store_data("$t0",CODE_LIST.MID_CODE[index].result);
                break;
            }
            case 32:{
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                load_data("$t1",CODE_LIST.MID_CODE[index].src2);
                fprintf(final_code,"div $t0 $t1\n");
                fprintf(final_code,"mflo $t0\n");
                store_data("$t0",CODE_LIST.MID_CODE[index].result);
                break;
            }
            case 33:{
                fprintf(final_code,"j %s\n",CODE_LIST.MID_CODE[index].src1);
                break;
            }
            case 34:{
                fprintf(final_code,"li $v0 10\n");
                fprintf(final_code,"syscall\n");
                break;
            }
            case 35:{
                load_data("$t0",CODE_LIST.MID_CODE[index].src1);
                fprintf(final_code,"subi $sp $sp 4\n");
                fprintf(final_code,"sw $t0 0($sp)\n");
                break;
            }
            case 36:{
                load_data("$t0",CODE_LIST.MID_CODE[index].src2);
                fprintf(final_code,"li $s6 4\n");
                fprintf(final_code,"mul $s6 $s6 $t0\n");
                address = new_index_in_tab(CODE_LIST.MID_CODE[index].src1);
                if(new_global_flag){
                    fprintf(final_code,"la $s7 %s\n",CODE_LIST.MID_CODE[index].src1);
                    fprintf(final_code,"add $s7 $s7 $s6\n");
                    fprintf(final_code,"lw $t1 0($s7)\n");
                    store_data("$t1",CODE_LIST.MID_CODE[index].result);
                }
                else{
                    fprintf(final_code,"subi $s7 $fp %d\n",(1+address)*4);
                    fprintf(final_code,"add $s7 $s7 $s6\n");
                    fprintf(final_code,"lw $t1 0($s7)\n");
                    store_data("$t1",CODE_LIST.MID_CODE[index].result);
                }
                break;
            }
        }
    }
}

int new_index_in_tab(char name[]){
    int index,i;
    new_const_flag = 0;
    new_var_flag = 0;
    new_para_flag = 0;
    new_global_flag = 0;
    if(strcmp(now_func_name,CODE_EMPTY)==0){ //全局量，在全局里面找
        for(index=0;new_Table.List[index].kind != 2;index++){
            if(strcmp(new_Table.List[index].name,name)==0)
                break;
        }
        if(new_Table.List[index].kind == 2)
            return -1;
        else{
            new_global_flag = 1;
            if(new_Table.List[index].kind == 0){
                new_const_flag = 1;
                return new_Table.List[index].value;
            }
            else if(new_Table.List[index].kind == 1){
                new_var_flag = 1;
                return new_Table.List[index].in_address;
            }
            return -1;
        }
    }
    else{
        for(index=0;index<new_Table.num_of_func;index++){
            i = new_Table.index_of_func[index];
            if(strcmp(new_Table.List[i].name,now_func_name)==0)
                break;
        }
        if(index < new_Table.num_of_func){
            for(index=i+1;(index <= new_Table.list_index) && (new_Table.List[index].kind != 2);index++)
                if(strcmp(new_Table.List[index].name,name)==0)
                    break;
            if((index <= new_Table.list_index) && (new_Table.List[index].kind != 2)){
                if(new_Table.List[index].kind == 0){
                    new_const_flag = 1;
                    return new_Table.List[index].value;
                }
                else if(new_Table.List[index].kind == 1){
                    new_var_flag = 1;
                    return new_Table.List[index].in_address;
                }
                else if(new_Table.List[index].kind == 3){
                    new_para_flag = 1;
                    return new_Table.List[index].in_address;
                }
                return -1;
            }
            else{//去全局量找
                for(index=0;new_Table.List[index].kind != 2;index++){
                    if(strcmp(new_Table.List[index].name,name)==0)
                        break;
                }
                if(new_Table.List[index].kind == 2)
                    return -1;
                else{
                    new_global_flag = 1;
                    if(new_Table.List[index].kind == 0){
                        new_const_flag = 1;
                        return new_Table.List[index].value;
                    }
                    else if(new_Table.List[index].kind == 1){
                        new_var_flag = 1;
                        return new_Table.List[index].in_address;
                    }
                    return -1;
                }
            }
        }
        else{//error
            return -1;
        }
    }
    return -1;
}

int get_func_para_num(char name[]){
    int index;
    for(index=0;index<new_Table.num_of_func;index++){
        if(strcmp(name,new_Table.List[new_Table.index_of_func[index]].name)==0)
            break;
    }
    if(index < new_Table.num_of_func){
        return(new_Table.List[new_Table.index_of_func[index]].size);
    }
    else{
        return -1;
    }
}

int get_func_total_size(char name[]){
    int i,index;
    for(i=0;i<new_Table.num_of_func;i++){
        index = new_Table.index_of_func[i];
        if(strcmp(name,new_Table.List[index].name) == 0)
            break;
    }
    if(i == new_Table.num_of_func){
        error(FUNC_NO_DEF_ERROR,line);
        return -1;
    }
    else{       //找到了同名函数
        if(i < new_Table.num_of_func-1){
            return(new_Table.List[new_Table.index_of_func[i+1]-1].in_address);
        }
        else{
            return(new_Table.List[new_Table.list_index].in_address);
        }
    }
}

//生成最终代码的总处理
void generate_all_final_code(){
    get_new_Table();
    strcmp(now_func_name,CODE_EMPTY);
    final_code = fopen("final.asm","w");
    fprintf(final_code,".data\n");
    data_area_generate();
    fprintf(final_code,".text\n");
    fprintf(final_code,"move $fp $sp\n");
    fprintf(final_code,"j main\n");
    generate_text();
    fclose(final_code);
}
