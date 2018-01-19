#include<stdio.h>
#define FILE_OPEN_FAIL 0                                  //δ�ܴ򿪴������ļ�
//�����Ǵʷ������еı���
#define ZERO_START_ERROR 1                                //�����������0��ͷ
#define INVALID_CHAR_ERROR 2                              //�ַ��е�char�ǷǷ���
#define NO_MATCH_SQ_ERROR 3                               //�����Ų�ƥ��
#define NO_MATCH_DQ_ERROR 4                               //˫���Ų�ƥ��
#define INVALID_CHARACTER_ERROR 5                         //�ļ��г����﷨�����ܵĳɷ�
#define OUT_OF_BOUND_ERROR 6                              //���ͳ�����ֵ����int
//�������﷨����������еı���
#define OUT_OF_TABLE_ERROR 7                              //���ű��������޷�����µı�ʶ��
#define PARA_NUM_ERROR 8                                  //���ú���ʱ��ֵ�κ�ԭ����������βε���Ŀ��ͬ
#define FUNC_REDEF_ERROR 9                                //���庯��ʱ�����ַ��ű��Ѿ���ͬ���ĺ���
#define VAR_REDEF_ERROR 10                                //��ͬ�����Ѿ���ͬ����������
#define ID_ERROR 11                                       //�����ͱ�ʶ�����治�ǵ�sym���Ǳ�ʶ��
#define CONST_DEF_ASSIGN_ERROR 12                         //�ڳ��������У�Ӧ���ǵȺŵ�λ�ò��ǵȺ�
#define CONST_DEF_TYPE_ERROR 13                           //���������У��Ⱥź����sym�����ͺͳ�����������Ͳ�һ�»���sym�������������ַ�
#define CONST_DEC_TYPE_ERROR 14                           //���������У����������ͱ�ʶ������int��char
#define SEMI_ERROR 15                                     //Ӧ���Ƿֺŵĵط����ɱ��sym����
#define VAR_DEF_ARRAYINDEX_ERROR 16                       //���������У���������������޷�������
#define RPAR_ERROR 17                                     //Ӧ��ʱ��С���ŵĵط�sym���Ͳ���ȷ
#define RBRA_ERROR 18                                     //Ӧ��ʱ�������ŵĵط�sym���Ͳ���ȷ
#define RBRAS_ERROR 19                                    //Ӧ��ʱ�Ҵ����ŵĵط�sym���Ͳ���ȷ
#define VAR_KIND_ERROR 20                                 //������еı�ʶ�����Ǳ���
#define MAINSY_ERROR 21                                   //Ӧ����main�����ֵĵط�sym���ʹ���
#define LPAR_ERROR 22                                     //Ӧ��ʱ��С���ŵĵط�sym���Ͳ���ȷ
#define LBRAS_ERROR 23                                    //Ӧ��ʱ������ŵĵط�sym���Ͳ���ȷ
#define AFTER_OP_DIGIT_ERROR 24                           //�������У�+-�ź���Ĳ����޷�������
#define FACTOR_ERROR 25                                   //��factor��������Ϊ������Ĳ��ֲ���factor,��ʶ���ʶ��ĵ�һ��sym������factor����һ����֧
#define STATE_AFTER_ID_ERROR 26                           //�����ɷַ����У������˱�ʶ����Ϊ��ʼ��������治��=,(,[�е�һ�֣�����Ҫ����
#define WRONG_RELA_OP_ERROR 27                            //����Ĺ�ϵ�����
#define WHILESY_ERROR 28                                  //ѭ������У�������do��state��Ӧ����while�����֣�������Ϊ����
#define READ_ARRAY_ERROR 29                               //������У�ֻ�ܽ������ֵ�浽��ͨ������
#define VOID_RT_ERROR 30                                  //��������У�void������return������ֵ
#define VAL_RT_ERROR 31                                   //��������У���void������return����û��ֵ
#define NOT_VOID_MAIN_ERROR 32                            //main��������ʱǰ�治��void
#define ID_DEC_ERROR 33                                   //��ʶ������ʱ������Ӧ���Ƕ��š���С���š��������Ż��߷ֺţ���������ı�ʶ������Ϊ�����ʽ(��)
#define NO_RT_FUNDEF_ERROR 34                             //�з���ֵ��������ʱû�з������
#define CASE_ERROR 35                                     //Ӧ����case�ĵط���������symbol
#define CASE_CONST_ERROR 36                               //case����Ӧ���ǳ����������򱨴�
#define COLON_ERROR 37                                    //Ӧ����ð�ŵĵط�������symbol
#define ASSIGN_ERROR 38                                   //Ӧ����=�ĵط�������symbol
#define FUNC_NO_DEF_ERROR 39                              //���õĺ���δ������߲��������Ͳ���
#define ID_NO_DEF_ERROR 40                                //��ʶ��δ����������Ͳ�ƥ��
//�м���������еı���
#define OUT_OF_CODE_ERROR 41                              //�м����ṹ�������������ٴ�Ŵ���
//�﷨�����������ӵ�����
#define CONST_ASSIGNMENT_ERROR 42                         //��������ֵ
#define ARRAY_ASSIGNMENT_ERROR 43                         //�������ֱ�ֱ�Ӹ�ֵ
#define CHAR_ASSIGNMENT_ERROR 44                          //char���ܱ�int��ֵ
//�Ż��еı���������ʾ�Լ�
#define OUT_OF_FUNC_BLOCK_ERROR 45                        //������̫��
#define OUT_OF_BASIC_BLOCK_ERROR 46                       //������̫��
#define OUT_OF_NODE_NUM 47                                //�ڵ����ߵ��������dag�Ľڵ���Ŀ̫��
#define OUT_OF_ONE_NODE_VAR 48                            //һ���ڵ��ϵı���������


extern FILE* output_file;                                         //�ʷ��������������ļ�
void error(int error_type,int line_index){
	switch(error_type){
		case 1:{
			fprintf(output_file,"line %d : non-zero integer shouldn't start with 0'\n",line_index);
			printf("line %d : non-zero integer shouldn't start with 0'\n",line_index);
			break;
		}
		case 2:{
			fprintf(output_file,"line %d : invalid char being declared\n",line_index);
			printf("line %d : invalid char being declared\n",line_index);
			break;
		}
		case 3:{
			fprintf(output_file,"line %d : single quatation don't match\n",line_index);
			printf("line %d : single quatation don't match\n",line_index);
			break;
		}
		case 4:{
			fprintf(output_file,"line %d : double quatation don't match\n",line_index);
			printf("line %d : double quatation don't match\n",line_index);
			break;
		}
		case 5:{
			fprintf(output_file,"line %d : invalid char appears\n",line_index);
			printf("line %d : invalid char appears\n",line_index);
			break;
		}
		case 6:{
			fprintf(output_file,"line %d : integer out of the range\n",line_index);
			printf("line %d : integer out of the range\n",line_index);
			break;
		}
		case 7:{
            fprintf(output_file,"line %d : table overflow!\n",line_index);
            printf("line %d : table overflow!\n",line_index);
			break;
		}
		case 8:{
            fprintf(output_file,"line %d: the parameter number doesn't match!\n",line_index);
            printf("line %d: the parameter number doesn't match!\n",line_index);
			break;
		}
		case 9:{
            fprintf(output_file,"line %d: this function name already exists!\n",line_index);
            printf("line %d: this function name already exists!\n",line_index);
			break;
		}
		case 10:{
            fprintf(output_file,"line %d: this variable name already exists!\n",line_index);
            printf("line %d: this variable name already exists!\n",line_index);
			break;
		}
		case 11:{
            fprintf(output_file,"line %d: this place should be an Identifier after type identifier!\n",line_index);
            printf("line %d: this place should be an Identifier after type identifier!\n",line_index);
			break;
		}
		case 12:{
            fprintf(output_file,"line %d: this place should be an equal in constant defination!\n",line_index);
            printf("line %d: this place should be an equal in constant defination!\n",line_index);
			break;
		}
        case 13:{
            fprintf(output_file,"line %d: the type doesn't match in cosntant defination!\n",line_index);
            printf("line %d: the type doesn't match in cosntant defination!\n",line_index);
			break;
        }
        case 14:{
            fprintf(output_file,"line %d: the type identifier is wrong in cosntant defination!\n",line_index);
            printf("line %d: the type identifier is wrong in cosntant defination!\n",line_index);
			break;
        }
        case 15:{
            fprintf(output_file,"line %d: this place should be a semicolon!\n",line_index);
            printf("line %d: this place should be a semicolon!\n",line_index);
			break;
        }
        case 16:{
            fprintf(output_file,"line %d: the index of array is not unsigned integer in variable defination!\n",line_index);
            printf("line %d: the index of array is not unsigned integer in variable defination!\n",line_index);
			break;
        }
        case 17:{
            fprintf(output_file,"line %d: this place should be a right parenthese!\n",line_index);
            printf("line %d: this place should be a right parenthese!\n",line_index);
			break;
        }
        case 18:{
            fprintf(output_file,"line %d: this place should be a right bracket!\n",line_index);
            printf("line %d: this place should be a right bracket!\n",line_index);
			break;
        }
        case 19:{
            fprintf(output_file,"line %d: this place should be a right brace!\n",line_index);
            printf("line %d: this place should be a right brace!\n",line_index);
			break;
        }
        case 20:{
            fprintf(output_file,"line %d: the identifier is not a variable in read statement!\n",line_index);
            printf("line %d: the identifier is not a variable in read statement!\n",line_index);
			break;
        }
        case 21:{
            fprintf(output_file,"line %d: this place should be the key word main!\n",line_index);
            printf("line %d: this place should be the key word main!\n",line_index);
			break;
        }
        case 22:{
            fprintf(output_file,"line %d: this place should be a left parenthese!\n",line_index);
            printf("line %d: this place should be a left parenthese!\n",line_index);
			break;
        }
        case 23:{
            fprintf(output_file,"line %d: this place should be a left brace!\n",line_index);
            printf("line %d: this place should be a left brace!\n",line_index);
			break;
        }
        case 24:{
            fprintf(output_file,"line %d: there should be an unsigned integer after + or -!\n",line_index);
            printf("line %d: there should be an unsigned integer after + or -!\n",line_index);
			break;
        }
        case 25:{
            fprintf(output_file,"line %d: there should be a factor here!\n",line_index);
            printf("line %d: there should be a factor here!\n",line_index);
			break;
        }
        case 26:{
            fprintf(output_file,"line %d: there should be a left parenthese or a left bracket or a left brace after identifier!\n",line_index);
            printf("line %d: there should be a left parenthese or a left bracket or a left brace after identifier!\n",line_index);
			break;
        }
        case 27:{
            fprintf(output_file,"line %d: wrong relational operater!\n",line_index);
            printf("line %d: wrong relational operater!\n",line_index);
			break;
        }
        case 28:{
            fprintf(output_file,"line %d: this place should be the key word while!\n",line_index);
            printf("line %d: this place should be the key word while!\n",line_index);
			break;
        }
        case 29:{
            fprintf(output_file,"line %d: array or const is not allowed to appear in read statement!\n",line_index);
            printf("line %d: array or const is not allowed to appear in read statement!\n",line_index);
			break;
        }
        case 30:{
            fprintf(output_file,"line %d: in void function, should not return value!\n",line_index);
            printf("line %d: in void function, should not return value!\n",line_index);
			break;
        }
        case 31:{
            fprintf(output_file,"line %d: in int or char function, should return value!\n",line_index);
			printf("line %d: in int or char function, should return value!\n",line_index);
			break;
        }
        case 32:{
            fprintf(output_file,"line %d: there should be void before the key word main!\n",line_index);
            printf("line %d: there should be void before the key word main!\n",line_index);
			break;
        }
        case 33:{
            fprintf(output_file,"line %d: mistake after identifier in identifier declararion!\n",line_index);
            printf("line %d: mistake after identifier in identifier declararion!\n",line_index);
			break;
        }
        case 34:{
            fprintf(output_file,"line %d: in int or char function, should have return statement!\n",line_index);
            printf("line %d: in int or char function, should have return statement!\n",line_index);
			break;
        }
        case 35:{
            fprintf(output_file,"line %d: this place should be the key word case!\n",line_index);
            printf("line %d: this place should be the key word case!\n",line_index);
            break;
        }
        case 36:{
            fprintf(output_file,"line %d: this place should be a constant after case!\n",line_index);
            printf("line %d: this place should be a constant after case!\n",line_index);
            break;
        }
        case 37:{
            fprintf(output_file,"line %d: this place should be a colon!\n",line_index);
            printf("line %d: this place should be a colon!\n",line_index);
			break;
        }
        case 38:{
            fprintf(output_file,"line %d: this place should be an equal!\n",line_index);
            printf("line %d: this place should be an equal!\n",line_index);
			break;
        }
        case 39:{
            fprintf(output_file,"line %d: undefined function call or the num of value para is not equal to defined para or the type is wrong!\n",line_index);
            printf("line %d: undefined function call or the num of value para is not equal to defined para or the type is wrong!\n",line_index);
			break;
        }
        case 40:{
            fprintf(output_file,"line %d: undefined identifier or wrong type!\n",line_index);
            printf("line %d: undefined identifier or wrong type!\n",line_index);
            break;
        }
        case 41:{
            fprintf(output_file,"line %d: mid_code out of index!\n",line_index);
            printf("line %d: mid_code out of index!\n",line_index);
            break;
        }
        case 42:{
            fprintf(output_file,"line %d: constant can't be assigned!\n",line_index);
            printf("line %d: constant can't be assigned!\n",line_index);
            break;
        }
        case 43:{
            fprintf(output_file,"line %d: array's name can't be assigned!\n",line_index);
            printf("line %d: array's name can't be assigned!\n",line_index);
            break;
        }
        case 44:{
            fprintf(output_file,"line %d: char shouldn't be assigned by int!(warning)\n",line_index);
            printf("line %d: char shouldn't be assigned by int!(warning)\n",line_index);
            break;
        }
        case 45:{
            fprintf(output_file,"line %d: function block out of range(optimizer)\n",line_index);
            printf("line %d: function block out of range(optimizer)\n",line_index);
            break;
        }
        case 46:{
            fprintf(output_file,"line %d: basic block out of range(optimizer)\n",line_index);
            printf("line %d: basic block out of range(optimizer)\n",line_index);
            break;
        }
        case 47:{
            fprintf(output_file,"line %d: num of node out of range(optimizer)\n",line_index);
            printf("line %d: num of node out of range(optimizer)\n",line_index);
            break;
        }
        case 48:{
            fprintf(output_file,"line %d: num of var one node out of range(optimizer)\n",line_index);
            printf("line %d: num of var one node out of range(optimizer)\n",line_index);
            break;
        }
	}
	//fclose(output_file);
	return;
}
