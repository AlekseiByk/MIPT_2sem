#ifndef HEADER_H
#define HEADER_H

const int MAXCOMMANDLEN = 200;
const int FUNCTIONMAXLEN = 20;
const int COMMANDSNUMBER = 12;
const int MAXCODELEN = 10000;
const int VARNUMBER = 100;


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include "ASSert.h"
#include "functions.h"

struct node_value_t{
    int type;
    double value;
};

struct unit_t{
    int type;
    int value;
};

struct Namenum_t{
    char name[FUNCTIONMAXLEN];
    int num = 0;
};

struct tree_node_t
{
    node_value_t data;
    tree_node_t* left;
    tree_node_t* right;
    tree_node_t* parant;
};

struct system_struct{
    char name[MAXCOMMANDLEN];
    char color[MAXCOMMANDLEN];
    char form[MAXCOMMANDLEN];
};

struct Namenumnum_t {
    char name[FUNCTIONMAXLEN];
    int num = -2;
    int numnum = 0;
};


tree_node_t* GetG();            // принимает формулу
tree_node_t* GetExpression();   // принимает формулу
tree_node_t* GetNumber();       // принимает число
tree_node_t* GetT();            // * или /
tree_node_t* GetK();            // ^
tree_node_t* GetE();            // + или -
tree_node_t* GetP();            // есть ли скобка
tree_node_t* GetId();           // переменная или функция

tree_node_t* GetTree(const char* str);  // главная 
tree_node_t* GetGeneral();              // основная
tree_node_t* GetMainCode();             // 
tree_node_t* GetAsign();                // присваивание
tree_node_t* GetFunc();                 // функция с параметрами и если есть операторами
tree_node_t* GetOp(int count);          // оператор
tree_node_t* GetObject(int count);      // объект
tree_node_t* GetIf(int count);          // сравнение
tree_node_t* GetWhile(int count);       // цикл
tree_node_t* GetArg();                  // аргументы функции
tree_node_t* GetComp();                 // аргументы сравнения

int findVariable(const char * val, int VarNum = -1);     // работа с переменными
void printERROR(const char *text);                      // распечатка синтаксической ошибки
void SkipSpaces();                                      // пропуск пробелов
tree_node_t* readTreeFrom(char* input);                 // читать дерево по стандарту
void DeleteComments(char *code);                        // удалить коментарии

void GraphTreeDump(tree_node_t* headNode);                              // графический дамп
void preWriteTree(const char *FileOut, tree_node_t* headNode);          // запись по стандарту
char* ReCode(char* input, int num);                                     // изменение от своего языка к си подобному
void WriteTreeToLang(const char *FileOut, tree_node_t* headNode);       // перево дерева в свой язык
void FromTreeToAssembler(const char *FileOut, tree_node_t* headNode);   // в код асемблера
void MakeVarList(tree_node_t* node);                                    // структурирует массив переменных
void _toassembler(FILE* file, tree_node_t* node);
void to_bin(tree_node_t* headnode);                     //to elf executable file

int FindBasePointer(int count);
int FindVarNumber(int count);

void Systemf(const char* format, ...);
int IfFunc(tree_node_t* node, int func);
int IfNum(tree_node_t* node, double Num);
int IfNumber(tree_node_t* node);
void treeDestruct(tree_node_t* headNode);
tree_node_t* TreeCopy(tree_node_t* node);
tree_node_t* Differencial(tree_node_t* headNode, int variable = 'x');
tree_node_t* treeOptimize(tree_node_t* node);
tree_node_t* CreateNode(int type = 0, double val = 0, tree_node_t* node_left = 0, tree_node_t* node_right = 0);
int CompChoose(const char * text);
int functionlist(const char *func);
char* takefunc(int num);

const struct MyCommands_t{
    char MyLang[MAXCOMMANDLEN];
    char CLang[MAXCOMMANDLEN];
} MyLangCommands[] = {
                        {"прымі","get"},
                        {"вывесці","put"},
                        {"калі","if"},
                        {"вярни","ret"},
                        {"інакш","else"},
                        {"пакуль","while"},
                        {"меньш","<"},
                        {"больш",">"},
                        {"роўны","=="},
                        {"раўно","="},
                        {"галоўная","main"},
                        {"вытворная","deriv"},
                        {"корань","sqrt"}
};

enum types{
    TYPENUMBER      = 1,
    TYPEFUNCTION    = 2,
    TYPEVARIABLE    = 3,
    TYPEOPERATOR    = 4,
    TYPECOMPARE     = 5,

    OPop        = 0,
    SEMICOLONop = 1,
    COMMAop     = 2,
    WHILEop     = 3,
    IFop        = 4,
    IFELSEop    = 5,
    RETURNop    = 6,
    ASSIGNop    = 7,
};

const system_struct operators[] = { { "op",         "orange", "circle"}, 
                                    { ";",          "orange", "circle"},
                                    { ",",          "orange", "circle"},
                                    { "while",      "orange", "pentagon"},
                                    { "if",         "orange", "hexagon"},
                                    { "if-else",    "orange", "hexagon"},
                                    { "ret",    "orange", "septagon"},
                                    { "=",          "orange", "larrow"}
                                };


const Namenum_t JumpWords[] = { { "jne", 0x85}, { "je", 0x84}, { "jbe", 0x86}, { "jb", 0x82}, { "jae", 0x83}, { "ja", 0x87}};
const Namenum_t JumpWords2[] = { { "je", 0x84}, { "jne", 0x85}, { "ja", 0x87}, { "jae", 0x83}, { "jb", 0x82}, { "jbe", 0x86}};

const Namenum_t compare_operators[] = { { "==", 0}, { "!=", 0}, { ">", 0}, { ">=", 0}, { "<", 0}, { "<=", 0} };
const int compnum = 6;

extern unit_t arr[MAXCODELEN];
extern int VarNumberArray[50];
extern Namenumnum_t variables[VARNUMBER];
extern int number_of_var;
extern const char * s;
extern const char * starts;


//************************************************************************

#include <elf.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct addres_label
{
    int position;
    char name[100];
    int addres;
};

#define WRITE_HEADER(HEADER, File)\
    for (int i = 0; i < sizeof(HEADER); i++)\
        fputc(((char*)&HEADER)[i], File);

#define B1 __uint8_t
#define B2 __uint16_t
#define B4 __uint32_t
#define B8 __uint64_t

#pragma pack(1)

struct ELF_Header {
  B4 ei_MAG         = 0x464C457F;               // signature
  B1 ei_CLASS       = ELFCLASS64;               // 64 bit format
  B1 ei_DATA        = ELFDATA2LSB;              // little endian
  B4 ei_VERSION     = 0x00000001; 
  B2 ei_OSABI       = ELFOSABI_NONE;            // UNIX System V ABI
  B4 ei_OSABIVER    = 0x00000000;
  B2 E_TYPE         = ET_EXEC;                  // exe
  B2 E_MACHINE      = EM_X86_64;                // x86_64
  B4 E_VERSION      = EV_CURRENT;       
  B8 E_ENTRY        = 0x00000000004000b0;       // entry point
  B8 E_PHOFF        = 0x0000000000000040;       // elf header size
  B8 E_SHOFF        = 0x0000000000000000;       // start of the section header table
  B4 E_FLAGS        = 0x00000000;
  B2 E_EHSIZE       = 0x0040;                   // size of this header
  B2 E_PHENTSIZE    = 0x0038;                   // size of program header table 
  B2 E_PHNUM        = 0x0002;                   // number of entries in the progtam header file
  B2 E_SHENTSIZE    = 0x0040;                   // size of the section header table
  B2 E_SHNUM        = 0x0000;
  B2 E_SHSTRNDX     = 0x0000;
};

struct Program_Header {
  B4 P_TYPE         = 0x00000000 | PT_LOAD;     // Segment will be loaded in memory
  B4 P_FLAGS        = 0x00000000 | PF_R;        // Read and Execute format
  B8 P_OFFSET       = 0x0000000000000000;       // Offset where it should be read
  B8 P_VADDR        = 0x0000000000400000;       // Virtual address where it should be loaded
  B8 P_PADDR        = 0x0000000000400000;       // Phsical address where it should be loaded
  B8 P_FILESZ       = 0x00000000000000b0;       // Size on file
  B8 P_MEMSZ        = 0x00000000000000b0;       // Size in memory
  B8 P_ALIGN        = 0x0000000000000000;
  B4 P_TYPE_exe     = 0x00000000 | PT_LOAD;     // Segment will be loaded in memory
  B4 P_FLAGS_exe    = 0x00000000 | PF_R | PF_X; // Read and Execute format
  B8 P_OFFSET_exe   = 0x00000000000000b0;       // Offset where it should be read
  B8 P_VADDR_exe    = 0x00000000004000b0;       // Virtual address where it should be loaded
  B8 P_PADDR_exe    = 0x00000000004000b0;       // Phsical address where it should be loaded
  B8 P_FILESZ_exe   = 0x0000000000000000;       // Size on file
  B8 P_MEMSZ_exe    = 0x0000000000000000;       // Size in memory
  B8 P_ALIGN_exe    = 0x0000000000000000;
};

//===============================================================

//write exit(0)
//input: char pointer to buffer

//===============================================================

void Write_exit(char* code_buf);

//===============================================================

// put call printf into buffer
// input: char pointer to buffer

//===============================================================

void call_print(char* code_buf);

//===============================================================

// put call scan into buffer
// input: char pointer to buffer

//===============================================================

void call_scan(char* code_buf);

//===============================================================

// return sqrt of the top stack element
// input: char pointer to buffer

//===============================================================

void call_sqrt(char* code_buf);

//===============================================================

// translate tree to binary code
// input: char pointer to buffer, pointer to head_tree_node

//===============================================================

void _to_bin(char* code_buf, tree_node_t* head);

//===============================================================

// this is for update/// now we can use only 12 registers
// 

//===============================================================

void check_number_of_var();
void call_mul(char* code_buf);                                      // перемножает 2 последних числа в стэке
void call_div(char* code_buf);                                      // deviding
int take_addres(const char * name);                                 // return address of name label
void write_addres(const char * name);                                // record address of name label
void mov_from_rax(char* code_buf, int variable);                    // mov from rax to ...
void mov_to_rax(char* code_buf, int variable);
void _to_bin_comma(char* code_buf, tree_node_t* node, int FuncNum, int num);
void _to_bin(char* code_buf, tree_node_t* node);

extern addres_label labels[100];
extern int number_of_labels;
extern int FunctionNumber;
extern int print_pointer;                         //pointer to print function
extern int scan_pointer ;                         //pointer to scan function
extern int size;                                  //size of executable code
extern int entery_point;                          //entery point of executable code
extern int entery_point_0;
extern int Number_Of_Labels_hex;

#endif
