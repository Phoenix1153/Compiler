#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordtest.c"
#include "optimizer.c"
//���뵥�����Ͷ���
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
#define RTSY 13                   //return          ǰ13���Ǳ�����
#define IDSY 14                   //��ͨ��ʶ��
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
#define DIGITSY 34                //�޷�������
#define CHAR2SY 35                //�ַ�
#define STRINGSY 36               //�ַ���
#define SEMISY 37                 //;

#define MAXLIST 1024
#define MAXNAME 50
#define MAXFUNC 100
/*
�Ը����ɷֵĶ���(����&����˼Ϊ��û�е�����Ƴ���)
void program();               ����
void const_dec();             ��������
void var_dec();               ��������                  (ʵ����������ڶ��ֻ������жϲ�ͬ����������������Щʱ�򲻻�ʹ��)
void val_func_def();          �з���ֵ��������
void void_func_def();         �޷���ֵ��������
void main_func();             ������
void const_def();             ��������
void var_def();               ��������
void dec_head();              ����ͷ��
void para_list();             ������
void comp_state();            �������
void integer();               &����
void type_id();               &����ʶ���
void state_list();            �����
void if_state();              �������
void state();                 ���
void condition();             ����
void rela_op();               ��ϵ�����
void loop_state();            ѭ�����
void switch_state();          ������
void situation_list();        �����
void case_state();            ��������
void constant();              &����
void val_fun_call();          �з���ֵ�����������
void void_fun_call();         �޷���ֵ�����������
void val_para_list();         ֵ������
void assign_state();          ��ֵ���
void read_state();            �����
void write_state();           д���
void return_sate();           �������
void expr();                  ���ʽ
void plus_op();               �ӷ������
void mult_op();               �˷������
void term();                  ��
void factor();                ����
*/
extern FILE* compile_file;
extern FILE* output_file;     //�ʷ��������������ļ�
int symbol;                   //����ĵ������ͺ�
//������Ƶ�ȫ�ֱ���
//1.�����ֳ��õ�
int pre_symbol;               //��ʱ��¼symbol
char pre_ch;                  //��ʱ��¼CHAR
int pre_line_index;           //��ʱ��¼index_in_line
char PRETOKEN[100];            //����������ʱ����ʱ��¼TOKEN��ֵ
//2.����ű��õ�ȫ�ֱ���
//(0)���õ�
int address = 0;                  //����ڵ�ǰAR��ƫ�Ƶ�ַ
char TEMP[100];
int array_flag = 0;               //��ʶ���ǲ�������
int var_flag = 0;
int const_flag = 0;
int para_flag = 0;
int global_flag = 0;              //��ǰ�ı������߳����ǲ���ȫ�ֵ�
//(1)���������͵�������Ҫ��
int type_symbol;              //�ں��������м�¼������type
char func_name[100];           //��¼�������ƣ������Ͳ�����Ӧ��
int para_num;                 //��¼�����Ĳ��������������
int rt_flag;                  //����з���ֵ�����ǲ�����return���
int main_flag = 0;            //�ڴ���RT���ʱ�������main��ֱ���˳�
int void_flag = 0;
int val_flag = 0;
//(2)������������Ҫ��
char name_of_id[100];          //���ڼ�¼��ʶ��������(���������ͱ���������)
int var_type;                 //���������м�¼���������� ���� ����������
int var_array_size;           //���������м�¼��������Ĵ�С
//(3)������������Ҫ��
int const_type;               //���������м�¼����������
int const_value;              //���������м�¼������ֵ
//(4)���ʽ����Ҫ��
int rela_op_symbol;           //��ϵ�������symbolֵ
int factor_type = 0;              //��¼��ǰfactor������
char now_result[100];
//���к�������
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
//���ű����
struct one_symbol{
    char name[MAXNAME];            //��ʶ������
    int kind;                 /*����
                                0������
                                1������
                                2������
                                3����������
                              */
    int type;                 /*����
                                0��void(�������޷���ֵ����)
                                1��int
                                2: char
                              */
    int value;                /*������ֵ��ascii��
                                ������0
                              */
    int size;                 /*��������������
                                ���飺Ԫ�ظ���
                                ������-1
                              */
    int in_address;              //���������AR�׵�ַ��ƫ����(�����¼�׵�ַ)
};
struct table{
    struct one_symbol List[MAXLIST]; //���ű�
    int list_index;           //����
    int num_of_func;          //�Ѿ������ĺ�������
    int index_of_func[MAXFUNC]; //��ʼ��ȫ-1����Ŷ�Ӯ˳��ĺ�����table����
};
struct table Table;
struct table new_Table;
struct table new_Table2;
//���ڷ��ű�Ĳ���
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
            if(strcmp(Table.List[func_index].name,name) == 0){                   //���������ظ�
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
            if(strcmp(Table.List[index].name,name) == 0){                        //��������ͬ��������
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
int index_in_table(char name[],int kind){                      //������
    int i,index,first_func_index;
    array_flag = 0;
    var_flag = 0;
    const_flag = 0;
    para_flag = 0;
    global_flag = 0;
    if(kind == 2){ //����
        for(i=0;i<Table.num_of_func;i++){
            index = Table.index_of_func[i];
            if(strcmp(name,Table.List[index].name) == 0)
                break;
        }
        if(i == Table.num_of_func){
            error(FUNC_NO_DEF_ERROR,line);
            return -1;
        }
        else{       //�ҵ���ͬ������
            index = Table.index_of_func[i];
            if(Table.List[index].size != para_num){
                error(PARA_NUM_ERROR,line);
                return index;
            }
            return index;
        }
    }
    else{ //��ͨ��ʶ��
        index = Table.index_of_func[Table.num_of_func-1];
        for(;index<=Table.list_index;index++){
            if(strcmp(Table.List[index].name,name) == 0){                        //��������ͬ��������
                break;
            }
        }
        if(index > Table.list_index){ //�����Ҳ�����ȥȫ�ֱ�����
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
            else if(Table.List[index].kind == 3){ //Ӧ�ò������
                para_flag = 1;
                return Table.List[index].type;
            }
        }
        else{                           //�����ҵ�
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
    else{       //�ҵ���ͬ������
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
    else{       //�ҵ���ͬ������
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
        if(strcmp(Table.List[index].name,name) == 0){                        //��������ͬ��������
            break;
        }
    }
    if(index > Table.list_index){ //�����Ҳ�����ȥȫ�ֱ�����
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
    else{                           //�����ҵ�
        return Table.List[index].value;
    }
}
//�����ֵĵݹ��ӳ���
void program(){               //����
    if(symbol == CONSY){
        const_dec();
    }
    while(symbol == INTSY || symbol == CHARSY){   //������������
        pre_ch = CHAR;                            //��Ϊint����char��ͷ�Ŀ����Ǳ����������ߺ������������������Ҫ�ָ��ֳ�
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
        symbol = insymbol();                      //���������һ�����ʣ��Ӷ��б��Ǳ������Ǻ���(������Ӧ����С����)
        if(symbol == LPARSY){                     //��⵽С���ţ�ȷ���Ǻ�������
            strcpy(TOKEN,PRETOKEN);               //�ָ��ֳ�������ѭ�������뺯����������
            CHAR = pre_ch;
            index_in_line = pre_line_index;
            symbol = pre_symbol;
            break;
        }
        else if(symbol == SEMISY || symbol == COMMASY || symbol == LBRASY){   //ȷ���Ǳ�������
            strcpy(TOKEN,PRETOKEN);               //�ָ��ֳ��������������ģ��
            CHAR = pre_ch;
            index_in_line = pre_line_index;
            symbol = pre_symbol;
            var_def();
            if(symbol != SEMISY){
                error(SEMI_ERROR,line);
                do{
                    symbol = insymbol();
                }while(symbol != INTSY && symbol != CHARSY && symbol != VOIDSY);        //������һ���������߱�������
                continue;
            }
            else{
                symbol = insymbol();
            }
        }
        else{
            //����Ӧ�����������Ϣ����ʶ��������ַ�����(�Ƿ�����)����skip����һ��int��char��void
            error(ID_DEC_ERROR,line);
            do{
                symbol = insymbol();
            }while(symbol != INTSY && symbol != CHARSY && symbol != VOIDSY);        //������һ���������߱�������
            continue;
        }
    }
    while(symbol == INTSY || symbol == CHARSY || symbol == VOIDSY){         //������������
        if(symbol == INTSY || symbol == CHARSY){
            val_func_def();
        }
        else{
            pre_ch = CHAR;                            //��¼�ֳ�
            pre_line_index = index_in_line;
            pre_symbol = symbol;
            strcpy(PRETOKEN,TOKEN);
            symbol = insymbol();
            if(symbol == MAINSY){
                strcpy(TOKEN,PRETOKEN);               //�ָ��ֳ���������������������ģ��
                CHAR = pre_ch;
                index_in_line = pre_line_index;
                symbol = pre_symbol;
                break;
            }
            else{
                strcpy(TOKEN,PRETOKEN);               //�ָ��ֳ��������޷���ֵ��������ģ��(�˴����Ż���)
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
            }while(symbol != INTSY && symbol != CHARSY /*&& symbol != VOIDSY*/ && symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != RTSY);        //������һ������������������
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
            }while(symbol != CONSY && symbol != INTSY && symbol != CHARSY && symbol != VOIDSY && symbol != IFSY && symbol != DOSY && symbol != SWISY && symbol != LBRASSY && symbol != IDSY && symbol != SCANSY && symbol != PRISY && symbol != RTSY);        //������һ�䳣���������߱��������������
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
            //����������+�������char
            generate_mid_code(8,func_name,CODE_EMPTY,CODE_EMPTY);
            add_func_block();
        }
        else{
            //����������+�������int
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
                    //������Ԫʽ�������Ľ������?
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
        //����������+�������void
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
                        //������Ԫʽ�������Ľ������
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
        return;                             //���Ķ�ô��
    }
    symbol = insymbol();
    if(symbol == MAINSY){
        //���+������Ԫʽ
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
            //������Ԫʽ�������Ľ������
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
        return;                             //���Ķ�ô��
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
                            //���������Ԫʽ
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
                        //���������Ԫʽ
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
                        //���������Ԫʽ
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
                //����ͨ�������ñ������
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
            else if(symbol == LBRASY){         //����
                symbol = insymbol();
                if(symbol == DIGITSY){
                    var_array_size = trans_num;
                    if(var_array_size != 0){
                        symbol = insymbol();
                        if(symbol == RBRASY){
                            //����������֡���С�����͵����
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
                            error(RBRA_ERROR,line);         //0�����޷�������
                            do{
                                symbol = insymbol();
                            }while(symbol !=COMMASY && symbol != SEMISY);
                            continue;
                        }
                    }
                    else{
                        error(VAR_DEF_ARRAYINDEX_ERROR,line);         //0�����޷�������
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
                error(ID_DEC_ERROR,line);          //��ʶ����ʽ����
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
        //������������+�������
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
                //�������(������var_type��)
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
    //�Ѻ����Ĳ�������������
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
        //�������߸�ֵ
        pre_ch = CHAR;                            //��¼�ֳ�
        pre_line_index = index_in_line;
        pre_symbol = symbol;
        strcpy(PRETOKEN,TOKEN);
        symbol = insymbol();
        if(symbol == LPARSY){
            //��������
            strcpy(TOKEN,PRETOKEN);               //�ָ��ֳ������뺯�����ô���
            CHAR = pre_ch;
            index_in_line = pre_line_index;
            symbol = pre_symbol;                  //��һ��������
            func_type = get_func_type(TOKEN);
            //���ȷ�������ֺ�������ʱ��д������
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
            strcpy(TOKEN,PRETOKEN);               //�ָ��ֳ������븳ֵ��䴦��
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
        return;               //��ʱ����
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
    if(symbol == EQUSY || symbol == LTSY || symbol == LESY || symbol == GTRSY || symbol == GTESY || symbol == NEQSY){                       //������жϱ���ô������������������������
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
    char for_compare[100];       //expr������Ľ��
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
    strcpy(my_func_name,TOKEN);           //�������ô����������func_name Ӧ�ò��ᱴ���ģ�
    symbol = insymbol(); //�����ʶ����(�Ѿ�����
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
    strcpy(my_func_name,TOKEN);           //�������ô����������func_name Ӧ�ò��ᱴ���ģ�
    symbol = insymbol(); //�����ʶ����(�Ѿ�����
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
    strcpy(my_func_name,TOKEN);           //�������ô����������func_name Ӧ�ò��ᱴ���ģ�
    symbol = insymbol(); //�����ʶ����(�Ѿ�����
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
        nothing = index_in_table(result,0); //����Ҫ�жϣ�1.������ֵ����  2.char��int��ֵ 3.�������帳ֵ
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
    else{                     //����Ԫ�ظ�ֵ
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
        //��ʱû��else
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
            if(symbol == PLUSSY){                   //Ĭ���õ���ͬһ����ʱ�����洢��
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
            if(symbol == PLUSSY){                   //Ĭ���õ���ͬһ����ʱ�����洢��
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
        //�����ֳ���
        pre_ch = CHAR;                            //�����ֳ�
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
            //����ط�Ӧ����Ҫ�������Ƿ���ڵ�
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
            strcpy(TOKEN,PRETOKEN);               //�ָ��ֳ�
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

