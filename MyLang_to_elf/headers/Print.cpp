#include "Header.h"

void GraphPrintTree(FILE *dump, tree_node_t* headNode)
{
    assert(dump);

    if (headNode -> data.type == TYPENUMBER)
        fprintf(dump, "\"%p\"[shape = egg, color = \"darkseagreen\", label = \"%lg\"];\n", headNode, headNode -> data.value);

    else if (headNode -> data.type == TYPEVARIABLE)
        fprintf(dump, "\"%p\"[shape = egg, color = \"darkolivegreen1\", label = \"%s\"];\n", headNode, variables[(int)headNode -> data.value].name);

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 6))
        fprintf(dump, "\"%p\"[shape = diamond, color = \"gold\", label = \"%s\"];\n", headNode, takefunc((int) headNode -> data.value));

    else if (headNode -> data.type == TYPEFUNCTION)
        fprintf(dump, "\"%p\"[shape = box, color = \"grey\", label = \"%s%s\"];\n", headNode, (headNode -> data.value > 24)? "$" : "", takefunc((int) headNode -> data.value));

    else if (headNode -> data.type == TYPEOPERATOR)
        fprintf(dump, "\"%p\"[shape = %s, color = \"%s\", label = \"%s\"];\n", headNode, operators[(int) headNode -> data.value].form, operators[(int) headNode -> data.value].color, operators[(int) headNode -> data.value].name);

    else
        fprintf(dump, "\"%p\"[shape = parallelogram, color = \"grey56\", label = \"%s\"];\n", headNode, compare_operators[(int) headNode -> data.value].name);

    if(headNode -> left !=  nullptr)
    {
        fprintf(dump, "\"%p\" -> \"%p\";\n", headNode, headNode -> left);

        GraphPrintTree(dump, headNode -> left);
    }

    if(headNode -> right !=  nullptr)
    {
        fprintf(dump, "\"%p\" -> \"%p\";\n", headNode, headNode -> right);

        GraphPrintTree(dump, headNode -> right);
    }
}

void GraphTreeDump(tree_node_t* headNode)
{
    assert(headNode);

    static unsigned int DumpNumber = 0;

    char FileIn[17] = "";
    snprintf(FileIn, sizeof(FileIn), "files/Dump%02d.dot", DumpNumber);
    FILE* dump = fopen(FileIn, "wb");
    assert(dump);

    fprintf(dump, "digraph listDump\n{\n\n");

    GraphPrintTree(dump, headNode);
    
    fprintf(dump, "}");
    fclose(dump);

    Systemf("dot -Tpdf  files/Dump%02d.dot -o files/Dump%02d.pdf", DumpNumber, DumpNumber);
    Systemf("rm files/Dump%02d.dot", DumpNumber);
    //Systemf("evince Dump%02d.pdf", DumpNumber);

    DumpNumber++;    
}

void WriteTree(FILE *Write, tree_node_t* headNode)
{
    assert(headNode);
    assert(Write);

    fprintf(Write, "{");

    if (headNode -> data.type == TYPENUMBER)
        fprintf(Write, "%lg", headNode -> data.value);

    else if (headNode -> data.type == TYPEVARIABLE)
        fprintf(Write, "%s", variables[(int)headNode -> data.value].name);

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value <= 24))
        fprintf(Write, "%s", takefunc((int) headNode -> data.value));

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value > 24))
        fprintf(Write, "$%s", takefunc((int) headNode -> data.value)); 

    else if (headNode -> data.type == TYPEOPERATOR)
        fprintf(Write, "%s", operators[(int) headNode -> data.value].name);   

    else
        fprintf(Write, "%s", compare_operators[(int) headNode -> data.value].name);


    if((headNode -> left !=  nullptr) || (headNode -> right !=  nullptr))
    {
        if(headNode -> left !=  nullptr)
            WriteTree(Write, headNode -> left);

        else
            fprintf(Write, "@");

        if(headNode -> right !=  nullptr)
            WriteTree(Write, headNode -> right);

        else
            fprintf(Write, "@");
    }

    fprintf(Write, "}");
}

void preWriteTree(const char *FileOut, tree_node_t* headNode)
{
    assert(headNode);

    FILE* Write = fopen(FileOut, "w");
    assert(Write);

    WriteTree(Write, headNode);
    
    fclose(Write);
}

tree_node_t* readTree()
{
    tree_node_t* node = nullptr;

    if ((*s) == '@')
    {
        s++;
        return node;
    }

    if ((*s) == '{')
    {
        s++;

        char text[MAXCOMMANDLEN] = "";
        int i = 0;

        sscanf(s, " %[^{}@ ] %n", text, &i);
        s += i;

        if (strcmp(";", text) == 0)
            node = CreateNode(TYPEOPERATOR, SEMICOLONop);

        else if (strcmp(",", text) == 0)
            node = CreateNode(TYPEOPERATOR, COMMAop);

        else if (strcmp("if", text) == 0)
            node = CreateNode(TYPEOPERATOR, IFop);

        else if (strcmp("if-else", text) == 0)
            node = CreateNode(TYPEOPERATOR, IFELSEop);

        else if (strcmp("while", text) == 0)
            node = CreateNode(TYPEOPERATOR, WHILEop);

        else if (strcmp("op", text) == 0)
            node = CreateNode(TYPEOPERATOR, OPop);

        else if (strcmp("ret", text) == 0)
            node = CreateNode(TYPEOPERATOR, RETURNop);

        else if (strcmp("=", text) == 0)
            node = CreateNode(TYPEOPERATOR, ASSIGNop);

        else if (isdigit(text[0]))
            node = CreateNode(TYPENUMBER, atof(text));

        else if (strcmp("==", text) == 0)
            node = CreateNode(TYPECOMPARE, 0);

        else if (strcmp("!=", text) == 0)
            node = CreateNode(TYPECOMPARE, 1);

        else if (strcmp(">", text) == 0)
            node = CreateNode(TYPECOMPARE, 2);

        else if (strcmp(">=", text) == 0)
            node = CreateNode(TYPECOMPARE, 3);

        else if (strcmp("<", text) == 0)
            node = CreateNode(TYPECOMPARE, 4);

        else if (strcmp("<=", text) == 0)
            node = CreateNode(TYPECOMPARE, 5);

        else if (text[0] == '$')
            node = CreateNode(TYPEFUNCTION, functionlist(text + 1));

        else if ((*s) == '}')
            node = CreateNode(TYPEVARIABLE, findVariable(text));

        else 
            node = CreateNode(TYPEFUNCTION, functionlist(text));

        node -> left = readTree();

        node -> right = readTree();

        if ((*s) == '}')
            s++;
    }

    return node;  
}

tree_node_t* readTreeFrom(char* input)
{
    assert(input);

    s = input;

    return readTree();
}

const char* ReplaceCode(const char* incode, int num)
{
    for (int i = 0; i < COMMANDSNUMBER; i++)
    {
        if (num == 1)
        {
            if ( strcmp (MyLangCommands[i].MyLang, incode) == 0 )
                return MyLangCommands[i].CLang;
        }

        else
            if ( strcmp (MyLangCommands[i].CLang, incode) == 0 )
                return MyLangCommands[i].MyLang;
    }

    return incode;
}

char* ReCode(char* input, int num)
{
    char* outcode = (char*) calloc(strlen(input), sizeof(outcode[0]));

    s = input;

    while ((*s) != '\0')
    {

        if ( isspace(*s) || ispunct(*s))
        {
            outcode[strlen(outcode)] = *s;

            s++;
        }
        else
        {
            int n = 0;
            char text[MAXCOMMANDLEN] = "";
            sscanf(s, "%[^ \t\n(),=><!/*+^/-]%n", text, &n);

            s += n;

            strcat(outcode, ReplaceCode(text, num));
        }
    }

    return outcode;
}

void WriteTreeLang(FILE *Write, tree_node_t* headNode, int tabs = 0)
{
    assert(headNode);
    assert(Write);

    if (headNode -> data.type == TYPENUMBER)
        fprintf(Write, "%lg", headNode -> data.value);

    else if (headNode -> data.type == TYPEVARIABLE)
        fprintf(Write, "%s", variables[(int)headNode -> data.value].name);

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 3))
    {
        fprintf(Write, "(");
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, " %s ", ReplaceCode(takefunc((int) headNode -> data.value), 0));
        WriteTreeLang(Write, headNode -> right);
        fprintf(Write, ")");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 5))
    {
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, " %s ", ReplaceCode(takefunc((int) headNode -> data.value), 0));
        WriteTreeLang(Write, headNode -> right);
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value == 5))
    {
        fprintf(Write, "(");
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, ")");
        fprintf(Write, " ^ ");
        fprintf(Write, "(");
        WriteTreeLang(Write, headNode -> right);
        fprintf(Write, ")");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 22))
    {
        fprintf(Write, "%s ( ", ReplaceCode(takefunc((int) headNode -> data.value), 0));
        WriteTreeLang(Write, headNode -> right);
        fprintf(Write, " )");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value < 24))
    {
        fprintf(Write, "%s ( ", ReplaceCode(takefunc((int) headNode -> data.value), 0));
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, " )");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value == 24))
    {
        fprintf(Write, "%s ( ", ReplaceCode(takefunc(24), 0));
        WriteTreeLang(Write, headNode -> left);
        fprintf(Write, ", ");
        WriteTreeLang(Write, headNode -> right);
        fprintf(Write, " )");
    }

    else if ((headNode -> data.type == TYPEFUNCTION) && (headNode -> data.value > 24))
    {
        fprintf(Write, "%s( ", ReplaceCode( takefunc((int) headNode -> data.value), 0));
        
        if ((headNode -> left) != nullptr)
            WriteTreeLang(Write, headNode -> left);

        fprintf(Write, " )");
        
        if ((headNode -> right) != nullptr)
            WriteTreeLang(Write, headNode -> right, 1);
    }

    else if (headNode -> data.type == TYPEOPERATOR)
        switch((int) headNode -> data.value)
        {
            case OPop:
                fprintf(Write, "\n");

                for (int i = 0; i < tabs; i++)
                    fprintf(Write, "\t");

                WriteTreeLang(Write, headNode -> left, tabs);

                if ((headNode -> right) != nullptr)
                    WriteTreeLang(Write, headNode -> right, tabs);

                break;

            case SEMICOLONop:
                WriteTreeLang(Write, headNode -> left, 0);

                fprintf(Write, "\n\n");

                if ((headNode -> right) != nullptr)
                    WriteTreeLang(Write, headNode -> right, 0);

                break;

            case COMMAop:
                WriteTreeLang(Write, headNode -> right);

                if ((headNode -> left != nullptr) && (headNode -> left -> data.type == TYPEOPERATOR) && (headNode -> left -> data.value == COMMAop))
                {
                    fprintf(Write, ", ");

                    WriteTreeLang(Write, headNode -> left, 0);
                }
                break;

            case WHILEop:
                fprintf(Write, "%s (", ReplaceCode("while", 0));
                
                WriteTreeLang(Write, headNode -> left);
                
                fprintf(Write, " )");
                
                WriteTreeLang(Write, headNode -> right, tabs + 1);
                
                break;

            case IFop:
                fprintf(Write, "%s ( ", ReplaceCode("if", 0));
                
                WriteTreeLang(Write, headNode -> left);
                
                fprintf(Write, " )");
                
                WriteTreeLang(Write, headNode -> right, tabs + 1);
                
                break;

            case IFELSEop:
                WriteTreeLang(Write, headNode -> left, tabs);
                
                if  ((headNode -> right) != 0) 
                {
                    fprintf(Write, "\n");
                    
                    for (int i = 0; i < tabs - 1; i++)
                        fprintf(Write, "\t");
                    
                    fprintf(Write, "%s", ReplaceCode("else", 0));
                    
                    WriteTreeLang(Write, headNode -> right, tabs);
                }

                break;

            case RETURNop:
                fprintf(Write, "%s ", ReplaceCode("ret", 0)); 
                
                if (headNode -> left != nullptr)
                    WriteTreeLang(Write, headNode -> left);
                
                fprintf(Write, "\n"); 
                
                break;

            case ASSIGNop:
                WriteTreeLang(Write, headNode -> left);
                
                fprintf(Write, " %s ", ReplaceCode("=", 0));
                
                WriteTreeLang(Write, headNode -> right);
                
                break;
    }
    else if (headNode -> data.type == TYPECOMPARE)
    {
        WriteTreeLang(Write, headNode -> left);
        
        fprintf(Write, " %s ", ReplaceCode(compare_operators[(int) headNode -> data.value].name, 0)); 
        
        WriteTreeLang(Write, headNode -> right);
    }
}

void WriteTreeToLang(const char *FileOut, tree_node_t* headNode)
{
    assert(headNode);

    FILE* Write = fopen(FileOut, "w");
    assert(Write);

    WriteTreeLang(Write, headNode);
    
    fclose(Write);
}


#define ASSEMBLERFUNC( number, name)\
                                            printf(#name"\n");                      \
                                            _toassembler(file, node -> left);       \
                                            _toassembler(file, node -> right);      \
                                            fprintf(file, #name"\n");

void _toassembler_comma(FILE* file, tree_node_t* node, int FuncNum, int num)
{
    _toassembler(file, node -> right);

    fprintf(file, "pop [ dx + %d ]\n", num + FuncNum);
    if ((node -> left) != nullptr)
        _toassembler_comma(file, node -> left, FuncNum, num + 1);
}

void _toassembler(FILE* file, tree_node_t* node)                                    //печать в ассемблер по ноде
{

    static int Number_Of_Labels = 0;
    static int FunctionNumber = 0;

    if ( ((node -> data.type) == TYPEOPERATOR) && ((((node -> data.value) == SEMICOLONop) && ((node -> left -> data.value) == ASSIGNop)) || ((node -> data.value) == OPop)) )
    {
        printf("SEMICOLON OP\n");fflush(0);
        _toassembler(file, node -> left);
        if ((node -> right) != nullptr)
            _toassembler(file, node -> right);
    }
    
    else if (IfFunc(node, 22))//get
    {
        printf("get\n");fflush(0);
        fprintf(file, "in\npop[ %s%d ]\n", (variables[(int) node -> left -> data.value].num > 0)? "dx + " : "", FindVarNumber( (int) node -> left -> data.value ));
    }

    else if (IfFunc(node, 23))//put
    {
        printf("put\n");fflush(0);
        _toassembler(file, node -> left);
        fprintf(file, "out\n");
    }

    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == ASSIGNop))
    {
        printf("assign\n");fflush(0);
        _toassembler(file, node -> right);
        fprintf(file, "pop [ %s%d ]\n", (variables[(int) node -> left -> data.value].num > 0)? "dx +  " : "", FindVarNumber( (int) node -> left -> data.value ));
    }
    else if (IfFunc(node, 1))
    {
        ASSEMBLERFUNC(1, add)
    }
    else if (IfFunc(node, 2))
    {
        ASSEMBLERFUNC(2, sub)
    }
    else if (IfFunc(node, 3))
    {
        ASSEMBLERFUNC(3, mul)
    }
    else if (IfFunc(node, 4))
    {
        ASSEMBLERFUNC(4, div)
    }
    else if (IfFunc(node, 5))
    {
        ASSEMBLERFUNC(5, pow)
    }
    else if (IfFunc(node, 21))
    {
        printf("sqrt\n");
        _toassembler(file, node -> right);
        fprintf(file, "sqrt\n");
    }
    else if ( (node -> data.type) == TYPEVARIABLE)
    {
        printf("var\n");fflush(0);
        fprintf(file, "push [ %s%d ]\n", (variables[(int) node -> data.value].num > 0)? "dx + " : "", FindVarNumber( (int) node -> data.value ));
    }

    else if (IfNumber(node))
    {
        printf("number\n");fflush(0);
        fprintf(file, "push %lg\n", node -> data.value);
    }

    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == IFop))
    {
        printf("if\n");fflush(0);
        int LabelNumber = Number_Of_Labels;
        Number_Of_Labels += 2;

        _toassembler(file, node -> left -> left);
        _toassembler(file, node -> left -> right);

        fprintf (file, "%s label%d:\n", JumpWords[(int)node -> left -> data.value].name, LabelNumber);

        _toassembler(file, node -> right -> left);

        if ((node -> right -> right) != nullptr)
        {
            fprintf(file, "jmp label%d:\nlabel%d:\n", LabelNumber + 1, LabelNumber);

            _toassembler(file, node -> right -> right);

            fprintf(file, "label%d:\n", LabelNumber + 1);
        }
        else
            fprintf(file, "label%d:\n", LabelNumber);
    }

    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == WHILEop))
    {
        printf("while\n");fflush(0);
        int LabelNumber = Number_Of_Labels;
        Number_Of_Labels += 2;

        fprintf(file, "label%d:\n", LabelNumber);

        _toassembler(file, node -> left -> left);
        _toassembler(file, node -> left -> right);

        fprintf (file, "%s label%d:\n", JumpWords[(int)node -> left -> data.value].name, LabelNumber + 1);

        _toassembler(file, node -> right);

        fprintf(file, "jmp label%d:\nlabel%d:\n", LabelNumber, LabelNumber + 1);
    }

    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == SEMICOLONop) && ((node -> left -> data.value) > 24))
    {
        printf("SEMICOLON FUNC\n");
        fflush(0);

        if (strcmp(takefunc((int)node -> left -> data.value), "main") == 0)
        {
            fprintf(file, "main:\n");
            fprintf(file, "push dx\npush %d\nadd\npop dx\n", number_of_var);
        }
        else
            fprintf(file, "func%d:\n", (int) node -> left -> data.value);

        FunctionNumber = node -> left -> data.value - 24;

        _toassembler(file, node -> left -> right);

        fprintf(file, "jmp endlabel:\n");
        
        if ((node -> right) != nullptr)
            _toassembler(file, node -> right);
    }
    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == RETURNop))
    {
        printf("ret %d\n", FunctionNumber);
        if (strcmp(takefunc(FunctionNumber + 24), "main") != 0)
        {
            _toassembler(file, node -> left);
            fprintf(file, "pop [ 0 ]\nret\n");
        }
    }
    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == COMMAop))
    {
        //-----------------------------------
    }
    else
    {
        if ((node -> left) != nullptr)
            _toassembler_comma(file, node -> left, VarNumberArray[FunctionNumber], 1);

        fprintf(file, "push dx\npush %d\nadd\npop dx\n", VarNumberArray[FunctionNumber]);
        fprintf(file, "call func%d:\n", (int)node -> data.value);
        fprintf(file, "push [ 0 ]\n");
        fprintf(file, "push dx\npush %d\nsub\npop dx\n", VarNumberArray[FunctionNumber]);
    }

}
#undef ASSEMBLERFUNC

void FromTreeToAssembler(const char *FileOut, tree_node_t* node)                        //печать в ассемблер
{
    assert(node);

    FILE* Write = fopen(FileOut, "w");
    assert(Write);
    fprintf(Write, "jmp main:\n");
    _toassembler(Write, node);
    fprintf(Write, "\n\nendlabel:\nend\n");
    
    fclose(Write);
}
