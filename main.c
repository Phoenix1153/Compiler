#include <stdio.h>
#include <stdlib.h>
#include "grammar.c"
#include "final_result.c"
#include "final_result2.c"
//全局变量声明
FILE* compile_file;                                        //待编译的文件
FILE* output_file;                                         //词法分析结果输出的文件
int main()
{
    //int i;
    char data_path[100];
    scanf("%s",data_path);
    if((compile_file = fopen(data_path, "r")) == NULL){
		printf("Can not find the file.\n") ;
        return 0 ;
	}
	output_file = fopen("result.txt", "w");
	initial_table();
	init_code_list();
	init_func_list();
	CHAR = get_ch();
    symbol = insymbol();
    program();
    fprintf(output_file,"finish compiling\n");
    print_mid_code();
    generate_all_final_code();
    whole_process();
    print_mid_code2();
    generate_all_final_code2();
    fclose(compile_file);
	fclose(output_file);
    return 0;
}
