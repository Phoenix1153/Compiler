# Compiler
a mini-compiler for C0 grammar
这是北航编译课设计的C0文法对应的编译器，可以生成源程序对应的mips汇编代码。

本程序共有8部分组成：
主程序                                                               main.c
词法分析程序                                                      wordtest.c
语法&语义分析程序                                                 grammar.c
中间代码生成程序                                                 mid_code.c                        
中间代码到mips汇编语言的转化程序                               final_result.c
错误处理程序                                                         error.c
代码优化程序                                                      optimizer.c
寄存器分配后的mips汇编转化程序                                 final_result2.c

