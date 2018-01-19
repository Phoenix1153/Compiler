#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "error.c"
#include "mid_code.c"
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
#define NEQSY 33                  //！=
#define DIGITSY 34                //无符号整数
#define CHAR2SY 35                //字符
#define STRINGSY 36               //字符串
#define SEMISY 37                 //;
///全局常量说明
const char CON[] = "const";
const char INT[] = "int";
const char _CHAR[] = "char";
const char VOID[] = "void";
const char MAIN[] = "main";
const char DO[] = "do";
const char WHILE[] = "while";
const char SWITCH[] = "switch";
const char CASE[] = "case";
const char IF[] = "if";
const char SCANF[] = "scanf";
const char PRINTF[] = "printf";
const char RETURN[] = "return";
const int imax = 2147483647;
//全局变量声明
char CHAR;
char TOKEN[100];
char read_line[512];
long trans_num = 0;
char out_symbol[50];
int line = 0;
int index_in_line = 0;
int line_lenth = 0;
extern FILE* compile_file;                                        //待编译的文件
extern FILE* output_file;                                         //词法分析结果输出的文件
//词法分析各函数
int get_ch(){
    if(index_in_line == line_lenth){
        line++;
        if(fgets(read_line,512,compile_file) != NULL){
            line_lenth = strlen(read_line);
            index_in_line = 0;
            return('\n');                                         //相当于都到了行末尾的回车
        }
        else
            return(EOF);
    }
    else{
        index_in_line += 1;
        return(read_line[index_in_line-1]);
    }
}

void translate(int sym_num){
	switch(sym_num){
		case CONSY:{
			strcpy(out_symbol,"CONSY");
			break;
		}
		case INTSY:{
			strcpy(out_symbol,"INTSY");
			break;
		}
		case CHARSY:{
			strcpy(out_symbol,"CHARSY");
			break;
		}
		case VOIDSY:{
			strcpy(out_symbol,"VOIDSY");
			break;
		}
		case MAINSY:{
			strcpy(out_symbol,"MAINSY");
			break;
		}
		case DOSY:{
			strcpy(out_symbol,"DOSY");
			break;
		}
		case WHISY:{
			strcpy(out_symbol,"WHISY");
			break;
		}
		case SWISY:{
			strcpy(out_symbol,"SWISY");
			break;
		}
		case CASESY:{
			strcpy(out_symbol,"CASESY");
			break;
		}
		case IFSY:{
			strcpy(out_symbol,"IFSY");
			break;
		}
		case SCANSY:{
			strcpy(out_symbol,"SCANSY");
			break;
		}
		case PRISY:{
			strcpy(out_symbol,"PRISY");
			break;
		}
		case RTSY:{
			strcpy(out_symbol,"RTSY");
			break;
		}
		case IDSY:{
			strcpy(out_symbol,"IDSY");
			break;
		}
		case PLUSSY:{
			strcpy(out_symbol,"PLUSSY");
			break;
		}
		case MINUSSY:{
			strcpy(out_symbol,"MINUSSY");
			break;
		}
		case STARSY:{
			strcpy(out_symbol,"STARSY");
			break;
		}
		case DIVSY:{
			strcpy(out_symbol,"DIVSY");
			break;
		}
		case COMMASY:{
			strcpy(out_symbol,"COMMASY");
			break;
		}
		case COLONSY:{
			strcpy(out_symbol,"COLONSY");
			break;
		}
		case LPARSY:{
			strcpy(out_symbol,"LPARSY");
			break;
		}
		case RPARSY:{
			strcpy(out_symbol,"RPARSY");
			break;
		}
		case LBRASY:{
			strcpy(out_symbol,"LBRASY");
			break;
		}
		case RBRASY:{
			strcpy(out_symbol,"RBRASY");
			break;
		}
		case LBRASSY:{
			strcpy(out_symbol,"LBRASSY");
			break;
		}
		case RBRASSY:{
			strcpy(out_symbol,"RBRASSY");
			break;
		}
		case ASSIGNSY:{
			strcpy(out_symbol,"ASSIGNSY");
			break;
		}
		case EQUSY:{
			strcpy(out_symbol,"EQUSY");
			break;
		}
		case GTRSY:{
			strcpy(out_symbol,"GTRSY");
			break;
		}
		case GTESY:{
			strcpy(out_symbol,"GTESY");
			break;
		}
		case LTSY:{
			strcpy(out_symbol,"LTSY");
			break;
		}
		case LESY:{
			strcpy(out_symbol,"LESY");
			break;
		}
		case NEQSY:{
			strcpy(out_symbol,"NEQSY");
			break;
		}
		case DIGITSY:{
			strcpy(out_symbol,"DIGITSY");
			break;
		}
		case CHAR2SY:{
			strcpy(out_symbol,"CHAR2SY");
			break;
		}
		case STRINGSY:{
			strcpy(out_symbol,"STRINGSY");
			break;
		}
		case SEMISY:{
			strcpy(out_symbol,"SEMISY");
			break;
		}

	}
}



void reset_token(){               //为下一次读入单词做准备
	int i;
	for(i=0;i<100;i++)
		TOKEN[i] = '\0';
	return;
}

int trans_to_num(){               //将数字字符串转化为数字
	int len,i;
	long long result;
	len = strlen(TOKEN);
	result = 0;
	for(i=0;i<len;i++){
		result = result*10 + TOKEN[i] - '0';
		if(result > imax){
			error(OUT_OF_BOUND_ERROR,line);
			return 0;
		}
	}
	return result;
}

void cat(char c){
	int i=0;
	while(TOKEN[i] != '\0'){
		i++;
		if(i == 99){
			printf("too long words\n");
			return;
		}
	}
	TOKEN[i] = c;
}

int is_letter(){
	return (isalpha(CHAR) || (CHAR == '_'));
}

int is_digit(){
	return isdigit(CHAR);
}

int is_colon(){
	if(CHAR == ':')
		return 1;
	return 0;
}

int is_semi(){
	if(CHAR == ';')
		return 1;
	return 0;
}

int is_comma(){
	if(CHAR == ',')
		return 1;
	return 0;
}

int is_plus(){
	if(CHAR == '+')
		return 1;
	return 0;
}

int is_minus(){
	if(CHAR == '-')
		return 1;
	return 0;
}

int is_star(){
	if(CHAR == '*')
		return 1;
	return 0;
}

int is_div(){
	if(CHAR == '/')
		return 1;
	return 0;
}

int is_squo(){
	if(CHAR == '\'')
		return 1;
	return 0;
}

int is_dquo(){
	if(CHAR == '\"')
		return 1;
	return 0;
}

int is_lparent(){
	if(CHAR == '\(')
		return 1;
	return 0;
}

int is_rparent(){
	if(CHAR == ')')
		return 1;
	return 0;
}

int is_lbracket(){
	if(CHAR == '[')
		return 1;
	return 0;
}

int is_rbracket(){
	if(CHAR == ']')
		return 1;
	return 0;
}

int is_lbrace(){
	if(CHAR == '{')
		return 1;
	return 0;
}

int is_rbrace(){
	if(CHAR == '}')
		return 1;
	return 0;
}

int is_gtr(){
	if(CHAR == '>')
		return 1;
	return 0;
}

int is_lt(){
	if(CHAR == '<')
		return 1;
	return 0;
}

int is_equ(){
	if(CHAR == '=')
		return 1;
	return 0;
}

int is_exc(){
	if(CHAR == '!')
		return 1;
	return 0;
}

int is_zero(){
	if(CHAR == '0')
		return 1;
	return 0;
}

int is_valid_character(){
	if(CHAR == 32)
		return 1;
	else if(CHAR == 33)
		return 1;
	else if((CHAR >= 35) && (CHAR <= 126))
		return 1;
	else
		return 0;
}

int is_key(){
	if(strcmp(TOKEN,CON) == 0)
		return CONSY;
	else if(strcmp(TOKEN,INT) == 0)
		return INTSY;
	else if(strcmp(TOKEN,_CHAR) == 0)
		return CHARSY;
	else if(strcmp(TOKEN,VOID) == 0)
		return VOIDSY;
	else if(strcmp(TOKEN,MAIN) == 0)
		return MAINSY;
	else if(strcmp(TOKEN,DO) == 0)
		return DOSY;
	else if(strcmp(TOKEN,WHILE) == 0)
		return WHISY;
	else if(strcmp(TOKEN,SWITCH) == 0)
		return SWISY;
	else if(strcmp(TOKEN,CASE) == 0)
		return CASESY;
	else if(strcmp(TOKEN,IF) == 0)
		return IFSY;
	else if(strcmp(TOKEN,SCANF) == 0)
		return SCANSY;
	else if(strcmp(TOKEN,PRINTF) == 0)
		return PRISY;
	else if(strcmp(TOKEN,RETURN) == 0)
		return RTSY;
	else
		return 0;
}

int insymbol(){
	int symbol;
	int zero_flag = 0;
	reset_token();
	while((CHAR == ' ') || (CHAR == '\t') || (CHAR == '\n')){
		CHAR = get_ch();
		if(CHAR == EOF)           //此时的情况为读过所有的无用符号之后，文档结束
        {
            printf("end of the file\n");
            fprintf(output_file,"end of the file\n");
            //fclose(compile_file);
            //fclose(output_file);
            //print_mid_code();
            exit(0);
            return -2;
        }
	}
	if(CHAR == EOF){
        printf("end of the file\n");
        fprintf(output_file,"end of the file\n");
        //fclose(compile_file);
        //fclose(output_file);
        //print_mid_code();
        exit(0);
        return -2;
	}
	if(is_letter()){
		do{
			if(isupper(CHAR))
				cat(tolower(CHAR));
			else
				cat(CHAR);
			CHAR = get_ch();
		}while(is_letter() || is_digit());
		symbol = is_key();
		if(symbol == 0)
			symbol = IDSY;
	}
	else if(is_digit()){
		if(is_zero()){
			zero_flag = 1;
		}
		do{
			cat(CHAR);
			CHAR = get_ch();
		}while(is_digit());
		symbol = DIGITSY;
		trans_num = trans_to_num();
		if((zero_flag == 1) && (trans_num != 0)){
			error(ZERO_START_ERROR,line);
		}
	}
	else if(is_plus()){
		cat(CHAR);
		CHAR = get_ch();
		symbol = PLUSSY;
	}
	else if(is_minus()){
		cat(CHAR);
		CHAR = get_ch();
		symbol = MINUSSY;
	}
	else if(is_star()){
		cat(CHAR);
		CHAR = get_ch();
		symbol = STARSY;
	}
	else if(is_div()){
		cat(CHAR);
		CHAR = get_ch();
		symbol = DIVSY;
	}
	else if(is_gtr()){
		cat(CHAR);
		CHAR = get_ch();
		if(is_equ()){
			cat(CHAR);
			symbol = GTESY;
			CHAR = get_ch();
		}
		else
			symbol = GTRSY;
	}
	else if(is_lt()){
		cat(CHAR);
		CHAR = get_ch();
		if(is_equ()){
			cat(CHAR);
			symbol = LESY;
			CHAR = get_ch();
		}
		else
			symbol = LTSY;
	}
	else if(is_equ()){
		cat(CHAR);
		CHAR = get_ch();
		if(is_equ()){
			cat(CHAR);
			symbol = EQUSY;
			CHAR = get_ch();
		}
		else
			symbol = ASSIGNSY;
	}
	else if(is_exc()){
		cat(CHAR);
		CHAR = get_ch();
		if(is_equ()){
			cat(CHAR);
			symbol = NEQSY;
			CHAR = get_ch();
		}
		else{
			error(INVALID_CHARACTER_ERROR,line);
			symbol = -1;                           //这里怎么处理？
		}
	}
	else if(is_lparent()){
		cat(CHAR);
		CHAR = get_ch();
		symbol = LPARSY;
	}
	else if(is_rparent()){
		cat(CHAR);
		CHAR = get_ch();
		symbol = RPARSY;
	}
	else if(is_lbracket()){
		cat(CHAR);
		CHAR = get_ch();
		symbol = LBRASY;
	}
	else if(is_rbracket()){
		cat(CHAR);
		CHAR = get_ch();
		symbol = RBRASY;
	}
	else if(is_lbrace()){
		cat(CHAR);
		CHAR = get_ch();
		symbol =LBRASSY;
	}
	else if(is_rbrace()){
		cat(CHAR);
		CHAR = get_ch();
		symbol =RBRASSY;
	}
	else if(is_comma()){
		cat(CHAR);
		CHAR = get_ch();
		symbol =COMMASY;
	}
	else if(is_colon()){
		cat(CHAR);
		CHAR = get_ch();
		symbol =COLONSY;
	}
	else if(is_semi()){
		cat(CHAR);
		CHAR = get_ch();
		symbol =SEMISY;
	}
	else if(is_squo()){
		cat(CHAR);
		CHAR = get_ch();
		if(is_plus() || is_minus() || is_star() || is_div() || is_letter() || is_digit()){
			cat(CHAR);
		}
		else{
			error(INVALID_CHAR_ERROR,line);
		}
		CHAR = get_ch();
		if(is_squo()){
			cat(CHAR);
			CHAR = get_ch();
			symbol = CHAR2SY;
		}
		else{
			error(NO_MATCH_SQ_ERROR,line);
			CHAR = get_ch();
			symbol = -1;
		}
	}
	else if(is_dquo()){
		cat(CHAR);
		CHAR = get_ch();
		while(is_valid_character()){
			if(CHAR == '\\')
				cat(CHAR);
			cat(CHAR);
			CHAR = get_ch();
		}
		if(is_dquo()){
			cat(CHAR);
			CHAR = get_ch();
			symbol = STRINGSY;
		}
		else{
			error(NO_MATCH_DQ_ERROR,line);
			symbol = STRINGSY;
		}
	}
	else{
		symbol = -1;
		error(INVALID_CHARACTER_ERROR,line);
	}
	return symbol;
}
