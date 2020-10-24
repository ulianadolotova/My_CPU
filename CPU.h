#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys\stat.h>
#include "Stack.h"

#define SYNTAX_ERROR  printf ("In file %s: in line %d: Syntax Error: There is no such function as %s.",  \
                               file_name, i, point_buff + i);                                            \
                                                                                                         \
                      assert (!"OK");                                                                    \

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
const size_t MAX_STRING_SIZE     = 30;
const int    IMPOSSIBLE_COMMAND  = -1;
const size_t PUSH_MODE_REG       = 1;
const size_t PUSH_MODE_VAL       = 2;
const int    IMPOSSIBLE_POS      = -2;
const char   IMPOSSIBLE_REG      = -1;


struct file_info
{
    size_t num_symbols;
    char*  point_buff;
};

struct CPU
{
    struct Stack_t Stack;
    char*          code;

    double         rax;
    double         rbx;
    double         rcx;
    double         rdx;
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


