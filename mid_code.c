#include<stdio.h>

#define MAX_MID_CODE 1024
//中间代码的结构体设计
struct mid_code{
    int op;
    char src1[100];
    char src2[100];
    char result[100];
    int is_effective;
};
struct mid_code_list{
    struct mid_code MID_CODE[MAX_MID_CODE];
    int code_num;
};
//全局常量说明
const char CODE_EMPTY[] = "";
const char CODE_INT[] = "INT";
const char CODE_CHAR[] = "CHAR";
const char CODE_CONST[] = "CONST";
const char CODE_ARRAY[] = "ARRAY";
const char CODE_FUNC[] = "FUNC";
const char CODE_PARA[] = "PARA";
const char CODE_CALL[] = "CALL";
const char CODE_RT[] = "RT";
const char CODE_EQU[] = "EQU";
const char CODE_NEQ[] = "NEQ";
const char CODE_LABEL[] = "LABEL";
const char CODE_SCAN[] = "SCAN";
const char CODE_PRINT[] = "PRINT";
const char CODE_GOTO[] = "GOTO";
const char CODE_END[] = "END";
const char CODE_ZERO[] = "0";
//外部变量
extern FILE* compile_file;
extern FILE* output_file;
extern int line;
//全局变量
FILE* mid_code_file;
FILE* mid_code_file2;
int label_num = 0;
int var_num = 0;
int new_var_num = 0;
struct mid_code_list CODE_LIST;
//中间代码的操作函数声明
void init_code_list();
void generate_mid_code(int op,char src1[],char scr2[],char result[]);
void generate_label(char label[]);
void generate_temp_var(char var[]);
void print_mid_code();
void print_mid_code2();
void print_one_code(int number,FILE* mid_code_file);

//函数具体实现
void init_code_list(){
    CODE_LIST.code_num = 0;
}

void init_var_num(){
    var_num = 0;
}

void generate_mid_code(int op,char src1[],char src2[],char result[]){
    int index;
    if(CODE_LIST.code_num >= MAX_MID_CODE){
        error(OUT_OF_CODE_ERROR,line);
        fclose(compile_file);
        fclose(output_file);
        exit(-1);
        return;
    }
    index = CODE_LIST.code_num;
    CODE_LIST.MID_CODE[index].op = op;
    strcpy(CODE_LIST.MID_CODE[index].src1,src1);
    strcpy(CODE_LIST.MID_CODE[index].src2,src2);
    strcpy(CODE_LIST.MID_CODE[index].result,result);
    CODE_LIST.MID_CODE[index].is_effective = 1;
    CODE_LIST.code_num++;
}

void generate_label(char label[]){
    sprintf(label,"LABEL%d",label_num);
    label_num++;
    return;
}

void generate_temp_var(char var[]){
    sprintf(var,"$VAR%d",var_num);
    var_num++;
    return;
}

void generate_new_temp_var(char var[]){
    sprintf(var,"$S%d",new_var_num);
    new_var_num++;
    return;
}
void print_mid_code(){
    int i;
    mid_code_file = fopen("mid_code.txt","w");
    for(i=0;i<CODE_LIST.code_num;i++){
        if(CODE_LIST.MID_CODE[i].is_effective == 1){
            print_one_code(i,mid_code_file);
        }
    }
    fclose(mid_code_file);
}

void print_mid_code2(){
    int i;
    mid_code_file2 = fopen("mid_code2.txt","w");
    for(i=0;i<CODE_LIST.code_num;i++){
        if(CODE_LIST.MID_CODE[i].is_effective == 1){
            print_one_code(i,mid_code_file2);
        }
    }
    fclose(mid_code_file2);
}

void print_one_code(int number,FILE* mid_code_file){
    int op_type;
    op_type = CODE_LIST.MID_CODE[number].op;
    switch(op_type){
        case 1:{
            fprintf(mid_code_file,"CONST INT %s %s\n",CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 2:{
            fprintf(mid_code_file,"CONST CHAR %s %s\n",CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 3:{
            fprintf(mid_code_file,"INT %s\n",CODE_LIST.MID_CODE[number].src1);
            break;
        }
        case 4:{
            fprintf(mid_code_file,"CHAR %s\n",CODE_LIST.MID_CODE[number].src1);
            break;
        }
        case 5:{
            fprintf(mid_code_file,"ARRAY INT %s %s\n",CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 6:{
            fprintf(mid_code_file,"ARRAY CHAR %s %s\n",CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 7:{
            fprintf(mid_code_file,"FUNC INT %s\n",CODE_LIST.MID_CODE[number].src1);
            break;
        }
        case 8:{
            fprintf(mid_code_file,"FUNC CHAR %s\n",CODE_LIST.MID_CODE[number].src1);
            break;
        }
        case 9:{
            fprintf(mid_code_file,"FUNC VOID %s\n",CODE_LIST.MID_CODE[number].src1);
            break;
        }
        case 10:{
            fprintf(mid_code_file,"PARA %s %s\n",CODE_LIST.MID_CODE[number].src2,CODE_LIST.MID_CODE[number].src1);
            break;
        }
        case 11:{
            fprintf(mid_code_file,"%s = %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src1);
            break;
        }
        case 12:{
            fprintf(mid_code_file,"%s[%s] = %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src2,CODE_LIST.MID_CODE[number].src1);
            break;
        }
        /*case 13:{//never use
            fprintf(mid_code_file,"SAVE\n");
            break;
        }*/
        case 14:{
            if(strcmp(CODE_EMPTY,CODE_LIST.MID_CODE[number].result) == 0){
                fprintf(mid_code_file,"CALL VALUE %s\n",CODE_LIST.MID_CODE[number].src1);
            }
            else{
                fprintf(mid_code_file,"CALL VALUE %s %s\n",CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].result);
            }
            break;
        }
        case 15:{
            fprintf(mid_code_file,"CALL VOID %s\n",CODE_LIST.MID_CODE[number].src1);
            break;
        }
        case 16:{
            fprintf(mid_code_file,"%s = %s <= %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 17:{
            fprintf(mid_code_file,"%s = %s < %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 18:{
            fprintf(mid_code_file,"%s = %s >= %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 19:{
            fprintf(mid_code_file,"%s = %s > %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 20:{
            fprintf(mid_code_file,"%s = %s == %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 21:{
            fprintf(mid_code_file,"%s = %s != %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 22:{
            fprintf(mid_code_file,"EQUZ %s %s\n",CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 23:{
            fprintf(mid_code_file,"NEQZ %s %s\n",CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 24:{
            fprintf(mid_code_file,"SET %s\n",CODE_LIST.MID_CODE[number].src1);
            break;
        }
        case 25:{
            fprintf(mid_code_file,"RT\n");
            break;
        }
        case 26:{
            fprintf(mid_code_file,"RT %s\n",CODE_LIST.MID_CODE[number].src1);
            break;
        }
        case 27:{
            fprintf(mid_code_file,"SCAN %s %s\n",CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].result);
            break;
        }
        case 28:{
            if(strcmp(CODE_EMPTY,CODE_LIST.MID_CODE[number].src1) == 0){
                fprintf(mid_code_file,"PRINT %s %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src2);
            }
            else if(strcmp(CODE_EMPTY,CODE_LIST.MID_CODE[number].src2) == 0){
                fprintf(mid_code_file,"PRINT %s\n",CODE_LIST.MID_CODE[number].src1);
            }
            else{
                fprintf(mid_code_file,"PRINT %s %s %s\n",CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src2);
            }
            break;
        }
        case 29:{
            fprintf(mid_code_file,"%s = %s + %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 30:{
            fprintf(mid_code_file,"%s = %s - %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 31:{
            fprintf(mid_code_file,"%s = %s * %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 32:{
            fprintf(mid_code_file,"%s = %s / %s\n",CODE_LIST.MID_CODE[number].result,CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2);
            break;
        }
        case 33:{
            fprintf(mid_code_file,"GOTO %s\n",CODE_LIST.MID_CODE[number].src1);
            break;
        }
        case 34:{
            fprintf(mid_code_file,"END\n");
            break;
        }
        case 35:{
            fprintf(mid_code_file,"PUSH %s\n",CODE_LIST.MID_CODE[number].src1);
            break;
        }
        case 36:{
            fprintf(mid_code_file,"GET %s[%s] %s\n",CODE_LIST.MID_CODE[number].src1,CODE_LIST.MID_CODE[number].src2,CODE_LIST.MID_CODE[number].result);
            break;
        }
        case 37:{
            fprintf(mid_code_file,"NOP\n");
            break;
        }
    }
}
