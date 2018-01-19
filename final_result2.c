#include<stdio.h>

//外部变量
extern struct table Table;
extern struct table new_Table2;
extern int var_num;
extern struct mid_code_list CODE_LIST;
extern struct func_blocks FUNC_BLOCKS;
//内部结构体设计，存放各个寄存器对应的变量名字
struct s_reg{
    char var_name[9][100];
    int total_var_num;
};


//全局变量
FILE* final_code2;                                          //最终结果
struct s_reg S_REG;
//struct reg_var REG_VAR_LIST[17];
//int t_reg_index = 0;
//int s_reg_index = 10;
char now_func_name[100];
int new_const_flag2 = 0;
int new_var_flag2 = 0;
int new_para_flag2 = 0;
int new_global_flag2 = 0;
//函数声明
void get_new_Table2();
void data_area_generate2();
void generate_all_final_code2();
void generate_text2();
int new_index_in_tab2(char name[]);
int get_func_para_num2(char name[]);
void init_s_reg();
void alloc_s_reg(char name[],int use_flag);
void get_s_reg(char name[],char reg_name[],int num,int use_flag);
void store_data2(char reg_name[],char var_name[]);
int get_t_reg(char reg_name[],char var_name[]);
void save_s_reg();
void save_t_reg();
void load_s_reg();
void load_t_reg();
int get_func_total_size2(char name[]);
//函数实现
void get_new_Table2(){
    int index,f_index,i;
    new_Table2.num_of_func = 0;
    new_Table2.list_index = -1;
    for(index=0;index<Table.index_of_func[0];index++){
         new_Table2.list_index++;
         new_Table2.List[new_Table2.list_index].in_address = Table.List[index].in_address;
         new_Table2.List[new_Table2.list_index].kind = Table.List[index].kind;
         new_Table2.List[new_Table2.list_index].size = Table.List[index].size;
         new_Table2.List[new_Table2.list_index].type = Table.List[index].type;
         new_Table2.List[new_Table2.list_index].value = Table.List[index].value;
         strcpy(new_Table2.List[new_Table2.list_index].name,Table.List[index].name);
    }
    for(f_index=0;f_index<Table.num_of_func;f_index++){
        new_Table2.index_of_func[new_Table2.num_of_func] = new_Table2.list_index + 1;
        if(f_index != Table.num_of_func-1){
            for(index=Table.index_of_func[f_index];index<Table.index_of_func[f_index+1];index++){
                if(Table.List[index].name[0] != '$'){
                    new_Table2.list_index++;
                    new_Table2.List[new_Table2.list_index].in_address = Table.List[index].in_address;
                    new_Table2.List[new_Table2.list_index].kind = Table.List[index].kind;
                    new_Table2.List[new_Table2.list_index].size = Table.List[index].size;
                    new_Table2.List[new_Table2.list_index].type = Table.List[index].type;
                    new_Table2.List[new_Table2.list_index].value = Table.List[index].value;
                    strcpy(new_Table2.List[new_Table2.list_index].name,Table.List[index].name);
                }
            }
            //接下来添加修改后代码的临时变量
            for(index=FUNC_BLOCKS.FUNC_LIST[f_index].start_num;index<FUNC_BLOCKS.FUNC_LIST[f_index+1].start_num;index++){
                if(CODE_LIST.MID_CODE[index].is_effective == 0)
                    continue;
                if(strcmp(CODE_LIST.MID_CODE[index].result,CODE_EMPTY)==0)
                    continue;
                if(CODE_LIST.MID_CODE[index].result[0] == '$'){
                    for(i=new_Table2.index_of_func[new_Table2.num_of_func];i<=new_Table2.list_index;i++){
                        if(strcmp(CODE_LIST.MID_CODE[index].result,new_Table2.List[i].name)==0)
                            break;
                    }
                    if(i>new_Table2.list_index){
                        new_Table2.list_index++;
                        new_Table2.List[new_Table2.list_index].in_address = new_Table2.List[new_Table2.list_index-1].in_address + 1;
                        new_Table2.List[new_Table2.list_index].kind = 1;
                        new_Table2.List[new_Table2.list_index].size = -1;
                        new_Table2.List[new_Table2.list_index].type = 1;
                        new_Table2.List[new_Table2.list_index].value = 0;
                        strcpy(new_Table2.List[new_Table2.list_index].name,CODE_LIST.MID_CODE[index].result);
                    }
                }
            }
        }
        else{
            for(index=Table.index_of_func[f_index];index<=Table.list_index;index++){
                if(Table.List[index].name[0] != '$'){
                    new_Table2.list_index++;
                    new_Table2.List[new_Table2.list_index].in_address = Table.List[index].in_address;
                    new_Table2.List[new_Table2.list_index].kind = Table.List[index].kind;
                    new_Table2.List[new_Table2.list_index].size = Table.List[index].size;
                    new_Table2.List[new_Table2.list_index].type = Table.List[index].type;
                    new_Table2.List[new_Table2.list_index].value = Table.List[index].value;
                    strcpy(new_Table2.List[new_Table2.list_index].name,Table.List[index].name);
                }
            }
            //接下来添加修改后代码的临时变量
            for(index=FUNC_BLOCKS.FUNC_LIST[f_index].start_num;index<CODE_LIST.code_num;index++){
                if(CODE_LIST.MID_CODE[index].is_effective == 0)
                    continue;
                if(strcmp(CODE_LIST.MID_CODE[index].result,CODE_EMPTY)==0)
                    continue;
                if(CODE_LIST.MID_CODE[index].result[0] == '$'){
                    for(i=new_Table2.index_of_func[new_Table2.num_of_func];i<=new_Table2.list_index;i++){
                        if(strcmp(CODE_LIST.MID_CODE[index].result,new_Table2.List[i].name)==0)
                            break;
                    }
                    if(i>new_Table2.list_index){
                        new_Table2.list_index++;
                        new_Table2.List[new_Table2.list_index].in_address = new_Table2.List[new_Table2.list_index-1].in_address + 1;
                        new_Table2.List[new_Table2.list_index].kind = 1;
                        new_Table2.List[new_Table2.list_index].size = -1;
                        new_Table2.List[new_Table2.list_index].type = 1;
                        new_Table2.List[new_Table2.list_index].value = 0;
                        strcpy(new_Table2.List[new_Table2.list_index].name,CODE_LIST.MID_CODE[index].result);
                    }
                }
            }
        }
        new_Table2.num_of_func++;
    }
}

//new

//函数
void init_s_reg(){
    S_REG.total_var_num = 0;
}

void alloc_s_reg(char name[],int use_flag){
    int index,address;
    char reg_name[100];
    if(strlen(name)>=3 && name[0] == '$' && name[1] == 'V')
        return;
    if(isdigit(name[0]))
        return;
    for(index=0;index<S_REG.total_var_num;index++){
        if(strcmp(S_REG.var_name[index],name)==0)
            break;
    }
    if(index<S_REG.total_var_num)
        return;
    else{
        if(S_REG.total_var_num<9){
            S_REG.total_var_num++;
            strcpy(S_REG.var_name[S_REG.total_var_num-1],name);
            if(S_REG.total_var_num<=8){
                sprintf(reg_name,"$s%d",S_REG.total_var_num-1);
            }
            else{
                sprintf(reg_name,"$t%d",S_REG.total_var_num-2);
            }
            if(use_flag){//要被使用的，应该load
                if(islower(name[0]) || name[0] == '_' || name[0] == '$'){//要取的是变量或者常量
                    address = new_index_in_tab2(name);
                    if(new_global_flag2){
                        if(new_const_flag2){
                            fprintf(final_code2,"li %s %d\n",reg_name,address);
                        }
                        else if(new_var_flag2){
                            fprintf(final_code2,"la $t8 %s\n",name);
                            fprintf(final_code2,"lw %s 0($t8)\n",reg_name);
                        }
                    }
                    else{
                        if(new_const_flag2){
                            fprintf(final_code2,"li %s %d\n",reg_name,address);
                        }
                        else if(new_var_flag2){
                            fprintf(final_code2,"lw %s %d($fp)\n",reg_name,-(address+1)*4);
                        }
                        else if(new_para_flag2){
                            fprintf(final_code2,"lw %s %d($fp)\n",reg_name,-(address+1)*4);
                        }
                    }
                }
                else{ //载入的是数字
                    fprintf(final_code2,"li %s %s\n",reg_name,name);
                }
            }
        }
    }
}

void get_s_reg(char name[],char reg_name[],int num,int use_flag){
    int index,address;
    for(index=0;index<S_REG.total_var_num;index++){
        if(strcmp(S_REG.var_name[index],name)==0)
            break;
    }
    if(index<S_REG.total_var_num){
        if(index<=7){
            sprintf(reg_name,"$s%d",index);
            return;
        }
        else{
            sprintf(reg_name,"$t%d",index-1);
            return;
        }
    }
    else{
        sprintf(reg_name,"$a%d",num);
        if(use_flag){//要被使用的，应该load
            if(islower(name[0]) || name[0] == '_' || name[0] == '$'){//要取的是变量或者常量
                address = new_index_in_tab2(name);
                if(new_global_flag2){
                    if(new_const_flag2){
                        fprintf(final_code2,"li %s %d\n",reg_name,address);
                    }
                    else if(new_var_flag2){
                        fprintf(final_code2,"la $t8 %s\n",name);
                        fprintf(final_code2,"lw %s 0($t8)\n",reg_name);
                    }
                }
                else{
                    if(new_const_flag2){
                        fprintf(final_code2,"li %s %d\n",reg_name,address);
                    }
                    else if(new_var_flag2){
                        fprintf(final_code2,"lw %s %d($fp)\n",reg_name,-(address+1)*4);
                    }
                    else if(new_para_flag2){
                        fprintf(final_code2,"lw %s %d($fp)\n",reg_name,-(address+1)*4);
                    }
                }
            }
            else{ //载入的是数字
                fprintf(final_code2,"li %s %s\n",reg_name,name);
            }
        }
            //默认，只有在语句处理中save那些$a系列(也就是要判断)
    }
}

void store_data2(char reg_name[],char var_name[]){
    address = new_index_in_tab2(var_name);
    if(new_global_flag2){ //全局变量，应该直接存回.data对应的名字中
        fprintf(final_code2,"la $t8 %s\n",var_name);
        fprintf(final_code2,"sw %s 0($t8)\n",reg_name);
    }
    else{
        fprintf(final_code2,"sw %s %d($fp)\n",reg_name,-(address+1)*4);
    }
}

int get_t_reg(char reg_name[],char var_name[]){
    if(strcmp(var_name,"$VAR0")==0){
        sprintf(reg_name,"$t0");
        return 1;
    }
    else if(strcmp(var_name,"$VAR1")==0){
        sprintf(reg_name,"$t1");
        return 1;
    }
    else if(strcmp(var_name,"$VAR2")==0){
        sprintf(reg_name,"$t2");
        return 1;
    }
    else if(strcmp(var_name,"$VAR3")==0){
        sprintf(reg_name,"$t3");
        return 1;
    }
    else if(strcmp(var_name,"$VAR4")==0){
        sprintf(reg_name,"$t4");
        return 1;
    }
    else if(strcmp(var_name,"$VAR5")==0){
        sprintf(reg_name,"$t5");
        return 1;
    }
    else if(strcmp(var_name,"$VAR6")==0){
        sprintf(reg_name,"$t6");
        return 1;
    }
    return 0;
}

void save_s_reg(){
    int index;
    char reg[100];
    for(index=0;index<S_REG.total_var_num;index++){
        if(islower(S_REG.var_name[index][0]) || S_REG.var_name[index][0] == '_' || S_REG.var_name[index][0] == '$'){
            if(index<8){
                sprintf(reg,"$s%d",index);
                store_data2(reg,S_REG.var_name[index]);
            }
            else{
                sprintf(reg,"$t7");
                store_data2(reg,S_REG.var_name[index]);
            }
        }
    }
}

void save_t_reg(){
    int index,flag;
    char reg[100];
    char var_name[100];
    for(index=0;index<7;index++){
        sprintf(var_name,"$VAR%d",index);
        sprintf(reg,"$t%d",index);
        flag = new_index_in_tab2(var_name);
        if(flag != -1){
            store_data2(reg,var_name);
        }
    }
}

void load_s_reg(){
    int index,address;
    char reg_name[100];
    char name[100];
    for(index=0;index<S_REG.total_var_num;index++){
        strcpy(name,S_REG.var_name[index]);
        if(index<8){
            sprintf(reg_name,"$s%d",index);
            if(islower(name[0]) || name[0] == '_' || name[0] == '$'){//要取的是变量或者常量
                address = new_index_in_tab2(name);
                if(new_global_flag2){
                    if(new_const_flag2){
                        fprintf(final_code2,"li %s %d\n",reg_name,address);
                    }
                    else if(new_var_flag2){
                        fprintf(final_code2,"la $t8 %s\n",name);
                        fprintf(final_code2,"lw %s 0($t8)\n",reg_name);
                    }
                }
                else{
                    if(new_const_flag2){
                        fprintf(final_code2,"li %s %d\n",reg_name,address);
                    }
                    else if(new_var_flag2){
                        fprintf(final_code2,"lw %s %d($fp)\n",reg_name,-(address+1)*4);
                    }
                    else if(new_para_flag2){
                        fprintf(final_code2,"lw %s %d($fp)\n",reg_name,-(address+1)*4);
                    }
                }
            }
            else{ //载入的是数字
                fprintf(final_code2,"li %s %s\n",reg_name,name);
            }
        }
        else{
            sprintf(reg_name,"$t7");
            if(islower(name[0]) || name[0] == '_' || name[0] == '$'){//要取的是变量或者常量
                address = new_index_in_tab2(name);
                if(new_global_flag2){
                    if(new_const_flag2){
                        fprintf(final_code2,"li %s %d\n",reg_name,address);
                    }
                    else if(new_var_flag2){
                        fprintf(final_code2,"la $t8 %s\n",name);
                        fprintf(final_code2,"lw %s 0($t8)\n",reg_name);
                    }
                }
                else{
                    if(new_const_flag2){
                        fprintf(final_code2,"li %s %d\n",reg_name,address);
                    }
                    else if(new_var_flag2){
                        fprintf(final_code2,"lw %s %d($fp)\n",reg_name,-(address+1)*4);
                    }
                    else if(new_para_flag2){
                        fprintf(final_code2,"lw %s %d($fp)\n",reg_name,-(address+1)*4);
                    }
                }
            }
            else{ //载入的是数字
                fprintf(final_code2,"li %s %s\n",reg_name,name);
            }
        }
    }
}

void load_t_reg(){
    int index,flag;
    char reg[100];
    char var_name[100];
    for(index=0;index<7;index++){
        sprintf(var_name,"$VAR%d",index);
        sprintf(reg,"$t%d",index);
        flag = new_index_in_tab2(var_name);
        if(flag != -1){
            fprintf(final_code2,"lw %s %d($fp)\n",reg,-(flag+1)*4);
        }
    }
}
//根据符号表把全局量，根据MIDCODE把字符串和中间变量写入.data
void data_area_generate2(){
    int index,first_func_index;
    first_func_index = new_Table2.index_of_func[0];
    for(index=0;index<first_func_index;index++){
        switch(new_Table2.List[index].kind){
            case 0:{ //const
                fprintf(final_code2,"%s: .word %d\n",new_Table2.List[index].name,new_Table2.List[index].value);
                break;
            }
            case 1:{ //var
                if(new_Table2.List[index].size != -1){
                    fprintf(final_code2,"%s: .space %d\n",new_Table2.List[index].name,new_Table2.List[index].size*4);
                }
                else{
                    fprintf(final_code2,"%s: .word 0\n",new_Table2.List[index].name);
                }
            }
        }
    }
    for(index=0;index<CODE_LIST.code_num;index++){
        if((CODE_LIST.MID_CODE[index].op == 28) && (strcmp(CODE_LIST.MID_CODE[index].src1,CODE_EMPTY)!= 0))
            fprintf(final_code2,"S%d: .asciiz %s\n",index,CODE_LIST.MID_CODE[index].src1);
    }
}
//生成.text段
void generate_text2(){
    int index,i,code_index;
    int address,size,para_num;
    char reg1[100];
    char reg2[100];
    char reg3[100];
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
                init_s_reg();
                strcpy(now_func_name,CODE_LIST.MID_CODE[index].src1);

                //形成新的运行栈
                size = get_func_total_size2(CODE_LIST.MID_CODE[index].src1);
                size = size + 2;
                //size一共对应的地方是所有局部变量常量+返回地址、返回上一个AR的fp地址
                fprintf(final_code2,"%s:\n",CODE_LIST.MID_CODE[index].src1);
                para_num = get_func_para_num2(now_func_name);
                for(i=0;i<para_num;i++){
                    fprintf(final_code2,"lw $t8 0($sp)\n");
                    fprintf(final_code2,"subi $sp $sp 8\n");
                    fprintf(final_code2,"sw $t8 0($sp)\n");
                    fprintf(final_code2,"addi $sp $sp 12\n");
                }
                fprintf(final_code2,"move $t8 $fp\n");
                fprintf(final_code2,"move $fp $sp\n");
                fprintf(final_code2,"subi $fp $fp 4\n");
                fprintf(final_code2,"sw $t8 0($fp)\n");
                fprintf(final_code2,"sw $ra -4($fp)\n");
                fprintf(final_code2,"subi $sp $sp %d\n",size*4);

                for(code_index = index+1;(code_index<CODE_LIST.code_num) && !(CODE_LIST.MID_CODE[code_index].op == 7 || CODE_LIST.MID_CODE[code_index].op == 8 ||CODE_LIST.MID_CODE[code_index].op == 9) && S_REG.total_var_num < 10;code_index++){
                    if(CODE_LIST.MID_CODE[code_index].is_effective == 0)
                        continue;
                    switch(CODE_LIST.MID_CODE[code_index].op){
                        case 3:
                        case 4:
                        case 10:
                        case 22:
                        case 23:
                        case 26:
                        case 35:
                        {
                            alloc_s_reg(CODE_LIST.MID_CODE[code_index].src1,1);
                            break;
                        }
                        case 11:
                        {
                            alloc_s_reg(CODE_LIST.MID_CODE[code_index].src1,1);
                            alloc_s_reg(CODE_LIST.MID_CODE[code_index].result,1);
                            break;
                        }
                        case 12:
                        {
                            alloc_s_reg(CODE_LIST.MID_CODE[code_index].src1,1);
                            alloc_s_reg(CODE_LIST.MID_CODE[code_index].src2,1);
                            break;
                        }
                        case 16:
                        case 17:
                        case 18:
                        case 19:
                        case 20:
                        case 21:
                        case 29:
                        case 30:
                        case 31:
                        case 32:
                        {
                            alloc_s_reg(CODE_LIST.MID_CODE[code_index].src1,1);
                            alloc_s_reg(CODE_LIST.MID_CODE[code_index].src2,1);
                            alloc_s_reg(CODE_LIST.MID_CODE[code_index].result,1);
                            break;
                        }
                        case 27:
                        {
                            alloc_s_reg(CODE_LIST.MID_CODE[code_index].result,1);
                            break;
                        }
                        case 36:
                        {
                            alloc_s_reg(CODE_LIST.MID_CODE[code_index].src2,1);
                            alloc_s_reg(CODE_LIST.MID_CODE[code_index].result,1);
                            break;
                        }
                    }
                }

                break;
            }
            case 11:{
                if(isdigit(CODE_LIST.MID_CODE[index].src1[0])){
                    if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                        get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                    }
                    fprintf(final_code2,"li %s %s\n",reg3,CODE_LIST.MID_CODE[index].src1);
                    if(reg3[1] == 'a'){
                        store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                    }
                    break;
                }
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                }
                fprintf(final_code2,"move %s %s\n",reg3,reg1);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                break;
            }
            case 12:{
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                }
                fprintf(final_code2,"li $t9 4\n");
                fprintf(final_code2,"mul $t9 $t9 %s\n",reg2);
                address = new_index_in_tab2(CODE_LIST.MID_CODE[index].result);
                if(new_global_flag2){
                    fprintf(final_code2,"la $t8 %s\n",CODE_LIST.MID_CODE[index].result);
                    fprintf(final_code2,"add $t8 $t8 $t9\n");
                    fprintf(final_code2,"sw %s 0($t8)\n",reg1);
                }
                else{
                    fprintf(final_code2,"subi $t8 $fp %d\n",(address+1)*4);
                    fprintf(final_code2,"add $t8 $t8 $t9\n");
                    fprintf(final_code2,"sw %s 0($t8)\n",reg1);
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
                /*
                fprintf(final_code2,"jal %s\n",CODE_LIST.MID_CODE[index].src1);
                if(strcmp(CODE_EMPTY,CODE_LIST.MID_CODE[index].result)!= 0){
                    fprintf(final_code2,"move $t0 $v0\n");
                    store_data2("$t0",CODE_LIST.MID_CODE[index].result);
                }
                */
                save_s_reg();
                save_t_reg();
                fprintf(final_code2,"jal %s\n",CODE_LIST.MID_CODE[index].src1);
                load_s_reg();
                load_t_reg();
                if(strcmp(CODE_EMPTY,CODE_LIST.MID_CODE[index].result)!= 0){
                    if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                        get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                    }
                    fprintf(final_code2,"move %s $v0\n",reg3);
                    if(reg3[1] == 'a'){
                        store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                    }
                }
                break;
            }
            case 15:{
                save_s_reg();
                save_t_reg();
                fprintf(final_code2,"jal %s\n",CODE_LIST.MID_CODE[index].src1);
                load_s_reg();
                load_t_reg();
                break;
            }
            case 16:{
                generate_label(label1);
                generate_label(label2);
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                }
                if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                }
                fprintf(final_code2,"ble %s %s %s\n",reg1,reg2,label1);
                fprintf(final_code2,"move %s $zero\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                fprintf(final_code2,"j %s\n",label2);
                fprintf(final_code2,"%s:\n",label1);
                fprintf(final_code2,"li %s 1\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                fprintf(final_code2,"%s:\n",label2);
                break;
            }
            case 17:{
                generate_label(label1);
                generate_label(label2);
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                }
                if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                }
                fprintf(final_code2,"blt %s %s %s\n",reg1,reg2,label1);
                fprintf(final_code2,"move %s $zero\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                fprintf(final_code2,"j %s\n",label2);
                fprintf(final_code2,"%s:\n",label1);
                fprintf(final_code2,"li %s 1\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                fprintf(final_code2,"%s:\n",label2);
                break;
            }
            case 18:{
                generate_label(label1);
                generate_label(label2);
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                }
                if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                }
                fprintf(final_code2,"bge %s %s %s\n",reg1,reg2,label1);
                fprintf(final_code2,"move %s $zero\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                fprintf(final_code2,"j %s\n",label2);
                fprintf(final_code2,"%s:\n",label1);
                fprintf(final_code2,"li %s 1\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                fprintf(final_code2,"%s:\n",label2);
                break;
            }
            case 19:{
                generate_label(label1);
                generate_label(label2);
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                }
                if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                }
                fprintf(final_code2,"bgt %s %s %s\n",reg1,reg2,label1);
                fprintf(final_code2,"move %s $zero\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                fprintf(final_code2,"j %s\n",label2);
                fprintf(final_code2,"%s:\n",label1);
                fprintf(final_code2,"li %s 1\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                fprintf(final_code2,"%s:\n",label2);
                break;
            }
            case 20:{
                generate_label(label1);
                generate_label(label2);
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                }
                if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                }
                fprintf(final_code2,"beq %s %s %s\n",reg1,reg2,label1);
                fprintf(final_code2,"move %s $zero\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                fprintf(final_code2,"j %s\n",label2);
                fprintf(final_code2,"%s:\n",label1);
                fprintf(final_code2,"li %s 1\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                fprintf(final_code2,"%s:\n",label2);
                break;
            }
            case 21:{
                generate_label(label1);
                generate_label(label2);
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                }
                if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                }
                fprintf(final_code2,"bne %s %s %s\n",reg1,reg2,label1);
                fprintf(final_code2,"move %s $zero\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                fprintf(final_code2,"j %s\n",label2);
                fprintf(final_code2,"%s:\n",label1);
                fprintf(final_code2,"li %s 1\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                fprintf(final_code2,"%s:\n",label2);
                break;
            }
            case 22:{
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                fprintf(final_code2,"beq %s $zero %s\n",reg1,CODE_LIST.MID_CODE[index].src2);
                break;
            }
            case 23:{
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                fprintf(final_code2,"bne %s $zero %s\n",reg1,CODE_LIST.MID_CODE[index].src2);
                break;
            }
            case 24:{
                fprintf(final_code2,"%s:\n",CODE_LIST.MID_CODE[index].src1);
                break;
            }
            //?
            case 25:{
                //保存所有的S！
                save_s_reg();
                fprintf(final_code2,"lw $ra -4($fp)\n");
                fprintf(final_code2,"addi $sp $fp 4\n");
                fprintf(final_code2,"lw $fp 0($fp)\n");
                fprintf(final_code2,"jr $ra\n");
                break;
            }
            case 26:{
                //保存所有的S！
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                fprintf(final_code2,"move $v0 %s\n",reg1);
                save_s_reg();
                fprintf(final_code2,"lw $ra -4($fp)\n");
                fprintf(final_code2,"addi $sp $fp 4\n");
                fprintf(final_code2,"lw $fp 0($fp)\n");
                fprintf(final_code2,"jr $ra\n");
                break;
            }
            case 27:{
                if(strcmp(CODE_INT,CODE_LIST.MID_CODE[index].src1) == 0){
                    fprintf(final_code2,"li $v0 5\n");
                    fprintf(final_code2,"syscall\n");
                    if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                        get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                    }
                    fprintf(final_code2,"move %s $v0\n",reg3);
                    if(reg3[1] == 'a'){
                        store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                    }
                }
                else{
                    fprintf(final_code2,"li $v0 12\n");
                    fprintf(final_code2,"syscall\n");
                    if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                        get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                    }
                    fprintf(final_code2,"move %s $v0\n",reg3);
                    if(reg3[1] == 'a'){
                        store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                    }
                }
                break;
            }
            case 28:{
                if(strcmp(CODE_LIST.MID_CODE[index].src1,CODE_EMPTY)!=0){
                    fprintf(final_code2,"la $a0 S%d\n",index);
                    fprintf(final_code2,"li $v0 4\n");
                    fprintf(final_code2,"syscall\n");
                }
                if(strcmp(CODE_LIST.MID_CODE[index].src2,CODE_EMPTY)!=0){
                    if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                        get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                    }
                    if(strcmp(CODE_LIST.MID_CODE[index].result,CODE_CHAR)==0){
                        fprintf(final_code2,"move $a0 %s\n",reg2);
                        fprintf(final_code2,"li $v0 11\n");
                        fprintf(final_code2,"syscall\n");
                    }
                    else{
                        fprintf(final_code2,"move $a0 %s\n",reg2);
                        fprintf(final_code2,"li $v0 1\n");
                        fprintf(final_code2,"syscall\n");
                    }
                }
                break;
            }
            case 29:{
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                }
                if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                }
                fprintf(final_code2,"add %s %s %s\n",reg3,reg1,reg2);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                break;
            }
            case 30:{
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                }
                if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                }
                fprintf(final_code2,"sub %s %s %s\n",reg3,reg1,reg2);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                break;
            }
            case 31:{
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                }
                if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                }
                fprintf(final_code2,"mul %s %s %s\n",reg3,reg1,reg2);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                break;
            }
            case 32:{
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                }
                if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                }
                fprintf(final_code2,"div %s %s\n",reg1,reg2);
                fprintf(final_code2,"mflo %s\n",reg3);
                if(reg3[1] == 'a'){
                    store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                }
                break;
            }
            case 33:{
                fprintf(final_code2,"j %s\n",CODE_LIST.MID_CODE[index].src1);
                break;
            }
            case 34:{
                fprintf(final_code2,"li $v0 10\n");
                fprintf(final_code2,"syscall\n");
                break;
            }
            case 35:{
                if(get_t_reg(reg1,CODE_LIST.MID_CODE[index].src1) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src1,reg1,1,1);
                }
                fprintf(final_code2,"subi $sp $sp 4\n");
                fprintf(final_code2,"sw %s 0($sp)\n",reg1);
                break;
            }
            case 36:{
                if(get_t_reg(reg2,CODE_LIST.MID_CODE[index].src2) == 0){
                    get_s_reg(CODE_LIST.MID_CODE[index].src2,reg2,2,1);
                }
                fprintf(final_code2,"li $t9 4\n");
                fprintf(final_code2,"mul $t9 $t9 %s\n",reg2);
                address = new_index_in_tab2(CODE_LIST.MID_CODE[index].src1);
                if(new_global_flag2){
                    fprintf(final_code2,"la $t8 %s\n",CODE_LIST.MID_CODE[index].src1);
                    fprintf(final_code2,"add $t8 $t8 $t9\n");
                    if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                        get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                    }
                    fprintf(final_code2,"lw %s 0($t8)\n",reg3);
                    if(reg3[1] == 'a'){
                        store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                    }
                }
                else{
                    fprintf(final_code2,"subi $t8 $fp %d\n",(1+address)*4);
                    fprintf(final_code2,"add $t8 $t8 $t9\n");
                    if(get_t_reg(reg3,CODE_LIST.MID_CODE[index].result) == 0){
                        get_s_reg(CODE_LIST.MID_CODE[index].result,reg3,3,0);
                    }
                    fprintf(final_code2,"lw %s 0($t8)\n",reg3);
                    if(reg3[1] == 'a'){
                        store_data2(reg3,CODE_LIST.MID_CODE[index].result);
                    }
                }
                break;
            }
        }
    }
}

int new_index_in_tab2(char name[]){
    int index,i;
    new_const_flag2 = 0;
    new_var_flag2 = 0;
    new_para_flag2 = 0;
    new_global_flag2 = 0;
    if(strcmp(now_func_name,CODE_EMPTY)==0){ //全局量，在全局里面找
        for(index=0;new_Table2.List[index].kind != 2;index++){
            if(strcmp(new_Table2.List[index].name,name)==0)
                break;
        }
        if(new_Table2.List[index].kind == 2)
            return -1;
        else{
            new_global_flag2 = 1;
            if(new_Table2.List[index].kind == 0){
                new_const_flag2 = 1;
                return new_Table2.List[index].value;
            }
            else if(new_Table2.List[index].kind == 1){
                new_var_flag2 = 1;
                return new_Table2.List[index].in_address;
            }
            return -1;
        }
    }
    else{
        for(index=0;index<new_Table2.num_of_func;index++){
            i = new_Table2.index_of_func[index];
            if(strcmp(new_Table2.List[i].name,now_func_name)==0)
                break;
        }
        if(index < new_Table2.num_of_func){
            for(index=i+1;(index <= new_Table2.list_index) && (new_Table2.List[index].kind != 2);index++)
                if(strcmp(new_Table2.List[index].name,name)==0)
                    break;
            if((index <= new_Table2.list_index) && (new_Table2.List[index].kind != 2)){
                if(new_Table2.List[index].kind == 0){
                    new_const_flag2 = 1;
                    return new_Table2.List[index].value;
                }
                else if(new_Table2.List[index].kind == 1){
                    new_var_flag2 = 1;
                    return new_Table2.List[index].in_address;
                }
                else if(new_Table2.List[index].kind == 3){
                    new_para_flag2 = 1;
                    return new_Table2.List[index].in_address;
                }
                return -1;
            }
            else{//去全局量找
                for(index=0;new_Table2.List[index].kind != 2;index++){
                    if(strcmp(new_Table2.List[index].name,name)==0)
                        break;
                }
                if(new_Table2.List[index].kind == 2)
                    return -1;
                else{
                    new_global_flag2 = 1;
                    if(new_Table2.List[index].kind == 0){
                        new_const_flag2 = 1;
                        return new_Table2.List[index].value;
                    }
                    else if(new_Table2.List[index].kind == 1){
                        new_var_flag2 = 1;
                        return new_Table2.List[index].in_address;
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

int get_func_para_num2(char name[]){
    int index;
    for(index=0;index<new_Table2.num_of_func;index++){
        if(strcmp(name,new_Table2.List[new_Table2.index_of_func[index]].name)==0)
            break;
    }
    if(index < new_Table2.num_of_func){
        return(new_Table2.List[new_Table2.index_of_func[index]].size);
    }
    else{
        return -1;
    }
}

int get_func_total_size2(char name[]){
    int i,index;
    for(i=0;i<new_Table2.num_of_func;i++){
        index = new_Table2.index_of_func[i];
        if(strcmp(name,new_Table2.List[index].name) == 0)
            break;
    }
    if(i == new_Table2.num_of_func){
        error(FUNC_NO_DEF_ERROR,line);
        return -1;
    }
    else{       //找到了同名函数
        if(i < new_Table2.num_of_func-1){
            return(new_Table2.List[new_Table2.index_of_func[i+1]-1].in_address);
        }
        else{
            return(new_Table2.List[new_Table2.list_index].in_address);
        }
    }
}

//生成最终代码的总处理
void generate_all_final_code2(){
    get_new_Table2();
    strcmp(now_func_name,CODE_EMPTY);
    final_code2 = fopen("final2.asm","w");
    fprintf(final_code2,".data\n");
    data_area_generate2();
    fprintf(final_code2,".text\n");
    fprintf(final_code2,"move $fp $sp\n");
    fprintf(final_code2,"j main\n");
    generate_text2();
    fclose(final_code2);
}












