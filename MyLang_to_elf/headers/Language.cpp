#include "Header.h"

tree_node_t* GetTree(const char* str)
{
    s = str;
    starts = s;

    return GetGeneral();
}

tree_node_t* GetGeneral()
{
    //printf("GetGeneral\n");
    while(((*s) == '\n') || ((*s) == ' '))
            s++;

    tree_node_t* val = nullptr;
    //printf("/%s/ %d\n", s, *s);
    if(!(iscntrl(*s) && (!isspace(*s))))
    {
        val = CreateNode(TYPEOPERATOR, SEMICOLONop);

        val -> left = GetMainCode();

        while(((*s) == '\n') || ((*s) == ' '))
            s++;

        val -> right = GetGeneral();
    }
    return val;
}


tree_node_t* GetMainCode()
{
    //printf("GetMain\n");
    int n = 0;
    char text[MAXCOMMANDLEN] = "";
    const char * s1 = s;

    sscanf(s1, "%[^ \t\n(),=><!/*+^/-]%n", text, &n);
    s1 += n;

    while(*s1 == ' ')
        s1++;

    if (*s1 == '=')
        return GetAsign();

    return GetFunc();
}

tree_node_t* GetAsign()
{
    //printf("GetAsign\n");
    tree_node_t* val = CreateNode(TYPEOPERATOR, ASSIGNop);

    val -> left = GetId();

    SkipSpaces();
    s++;
    SkipSpaces();

    val -> right = GetG();
    return val;
}

tree_node_t* GetFunc()
{
    //printf("GetFunc\n");
    tree_node_t* val = GetId();

    while ((*s) == '\n')
        s++;

    val -> right = GetOp(1);

    return val;
}

tree_node_t* GetOp(int count)
{
    //printf("GetOp\n");
    if ((*(s + count - 1)) != '\t')
        return nullptr;
    s += count;

    tree_node_t* val = CreateNode(TYPEOPERATOR, OPop);
    val -> left = GetObject(count);

    SkipSpaces();
    while ((*s) == '\n')
        s++;

    val -> right = GetOp(count);

    return val;
}

tree_node_t* GetObject(int count)
{
    //printf("GetObject\n");
    int n = 0;
    char text[MAXCOMMANDLEN] = "";
    const char * s1 = s;
    sscanf(s1, "%[^ \t\n(),=><!/*+^/-]%n", text, &n);

    s1 += n;
    while(*s1 == ' ')
        s1++;

    if (strcmp(text, "if") == 0)
    {
        s += n;
        return GetIf(count);
    }

    if (strcmp(text, "while") == 0)
    {
        s += n;
        return GetWhile(count);
    }

    if (strcmp(text, "while") == 0)
    {
        s += n;
        return GetWhile(count);
    }

    if (strcmp(text, "ret") == 0)
    {
        tree_node_t* val = CreateNode(TYPEOPERATOR, RETURNop);
        s += n;
        SkipSpaces();

        if ((*s) != '\n')
            val -> left = GetG();
        else 
            s++;

        return val;
    }

    if (*s1 == '=')
        return GetAsign();

    return GetId();
}

tree_node_t* GetIf(int count)
{
    //printf("GetIf\n");
    SkipSpaces();

    if ((*s) != '(')
        printERROR("Something missed here, maybe \'(\'");

    s++;
    SkipSpaces();

    tree_node_t* val = CreateNode(TYPEOPERATOR, IFop, nullptr, CreateNode(TYPEOPERATOR, IFELSEop));
    val -> left = GetComp();

    SkipSpaces();
    if ((*s) != ')')
        printERROR("Something missed here, maybe \')\'");
    s++;
    SkipSpaces();

    while ((*s) == '\n')
        s++;

    val -> right -> left = GetOp(count + 1);

    int n = 0;
    char text[MAXCOMMANDLEN] = "";
    const char * s1 = s + count;
    sscanf(s1, "%[^ \t\n(),=><!/*+^/-]%n", text, &n);

    if (strcmp(text, "else") != 0)
        return val;

    s += count + n;
    SkipSpaces();
    while ((*s) == '\n')
        s++;

    val -> right -> right = GetOp(count + 1);

    return val;
}

tree_node_t* GetComp()
{
    //printf("GetComp\n");
    tree_node_t* val = GetExpression();

    SkipSpaces();
    int n = 0;
    char text[MAXCOMMANDLEN] = "";

    sscanf(s, "%[=><!]%n", text, &n);

    s += n;
    SkipSpaces();

    if ((n > 2) || (n == 0))
        printERROR("what means this comparating");

    val = CreateNode(TYPECOMPARE, CompChoose(text), val, nullptr);


    val -> right = GetExpression();

    return val;
}

tree_node_t* GetWhile(int count)
{
    //printf("GetWhile\n");
    SkipSpaces();

    if ((*s) != '(')
        printERROR("Something missed here, maybe \'(\'");

    s++;
    SkipSpaces();

    tree_node_t* val = CreateNode(TYPEOPERATOR, WHILEop);

    val -> left = GetComp();

    SkipSpaces();

    if ((*s) != ')')
        printERROR("Something missed here, maybe \')\'");

    s++;
    SkipSpaces();

    while ((*s) == '\n')
        s++;

    val -> right = GetOp(count + 1);

    return val;
}

tree_node_t* GetArg()
{
    //printf("GetArg\n");
    tree_node_t* val = CreateNode(TYPEOPERATOR, COMMAop);

    val -> right = GetG();

    SkipSpaces();

    if ((*s) == ',')
    {
        s++;

        val -> left = GetArg();
    }

    return val;
}


tree_node_t* GetG()
{
    //printf("GetG\n");

    tree_node_t* val = GetExpression();

    SkipSpaces();

    if (!((*s == '\n') || (*s == '\0') || (*s == ',') || (*s == ')')))
        printERROR("the last symbol of expression is no a \'\\n\', \',\', \'\\0\', \')\'");

    return val;
}

tree_node_t* GetExpression()
{
    //printf("GetExp\n");

    tree_node_t* node = GetT();
    
    SkipSpaces();

    while (*s == '+' || *s == '-')
    {
        char OP = *s;

        s++;

        tree_node_t* node2 = GetT();

        if (OP == '+')
            node = CreateNode(TYPEFUNCTION, functionlist("+"), node, node2);
        else
            node = CreateNode(TYPEFUNCTION, functionlist("-"), node, node2);
    }

    return node;
}

tree_node_t* GetT()
{
    //printf("GetT\n");

    tree_node_t* node = GetK();

    while (*s == '*' || *s == '/')
    {
        char OP = *s;

        s++;

        tree_node_t* node2 = GetK();

        if (OP == '*')
            node = CreateNode(TYPEFUNCTION, functionlist("*"), node, node2);
        else
            node = CreateNode(TYPEFUNCTION, functionlist("/"), node, node2);
    }

    return node;
}

tree_node_t* GetK()
{
    //printf("GetK\n");
    tree_node_t* node = GetP();

    SkipSpaces();

    if (*s == '^')
    {
        s++;

        tree_node_t* node2 = GetP();

        node = CreateNode(TYPEFUNCTION, 5, node, node2);
    }

    return node;
}

tree_node_t* GetP()
{
    //printf("GetP\n");
    SkipSpaces();

    if (*s == '(')
    {
        s++;

        tree_node_t* node = GetExpression();

        SkipSpaces();

        if (*s != ')')
        {
            printERROR("missing \')\'");
        }

        s++;

        return node;
    }

    if (isdigit(*s))
        return GetNumber();

    return GetId();
}

tree_node_t* GetNumber()
{   
    //printf("GetN\n");
    double val = 0;
    int n = 0;

    sscanf( s,"%lf%n", &val, &n);

    s += n;
    SkipSpaces();

    return CreateNode(TYPENUMBER, val);
}

tree_node_t* GetId()
{
    //printf("GetId\n");
    char func[FUNCTIONMAXLEN] = {};
    int n = 0;

    SkipSpaces();

    sscanf(s, "%[^ \t\n(),=><!/*+^/-]%n", func, &n);

    s += n;
    SkipSpaces();

    tree_node_t* node = nullptr;

    if((*s) == '(')
    {
        if (functionlist(func) < 22)
            node = CreateNode(TYPEFUNCTION, functionlist(func), nullptr, GetP());

        else if (functionlist(func) < 24)
            node = CreateNode(TYPEFUNCTION, functionlist(func), GetP(), nullptr);

        else if (functionlist(func) == 24)
        {
            node = CreateNode(TYPEFUNCTION, functionlist(func));

            s++;

            node -> left = GetId();

            if ((*s) != ',')
                printERROR("Missing \',\'");

            s++;

            node -> right = GetG();

            if ((*s) != ')')
                printERROR("Missing \')\'");

            s++;
        }
        else
        {
            node = CreateNode(TYPEFUNCTION, functionlist(func));

            s++;

            SkipSpaces();

            if ((*s) != ')')
                node -> left = GetArg();

            if ((*s) == ')')
                s++;
        }

    }
    else
        node = CreateNode(TYPEVARIABLE, findVariable(func));

    return node;
}

void DeleteComments(char *code)
{
    char* str = code;

    while((*str) != '\0')
    {
        if ((*str) == '@')
        {
            *str = ' ';

            while ((*str) != '@')
            {
                *str = ' ';

                str++;
            }

            *str = ' ';
        }

        str++;
    }

    char * last = code;
    str = code;

    while((*str) != '\0')
    {
        if ((*str) > ' ')
            last = str;

        str++;
    }

    *(last + 1) = '\0';

}

void SkipSpaces()
{
    while(*s == ' ')
        s++;
}

void printERROR(const char *text)
{
    if (s - starts >= 8)
        printf("Syntax Error: %s\n\t%-.8s\n\t\t^\n\t\t%lu\n", text, s - 5, s - starts);

    else
        printf("Syntax Error: %s\n\t%-.8s\n\t   ^^^^^^\n\t     %lu\n", text, starts, s - starts);
    
    fflush(0);
    abort();
}


int findVariable(const char * var, int VarNum)
{
    for (int i = 0; i < number_of_var; i++)
    {
        if ((strcmp(var, variables[i].name) == 0) && (variables[i].num == VarNum) )
            return i;
    }

    strcpy(variables[number_of_var].name, var);
    variables[number_of_var].num = VarNum;
    variables[number_of_var].numnum = 100;

    number_of_var++;

    return number_of_var - 1;
}

void _MakeVarList(tree_node_t* node)
{
    static int VarVar = 0;

    if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == SEMICOLONop) && ((node -> left -> data.type) == TYPEOPERATOR) && ((node -> left -> data.value) == ASSIGNop))
        if ((node -> left -> left -> data.type) == TYPEVARIABLE)
            variables[(int) node -> left -> left -> data.value].num = 0;

    if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == SEMICOLONop) && ((node -> left -> data.type) == TYPEFUNCTION))
        VarVar = node -> left -> data.value - 24;

    if ((node -> data.type) == TYPEVARIABLE)
        if ((variables[(int) node -> data.value].num != VarVar) && (variables[(int) node -> data.value].num != 0))
            node -> data.value = findVariable(variables[(int) node -> data.value].name, VarVar);

    if ((node -> left) != nullptr)
        _MakeVarList(node -> left);

    if ((node -> right) != nullptr)
        _MakeVarList(node -> right);
}

void _MakeVarList_for_function(tree_node_t* node)
{
    static int VarVar = 0;

    if ((node -> left != nullptr) && (((node -> left -> data.type) == TYPEFUNCTION) && ((node -> left -> left) != nullptr)))
    {
        VarVar = 0;
        _MakeVarList_for_function(node -> left-> left);
    }
    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == COMMAop))
    {
        variables[(int)node -> right -> data.value].numnum = VarVar;
        VarVar++;
        if ((node -> left) != nullptr)
            _MakeVarList_for_function(node -> left);
    }
    else if ((node -> right) != nullptr)
    {
        _MakeVarList_for_function(node -> right);
    }
}

int comp(const void* a, const void* b)
{
    if ((((Namenumnum_t*) a) -> num) != (((Namenumnum_t*) b) -> num))
        return ((Namenumnum_t*) a) -> num - ((Namenumnum_t*) b) -> num;

    return ((Namenumnum_t*) a) -> numnum - ((Namenumnum_t*) b) -> numnum;
}

void MakeVarList(tree_node_t* node)
{
    _MakeVarList(node);

    _MakeVarList_for_function(node);

    qsort(variables, number_of_var, sizeof(Namenumnum_t), comp);

    int counter = -1;
    int counter_ = 1;
    for (int i = 0; i < number_of_var; i++)
        if (variables[i].num == counter)
        {
            variables[i].numnum = counter_;
            counter_++;
        }
        else
        {
            if (counter >= 0)
                VarNumberArray[counter] = counter_ - 1;
            counter++;
            counter_ = 1;
            i--;
        }
    VarNumberArray[counter] = counter_ - 1;

    for (int i = 0; i <= counter; i++)
        printf("%d) %d\n", i, VarNumberArray[i]);
             
}

int FindVarNumber(int count)
{
    for (int i = 0;;i++)
        if (variables[ count ].num == variables[ i ].num)
        {
            return variables[ count ].numnum;
            break;
        }
    return 0;
}

int FindBasePointer(int count)
{
    for (int i = 0;;i++)
        if (variables[ count ].num == variables[ i ].num)
        {
            return i;
            break;
        }
    return 0;
}

