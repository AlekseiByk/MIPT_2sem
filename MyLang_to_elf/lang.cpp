#include "headers/Header.h"
#include "headers/Text.h"

unit_t arr[MAXCODELEN] = {};
int VarNumberArray[50] = {};
Namenumnum_t variables[VARNUMBER] = {};
int number_of_var = 0;
const char * s = "";
const char * starts = "";

addres_label labels[100];
int number_of_labels = 0;
int FunctionNumber = 0;
int print_pointer = 0;                         //pointer to print function
int scan_pointer  = 0;                         //pointer to scan function
int size = 0;                                  //size of executable code
int entery_point = 0;                          //entery point of executable code
int entery_point_0 = 0;
int Number_Of_Labels_hex = 0;

int main() 
{
    int length = 0;
    int inputPhrase = 0;
    tree_node_t* head = nullptr;
    
    while (inputPhrase < 7)
    {
        printf("\n\
			\n0.Из LangIn.txt в дерево\
			\n1.Из StandartIn.txt(файл стандарта) в дерево\
			\n2.в язык\
			\n3.в асемблер\
			\n4.графический дамп\
			\n5.optimize\
			\n6.to Elf\
			\n7+.exit\n\n");

        scanf("%d", &inputPhrase);
        switch(inputPhrase)
        {
            case 0:
            {
                char* input = TextFromFile("files/LangIn.txt", &length);
                DeleteComments(input);
                printf("----/----\n%s\n----/----\n", input);

                input = ReCode(input, 1);
                printf("----/----\n%s\n----/----\n", input);

                head = GetTree(input);
                free(input);

                MakeVarList(head);

                for (int i = 0; i < number_of_var; i++)
                    if (variables[ i ].num != -1)
                        printf("%d. %s -> %d -> %d\n", i, variables[ i ].name, variables[ i ].num, variables[ i ].numnum);


                preWriteTree("files/Standartout.txt", head);
                break;
            }
            case 1:
            {
                char *input = TextFromFile("files/StandartIn.txt", &length);
                head = readTreeFrom(input);
                free(input);
                
                MakeVarList(head);

                for (int i = 0; i < number_of_var; i++)
                    if (variables[ i ].num != -1)
                        printf("%d. %s -> %d -> %d\n", i, variables[ i ].name, variables[ i ].num, variables[ i ].numnum);

                     for (int i = 0; i < 5; i++)
                	printf("%d:%d\n", i, VarNumberArray[i]);

                preWriteTree("files/Standartout.txt", head);
                break;
            }
            case 2:
            {
                WriteTreeToLang("files/Langout.txt", head);
                break;
            }
            case 3:
            {
                FromTreeToAssembler("temp/input.asmc", head);
                system("make toasm");
                break;
            }
            case 4:
            {
                GraphTreeDump(head);
                break;
            }
            case 5:
            {
                head = treeOptimize(head);
                break;
            }
            case 6:
            {
            	to_bin(head);
            	break;
            }
            /*case 7:
            {
            	//ToElf("ASM_x86_64/output.elf", head);
            	break;
            }*/
        }
    }

    treeDestruct(head);
    return 0;
}
           