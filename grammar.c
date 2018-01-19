#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordtest.c"
#include "optimizer.c"
//读入单词类型定义
#define CONSY 1                   //const
#define INTSY 2                   //int
#define CHARSY 3                  //char
#define VOIDSY 4                  //void
#define MAINSY 5                  //main
#define DOSY 6                    //do
#define WHISY 7                   //while
#define SWISY 8                   //switch
#define CASESY 9                  //case
#define IFSY 10                   //if
#define SCANSY 11                 //scanf
#define PRISY 12                  //printf
#define RTSY 13                   //return          前13个是保留字
#define IDSY 14                   //普通标识符
#define PLUSSY 15                 //+
#define MINUSSY 16                //-
#define STARSY 17                 //*
#define DIVSY 18                  //'/'
#define COMMASY 19                //,
#define COLONSY 20                //:
#define LPARSY 21                 //(
#define RPARSY 22                 //)
#define LBRASY 23                 //[
#define RBRASY 24                 //]
#define LBRASSY 25                //{
#define RBRASSY 26                //}
#define ASSIGNSY 27               //=
#define EQUSY 28                  //==
#define GTRSY 29                  //>
#define GTESY 30                  //>=
#define LTSY 31                   //<
#define LESY 32                   //<=
#define NEQSY 33                  // !=
#define DIGITSY 34                //无符号整数
#define CHAR2SY 35                //字符
#define STRINGSY 36               //字符串
#define SEMISY 37                 //;

#define MAXLIST 1024
#define MAXNAME 50
#define MAXFUNC 100
/*
对各个成分的定义(含有&的意思为：没有单独设计出来)
void program();               程序
void const_dec();             常量声明
void var_dec();               变量声明                  (实际设计中由于多种环境下判断不同，所以这个设计在有些时候不会使用)
void val_func_def();          有返回值函数定义
void void_func_def();         无返回值函数定义
void main_func();             主函数
void const_def();             常量定义
void var_def();               变量定义
void dec_head();              声明头部
void para_list();             参数表
void comp_state();            复合语句
void integer();               &整数
void type_id();               &类型识别符
void state_list();            语句列
void if_state();              条件语句
void state();                 语句
void condition();             条件
void rela_op();               关系运算符
void loop_state();            循环语句
void switch_state();          情况语句
void situation_list();        情况表
void case_state();            情况子语句
void constant();              &常量
void val_fun_call();          有返回值函数调用语句
void void_fun_call();         无返回值函数调用语句
void val_para_list();         值参数表
void assign_state();          赋值语句
void read_state();            读语句
void write_state();           写语句
void return_sate();           返回语句
void expr();                  表达式
void plus_op();               加法运算符
void mult_op();               乘法运算符
void term();                  项
void factor();                因子
*/
extern FILE* compile_file;
extern FILE* output_file;     //词法分析结果输出的文件
int symbol;                   //读入的单词类型号
//特殊设计的全局变量
//1.保存现场用的
int pre_symbol;               //暂时记录symbol
char pre_ch;                  //暂时记录CHAR
int pre_line_index;           //暂时记录index_in_line
char PRETOKEN[100];            //用于在特殊时刻暂时记录TOKEN的值
//2.填符号表用的全局变量
//(0)公用的
int address = 0;                  //相对于当前AR的偏移地址
char TEMP[100];
int array_flag = 0;               //标识符是不是数组
int var_flag = 0;
int const_flag = 0;
int para_flag = 0;
int global_flag = 0;              //当前的变量或者常量是不是全局的
//(1)函数声明和调用中需要的
int type_symbol;              //在函数声明中记录函数的type
char func_name[100];           //记录函数名称，供填表和参数表应用
int para_num;                 //记录函数的参数个数，填表用
int rt_flag;                  //检查有返回值函数是不是有return语句
int main_flag = 0;            //在处理RT语句时，如果是main，直接退出
int void_flag = 0;
int val_flag = 0;
//(2)变量声明中需要的
char name_of_id[100];          //用于记录标识符的名字(包括常量和变量的名字)
int var_type;                 //变量定义中记录变量的类型 或者 参数的类型
int var_array_size;           //变量定义中记录数组变量的大小
//(3)常量声明中需要的
int const_type;               //常量定义中记录常量的类型
int const_value;              //常量定义中记录常量的值
//(4)表达式等需要的
int rela_op_symbol;           //关系运算符的symbol值
int factor_type = 0;              //记录当前factor的类型
char now_result[100];
//所有函数声明
void program();
void const_dec();
void var_dec();
void val_func_def();
void void_func_def();
void main_func();
void const_def();
void var_def();
void dec_head();
void para_list();
void comp_state();
void state_list();
void if_state();
void state();
void condition();
void rela_op();
void loop_state();
void switch_state();
void situation_list();
void case_state();
void val_fun_call();
void val_fun_call2();
void void_fun_call();
void val_para_list();
void assign_state();
void read_state();
void write_state();
void return_state();
void expr();
void term();
void factor();
void initial_table();
void append_to_table(char name[],int kind,int type,int value,int size,int in_address);
void append_para(int num);
int index_in_table(char name[],int kind);
int get_func_type(char name[]);
//int get_func_total_size(char name[]);
int get_const_value(char name[]);
//符号表设计
struct one_symbol{
    char name[MAXNAME];            //标识符名称
    int kind;                 /*种类
                                0：常量
                                1：变量
                                2：函数
                                3：函数参数
                              */
    int type;                 /*类型
                                0：void(仅对于无返回值函数)
                                1：int
                                2: char
                              */
    int value;                /*常量：值或ascii码
                                其他：0
                              */
    int size;                 /*函数：参数个数
                                数组：元素个数
                                其他：-1
                              */
    int in_address;              //相对于所在AR首地址的偏移量(数组记录首地址)
};
struct table{
    struct one_symbol List[MAXLIST]; //符号表
    int list_index;           //索引
    int num_of_func;          //已经生命的函数个数
    int index_of_func[MAXFUNC]; //初始化全-1，存放赌赢顺序的函数的table索引
};
struct table Table;
struct table new_Table;
struct table new_Table2;
//关于符号表的操作
void initial_table(){
    Table.num_of_func = 0;
    Table.list_index = -1;
}
void append_to_table(char name[],int kind,int type,int value,int size,int in_address){
    int index,func_index;
    if(Table.list_index >= (MAXLIST -1)){
        error(OUT_OF_TABLE_ERROR,line);
        fclose(compile_file);
        fclose(output_file);
        exit(-1);
        return;
    }
    if(kind == 2){
        for(index=0;index<Table.num_of_func;index++){
            func_index = Table.index_of_func[index];
            if(strcmp(Table.List[func_index].name,name) == 0){                   //函数名字重复
                error(FUNC_REDEF_ERROR,line);
                return;
            }
        }
        Table.index_of_func[Table.num_of_func] = Table.list_index+1;
        Table.num_of_func++;
    }
    else{
        func_index = Table.index_of_func[Table.num_of_func-1];
        for(index=func_index;index<=Table.list_index;index++){
            if(strcmp(Table.List[index].name,name) == 0){                        //变量或常量同层有重名
                error(VAR_REDEF_ERROR,line);
                return;
            }
        }
    }
    Table.list_index++;
    strcpy(Table.List[Table.list_index].name,name);
    Table.List[Table.list_index].kind = kind;
    Table.List[Table.list_index].type = type;
    Table.List[Table.list_index].size = size;
    Table.List[Table.list_index].value = value;
    Table.List[Table.list_index].in_address = in_address;
}
void append_para(int num){
    int index;
    index = Table.index_of_func[Table.num_of_func-1];
    Table.List[index].size = num;
}
int index_in_table(char name[],int kind){                      //待改善
    int i,index,first_func_index;
    array_flag = 0;
    var_flag = 0;
    const_flag = 0;
    para_flag = 0;
    global_flag = 0;
    if(kind == 2){ //函数
        for(i=0;i<Table.num_of_func;i++){
            index = Table.index_of_func[i];
            if(strcmp(name,Table.List[index].name) == 0)
                break;
        }
        if(i == Table.num_of_func){
            error(FUNC_NO_DEF_ERROR,line);
            return -1;
        }
        else{       //找到了同名函数
            index = Table.index_of_func[i];
            if(Table.List[index].size != para_num){
                error(PARA_NUM_ERROR,line);
                return index;
            }
            return index;
        }
    }
    else{ //普通标识符
        index = Table.index_of_func[Table.num_of_func-1];
        for(;index<=Table.list_index;index++){
            if(strcmp(Table.List[index].name,name) == 0){                        //变量或常量同层有重名
                break;
            }
        }
        if(index > Table.list_index){ //本层找不到，去全局变量找
            first_func_index = Table.index_of_func[0];
            for(index = 0;index < first_func_index;index++){
                if(strcmp(name,Table.List[index].name)==0)
                    break;
            }
            if(index == first_func_index){
                error(ID_NO_DEF_ERROR,line);
                printf("wrong: %s\n",name);
                return -1;
            }
            global_flag = 1;
            if(Table.List[index].kind == 1){
                if(Table.List[index].size != -1)
                    array_flag = 1;
                else
                    var_flag = 1;
                return Table.List[index].type;
            }
            else if(Table.List[index].kind == 0){
                const_flag = 1;
                return Table.List[index].type;
            }
            else if(Table.List[index].kind == 3){ //应该不会出现
                para_flag = 1;
                return Table.List[index].type;
            }
        }
        else{                           //本层找到
            if(Table.List[index].kind == 1){
                if(Table.List[index].size != -1)
                    array_flag = 1;
                else
                    var_flag = 1;
                return Table.List[index].type;
            }
            else if(Table.List[index].kind == 0){
                const_flag = 1;
                return Table.List[index].type;
            }
            else if(Table.List[index].kind == 3){
                para_flag = 1;
                return Table.List[index].type;
            }
        }
    }
    return 0;
}
int get_func_type(char name[]){
    int i,index;
    for(i=0;i<Table.num_of_func;i++){
        index = Table.index_of_func[i];
        if(strcmp(name,Table.List[index].name) == 0)
            break;
    }
    if(i == Table.num_of_func){
        error(FUNC_NO_DEF_ERROR,line);
        return -1;
    }
    else{       //找到了同名函数
        return Table.List[index].type;
    }
}
/*
int get_func_total_size(char name[]){
    int i,index;
    for(i=0;i<Table.num_of_func;i++){
        index = Table.index_of_func[i];
        if(strcmp(name,Table.List[index].name) == 0)
            break;
    }
    if(i == Table.num_of_func){
        error(FUNC_NO_DEF_ERROR,line);
        return -1;
    }
    else{       //找到了同名函数
        if(i < Table.num_of_func-1){
            return(Table.List[Table.index_of_func[i+1]-1].in_address);
        }
        else{
            return(Table.List[Table.list_index].in_address);
        }
    }
}
*/
int get_const_value(char name[]){
    int index,first_func_index;
    index = Table.index_of_func[Table.num_of_func-1];
    for(;index<=Table.list_index;index++){
        if(strcmp(Table.List[index].name,name) == 0){                        //变量或常量同层有重名
            break;
        }
    }
    if(index > Table.list_index){ //本层找不到，去全局变量找
        first_func_index = Table.index_of_func[0];
        for(index = 0;index < first_func_index;index++){
            if(strcmp(name,Table.List[index].name)==0)
                break;
        }
        if(index == first_func_index){
            error(ID_NO_DEF_ERROR,line);
            printf("wrong: %s\n",name);
            return -1;
        }
        return Table.List[index].value;
    }
    else{                           //本层找到
        return Table.List[index].value;
    }
}
//各部分的递归子程序
void program(){               //程序
    if(symbol == CONSY){
        const_dec();
    }
    while(symbol == INTSY || symbol == CHARSY){   //变量声明部分
        pre_ch = CHAR;                            //因为int或者char开头的可能是变量声明或者函数声明，那种情况需要恢复现场
        pre_line_index = index_in_line;
        pre_symbol = symbol;
        strcpy(PRETOKEN,TOKEN);
        symbol = insymbol();
        if(symbol != IDSY){
            error(ID_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != INTSY && symbol != CHARSY && symbol != VOIDSY);
            continue;
        }
        symbol = insymbol();                      //用来检测下一个单词，从而判别是变量还是函数(函数对应的是小括号)
        if(symbol == LPARSY){                     //检测到小括号，确定是函数声明
            strcpy(TOKEN,PRETOKEN);               //恢复现场，跳出循环，进入函数声明处理
            CHAR = pre_ch;
            index_in_line = pre_line_index;
            symbol = pre_symbol;
            break;
        }
        else if(symbol == SEMISY || symbol == COMMASY || symbol == LBRASY){   //确认是变量声明
            strcpy(TOKEN,PRETOKEN);               //恢复现场，进入变量定义模块
            CHAR = pre_ch;
            index_in_line = pre_line_index;
            symbol = pre_symbol;
            var_def();
            if(symbol != SEMISY){
                error(SEMI_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != INTSY && symbol != CHARSY && symbol != VOIDSY);        //跳到下一个函数或者变量声明
                continue;
            }
            else{
                symbol = insymbol();
            }
        }
        else{
            //这里应该输出错误信息：标识符后面的字符错误(非法声明)，并skip到下一个int、char、void
            error(ID_DEC_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != INTSY && symbol != CHARSY && symbol != VOIDSY);        //跳到下一个函数或者变量声明
            continue;
        }
    }
    while(symbol == INTSY || symbol == CHARSY || symbol == VOIDSY){         //函数声明部分
        if(symbol == INTSY || symbol == CHARSY){
            val_func_def();
        }
        else{
            pre_ch = CHAR;                            //记录现场
            pre_line_index = index_in_line;
            pre_symbol = symbol;
            strcpy(PRETOKEN,TOKEN);
            symbol = insymbol();
            if(symbol == MAINSY){
                strcpy(TOKEN,PRETOKEN);               //恢复现场，跳出进入主函数定义模块
                CHAR = pre_ch;
                index_in_line = pre_line_index;
                symbol = pre_symbol;
                break;
            }
            else{
                strcpy(TOKEN,PRETOKEN);               //恢复现场，进入无返回值函数定义模块(此处可优化？)
                CHAR = pre_ch;
                index_in_line = pre_line_index;
                symbol = pre_symbol;
                void_func_def();
            }
        }
    }
    main_func();
    fprintf(output_file,"This is a program!\n");
    //printf("This is a program!\n");
}

void var_dec(){
    while(symbol == INTSY || symbol == CHARSY){
        var_def();
        if(symbol != SEMISY){
            error(SEMI_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != INTSY && symbol != CHARSY /*&& symbol != VOIDSY*/ && symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != RTSY);        //跳到下一句变量声明或者语句列
        }
        else{
            symbol = insymbol();
        }
    }
    fprintf(output_file,"This is a variable declaration!\n");
    //printf("This is a variable declaration!\n");
}

void const_dec(){
    while(symbol == CONSY){
        symbol = insymbol();
        const_def();
        if(symbol != SEMISY){
            error(SEMI_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != CONSY && symbol != INTSY && symbol != CHARSY && symbol != VOIDSY && symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != RTSY);        //跳到下一句常量声明或者变量声明或语句列
        }
        else{
            symbol = insymbol();
        }
    }
    fprintf(output_file,"This is a constant declaration!\n");
    //printf("This is a constant declaration!\n");
}

void val_func_def(){
    type_symbol = symbol;
    rt_flag = 0;
    dec_head();
    if(symbol == LPARSY){
        val_flag = 1;
        void_flag = 0;
        if(type_symbol == CHARSY){
            //将函数名称+类型填表char
            generate_mid_code(8,func_name,CODE_EMPTY,CODE_EMPTY);
            add_func_block();
        }
        else{
            //将函数名称+类型填表int
            generate_mid_code(7,func_name,CODE_EMPTY,CODE_EMPTY);
            add_func_block();
        }
        symbol = insymbol();
        para_list();
        if(symbol == RPARSY){
            symbol = insymbol();
            if(symbol == LBRASSY){
                symbol = insymbol();
                comp_state();
                if(symbol == RBRASSY){
                    symbol = insymbol();
                    if(rt_flag == 0){
                        printf("%s\n",func_name);
                        error(NO_RT_FUNDEF_ERROR,line);
                    }
                    //生成四元式：函数的结束语句?
                    rt_flag = 0;
                    fprintf(output_file,"This is a value function defination!\n");
                    //printf("This is a value function defination!\n");
                    return;
                }
                else{
                    error(RBRAS_ERROR,line);
                    do{
                        symbol = insymbol();
                    }while(symbol != VOIDSY && symbol != INTSY && symbol != CHARSY);
                    return;
                }
            }
            else{
                error(LBRAS_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != RBRASSY);
                symbol = insymbol();
                return;
            }
        }
        else{
            error(RPAR_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != RBRASSY);
            symbol = insymbol();
            return;
        }
    }
    else{
        error(LPAR_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != RBRASSY);
        symbol = insymbol();
        return;
    }
}

void void_func_def(){
    type_symbol = VOIDSY;
    symbol = insymbol();
    if(symbol == IDSY){
        strcpy(func_name,TOKEN);
        //将函数名称+类型填表void
        void_flag = 1;
        val_flag = 0;
        generate_mid_code(9,func_name,CODE_EMPTY,CODE_EMPTY);
        add_func_block();
        address = 0;
        append_to_table(func_name,2,0,0,0,address);
        symbol = insymbol();
        if(symbol == LPARSY){
            symbol = insymbol();
            para_list();
            if(symbol == RPARSY){
                symbol = insymbol();
                if(symbol == LBRASSY){
                    symbol = insymbol();
                    comp_state();
                    if(symbol == RBRASSY){
                        symbol = insymbol();
                        //生成四元式：函数的结束语句
                        generate_mid_code(25,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
                        //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
                        fprintf(output_file,"This is a void function defination!\n");
                        //printf("This is a void function defination!\n");
                        return;
                    }
                    else{
                        error(RBRAS_ERROR,line);
                        do{
                            symbol = insymbol();
                        }while(symbol != VOIDSY && symbol != INTSY && symbol != CHARSY);
                        return;
                    }
                }
                else{
                    error(LBRAS_ERROR,line);
                    do{
                        symbol = insymbol();
                    }while(symbol != RBRASSY);
                    symbol = insymbol();
                    return;
                }
            }
            else{
                error(RPAR_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != RBRASSY);
                symbol = insymbol();
                return;
            }
        }
        else{
            error(LPAR_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != RBRASSY);
            symbol = insymbol();
            return;
        }
    }
    else{
        error(ID_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != RBRASSY);
        symbol = insymbol();
        return;
    }
}

void main_func(){
    if(symbol != VOIDSY){
        error(NOT_VOID_MAIN_ERROR,line);
        return;                             //跳的多么？
    }
    symbol = insymbol();
    if(symbol == MAINSY){
        //填表+生成四元式
        strcpy(func_name,TOKEN);
        val_flag = 0;
        void_flag = 1;
        main_flag = 1;
        generate_mid_code(9,func_name,CODE_EMPTY,CODE_EMPTY);
        add_func_block();
        address = 0;
        append_to_table(func_name,2,0,0,0,address);
        symbol = insymbol();
        if(symbol != LPARSY){
            error(LPAR_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != LBRASSY);
        }
        else{
            symbol = insymbol();
        }
        if(symbol != RPARSY){
            error(RPAR_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != LBRASSY);
        }
        else{
            symbol = insymbol();
        }
        if(symbol != LBRASSY){
            error(LBRAS_ERROR,line);
            return;
        }
        symbol = insymbol();
        comp_state();
        if(symbol == RBRASSY){
            //生成四元式：函数的结束语句
            generate_mid_code(34,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
            fprintf(output_file,"This is a main function defination!\n");
            //sprintf("This is a main function defination!\n");
            return;
        }
        else{
            error(RBRAS_ERROR,line);
            return;
        }
    }
    else{
        error(MAINSY_ERROR,line);
        return;                             //跳的多么？
    }
}

void const_def(){
    int sign = 1;   //1:+ 0:-
    if(symbol == INTSY || symbol == CHARSY){
        const_type = symbol;
        do{
            symbol = insymbol();
            if(symbol == IDSY){
                strcpy(name_of_id,TOKEN);
                symbol = insymbol();
                if(symbol == ASSIGNSY){
                    symbol = insymbol();
                    if(symbol == PLUSSY || symbol == MINUSSY){
                        sign = (symbol == PLUSSY) ? 1 : 0;
                        symbol = insymbol();
                        if(symbol == DIGITSY || const_type == INTSY){
                            if(trans_num == 0){
                                error(AFTER_OP_DIGIT_ERROR,line);
                                do{
                                    symbol = insymbol();
                                }while(symbol !=COMMASY && symbol != SEMISY);
                                continue;
                            }
                            if(sign == 1)
                                const_value = trans_num;
                            else
                                const_value = 0 - trans_num;
                            address = address + 1;
                            append_to_table(name_of_id,0,1,const_value,-1,address);
                            sprintf(TEMP,"%d",const_value);
                            generate_mid_code(1,name_of_id,TEMP,CODE_EMPTY);
                            //填表，生成四元式
                            symbol = insymbol();
                            fprintf(output_file,"This is an integer!\n");
                            //printf("This is an integer!\n");
                        }
                        else{
                            error(CONST_DEF_TYPE_ERROR,line);
                            do{
                                symbol = insymbol();
                            }while(symbol !=COMMASY && symbol != SEMISY);
                            continue;
                        }
                    }
                    else if(symbol == DIGITSY && const_type == INTSY){
                        sign = 1;
                        const_value = trans_num;
                        //填表，生成四元式
                        address = address + 1;
                        append_to_table(name_of_id,0,1,const_value,-1,address);
                        sprintf(TEMP,"%d",const_value);
                        generate_mid_code(1,name_of_id,TEMP,CODE_EMPTY);
                        symbol = insymbol();
                        fprintf(output_file,"This is an integer!\n");
                        //printf("This is an integer!\n");
                    }
                    else if(symbol == CHAR2SY && const_type == CHARSY){
                        sign = 1;
                        const_value = TOKEN[1];
                        //填表，生成四元式
                        address = address + 1;
                        append_to_table(name_of_id,0,2,const_value,-1,address);
                        sprintf(TEMP,"%d",const_value);
                        generate_mid_code(2,name_of_id,TEMP,CODE_EMPTY);
                        symbol = insymbol();
                        fprintf(output_file,"This is a char!\n");
                        //printf("This is a char!\n");
                    }
                    else{
                        error(CONST_DEF_TYPE_ERROR,line);
                        do{
                            symbol = insymbol();
                        }while(symbol !=COMMASY && symbol != SEMISY);
                        continue;
                    }
                }
                else{
                    error(CONST_DEF_ASSIGN_ERROR,line);
                    do{
                        symbol = insymbol();
                    }while(symbol !=COMMASY && symbol != SEMISY);
                    continue;
                }
            }
            else{
                error(ID_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol !=COMMASY && symbol != SEMISY);
                continue;                                 //?
            }
        }while(symbol == COMMASY);
        fprintf(output_file,"This is a constant defination!\n");
        //printf("This is a constant defination!\n");
        return;
    }
    else{
        error(CONST_DEC_TYPE_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != SEMISY);
        return;
    }
}

void var_def(){
    int real_type;
    var_type = symbol;
    if(var_type == INTSY){
        real_type = 1;
    }
    else{
        real_type = 2;
    }
    do{
        symbol = insymbol();
        if(symbol == IDSY){
            strcpy(name_of_id,TOKEN);
            symbol = insymbol();
            var_array_size = 0;
            if(symbol == COMMASY || symbol == SEMISY){
                //是普通变量，该变量填表
                address = address + 1;
                append_to_table(name_of_id,1,real_type,0,-1,address);
                if(real_type == 1){
                    generate_mid_code(3,name_of_id,CODE_EMPTY,CODE_EMPTY);
                }
                else{
                    generate_mid_code(4,name_of_id,CODE_EMPTY,CODE_EMPTY);
                }
                continue;
            }
            else if(symbol == LBRASY){         //数组
                symbol = insymbol();
                if(symbol == DIGITSY){
                    var_array_size = trans_num;
                    if(var_array_size != 0){
                        symbol = insymbol();
                        if(symbol == RBRASY){
                            //把数组的名字、大小、类型等填表
                            address = address + var_array_size;
                            append_to_table(name_of_id,1,real_type,0,var_array_size,address);
                            sprintf(TEMP,"%d",var_array_size);
                            if(real_type == 1){
                                generate_mid_code(5,name_of_id,TEMP,CODE_EMPTY);
                            }
                            else{
                                generate_mid_code(6,name_of_id,TEMP,CODE_EMPTY);
                            }
                            symbol = insymbol();
                        }
                        else{
                            error(RBRA_ERROR,line);         //0不是无符号整数
                            do{
                                symbol = insymbol();
                            }while(symbol !=COMMASY && symbol != SEMISY);
                            continue;
                        }
                    }
                    else{
                        error(VAR_DEF_ARRAYINDEX_ERROR,line);         //0不是无符号整数
                        do{
                            symbol = insymbol();
                        }while(symbol !=COMMASY && symbol != SEMISY);
                        continue;
                    }
                }
                else{
                    error(VAR_DEF_ARRAYINDEX_ERROR,line);
                    do{
                        symbol = insymbol();
                    }while(symbol !=COMMASY && symbol != SEMISY);
                    continue;
                }
            }
            else{
                error(ID_DEC_ERROR,line);          //标识符格式错误
                do{
                    symbol = insymbol();
                }while(symbol !=COMMASY && symbol != SEMISY);
                continue;
            }
        }
        else{
            error(ID_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol !=COMMASY && symbol != SEMISY);
            continue;
        }
    }while(symbol == COMMASY);
    fprintf(output_file,"This is a variable defination!\n");
    //printf("This is a variable defination!\n");
}

void dec_head(){
    int real_type;
    if(type_symbol == INTSY){
        real_type = 1;
    }
    else{
        real_type = 2;
    }
    symbol = insymbol();
    if(symbol == IDSY){
        strcpy(func_name,TOKEN);
        address = 0;
        append_to_table(func_name,2,real_type,0,0,address);
        //将函数的类型+名称填表
        symbol = insymbol();
        fprintf(output_file,"This is a defination head!\n");
        //printf("This is a defination head!\n");
        return;
    }
    else{
        error(ID_ERROR,line);
        return;
    }
}

void para_list(){
    int real_type;
    para_num = 0;
    do{
        if(symbol == COMMASY){
            symbol = insymbol();
        }
        if(symbol == INTSY || symbol == CHARSY){
            var_type = symbol;
            if(var_type == INTSY){
                real_type = 1;
            }
            else{
                real_type = 2;
            }
            symbol = insymbol();
            if(symbol == IDSY){
                para_num++;
                strcpy(name_of_id,TOKEN);
                address = address + 1;
                append_to_table(name_of_id,3,real_type,0,-1,address);
                if(real_type == 1){
                    generate_mid_code(10,name_of_id,CODE_INT,CODE_EMPTY);
                }
                else{
                    generate_mid_code(10,name_of_id,CODE_CHAR,CODE_EMPTY);
                }
                //参数填表(类型在var_type中)
                symbol = insymbol();
            }
            else{
                error(ID_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != RPARSY);
                return;
            }
        }
    }while(symbol == COMMASY);
    append_para(para_num);
    //把函数的参数总体个数填表
    fprintf(output_file,"This is a parameter list!\n");
    //printf("This is a parameter list!\n");
}

void comp_state(){
    if(symbol == CONSY){
        const_dec();
    }
    if(symbol == INTSY || symbol == CHARSY){
        var_dec();
    }
    state_list();
    fprintf(output_file,"This is a composite statement!\n");
    //printf("This is a composite statement!\n");
}

void state_list(){
    while(symbol == IFSY || symbol == DOSY || symbol == SWISY || symbol == LBRASSY || symbol == IDSY || symbol == SCANSY || symbol == PRISY || symbol == SEMISY || symbol == RTSY){
        state();
    }
    fprintf(output_file,"This is a statement list!\n");
    //printf("This is a statement list!\n");
}

void state(){
    int func_type;
    init_var_num();
    if(symbol == IFSY){
        if_state();
    }
    else if(symbol == DOSY){
        loop_state();
    }
    else if(symbol == SWISY){
        switch_state();
    }
    else if(symbol == LBRASSY){
        symbol = insymbol();
        state_list();
        if(symbol == RBRASSY){
            symbol = insymbol();
        }
        else{
            error(RBRAS_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
    }
    else if(symbol == IDSY){
        //函数或者赋值
        pre_ch = CHAR;                            //记录现场
        pre_line_index = index_in_line;
        pre_symbol = symbol;
        strcpy(PRETOKEN,TOKEN);
        symbol = insymbol();
        if(symbol == LPARSY){
            //函数调用
            strcpy(TOKEN,PRETOKEN);               //恢复现场，进入函数调用处理
            CHAR = pre_ch;
            index_in_line = pre_line_index;
            symbol = pre_symbol;                  //先一并处理了
            func_type = get_func_type(TOKEN);
            //查表，确认是哪种函数，暂时都写成这样
            if(func_type == 0){
                void_fun_call();
            }
            else if(func_type == 1 || func_type == 2){
                val_fun_call();
            }
            else{
                error(FUNC_NO_DEF_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
                return;
            }
            if(symbol == SEMISY){
                symbol = insymbol();
                fprintf(output_file,"This is a statement!\n");
                //printf("This is a statement!\n");
                return;
            }
            else{
                error(SEMI_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
                return;
            }
        }
        else if(symbol == ASSIGNSY || symbol == LBRASY){
            strcpy(TOKEN,PRETOKEN);               //恢复现场，进入赋值语句处理
            CHAR = pre_ch;
            index_in_line = pre_line_index;
            symbol = pre_symbol;
            assign_state();
            if(symbol == SEMISY){
                symbol = insymbol();
                fprintf(output_file,"This is a statement!\n");
                //printf("This is a statement!\n");
                return;
            }
            else{
                error(SEMI_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
                return;
            }
        }
        else{
            error(STATE_AFTER_ID_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
    }
    else if(symbol == SCANSY){
        read_state();
        if(symbol == SEMISY){
            symbol = insymbol();
        }
        else{
            error(SEMI_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
    }
    else if(symbol == PRISY){
        write_state();
        if(symbol == SEMISY){
            symbol = insymbol();
        }
        else{
            error(SEMI_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
    }
    else if(symbol == RTSY){
        return_state();
        if(symbol == SEMISY){
            symbol = insymbol();
        }
        else{
            error(SEMI_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
    }
    else if(symbol == SEMISY){
        symbol = insymbol();
    }
    else{
        return;               //暂时跳过
    }
    fprintf(output_file,"This is a statement!\n");
    //printf("This is a statement!\n");
}

void if_state(){
    char label[100];
    char condition_result[100];
    generate_label(label);
    symbol = insymbol();
    if(symbol == LPARSY){
        symbol = insymbol();
        condition();
        strcpy(condition_result,now_result);
        generate_mid_code(22,condition_result,label,CODE_EMPTY);
        //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
        if(symbol == RPARSY){
            symbol = insymbol();
            state();
            //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
            generate_mid_code(24,label,CODE_EMPTY,CODE_EMPTY);
            fprintf(output_file,"This is an if statement!\n");
            //printf("This is an if statement!\n");
            return;
        }
        else{
            error(RPAR_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
    }
    else{
        error(LPAR_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
        return;
    }
}

void condition(){
    char num1[100];
    char num2[100];
    strcpy(num1,CODE_EMPTY);
    strcpy(num2,CODE_EMPTY);
    expr();
    strcpy(num1,now_result);
    if(symbol == EQUSY || symbol == LTSY || symbol == LESY || symbol == GTRSY || symbol == GTESY || symbol == NEQSY){                       //这里的判断保险么？？？？？？？？？？？？
        rela_op();
        expr();
        strcpy(num2,now_result);
        generate_temp_var(now_result);
        switch(rela_op_symbol){
            case EQUSY:{
                generate_mid_code(20,num1,num2,now_result);
                break;
            }
            case LTSY:{
                generate_mid_code(17,num1,num2,now_result);
                break;
            }
            case LESY:{
                generate_mid_code(16,num1,num2,now_result);
                break;
            }
            case GTRSY:{
                generate_mid_code(19,num1,num2,now_result);
                break;
            }
            case GTESY:{
                generate_mid_code(18,num1,num2,now_result);
                break;
            }
            case NEQSY:{
                generate_mid_code(21,num1,num2,now_result);
                break;
            }
        }
    }
    else{
        generate_temp_var(now_result);
        generate_mid_code(21,num1,CODE_ZERO,now_result);
    }
    //address = address+1;
    //append_to_table(now_result,1,1,0,-1,address);   233
    fprintf(output_file,"This is a condition!\n");
    //printf("This is a condition!\n");
}

void rela_op(){
    if(symbol == EQUSY || symbol == GTRSY || symbol == GTESY || symbol == LTSY || symbol == LESY || symbol == NEQSY){
        rela_op_symbol = symbol;
        symbol = insymbol();
    }
    else{
        error(WRONG_RELA_OP_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
        return;
    }
    fprintf(output_file,"This is a relational operation!\n");
    //printf("This is a relational operation!\n");
}

void loop_state(){
    char label1[100];
    generate_label(label1);
    //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
    generate_mid_code(24,label1,CODE_EMPTY,CODE_EMPTY);
    symbol = insymbol();
    state();
    if(symbol == WHISY){
        symbol = insymbol();
        if(symbol == LPARSY){
            symbol = insymbol();
            condition();
            if(symbol == RPARSY){
                symbol = insymbol();
                generate_mid_code(23,now_result,label1,CODE_EMPTY);
                //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
                fprintf(output_file,"This is a loop statement!\n");
                //printf("This is a loop statement!\n");
                return;
            }
            else{
                error(RPAR_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
                return;
            }
        }
        else{
            error(LPAR_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
    }
    else{
        error(WHILESY_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
        return;
    }
}

void switch_state(){
    char label[100];
    char for_compare[100];       //expr算出来的结果
    generate_label(label);
    symbol = insymbol();
    if(symbol == LPARSY){
        symbol = insymbol();
        expr();
        strcpy(for_compare,now_result);
        if(symbol == RPARSY){
            symbol = insymbol();
            if(symbol == LBRASSY){
                symbol = insymbol();
                situation_list(label,for_compare);
                if(symbol == RBRASSY){
                    symbol = insymbol();
                    //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
                    generate_mid_code(24,label,CODE_EMPTY,CODE_EMPTY);
                    fprintf(output_file,"This is a switch statement!\n");
                    //printf("This is a switch statement!\n");
                    return;
                }
                else{
                    error(RBRAS_ERROR,line);
                    do{
                        symbol = insymbol();
                    }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
                    return;
                }
            }
            else{
                error(LBRAS_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
                return;
            }
        }
        else{
            error(RPAR_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
    }
    else{
        error(LPAR_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
        return;
    }
}

void situation_list(char label[],char for_compare[]){
    do{
        case_state(label,for_compare);
    }while(symbol == CASESY);
    fprintf(output_file,"This is a situation list!\n");
    //printf("This is a situation list!\n");
}

void case_state(char label[],char for_compare[]){
    char compare_result[100];
    char my_label[100];
    //generate_temp_var(compare_result);
    generate_new_temp_var(compare_result);
    //address = address+1;
    //append_to_table(compare_result,1,1,0,-1,address);
    generate_label(my_label);
    if(symbol == CASESY){
        symbol = insymbol();
        if(symbol == CHAR2SY){
            const_value = TOKEN[1];
            sprintf(TEMP,"%d",const_value);
            generate_mid_code(20,for_compare,TEMP,compare_result);
            generate_mid_code(22,compare_result,my_label,CODE_EMPTY);
            //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
            fprintf(output_file,"This is a char!\n");
            //printf("This is a char!\n");
            fprintf(output_file,"This is a constant!\n");
            //printf("This is a constant!\n");
        }
        else if(symbol == PLUSSY || symbol == MINUSSY){
            symbol = insymbol();
            if(symbol != DIGITSY){
                error(AFTER_OP_DIGIT_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
                return;
            }
            if(trans_num == 0){
                error(AFTER_OP_DIGIT_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
                return;
            }
            generate_mid_code(20,for_compare,TOKEN,compare_result);
            generate_mid_code(22,compare_result,my_label,CODE_EMPTY);
            //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
            fprintf(output_file,"This is an integer!\n");
            //printf("This is an integer!\n");
            fprintf(output_file,"This is a constant!\n");
            //printf("This is a constant!\n");
        }
        else if(symbol == DIGITSY){
            generate_mid_code(20,for_compare,TOKEN,compare_result);
            generate_mid_code(22,compare_result,my_label,CODE_EMPTY);
            //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
            fprintf(output_file,"This is an integer!\n");
            //printf("This is an integer!\n");
            fprintf(output_file,"This is a constant!\n");
            //printf("This is a constant!\n");
        }
        else{
            error(CASE_CONST_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
        symbol = insymbol();
        if(symbol != COLONSY){
            error(COLON_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
        symbol = insymbol();
        state();
        generate_mid_code(33,label,CODE_EMPTY,CODE_EMPTY);
        //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
        generate_mid_code(24,my_label,CODE_EMPTY,CODE_EMPTY);
        fprintf(output_file,"This is a case statement!\n");
        //printf("This is a case statement!\n");
        return;
    }
    else{
        error(CASE_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
        return;
    }
}

void val_fun_call2(){
    int type;
    char my_func_name[100];
    strcpy(my_func_name,TOKEN);           //这个保险么？？？？？func_name 应该不会贝更改？
    symbol = insymbol(); //假设标识符和(已经检查过
    symbol = insymbol();
    val_para_list();
    type = get_func_type(my_func_name);
    if(type != 1 && type != 2){
        error(FUNC_NO_DEF_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != RPARSY);
        symbol = insymbol();
        return;
    }
    if(symbol != RPARSY){
        error(RPAR_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
        return;
    }
    generate_temp_var(now_result);
    //address = address+1;
    //append_to_table(now_result,1,1,0,-1,address);
    generate_mid_code(14,my_func_name,CODE_EMPTY,now_result);
    if(factor_type == 0){
        if(type == 1){
            factor_type = 1;
        }
        else{
            factor_type = 2;
        }
    }
    symbol = insymbol();
    fprintf(output_file,"This is a value function call!\n");
    //printf("This is a value function call!\n");
}

void val_fun_call(){
    int type;
    char my_func_name[100];
    strcpy(my_func_name,TOKEN);           //这个保险么？？？？？func_name 应该不会贝更改？
    symbol = insymbol(); //假设标识符和(已经检查过
    symbol = insymbol();
    val_para_list();
    type = get_func_type(my_func_name);
    if(type != 1 && type != 2){
        error(FUNC_NO_DEF_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != RPARSY);
        symbol = insymbol();
        return;
    }
    if(symbol != RPARSY){
        error(RPAR_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
        return;
    }
    generate_mid_code(14,my_func_name,CODE_EMPTY,CODE_EMPTY);
    symbol = insymbol();
    fprintf(output_file,"This is a value function call!\n");
    //printf("This is a value function call!\n");
}

void void_fun_call(){
    int type;
    char my_func_name[100];
    strcpy(my_func_name,TOKEN);           //这个保险么？？？？？func_name 应该不会贝更改？
    symbol = insymbol(); //假设标识符和(已经检查过
    symbol = insymbol();
    val_para_list();
    if(symbol != RPARSY){
        error(RPAR_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
        return;
    }
    type = get_func_type(my_func_name);
    if(type != 0){
        error(FUNC_NO_DEF_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != RPARSY);
        symbol = insymbol();
        return;
    }
    symbol = insymbol();
    generate_mid_code(15,my_func_name,CODE_EMPTY,CODE_EMPTY);
    fprintf(output_file,"This is a void function call!\n");
    //printf("This is a void function call!\n");
}

void val_para_list(){
    para_num = 0;
    do{
        if(symbol == COMMASY){
            symbol = insymbol();
        }
        if(symbol == PLUSSY || symbol == MINUSSY || symbol == IDSY || symbol == CHAR2SY || symbol == DIGITSY || symbol == LPARSY){
            expr();
            generate_mid_code(35,now_result,CODE_EMPTY,CODE_EMPTY);
            para_num++;
        }
    }while(symbol == COMMASY);
    fprintf(output_file,"This is a value parameter list!\n");
    //printf("This is a value parameter list!\n");
    return;
}

void assign_state(){
    char src1[100];
    char src2[100];
    char result[100];
    int nothing;
    //char temp[50];
    strcpy(result,TOKEN);
    symbol = insymbol();
    if(symbol == ASSIGNSY){
        symbol = insymbol();
        expr();
        nothing = index_in_table(result,0); //这里要判断：1.常量赋值错误  2.char被int赋值 3.数组整体赋值
        if(const_flag){
            error(CONST_ASSIGNMENT_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
        if(array_flag){
            error(ARRAY_ASSIGNMENT_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
        if(nothing == 2 && factor_type == 1){
            error(CHAR_ASSIGNMENT_ERROR,line);
        }
        strcpy(src1,now_result);
        generate_mid_code(11,src1,CODE_EMPTY,result);
        fprintf(output_file,"This is an assignment statement!\n");
        //printf("This is an assignment statement!\n");
        return;
    }
    else{                     //数组元素赋值
        symbol = insymbol();
        expr();
        strcpy(src2,now_result);
        if(symbol != RBRASY){
            error(RBRA_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
        symbol = insymbol();
        if(symbol != ASSIGNSY){
            error(ASSIGN_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
        symbol = insymbol();
        expr();
        strcpy(src1,now_result);
        generate_mid_code(12,src1,src2,result);
        fprintf(output_file,"This is an assignment statement!\n");
        //printf("This is an assignment statement!\n");
        return;
    }
}

void read_state(){
    char var_name[100];
    int get_index;
    symbol = insymbol();
    if(symbol != LPARSY){
        error(LPAR_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
        return;
    }
    do{
        symbol = insymbol();
        if(symbol != IDSY){
            error(ID_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
        strcpy(var_name,TOKEN);
        get_index = index_in_table(var_name,0);
        factor_type = get_index;
        if(var_flag == 0){
            error(READ_ARRAY_ERROR,line);
        }
        if(factor_type == 1){
            generate_mid_code(27,CODE_INT,CODE_EMPTY,var_name);
        }
        else{
            generate_mid_code(27,CODE_CHAR,CODE_EMPTY,var_name);
        }
        symbol = insymbol();
    }while(symbol == COMMASY);
    if(symbol != RPARSY){
        error(RPAR_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
        return;
    }
    symbol = insymbol();
    fprintf(output_file,"This is a read statement!\n");
    //printf("This is a read statement!\n");
}

void write_state(){
    char src1[100];
    char src2[100];
    strcpy(src1,CODE_EMPTY);
    strcpy(src2,CODE_EMPTY);
    symbol = insymbol();
    if(symbol != LPARSY){
        error(LPAR_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
        return;
    }
    symbol = insymbol();
    if(symbol == STRINGSY){
        strcpy(src1,TOKEN);
        fprintf(output_file,"This is a string!\n");
        //printf("This is a string!\n");
        symbol = insymbol();
        if(symbol == COMMASY){
            symbol = insymbol();
            expr();
            if(symbol != RPARSY){
                error(RPAR_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
                return;
            }
            strcpy(src2,now_result);
            symbol = insymbol();
            if(factor_type == 1){
                generate_mid_code(28,src1,src2,CODE_INT);
            }
            else{
                generate_mid_code(28,src1,src2,CODE_CHAR);
            }
            fprintf(output_file,"This is a write statement!\n");
            //printf("This is a write statement!\n");
            return;
        }
        else if(symbol == RPARSY){
            symbol = insymbol();
            generate_mid_code(28,src1,src2,CODE_EMPTY);
            fprintf(output_file,"This is a write statement!\n");
            //printf("This is a write statement!\n");
            return;
        }
        else{
            error(RPAR_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
        //暂时没有else
    }
    else{
        expr();
        if(symbol != RPARSY){
            error(RPAR_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
        strcpy(src2,now_result);
        if(factor_type == 1){
            generate_mid_code(28,src1,src2,CODE_INT);
        }
        else{
            generate_mid_code(28,src1,src2,CODE_CHAR);
        }
        symbol = insymbol();
        fprintf(output_file,"This is a write statement!\n");
        //printf("This is a write statement!\n");
        return;
    }
}

void return_state(){
    char src[100];
    symbol = insymbol();
    if(symbol == LPARSY){
        if(void_flag == 1){
            error(VOID_RT_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != SEMISY);
            return;
        }
        rt_flag = 1;
        symbol = insymbol();
        expr();
        strcpy(src,now_result);
        if(symbol != RPARSY){
            error(RPAR_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
        generate_mid_code(26,src,CODE_EMPTY,CODE_EMPTY);
        //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
        symbol = insymbol();
        fprintf(output_file,"This is a return statement!\n");
        //printf("This is a return statement!\n");
        return;
    }
    else{
        if(val_flag == 1){
            error(VAL_RT_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != SEMISY && symbol != RTSY);
            return;
        }
        if(main_flag){
            generate_mid_code(34,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
            //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
        }
        else{
            generate_mid_code(25,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
            //generate_mid_code(37,CODE_EMPTY,CODE_EMPTY,CODE_EMPTY);
        }
        fprintf(output_file,"This is a return statement!\n");
        //printf("This is a return statement!\n");
        return;
    }
}

void expr(){
    factor_type = 0;
    char src1[100];
    char src2[100];
    char result[100];
    if(symbol == PLUSSY || symbol == MINUSSY){
        factor_type = 1;
        if(symbol == PLUSSY){
            symbol = insymbol();
            term();
            strcpy(result,now_result);
        }
        else{
            symbol = insymbol();
            term();
            strcpy(src2,now_result);
            generate_temp_var(result);
            //address = address+1;
            //append_to_table(result,1,1,0,-1,address);
            generate_mid_code(30,CODE_ZERO,src2,result);
        }
        while(symbol == PLUSSY || symbol == MINUSSY){
            factor_type = 1;
            strcpy(src1,result);
            generate_temp_var(result);
            //address = address+1;
            //append_to_table(result,1,1,0,-1,address);
            if(symbol == PLUSSY){                   //默认用的是同一个临时变量存储的
                symbol = insymbol();
                term();
                strcpy(src2,now_result);
                generate_mid_code(29,src1,src2,result);
            }
            else{
                symbol = insymbol();
                term();
                strcpy(src2,now_result);
                generate_mid_code(30,src1,src2,result);
            }
        }
    }
    else{
        term();
        strcpy(result,now_result);
        while(symbol == PLUSSY || symbol == MINUSSY){
            factor_type = 1;
            strcpy(src1,result);
            generate_temp_var(result);
            //address = address+1;
            //append_to_table(result,1,1,0,-1,address);
            if(symbol == PLUSSY){                   //默认用的是同一个临时变量存储的
                symbol = insymbol();
                term();
                strcpy(src2,now_result);
                generate_mid_code(29,src1,src2,result);
            }
            else{
                symbol = insymbol();
                term();
                strcpy(src2,now_result);
                generate_mid_code(30,src1,src2,result);
            }
        }
    }
    strcpy(now_result,result);
    fprintf(output_file,"This is an expression!\n");
    //printf("This is an expression!\n");
}

void term(){
    char src1[100];
    char src2[100];
    char result[100];
    factor();
    strcpy(result,now_result);
    while(symbol == STARSY || symbol == DIVSY){
        strcpy(src1,result);
        generate_temp_var(result);
        //address = address+1;
        //append_to_table(result,1,1,0,-1,address);
        if(symbol == STARSY){
            symbol = insymbol();
            factor();
            strcpy(src2,now_result);
            generate_mid_code(31,src1,src2,result);
        }
        else{
            symbol = insymbol();
            factor();
            strcpy(src2,now_result);
            generate_mid_code(32,src1,src2,result);
        }

    }
    strcpy(now_result,result);
    fprintf(output_file,"This is a term!\n");
    //printf("This is a term!\n");
}

void factor(){
    char result[100];
    char id_record[100];
    int temp_type;
    int sign;
    strcpy(result,CODE_EMPTY);
    if(symbol == IDSY){
        strcpy(id_record,TOKEN);
        //保存现场？
        pre_ch = CHAR;                            //保护现场
        pre_line_index = index_in_line;
        pre_symbol = symbol;
        strcpy(PRETOKEN,TOKEN);
        symbol = insymbol();
        if(symbol == LBRASY){
            symbol = insymbol();
            temp_type = factor_type;
            expr();
            factor_type = temp_type;
            if(symbol != RBRASY){
                error(RBRA_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != SEMISY && symbol != ASSIGNSY && symbol != PLUSSY && symbol != MINUSSY && symbol != STARSY && symbol != DIVSY && symbol != RPARSY && symbol != COMMASY && symbol != LTSY && symbol != LESY && symbol != GTRSY && symbol != GTESY && symbol != EQUSY && symbol != NEQSY);
                return;
            }
            //这个地方应该是要检查变量是否存在的
            temp_type = index_in_table(id_record,0);
            if(array_flag == 0){
                error(ID_NO_DEF_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != SEMISY && symbol != ASSIGNSY && symbol != PLUSSY && symbol != MINUSSY && symbol != STARSY && symbol != DIVSY && symbol != RPARSY && symbol != COMMASY && symbol != LTSY && symbol != LESY && symbol != GTRSY && symbol != GTESY && symbol != EQUSY && symbol != NEQSY);
                return;
            }
            if(factor_type == 0){
                factor_type = temp_type;
            }
            strcpy(result,now_result);
            generate_temp_var(now_result);
            //address = address+1;
            //append_to_table(now_result,1,1,0,-1,address);
            generate_mid_code(36,id_record,result,now_result);
            symbol = insymbol();
            fprintf(output_file,"This is a factor!\n");
            //printf("This is a factor!\n");
            return;
        }
        else if(symbol == LPARSY){
            strcpy(TOKEN,PRETOKEN);               //恢复现场
            CHAR = pre_ch;
            index_in_line = pre_line_index;
            symbol = pre_symbol;
            val_fun_call2();
            fprintf(output_file,"This is a factor!\n");
            //printf("This is a factor!\n");
            return;
        }
        else{
            if(factor_type == 0){
                factor_type = index_in_table(id_record,0);
            }
            temp_type = index_in_table(id_record,0);
            if(const_flag == 1){
                temp_type = get_const_value(id_record);
                sprintf(now_result,"%d",temp_type);
                const_flag = 0;
            }
            else{
                strcpy(now_result,id_record);
            }
            fprintf(output_file,"This is a factor!\n");
            //printf("This is a factor!\n");
            return;
        }
    }
    else if(symbol == CHAR2SY){
        if(factor_type == 0){
            factor_type = 2;
        }
        const_value = TOKEN[1];
        sprintf(now_result,"%d",const_value);
        symbol = insymbol();
        fprintf(output_file,"This is a char!\n");
        //printf("This is a char!\n");
        fprintf(output_file,"This is a factor!\n");
        //printf("This is a factor!\n");
        return;
    }
    else if(symbol == PLUSSY || symbol == MINUSSY){
        if(symbol == PLUSSY){
            sign = 1;
        }
        else{
            sign = -1;
        }
        symbol = insymbol();
        if(symbol != DIGITSY){
            error(AFTER_OP_DIGIT_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != SEMISY && symbol != ASSIGNSY && symbol != PLUSSY && symbol != MINUSSY && symbol != STARSY && symbol != DIVSY && symbol != RPARSY && symbol != COMMASY && symbol != LTSY && symbol != LESY && symbol != GTRSY && symbol != GTESY && symbol != EQUSY && symbol != NEQSY);
            return;
        }
        if(trans_num == 0){
            error(AFTER_OP_DIGIT_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != SEMISY && symbol != ASSIGNSY && symbol != PLUSSY && symbol != MINUSSY && symbol != STARSY && symbol != DIVSY && symbol != RPARSY && symbol != COMMASY && symbol != LTSY && symbol != LESY && symbol != GTRSY && symbol != GTESY && symbol != EQUSY && symbol != NEQSY);
            return;
        }
        trans_num = sign*trans_num;
        sprintf(now_result,"%ld",trans_num);
        factor_type = 1;
        symbol = insymbol();
        fprintf(output_file,"This is an integer!\n");
        //printf("This is an integer!\n");
        fprintf(output_file,"This is a factor!\n");
        //printf("This is a factor!\n");
        return;
    }
    else if(symbol == DIGITSY){
        sprintf(now_result,"%ld",trans_num);
        factor_type = 1;
        symbol = insymbol();
        fprintf(output_file,"This is an integer!\n");
        //printf("This is an integer!\n");
        fprintf(output_file,"This is a factor!\n");
        //printf("This is a factor!\n");
        return;
    }
    else if(symbol == LPARSY){
        symbol = insymbol();
        expr();
        if(symbol != RPARSY){
            error(RPAR_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != SEMISY && symbol != ASSIGNSY && symbol != PLUSSY && symbol != MINUSSY && symbol != STARSY && symbol != DIVSY && symbol != RPARSY && symbol != COMMASY && symbol != LTSY && symbol != LESY && symbol != GTRSY && symbol != GTESY && symbol != EQUSY && symbol != NEQSY);
            return;
        }
        symbol = insymbol();
        fprintf(output_file,"This is a factor!\n");
        //printf("This is a factor!\n");
        return;
    }
    else{
        error(FACTOR_ERROR,line);
        do{
            symbol = insymbol();
        }while(symbol != SEMISY && symbol != ASSIGNSY && symbol != PLUSSY && symbol != MINUSSY && symbol != STARSY && symbol != DIVSY && symbol != RPARSY && symbol != COMMASY && symbol != LTSY && symbol != LESY && symbol != GTRSY && symbol != GTESY && symbol != EQUSY && symbol != NEQSY);
        return;
    }
}

