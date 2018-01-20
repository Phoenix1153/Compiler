# Compiler
a mini-compiler for C0 grammar
这是北航编译课设计的C0文法对应的编译器，可以生成源程序对应的mips汇编代码。

本程序共有8部分组成：
1.主程序                                                               main.c
2.词法分析程序                                                      wordtest.c
3.语法&语义分析程序                                                 grammar.c
4.中间代码生成程序                                                 mid_code.c                        
5.中间代码到mips汇编语言的转化程序                               final_result.c
6.错误处理程序                                                         error.c
7.代码优化程序                                                      optimizer.c
8.寄存器分配后的mips汇编转化程序                                 final_result2.c

生成的txt文档包括：
1.语法成分递归检查的成分输出    result.txt
2.优化前的中间代码输出         mid_code.txt
3.优化后的中间代码输出         mid_code2.txt
4.优化前的mips代码输出         final.asm
5.优化后的mips代码输出         final2.asm

