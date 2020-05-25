#include "headers/Header.h"

int main() 
{
    int length = 0;
    int inputPhrase = 0;
    tree_node_t* head = nullptr;
    
    while (inputPhrase < 8)
    {
        printf("\n\
			\n0.Из LangIn.txt в дерево\
			\n1.Из StandartIn.txt(файл стандарта) в дерево\
			\n2.в язык\
			\n3.в асемблер\
			\n4.графический дамп\
			\n5.optimize\
			\n6.to Asm x86_64\
			\n7.to Elf\
			\n8+.exit\n\n");

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
            case 7:
            {
            	//ToElf("ASM_x86_64/output.elf", head);
            	break;
            }
        }
    }

    treeDestruct(head);
    return 0;
}

#include "headers/Language.cpp"
#include "headers/Print.cpp"
#include "headers/ASM.cpp"
