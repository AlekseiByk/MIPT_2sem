#ifndef HEADER_H
#define HEADER_H

#include <ctype.h>
#include <stdio.h>
#include "FuncTree.h"
#include "ASSert.h"
#include "Text.h"

const int MAXCODELEN = 10000;
const char * s = "";
const char * starts = "";
const int VARNUMBER = 100;
unit_t arr[MAXCODELEN] = {};

int VarNumberArray[50] = {};

struct Namenumnum_t {
    char name[FUNCTIONMAXLEN];
    int num = -2;
    int numnum = 0;
}variables[VARNUMBER] = {};

int number_of_var = 0;

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

struct MyCommands_t{
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
const int COMMANDSNUMBER = 12;

#endif
