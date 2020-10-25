#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys\stat.h>
#include "Stack\Stack.h"

#define SYNTAX_ERROR  printf ("\nIn file %s: symbol %d: Syntax Error: There is no such function as %s.\n",  \
                               file_name, i, point_buff + i);                                               \
                                                                                                            \
                      assert (!"OK");                                                                       \


#define EXECUTE_ERROR  printf ("\nIn file %s: Execute Error: Code is not correct. PC is %d\n",    \
                               file_name, cpu->pc);                                               \
                                                                                                  \
                       assert (!"OK");                                                            \


#define ARYTHMETIC_ERROR  printf ("\nIn file %s: Arythmetic Error: You cannot divide by zero. PC is %d\n",    \
                               file_name, cpu->pc);                                                           \
                                                                                                              \
                       assert (!"OK");


#define DEF_CMD(name, num, argc, code) \
                name##_CMD = num,      \

enum commands
{
    #include "Commands.h"
};

#undef DEF_CMD

enum reg_names
{
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4,
};

const size_t NUM_COMMANDS        = 12;
const size_t NUM_REGISTERS       = 4;
const size_t MAX_STRING_SIZE     = 30;
const int    IMPOSSIBLE_COMMAND  = -1;
const size_t PUSH_MODE_REG       = 1;
const size_t PUSH_MODE_VAL       = 2;
const int    IMPOSSIBLE_POS      = -2;
const char   IMPOSSIBLE_REG      = -1;
const size_t MAX_BUFF_SIZE       = 10000;


struct file_info
{
    size_t num_symbols;
    char*  point_buff;
};

struct CPU
{
    struct Stack_t* Stack;
    char*           code;
    size_t          pc;

    double*        registers;
};

void             Translator              (const struct file_info buff_info);
struct file_info File_Interactor         (const char* file_name);
size_t           Size_File_Counter       (const char* file_name);
size_t           Buff_Filler             (char* point_buff, const char* file_name, size_t file_size);
char*            Buff_Maker              (size_t file_size);
size_t           Strings_Counter         (char* point_buff, size_t num_symbols);
char             Command_Compare         (char* string_temp);
char             Reg_Compare             (char* string_temp);
int              Processing_PushCommand  (char* point_buff, char* buff_binary, size_t* offset);
int              Processing_PopCommand   (char* point_buff, char* buff_binary, size_t* offset);
void             Translator              (const struct file_info buff_info, const char* file_name);

void             Reader_To_Code          (const char* file_name, struct CPU* pointer_cpu, size_t num_symbols);
void             Poison_Space            (double* begin, double* end);
void             Executor                (struct CPU* cpu, size_t num_symbols, const char* file_name);
