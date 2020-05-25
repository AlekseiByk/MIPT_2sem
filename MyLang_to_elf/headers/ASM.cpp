#include <elf.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

                                                //pointer to print function
int print_pointer = 0;

                                                //pointer to scan function
int scan_pointer  = 0;

                                                //size of executable code
int size = 0;

                                                //entery point of executable code
int entery_point = 0;
int entery_point_0 = 0;

int Number_Of_Labels_hex = 0;

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

void to_bin(tree_node_t* headnode)
{
     check_number_of_var();
    FILE* output = fopen("My_elf.out", "wb");
    assert(output);

                                                                                    //transport print function realization into print_buffer
    FILE* put_bin = fopen("compiled files/print_bin.out", "rb");
    assert(put_bin);

    char* print_buffer = (char*) calloc(160, sizeof(print_buffer[0]));
    int   print_size = fread(print_buffer, sizeof(print_buffer[0]), 192, put_bin);
    fclose(put_bin);
    
                                                                                    //transport scan function realization into scan_buffer
    FILE* get_bin = fopen("compiled files/scan_bin.out", "rb");
    assert(get_bin);

    char* scan_buffer = (char*) calloc(192, sizeof(scan_buffer[0]));
    int   scan_size = fread(scan_buffer, sizeof(scan_buffer[0]), 192, get_bin);
    fclose(get_bin);
                                                                                    
                                                                                    //set scan_pointer value for next call operations
    scan_pointer += print_size;
    entery_point_0 =  print_size + scan_size;

                                                                                    //create array of binary code  
    char* code_buf = (char*) calloc(10000, 1);
    _to_bin(code_buf, headnode);                                                        //  first time to fullfill labels array
    Number_Of_Labels_hex = 0;                                                           //  set label number to 0 for second compilation
    size = 0;                                                                           //  set size to zero ...
    entery_point =  print_size + scan_size;
    _to_bin(code_buf, headnode);                                                        //  second compilation with fullfilled labels array
    Write_exit(code_buf);



    size += print_size + scan_size + 10;


    ELF_Header elf_h = {};
        elf_h.E_ENTRY = entery_point  + 0x4000b0;                                   //point where main starts

    printf("0x%lx\n", elf_h.E_ENTRY);
    
    Program_Header pr_h = {};                                                       //set size in program header
        pr_h.P_FILESZ_exe = size;
        pr_h.P_MEMSZ_exe = size;

    WRITE_HEADER(elf_h, output);            
    WRITE_HEADER(pr_h, output);
    
    fwrite(print_buffer, sizeof(print_buffer[0]), print_size, output);
    fwrite(scan_buffer, sizeof(scan_buffer[0]), scan_size, output);

                                                                                    //write program
    fwrite(code_buf, sizeof(code_buf[0]), size - print_size - scan_size - 10, output);

    

    printf("%x\n", entery_point + 0xb0);

    fclose(output);
    free(code_buf);
    free(scan_buffer);
    free(print_buffer);
    return;
}

void call_print(char* code_buf)
{
    int temp = print_pointer - entery_point_0 - size - 5;                           //calculating call addres
    code_buf[size++] = 0xe8;                                                        //call opcode                             
    *((int*)(code_buf + size)) = temp;
    size += sizeof(int);                                                            //"address"
    return;
}

void call_scan(char* code_buf)
{
    int temp = scan_pointer - entery_point_0 - size - 5; //calculating call addres
    code_buf[size++] = 0xe8;
    *((int*)(code_buf + size)) = temp;
    size += sizeof(int);
    return;
}

void check_number_of_var()
{
    if ((VarNumberArray[takefunc("main")] + VarNumberArray[0])> 12)
    {
        printf("\033[01;31mError:\033[00m A lot of Variables");
        abort();
    }
    return;
}

#define PUTHEX4(hex)                                                \
                        *((int*)(code_buf + size)) = hex;           \
                        size += 4; 
#define PUTHEX2(hex)                                                \
                        *((short int*)(code_buf + size)) = hex;     \
                        size += 2;
#define PUTHEX1(hex)                                                \
                        *((char*)(code_buf + size)) = hex;          \
                        size += 1; 

void Write_exit(char* code_buf)
{
    PUTHEX4(0x00003cb8);     //mov rax, 0x3C      ; exit64 (rdi)   
    PUTHEX4(0xff314800)      //xor rdi, rdi
    PUTHEX2(0x050f);     //syscall
    return;
}

void call_sqrt(char* code_buf)                                      // берет корень из последнего число в стэке
{
    // FILD QWORD [RSP]
    // ADD RSP, 8 
    // fsqrt
    // PUSH 100
    // FILD QWORD [RSP]
    // ADD RSP, 8
    // fmulp
    // sub rsp, 8
    // fistp QWORD [RSP]
    PUTHEX4(0x48242cdf);        
    PUTHEX4(0xd908c483);
    PUTHEX4(0xdf646afa);
    PUTHEX4(0x8348242c);
    PUTHEX4(0xc9de08c4);
    PUTHEX4(0x08ec8348);
    PUTHEX2(0x3cdf);
    PUTHEX1(0x24);
    return;
}

void call_mul(char* code_buf)                                       // перемножает 2 последних числа в стэке
{
    // FILD QWORD [RSP + 8]
    // FILD QWORD [RSP]
    // ADD RSP,16
    // fmulp
    // PUSH 10000
    // FILD QWORD [RSP]
    // ADD RSP,8 
    // fdivp
    // sub rsp, 8
    // fistp QWORD [RSP]
    PUTHEX4(0x08246cdf);        
    PUTHEX4(0x48242cdf);
    PUTHEX4(0xde10c483);
    PUTHEX4(0x271068c9);
    PUTHEX4(0x2cdf0000);
    PUTHEX4(0xc4834824);
    PUTHEX4(0x48f9de08);
    PUTHEX4(0xdf08ec83);
    PUTHEX2(0x243c);
    return;
}

void call_div(char* code_buf)                                       // deviding
{
    // FILD QWORD [RSP + 8]
    // FILD QWORD [RSP]
    // ADD RSP,16
    // fdivp
    // PUSH 10000
    // FILD QWORD [RSP]
    // ADD RSP,8 
    // fmulp
    // sub rsp, 8
    // fistp QWORD [RSP]
    PUTHEX4(0x08246cdf);        
    PUTHEX4(0x48242cdf);
    PUTHEX4(0xde10c483);
    PUTHEX4(0x271068f9);
    PUTHEX4(0x2cdf0000);
    PUTHEX4(0xc4834824);
    PUTHEX4(0x48c9de08);
    PUTHEX4(0xdf08ec83);
    PUTHEX2(0x243c);
    return;
}

int take_addres(const char * name)                                  //return address of name label
{
    for (int i = 0;i < number_of_labels; i++)
        if (!strcmp(name, labels[i].name))
            return labels[i].addres;

    strcpy(labels[number_of_labels].name, name);
    labels[number_of_labels].position = size;
    number_of_labels++;

    return 0;
}

void write_addres(const char * name)                                //record address of name label
{
    for (int i = 0;i < number_of_labels; i++)
        if (!strcmp(name, labels[i].name))
        {
            labels[i].addres = size;
            return;
        }

    strcpy(labels[number_of_labels].name, name);
    labels[number_of_labels].addres = size;
    number_of_labels++;
    return;
}

int FunctionNumber = 0;

void mov_from_rax(char* code_buf, int variable)                     //mov from rax to ...
{
    if ((VarNumberArray[variables[variable].num] > 0) && strcmp(takefunc(FunctionNumber + 24), "main"))
        {PUTHEX4(0x00458948 | ((variables[variable].numnum ) * 8) << 24);}
    else
    {
        if (variables[variable].num != 0)
        {
            PUTHEX2(0x8949);
            PUTHEX1(0xc0 + (char)(variables[variable].numnum - 1));
        }
        else
        {
            PUTHEX2(0x8948);
            switch(variable)
            {
                case 1:
                    PUTHEX1(0xc2);
                    break;
                case 2:
                    PUTHEX1(0xc3);
                    break;
                case 3:
                    PUTHEX1(0xc6);
                    break;
                case 4:
                    PUTHEX1(0xc7);
                    break;
                default:
                    printf("\033[01;31mError:\033[00m Something went wrong");
                    abort();
                    break;
        }
    }
}

void mov_to_rax(char* code_buf, int variable)
{
    if ((VarNumberArray[variables[variable].num] > 0) && strcmp(takefunc(FunctionNumber + 24), "main"))
        {PUTHEX4(0x00458b48 | ((variables[variable].numnum ) * 8) << 24);}                  //stack for variables from functions
    else
    {
        if ( variables[variable].num != 0)
        {
            
            PUTHEX2(0x894c);                                           //using registers r8-r15 for main variables
            PUTHEX1(0xc0 + ((char)(variables[variable].numnum - 1) * 8));
        }
        else
        {
            PUTHEX2(0x8948);                                           //using registers rbx, rdx, rsi, rdi for global variables 
            switch(variable)    
            {
                case 1:
                    PUTHEX1(0xd0);
                    break;
                case 2:
                    PUTHEX1(0xd8);
                    break;
                case 3:
                    PUTHEX1(0xf0);
                    break;
                case 4:
                    PUTHEX1(0xf8);
                    break;
                default:
                    printf("\033[01;31mError:\033[00m Something went wrong");
                    abort();
                    break;
            }
        }
    }
}

void _to_bin_comma(char* code_buf, tree_node_t* node, int FuncNum, int num)
{
    printf("<%d %d>\n", FuncNum, num);

    if ((node -> left) != nullptr)
        _to_bin_comma(code_buf, node -> left, FuncNum, num + 1);
    else
    {
        if ((FuncNum - num) != 0)
        {
            PUTHEX2(0x8148);                                        //sub rsp, number of variables - arguments
            PUTHEX1(0xec);
            PUTHEX4((FuncNum - num) * 8);
        }
    }

    _to_bin(code_buf, node -> right);                               //load function arguments

    PUTHEX1(0x50);                                                  //push rax
}

void _to_bin(char* code_buf, tree_node_t* node)
{
    if ( ((node -> data.type) == TYPEOPERATOR) && ((((node -> data.value) == SEMICOLONop) && ((node -> left -> data.value) == ASSIGNop)) || ((node -> data.value) == OPop)) )
    {
        printf("SEMICOLON OP\n");fflush(0);

        _to_bin(code_buf, node -> left);

        if ((node -> right) != nullptr)
            _to_bin(code_buf, node -> right);
    }
    
    else if (IfFunc(node, 22))                                      //get
    {
        printf("get\n");fflush(0);

        call_scan(code_buf);

        mov_from_rax(code_buf, (int) node -> left -> data.value);  
    }

    else if (IfFunc(node, 23))                                      //put
    {
        printf("put\n");fflush(0);

        if (node -> left -> data.type == TYPEVARIABLE)
            mov_to_rax(code_buf, (int) node -> left -> data.value);
        else
            _to_bin(code_buf, node -> left);
        
        call_print(code_buf);
    }

    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == ASSIGNop))
    {
        printf("assign\n");fflush(0);

        _to_bin(code_buf, node -> right);

        mov_from_rax(code_buf, (int) node -> left -> data.value);
    }
    else if (IfFunc(node, 1))                                       //add
    {
        _to_bin(code_buf, node -> left);
        PUTHEX1(0x50);                                              //push rax

        _to_bin(code_buf, node -> right);
        PUTHEX1(0x59);                                              //pop rcx

        PUTHEX2(0x0148);                                            //add rax, rcx
        PUTHEX1(0xc8);
        
    }
    else if (IfFunc(node, 2))                                       //sub
    {
        _to_bin(code_buf, node -> left);
        PUTHEX1(0x50);                                              //push rax

        _to_bin(code_buf, node -> right);
        PUTHEX1(0x59);                                              //pop rcx

        PUTHEX2(0x2948);
        PUTHEX1(0xc1);                                              //sub rcx, rax
        PUTHEX2(0x8948);                                            // mov rax, rcx
        PUTHEX1(0xc8);

        
    }
    else if (IfFunc(node, 3))                                       //mul
    {
        _to_bin(code_buf, node -> left);
        PUTHEX1(0x50);
        _to_bin(code_buf, node -> right);
        PUTHEX1(0x50);
        call_mul(code_buf);
        PUTHEX1(0x58);
    }
    else if (IfFunc(node, 4))                                       //div
    {
        _to_bin(code_buf, node -> left);
        PUTHEX1(0x50);                                              //push rax
        _to_bin(code_buf, node -> right);
        PUTHEX1(0x50);                                              //push rax
        call_div(code_buf);
        PUTHEX1(0x58);                                              //pop rax
    }   
    // else if (IfFunc(node, 5))
    // {
        
    // }
    else if (IfFunc(node, 21))      //sqrt
    {
        printf("sqrt\n");
        _to_bin(code_buf, node -> right);
        PUTHEX1(0x50);
        call_sqrt(code_buf);
        PUTHEX1(0x58);
    }
    else if ( (node -> data.type) == TYPEVARIABLE)
    {
        printf("var\n");fflush(0);
        mov_to_rax(code_buf, (int) node -> data.value);
    }

    else if (IfNumber(node))
    {
        printf("number\n");fflush(0);
        PUTHEX1(0xb8);
        PUTHEX4((int)(node -> data.value * 10000));
    }

    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == IFop))
    {
        printf("if\n");fflush(0);
        int LabelNumber = Number_Of_Labels_hex;
        Number_Of_Labels_hex += 2;
        char label_name[8] = "";

        _to_bin(code_buf, node -> left -> left);
        PUTHEX1(0x50);              //rush rax

        _to_bin(code_buf, node -> left -> right);
        PUTHEX1(0x59);              //pop rcx

        PUTHEX2(0xc139);            //cmp rcx, rax        

        PUTHEX1(0x0f);
        PUTHEX1((char)JumpWords[(int)node -> left -> data.value].num);

        
        sprintf(label_name, "label%d", LabelNumber);
        int temp = take_addres(label_name) - size - 4;
        PUTHEX4(temp);

        _to_bin(code_buf, node -> right -> left);

        if ((node -> right -> right) != nullptr)
        {
            PUTHEX1(0xe9);
            sprintf(label_name, "label%d", LabelNumber + 1);
            temp = take_addres(label_name) - size - 4;
            PUTHEX4(temp);

            sprintf(label_name, "label%d", LabelNumber);
            write_addres(label_name);

            _to_bin(code_buf, node -> right -> right);

            sprintf(label_name, "label%d", LabelNumber + 1);
            write_addres(label_name);
        }
        else
        {
            sprintf(label_name, "label%d", LabelNumber);
            write_addres(label_name);
        }
    }

    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == WHILEop))
    {//*************************************************************************************************************************************
        printf("while\n");fflush(0);
        int LabelNumber = Number_Of_Labels_hex;
        Number_Of_Labels_hex += 2;
        char label_name[8] = "";

                                //  --------------------------jmp comparing
        PUTHEX1(0xe9);
        sprintf(label_name, "label%d", LabelNumber + 1);
        int temp = take_addres(label_name) - size - 4;
        PUTHEX4(temp);

        sprintf(label_name, "label%d", LabelNumber);
        write_addres(label_name);

        _to_bin(code_buf, node -> right);       //------------while body

        sprintf(label_name, "label%d", LabelNumber + 1);
        write_addres(label_name);
                                //  --------------------------comparing
        _to_bin(code_buf, node -> left -> left);
        PUTHEX1(0x50);              //rush rax

        _to_bin(code_buf, node -> left -> right);
        PUTHEX1(0x59);              //pop rcx

        PUTHEX2(0xc139);            //cmp rcx, rax        

        PUTHEX1(0x0f);              //j..
        PUTHEX1((char)JumpWords2[(int)node -> left -> data.value].num);
        sprintf(label_name, "label%d", LabelNumber);
        temp = take_addres(label_name) - size - 4;
        PUTHEX4(temp);

    }

    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == SEMICOLONop) && ((node -> left -> data.value) > 24))
    {
        if (strcmp(takefunc((int)node -> left -> data.value), "main") == 0)
            entery_point += size;
        else
            write_addres(takefunc((int)node -> left -> data.value));

        FunctionNumber = node -> left -> data.value - 24;

        printf("SEMICOLON FUNC %d\n", FunctionNumber);
        fflush(0);

        PUTHEX1(0x55);          //push rbp
        PUTHEX4(0x246c8d48);        //mov rbp, rsp + 8
        PUTHEX1(0x08);              
        
        _to_bin(code_buf, node -> left -> right);

        if ((node -> right) != nullptr)
            _to_bin(code_buf, node -> right);
    }
    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == RETURNop))
    {
        printf("ret %d\n", FunctionNumber);
        if (strcmp(takefunc(FunctionNumber + 24), "main") != 0)
        {
            _to_bin(code_buf, node -> left);
            PUTHEX1(0x5d);         //pop rbp
            PUTHEX1(0xc3);         //ret
        }
    }
    else if (((node -> data.type) == TYPEOPERATOR) && ((node -> data.value) == COMMAop))
    {
        //-----------------------------------
    }
    else//      function call
    {
        if ((node -> left) != nullptr)
            _to_bin_comma(code_buf, node -> left, VarNumberArray[(int)node -> data.value - 24], 1);

        int temp = take_addres(takefunc((int)node -> data.value)) - size - 5; //calculating call addres
        printf("0x%x, %d\n", temp, temp);
        PUTHEX1(0xe8);  //call
        PUTHEX4(temp);

        PUTHEX2(0x8148);    //add rsp number of vars
        PUTHEX1(0xc4);
        PUTHEX4((VarNumberArray[(int) node -> data.value - 24])*8);
    }

}
