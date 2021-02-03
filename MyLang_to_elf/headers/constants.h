#pragma once

const int MAXCOMMANDLEN = 200;
const int FUNCTIONMAXLEN = 20;
const int COMMANDSNUMBER = 12;
const int MAXCODELEN = 10000;
const int VARNUMBER = 100;
const int compnum = 6;

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

const struct system_struct{
    char name[MAXCOMMANDLEN];
    char color[MAXCOMMANDLEN];
    char form[MAXCOMMANDLEN];
} operators[] = { { "op",         "orange", "circle"}, 
                                    { ";",          "orange", "circle"},
                                    { ",",          "orange", "circle"},
                                    { "while",      "orange", "pentagon"},
                                    { "if",         "orange", "hexagon"},
                                    { "if-else",    "orange", "hexagon"},
                                    { "ret",    "orange", "septagon"},
                                    { "=",          "orange", "larrow"}
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


const struct Namenum_t{
    char name[FUNCTIONMAXLEN];
    int num = 0;
} JumpWords[] = { { "jne", 0x85}, { "je", 0x84}, { "jbe", 0x86}, { "jb", 0x82}, { "jae", 0x83}, { "ja", 0x87}};
const Namenum_t JumpWords2[] = { { "je", 0x84}, { "jne", 0x85}, { "ja", 0x87}, { "jae", 0x83}, { "jb", 0x82}, { "jbe", 0x86}};

const Namenum_t compare_operators[] = { { "==", 0}, { "!=", 0}, { ">", 0}, { ">=", 0}, { "<", 0}, { "<=", 0} };