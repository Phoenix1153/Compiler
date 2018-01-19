#include<stdio.h>
#include<ctype.h>
#include<string.h>
#define MAX_FUNC_BLOCK 128
#define MAX_BASIC_BLOCK 256
#define MAX_NODE_NUM 100
#define MAX_NUM_ONE_NODE 10
//外部变量
extern struct mid_code_list CODE_LIST;
extern int line;
//定义结构
struct func_block{
    int start_num;
    int basic_blocks[MAX_BASIC_BLOCK];
    int b_block_index;
};

struct func_blocks{
    struct func_block FUNC_LIST[MAX_FUNC_BLOCK];
    int func_index;
};

struct node{
    char name[100];
    int node_num;  //节点号码
};

struct node_table{
    struct node NODE_LIST[MAX_NODE_NUM];
    int var_num;
};

struct node_for_dag{
    int var_num;  //一个节点对应的变量有多少
    int op;
    int left_son_num;
    int right_son_num;
    char name[MAX_NUM_ONE_NODE][100];
};

struct like_dag{
    struct node_for_dag NODE_LIST[MAX_NODE_NUM];
    int node_num;
};
//全局变量
struct func_blocks FUNC_BLOCKS;
struct node_table NODE_TABLE;
struct node_table EMPTY_NODE_TABLE;
struct like_dag DAG;
//struct mid_code_list TEMP_CODE_LIST;
//函数声明
void init_func_list();
void add_func_block();
void add_basic_block();
void delete_same_block();
void print_basic_block();
void init_node_list();
int get_node_op(char var_name[]);
void get_node_result(char var_name[],char left_son_name[],char right_son_name[],int op);
int is_calc(int op);
int already_derive(int node_num);
int get_node_num(char var_name[]);
void append_node(int node_num,char var_name[]);
void one_dag_opt(int start,int end);
void get_dag_and_opt();
void delete_unuseful_code();
void algebraic_trans();
void change_invalid_code();
void merge_temp_var();
void whole_process();
//具体实现
void init_func_list(){
    FUNC_BLOCKS.func_index = -1;
}

void add_func_block(){
    if(FUNC_BLOCKS.func_index == MAX_FUNC_BLOCK - 1){
        error(OUT_OF_FUNC_BLOCK_ERROR,line);
        return;
    }
    FUNC_BLOCKS.func_index++;
    FUNC_BLOCKS.FUNC_LIST[FUNC_BLOCKS.func_index].start_num = CODE_LIST.code_num - 1;
    FUNC_BLOCKS.FUNC_LIST[FUNC_BLOCKS.func_index].b_block_index = -1;
    //printf("func: %d\n",FUNC_BLOCKS.FUNC_LIST[FUNC_BLOCKS.func_index].start_num);
}

void add_basic_block(){
    int index,mid_code_num;
    for(index=0;index<=FUNC_BLOCKS.func_index;index++){
        mid_code_num = FUNC_BLOCKS.FUNC_LIST[index].start_num;
        FUNC_BLOCKS.FUNC_LIST[index].b_block_index++;
        FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[FUNC_BLOCKS.FUNC_LIST[index].b_block_index] = mid_code_num;
        mid_code_num++;
        for(;mid_code_num < CODE_LIST.code_num;mid_code_num++){
            if(CODE_LIST.MID_CODE[mid_code_num].op == 7 || CODE_LIST.MID_CODE[mid_code_num].op == 8 || CODE_LIST.MID_CODE[mid_code_num].op == 9)
                break;
            else if(CODE_LIST.MID_CODE[mid_code_num].op == 24){
                if((FUNC_BLOCKS.FUNC_LIST[index].b_block_index+1) >= MAX_BASIC_BLOCK){
                    error(OUT_OF_BASIC_BLOCK_ERROR,line);
                    return;
                }
                FUNC_BLOCKS.FUNC_LIST[index].b_block_index++;
                FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[FUNC_BLOCKS.FUNC_LIST[index].b_block_index] = mid_code_num;
            }
            else if(CODE_LIST.MID_CODE[mid_code_num].op == 22 || CODE_LIST.MID_CODE[mid_code_num].op == 23 || CODE_LIST.MID_CODE[mid_code_num].op == 25 || CODE_LIST.MID_CODE[mid_code_num].op == 26 || CODE_LIST.MID_CODE[mid_code_num].op == 33 || CODE_LIST.MID_CODE[mid_code_num].op == 34){
                if((mid_code_num+1) < CODE_LIST.code_num){
                    if(!(CODE_LIST.MID_CODE[mid_code_num+1].op == 7 || CODE_LIST.MID_CODE[mid_code_num+1].op == 8 || CODE_LIST.MID_CODE[mid_code_num+1].op == 9)){
                        if((FUNC_BLOCKS.FUNC_LIST[index].b_block_index+1) >= MAX_BASIC_BLOCK){
                            error(OUT_OF_BASIC_BLOCK_ERROR,line);
                            return;
                        }
                        FUNC_BLOCKS.FUNC_LIST[index].b_block_index++;
                        FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[FUNC_BLOCKS.FUNC_LIST[index].b_block_index] = mid_code_num+1;
                    }
                }
            }
        }
    }
}

void delete_same_block(){
    int index,in_index,i;
    for(index=0;index<=FUNC_BLOCKS.func_index;index++){
        for(in_index=0;in_index<=FUNC_BLOCKS.FUNC_LIST[index].b_block_index;in_index++){
            if(in_index>0 && FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index]  == FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index-1]){
                for(i=in_index;i<FUNC_BLOCKS.FUNC_LIST[index].b_block_index;i++){
                    FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[i] = FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[i+1];
                }
                FUNC_BLOCKS.FUNC_LIST[index].b_block_index--;
                in_index--;
            }
        }
    }
}

void print_basic_block(){
    int index,in_index;
    for(index=0;index<=FUNC_BLOCKS.func_index;index++){
        printf("func%d\n",index+1);
        for(in_index=0;in_index<=FUNC_BLOCKS.FUNC_LIST[index].b_block_index;in_index++){
            printf("%d\n",FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index]+1);
        }
    }
}

void init_node_list(){
    int i;
    NODE_TABLE.var_num = 0;
    EMPTY_NODE_TABLE.var_num = 0;
    DAG.node_num = 0;
    for(i=0;i<MAX_NODE_NUM;i++){
        DAG.NODE_LIST[i].var_num = 0;
    }
}

int get_node_op(char var_name[]){
    int i,node_num;
    for(i=0;i<NODE_TABLE.var_num;i++){
        if(strcmp(var_name,NODE_TABLE.NODE_LIST[i].name)==0)
            break;
    }
    if(i<NODE_TABLE.var_num){
        node_num = NODE_TABLE.NODE_LIST[i].node_num;
        return node_num;
    }
    else{
        EMPTY_NODE_TABLE.var_num++;
        if(EMPTY_NODE_TABLE.var_num >= MAX_NODE_NUM){
            error(OUT_OF_NODE_NUM,line);
            fclose(compile_file);
            fclose(output_file);
            exit(-1);
            return(-1);
        }
        NODE_TABLE.var_num++;
        if(NODE_TABLE.var_num >= MAX_NODE_NUM){
            error(OUT_OF_NODE_NUM,line);
            fclose(compile_file);
            fclose(output_file);
            exit(-1);
            return(-1);
        }
        DAG.node_num++;
        if(DAG.node_num >= MAX_NODE_NUM){
            error(OUT_OF_NODE_NUM,line);
            fclose(compile_file);
            fclose(output_file);
            exit(-1);
            return(-1);
        }
        NODE_TABLE.NODE_LIST[NODE_TABLE.var_num-1].node_num =  DAG.node_num - 1;
        strcpy(NODE_TABLE.NODE_LIST[NODE_TABLE.var_num-1].name,var_name);
        EMPTY_NODE_TABLE.NODE_LIST[EMPTY_NODE_TABLE.var_num-1].node_num =  DAG.node_num - 1;
        strcpy(EMPTY_NODE_TABLE.NODE_LIST[EMPTY_NODE_TABLE.var_num-1].name,var_name);
        DAG.NODE_LIST[DAG.node_num-1].var_num++;
        if(DAG.NODE_LIST[DAG.node_num-1].var_num >= MAX_NUM_ONE_NODE){
            error(OUT_OF_ONE_NODE_VAR,line);
            fclose(compile_file);
            fclose(output_file);
            exit(-1);
            return(-1);
        }
        DAG.NODE_LIST[DAG.node_num-1].op = -1;
        DAG.NODE_LIST[DAG.node_num-1].left_son_num = -1;
        DAG.NODE_LIST[DAG.node_num-1].right_son_num = -1;
        strcpy(DAG.NODE_LIST[DAG.node_num-1].name[DAG.NODE_LIST[DAG.node_num-1].var_num-1],var_name);
        return(DAG.node_num-1);
    }
}

void get_node_result(char var_name[],char left_son_name[],char right_son_name[],int op){
    int left_son,right_son;
    int i,j;
    left_son = get_node_op(left_son_name);
    if(op == 11){
        right_son = left_son;
    }
    else{
        right_son = get_node_op(right_son_name);
    }
    for(i=0;i<DAG.node_num;i++){
        if(DAG.NODE_LIST[i].op == op && DAG.NODE_LIST[i].left_son_num == left_son && DAG.NODE_LIST[i].right_son_num == right_son)
            break;
    }
    if(i<DAG.node_num){
        DAG.NODE_LIST[i].var_num++;
        if(DAG.NODE_LIST[i].var_num >= MAX_NUM_ONE_NODE){
            error(OUT_OF_ONE_NODE_VAR,line);
            fclose(compile_file);
            fclose(output_file);
            exit(-1);
            return;
        }
        strcpy(DAG.NODE_LIST[i].name[DAG.NODE_LIST[i].var_num-1],var_name);
        //更新节点表信息
        for(j=0;j<NODE_TABLE.var_num;j++){
            if(strcmp(var_name,NODE_TABLE.NODE_LIST[j].name)==0)
                break;
        }
        if(j<NODE_TABLE.var_num){
            NODE_TABLE.NODE_LIST[j].node_num = i;
        }
        else{
            NODE_TABLE.var_num++;
            if(NODE_TABLE.var_num >= MAX_NODE_NUM){
                error(OUT_OF_NODE_NUM,line);
                fclose(compile_file);
                fclose(output_file);
                exit(-1);
                return;
            }
            NODE_TABLE.NODE_LIST[NODE_TABLE.var_num-1].node_num = i;
            strcpy(NODE_TABLE.NODE_LIST[NODE_TABLE.var_num-1].name,var_name);
        }
    }
    else{
        DAG.node_num++;
        if(DAG.node_num >= MAX_NODE_NUM){
            error(OUT_OF_NODE_NUM,line);
            fclose(compile_file);
            fclose(output_file);
            exit(-1);
            return;
        }
        DAG.NODE_LIST[DAG.node_num-1].var_num++;
        if(DAG.NODE_LIST[DAG.node_num-1].var_num >= MAX_NUM_ONE_NODE){
            error(OUT_OF_ONE_NODE_VAR,line);
            fclose(compile_file);
            fclose(output_file);
            exit(-1);
            return;
        }
        DAG.NODE_LIST[DAG.node_num-1].op = op;
        DAG.NODE_LIST[DAG.node_num-1].left_son_num = left_son;
        DAG.NODE_LIST[DAG.node_num-1].right_son_num = right_son;
        strcpy(DAG.NODE_LIST[DAG.node_num-1].name[DAG.NODE_LIST[DAG.node_num-1].var_num-1],var_name);
        for(j=0;j<NODE_TABLE.var_num;j++){
            if(strcmp(var_name,NODE_TABLE.NODE_LIST[j].name)==0)
                break;
        }
        if(j<NODE_TABLE.var_num){
            NODE_TABLE.NODE_LIST[j].node_num = DAG.node_num-1;
        }
        else{
            NODE_TABLE.var_num++;
            if(NODE_TABLE.var_num >= MAX_NODE_NUM){
                error(OUT_OF_NODE_NUM,line);
                fclose(compile_file);
                fclose(output_file);
                exit(-1);
                return;
            }
            NODE_TABLE.NODE_LIST[NODE_TABLE.var_num-1].node_num = DAG.node_num-1;
            strcpy(NODE_TABLE.NODE_LIST[NODE_TABLE.var_num-1].name,var_name);
        }
    }
}

int is_calc(int op){
    if(op == 11 || op == 16 || op == 17 || op == 18 || op == 19 || op == 20 || op == 21 || op == 29 || op == 30 || op == 31 || op == 32)
        return 1;
    return 0;
}

int already_derive(int node_num){
    int index;
    for(index=0;index<EMPTY_NODE_TABLE.var_num;index++){
        if(node_num == EMPTY_NODE_TABLE.NODE_LIST[index].node_num)
            break;
    }
    if(index<EMPTY_NODE_TABLE.var_num)
        return 1;
    return 0;
}

int get_node_num(char var_name[]){
    int index;
    for(index=0;index<NODE_TABLE.var_num;index++){
        if(strcmp(NODE_TABLE.NODE_LIST[index].name,var_name)==0)
            break;
    }
    return NODE_TABLE.NODE_LIST[index].node_num;
}

void append_node(int node_num,char var_name[]){
    EMPTY_NODE_TABLE.var_num++;
    if(EMPTY_NODE_TABLE.var_num >= MAX_NODE_NUM){
        error(OUT_OF_NODE_NUM,line);
        fclose(compile_file);
        fclose(output_file);
        exit(-1);
        return;
    }
    EMPTY_NODE_TABLE.NODE_LIST[EMPTY_NODE_TABLE.var_num-1].node_num = node_num;
    strcpy(EMPTY_NODE_TABLE.NODE_LIST[EMPTY_NODE_TABLE.var_num-1].name,var_name);
}

void one_dag_opt(int start,int end){
    int i,j,index,k;
    char temp_var[50];
    int have_var;
    init_node_list();
    for(i=start;i<=end;i++){
        get_node_result(CODE_LIST.MID_CODE[i].result,CODE_LIST.MID_CODE[i].src1,CODE_LIST.MID_CODE[i].src2,CODE_LIST.MID_CODE[i].op);
    }
    //尝试节点号依次导出
    j = start;
    for(i=0;i<DAG.node_num;i++){
        have_var = 0;
        if(DAG.NODE_LIST[i].op == -1)
            continue;
        else{
            for(index=0;index<DAG.NODE_LIST[i].var_num;index++){
                if(get_node_num(DAG.NODE_LIST[i].name[index]) == i){
                    if(!already_derive(i)){
                        have_var = 1;
                        append_node(i,DAG.NODE_LIST[i].name[index]);
                        CODE_LIST.MID_CODE[j].op = DAG.NODE_LIST[i].op;
                        for(k=0;k<EMPTY_NODE_TABLE.var_num;k++){
                            if(DAG.NODE_LIST[i].left_son_num == EMPTY_NODE_TABLE.NODE_LIST[k].node_num)
                                break;
                        }
                        strcpy(CODE_LIST.MID_CODE[j].src1,EMPTY_NODE_TABLE.NODE_LIST[k].name);
                        for(k=0;k<EMPTY_NODE_TABLE.var_num;k++){
                            if(DAG.NODE_LIST[i].right_son_num == EMPTY_NODE_TABLE.NODE_LIST[k].node_num)
                                break;
                        }
                        strcpy(CODE_LIST.MID_CODE[j].src2,EMPTY_NODE_TABLE.NODE_LIST[k].name);
                        strcpy(CODE_LIST.MID_CODE[j].result,DAG.NODE_LIST[i].name[index]);
                        CODE_LIST.MID_CODE[j].is_effective = 1;
                        j++;
                    }
                    else{
                        have_var = 1;
                        CODE_LIST.MID_CODE[j].op = 11;
                        CODE_LIST.MID_CODE[j].is_effective = 1;
                        for(k=0;k<EMPTY_NODE_TABLE.var_num;k++){
                            if(i == EMPTY_NODE_TABLE.NODE_LIST[k].node_num)
                                break;
                        }
                        strcpy(CODE_LIST.MID_CODE[j].src1,EMPTY_NODE_TABLE.NODE_LIST[k].name);
                        strcpy(CODE_LIST.MID_CODE[j].result,DAG.NODE_LIST[i].name[index]);
                        j++;
                    }
                }
            }
            if(have_var == 0){//该节点对应的变量是没有一个导出的，需要生成临时变量，导出
                have_var = 1;
                generate_new_temp_var(temp_var);
                append_node(i,temp_var);
                CODE_LIST.MID_CODE[j].op = DAG.NODE_LIST[i].op;
                for(k=0;k<EMPTY_NODE_TABLE.var_num;k++){
                    if(DAG.NODE_LIST[i].left_son_num == EMPTY_NODE_TABLE.NODE_LIST[k].node_num)
                        break;
                }
                strcpy(CODE_LIST.MID_CODE[j].src1,EMPTY_NODE_TABLE.NODE_LIST[k].name);
                for(k=0;k<EMPTY_NODE_TABLE.var_num;k++){
                    if(DAG.NODE_LIST[i].right_son_num == EMPTY_NODE_TABLE.NODE_LIST[k].node_num)
                        break;
                }
                strcpy(CODE_LIST.MID_CODE[j].src2,EMPTY_NODE_TABLE.NODE_LIST[k].name);
                strcpy(CODE_LIST.MID_CODE[j].result,temp_var);
                CODE_LIST.MID_CODE[j].is_effective = 1;
                j++;
            }
        }
    }
    for(;j<=end;j++){
        CODE_LIST.MID_CODE[j].is_effective = 0;
    }
}

void get_dag_and_opt(){
    int index,in_index,i,j,start,end;
    //寻找连续的三目计算式
    for(index=0;index<=FUNC_BLOCKS.func_index;index++){
        for(in_index=0;in_index<=FUNC_BLOCKS.FUNC_LIST[index].b_block_index;in_index++){
            //if(is_calc(FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index]))
            //要分类讨论
            if(index == FUNC_BLOCKS.func_index && in_index == FUNC_BLOCKS.FUNC_LIST[index].b_block_index){
                for(i=FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index];i<CODE_LIST.code_num;i++){
                    if(is_calc(CODE_LIST.MID_CODE[i].op) && (i+1) < CODE_LIST.code_num && is_calc(CODE_LIST.MID_CODE[i+1].op)){
                        //找到连续的三目计算式
                        start = i;
                        for(j=i;j < CODE_LIST.code_num && is_calc(CODE_LIST.MID_CODE[j].op);j++)
                            end = j;
                        one_dag_opt(start,end);
                        i = j-1;
                    }
                }
            }
            else if(index == FUNC_BLOCKS.func_index && in_index < FUNC_BLOCKS.FUNC_LIST[index].b_block_index){
                for(i=FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index];i<FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index+1];i++){
                    if(is_calc(CODE_LIST.MID_CODE[i].op) && (i+1) < FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index+1] && is_calc(CODE_LIST.MID_CODE[i+1].op)){
                        //找到连续的三目计算式
                        start = i;
                        for(j=i;j < FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index+1] && is_calc(CODE_LIST.MID_CODE[j].op);j++)
                            end = j;
                        one_dag_opt(start,end);
                        i = j-1;
                    }
                }
            }
            else if(in_index == FUNC_BLOCKS.FUNC_LIST[index].b_block_index){
                for(i=FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index];i<FUNC_BLOCKS.FUNC_LIST[index+1].basic_blocks[0];i++){
                    if(is_calc(CODE_LIST.MID_CODE[i].op) && (i+1) < FUNC_BLOCKS.FUNC_LIST[index+1].basic_blocks[0] && is_calc(CODE_LIST.MID_CODE[i+1].op)){
                        //找到连续的三目计算式
                        start = i;
                        for(j=i;j < FUNC_BLOCKS.FUNC_LIST[index+1].basic_blocks[0] && is_calc(CODE_LIST.MID_CODE[j].op);j++)
                            end = j;
                        one_dag_opt(start,end);
                        i = j-1;
                    }
                }
            }
            else{
                for(i=FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index];i<FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index+1];i++){
                    if(is_calc(CODE_LIST.MID_CODE[i].op) && (i+1) < FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index+1] && is_calc(CODE_LIST.MID_CODE[i+1].op)){
                        //找到连续的三目计算式
                        start = i;
                        for(j=i;j < FUNC_BLOCKS.FUNC_LIST[index].basic_blocks[in_index+1] && is_calc(CODE_LIST.MID_CODE[j].op);j++)
                            end = j;
                        one_dag_opt(start,end);
                        i = j-1;
                    }
                }
            }
        }
    }
}

void delete_unuseful_code(){
    int index,code_num,code_index,use_flag,define_flag;
    for(index=0;index<=FUNC_BLOCKS.func_index;index++){
        if(index != FUNC_BLOCKS.func_index){
            for(code_num=FUNC_BLOCKS.FUNC_LIST[index].start_num;code_num<FUNC_BLOCKS.FUNC_LIST[index+1].start_num;code_num++){
                if(CODE_LIST.MID_CODE[code_num].is_effective == 0)
                    continue;
                if(CODE_LIST.MID_CODE[code_num].op == 11){
                    if(CODE_LIST.MID_CODE[code_num].result[0] == '$'){
                        use_flag = 0;
                        define_flag = 0;
                        for(code_index=code_num+1;code_index<FUNC_BLOCKS.FUNC_LIST[index+1].start_num;code_index++){
                            if(CODE_LIST.MID_CODE[code_index].is_effective == 0)
                                continue;
                            if(strcmp(CODE_LIST.MID_CODE[code_index].result,CODE_LIST.MID_CODE[code_num].result)==0){
                                if(strcmp(CODE_LIST.MID_CODE[code_index].src1,CODE_LIST.MID_CODE[code_num].result)==0){
                                    use_flag = 1;
                                    continue;
                                }
                                if(strcmp(CODE_LIST.MID_CODE[code_index].src2,CODE_LIST.MID_CODE[code_num].result)==0){
                                    use_flag = 1;
                                    continue;
                                }
                                define_flag = 1;
                            }
                            if(strcmp(CODE_LIST.MID_CODE[code_index].src1,CODE_LIST.MID_CODE[code_num].result)==0 && define_flag==0)
                                use_flag = 1;
                            if(strcmp(CODE_LIST.MID_CODE[code_index].src2,CODE_LIST.MID_CODE[code_num].result)==0 && define_flag==0)
                                use_flag = 1;
                        }
                        if(use_flag==0){
                            CODE_LIST.MID_CODE[code_num].is_effective = 0;
                        }
                    }
                }
            }
        }
        else{
            for(code_num=FUNC_BLOCKS.FUNC_LIST[index].start_num;code_num<CODE_LIST.code_num;code_num++){
                if(CODE_LIST.MID_CODE[code_num].is_effective == 0)
                    continue;
                if(CODE_LIST.MID_CODE[code_num].op == 11){
                    if(CODE_LIST.MID_CODE[code_num].result[0] == '$'){
                        use_flag = 0;
                        define_flag = 0;
                        for(code_index=code_num+1;code_index<CODE_LIST.code_num;code_index++){
                            if(CODE_LIST.MID_CODE[code_index].is_effective == 0)
                                continue;
                            if(strcmp(CODE_LIST.MID_CODE[code_index].result,CODE_LIST.MID_CODE[code_num].result)==0){
                                if(strcmp(CODE_LIST.MID_CODE[code_index].src1,CODE_LIST.MID_CODE[code_num].result)==0){
                                    use_flag = 1;
                                    continue;
                                }
                                if(strcmp(CODE_LIST.MID_CODE[code_index].src2,CODE_LIST.MID_CODE[code_num].result)==0){
                                    use_flag = 1;
                                    continue;
                                }
                                define_flag = 1;
                            }
                            if(strcmp(CODE_LIST.MID_CODE[code_index].src1,CODE_LIST.MID_CODE[code_num].result)==0 && define_flag==0)
                                use_flag = 1;
                            if(strcmp(CODE_LIST.MID_CODE[code_index].src2,CODE_LIST.MID_CODE[code_num].result)==0 && define_flag==0)
                                use_flag = 1;
                        }
                        if(use_flag==0){
                            CODE_LIST.MID_CODE[code_num].is_effective = 0;
                        }
                    }
                }
            }
        }
    }
}

void algebraic_trans(){
    int index,temp_result;
    for(index=0;index<CODE_LIST.code_num;index++){
        if(CODE_LIST.MID_CODE[index].is_effective == 0)
            continue;
        switch(CODE_LIST.MID_CODE[index].op){
            case 16:{
                if(isdigit(CODE_LIST.MID_CODE[index].src1[0]) && isdigit(CODE_LIST.MID_CODE[index].src2[0])){
                    if(atoi(CODE_LIST.MID_CODE[index].src1) <= atoi(CODE_LIST.MID_CODE[index].src2))
                        temp_result = 1;
                    else
                        temp_result = 0;
                    sprintf(CODE_LIST.MID_CODE[index].src1,"%d",temp_result);
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                break;
            }
            case 17:{
                if(isdigit(CODE_LIST.MID_CODE[index].src1[0]) && isdigit(CODE_LIST.MID_CODE[index].src2[0])){
                    if(atoi(CODE_LIST.MID_CODE[index].src1) < atoi(CODE_LIST.MID_CODE[index].src2))
                        temp_result = 1;
                    else
                        temp_result = 0;
                    sprintf(CODE_LIST.MID_CODE[index].src1,"%d",temp_result);
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                break;
            }
            case 18:{
                if(isdigit(CODE_LIST.MID_CODE[index].src1[0]) && isdigit(CODE_LIST.MID_CODE[index].src2[0])){
                    if(atoi(CODE_LIST.MID_CODE[index].src1) >= atoi(CODE_LIST.MID_CODE[index].src2))
                        temp_result = 1;
                    else
                        temp_result = 0;
                    sprintf(CODE_LIST.MID_CODE[index].src1,"%d",temp_result);
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                break;
            }
            case 19:{
                if(isdigit(CODE_LIST.MID_CODE[index].src1[0]) && isdigit(CODE_LIST.MID_CODE[index].src2[0])){
                    if(atoi(CODE_LIST.MID_CODE[index].src1) > atoi(CODE_LIST.MID_CODE[index].src2))
                        temp_result = 1;
                    else
                        temp_result = 0;
                    sprintf(CODE_LIST.MID_CODE[index].src1,"%d",temp_result);
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                break;
            }
            case 20:{
                if(isdigit(CODE_LIST.MID_CODE[index].src1[0]) && isdigit(CODE_LIST.MID_CODE[index].src2[0])){
                    if(atoi(CODE_LIST.MID_CODE[index].src1) == atoi(CODE_LIST.MID_CODE[index].src2))
                        temp_result = 1;
                    else
                        temp_result = 0;
                    sprintf(CODE_LIST.MID_CODE[index].src1,"%d",temp_result);
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                break;
            }
            case 21:{
                if(isdigit(CODE_LIST.MID_CODE[index].src1[0]) && isdigit(CODE_LIST.MID_CODE[index].src2[0])){
                    if(atoi(CODE_LIST.MID_CODE[index].src1) != atoi(CODE_LIST.MID_CODE[index].src2))
                        temp_result = 1;
                    else
                        temp_result = 0;
                    sprintf(CODE_LIST.MID_CODE[index].src1,"%d",temp_result);
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                break;
            }
            case 29:{
                if(isdigit(CODE_LIST.MID_CODE[index].src1[0]) && isdigit(CODE_LIST.MID_CODE[index].src2[0])){
                    temp_result = atoi(CODE_LIST.MID_CODE[index].src1) + atoi(CODE_LIST.MID_CODE[index].src2);
                    sprintf(CODE_LIST.MID_CODE[index].src1,"%d",temp_result);
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                break;
            }
            case 30:{
                if(isdigit(CODE_LIST.MID_CODE[index].src1[0]) && isdigit(CODE_LIST.MID_CODE[index].src2[0])){
                    temp_result = atoi(CODE_LIST.MID_CODE[index].src1) - atoi(CODE_LIST.MID_CODE[index].src2);
                    sprintf(CODE_LIST.MID_CODE[index].src1,"%d",temp_result);
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                break;
            }
            case 31:{
                if(isdigit(CODE_LIST.MID_CODE[index].src1[0]) && isdigit(CODE_LIST.MID_CODE[index].src2[0])){
                    temp_result = atoi(CODE_LIST.MID_CODE[index].src1) * atoi(CODE_LIST.MID_CODE[index].src2);
                    sprintf(CODE_LIST.MID_CODE[index].src1,"%d",temp_result);
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                break;
            }
            case 32:{
                if(isdigit(CODE_LIST.MID_CODE[index].src1[0]) && isdigit(CODE_LIST.MID_CODE[index].src2[0])){
                    temp_result = atoi(CODE_LIST.MID_CODE[index].src1) / atoi(CODE_LIST.MID_CODE[index].src2);
                    sprintf(CODE_LIST.MID_CODE[index].src1,"%d",temp_result);
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                break;
            }
        }
    }
}

void change_invalid_code(){
    int index;
    for(index=0;index<CODE_LIST.code_num;index++){
        if(CODE_LIST.MID_CODE[index].is_effective == 0)
            continue;
        switch(CODE_LIST.MID_CODE[index].op){
            case 11:{
                if(strcmp(CODE_LIST.MID_CODE[index].src1,CODE_LIST.MID_CODE[index].result)== 0){
                    CODE_LIST.MID_CODE[index].is_effective = 0;
                }
                break;
            }
            case 29:{
                if(strcmp(CODE_LIST.MID_CODE[index].src2,"0")==0){
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                else if(strcmp(CODE_LIST.MID_CODE[index].src1,"0")==0){
                    CODE_LIST.MID_CODE[index].op = 11;
                    strcpy(CODE_LIST.MID_CODE[index].src1,CODE_LIST.MID_CODE[index].src2);
                }
                break;
            }
            case 30:{
                if(strcmp(CODE_LIST.MID_CODE[index].src2,"0")==0){
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                break;
            }
            case 31:{
                if(strcmp(CODE_LIST.MID_CODE[index].src2,"1")==0){
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                else if(strcmp(CODE_LIST.MID_CODE[index].src1,"1")==0){
                    CODE_LIST.MID_CODE[index].op = 11;
                    strcpy(CODE_LIST.MID_CODE[index].src1,CODE_LIST.MID_CODE[index].src2);
                }
                break;
            }
            case 32:{
                if(strcmp(CODE_LIST.MID_CODE[index].src2,"1")==0){
                    CODE_LIST.MID_CODE[index].op = 11;
                }
                break;
            }
        }
    }
}


void merge_temp_var(){
    int index,code_num,code_index,use_flag,define_flag;
    for(index=0;index<=FUNC_BLOCKS.func_index;index++){
        if(index != FUNC_BLOCKS.func_index){
            for(code_num=FUNC_BLOCKS.FUNC_LIST[index].start_num+1;code_num<FUNC_BLOCKS.FUNC_LIST[index+1].start_num;code_num++){
                if(CODE_LIST.MID_CODE[code_num].is_effective == 0)
                    continue;
                if(CODE_LIST.MID_CODE[code_num].op == 11){
                    if(CODE_LIST.MID_CODE[code_num-1].is_effective == 1 && CODE_LIST.MID_CODE[code_num].src1[0] == '$' && strcmp(CODE_LIST.MID_CODE[code_num].src1,CODE_LIST.MID_CODE[code_num-1].result)==0){
                        use_flag = 0;
                        define_flag = 0;
                        for(code_index=code_num+1;code_index<FUNC_BLOCKS.FUNC_LIST[index+1].start_num;code_index++){
                            if(CODE_LIST.MID_CODE[code_index].is_effective == 0)
                                continue;
                            if(strcmp(CODE_LIST.MID_CODE[code_index].result,CODE_LIST.MID_CODE[code_num-1].result)==0){
                                if(strcmp(CODE_LIST.MID_CODE[code_index].src1,CODE_LIST.MID_CODE[code_num-1].result)==0){
                                    use_flag = 1;
                                    continue;
                                }
                                if(strcmp(CODE_LIST.MID_CODE[code_index].src2,CODE_LIST.MID_CODE[code_num-1].result)==0){
                                    use_flag = 1;
                                    continue;
                                }
                                define_flag = 1;
                            }
                            if(strcmp(CODE_LIST.MID_CODE[code_index].src1,CODE_LIST.MID_CODE[code_num-1].result)==0 && define_flag == 0)
                                use_flag = 1;
                            if(strcmp(CODE_LIST.MID_CODE[code_index].src2,CODE_LIST.MID_CODE[code_num-1].result)==0 && define_flag == 0)
                                use_flag = 1;
                        }
                        if(use_flag==0){
                            CODE_LIST.MID_CODE[code_num-1].is_effective = 0;
                            strcpy(CODE_LIST.MID_CODE[code_num].src1,CODE_LIST.MID_CODE[code_num-1].src1);
                            strcpy(CODE_LIST.MID_CODE[code_num].src2,CODE_LIST.MID_CODE[code_num-1].src2);
                            CODE_LIST.MID_CODE[code_num].op = CODE_LIST.MID_CODE[code_num-1].op;
                        }
                    }
                }
            }
        }
        else{
            for(code_num=FUNC_BLOCKS.FUNC_LIST[index].start_num+1;code_num<CODE_LIST.code_num;code_num++){
                if(CODE_LIST.MID_CODE[code_num].is_effective == 0)
                    continue;
                if(CODE_LIST.MID_CODE[code_num].op == 11){
                    if(CODE_LIST.MID_CODE[code_num-1].is_effective == 1 && CODE_LIST.MID_CODE[code_num].src1[0] == '$' && strcmp(CODE_LIST.MID_CODE[code_num].src1,CODE_LIST.MID_CODE[code_num-1].result)==0){
                        use_flag = 0;
                        define_flag = 0;
                        for(code_index=code_num+1;code_index<CODE_LIST.code_num;code_index++){
                            if(CODE_LIST.MID_CODE[code_index].is_effective == 0)
                                continue;
                            if(strcmp(CODE_LIST.MID_CODE[code_index].result,CODE_LIST.MID_CODE[code_num-1].result)==0){
                                if(strcmp(CODE_LIST.MID_CODE[code_index].src1,CODE_LIST.MID_CODE[code_num-1].result)==0){
                                    use_flag = 1;
                                    continue;
                                }
                                if(strcmp(CODE_LIST.MID_CODE[code_index].src2,CODE_LIST.MID_CODE[code_num-1].result)==0){
                                    use_flag = 1;
                                    continue;
                                }
                                define_flag = 1;
                            }
                            if(strcmp(CODE_LIST.MID_CODE[code_index].src1,CODE_LIST.MID_CODE[code_num-1].result)==0 && define_flag == 0)
                                use_flag = 1;
                            if(strcmp(CODE_LIST.MID_CODE[code_index].src2,CODE_LIST.MID_CODE[code_num-1].result)==0 && define_flag == 0)
                                use_flag = 1;
                        }
                        if(use_flag==0){
                            CODE_LIST.MID_CODE[code_num-1].is_effective = 0;
                            strcpy(CODE_LIST.MID_CODE[code_num].src1,CODE_LIST.MID_CODE[code_num-1].src1);
                            strcpy(CODE_LIST.MID_CODE[code_num].src2,CODE_LIST.MID_CODE[code_num-1].src2);
                            CODE_LIST.MID_CODE[code_num].op = CODE_LIST.MID_CODE[code_num-1].op;
                        }
                    }
                }
            }
        }
    }
}


void whole_process(){
    add_basic_block();
    delete_same_block();
    get_dag_and_opt();
    delete_unuseful_code();
    algebraic_trans();
    change_invalid_code();
    merge_temp_var();
}


