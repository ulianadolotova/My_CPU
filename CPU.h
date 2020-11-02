#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys\stat.h>
#include "Stack\Stack.h"


#define CHECK_POS(pos) if (pos == IMPOSSIBLE_POS  || pos > MAX_STRING_SIZE)                                        \
                       {                                                                                           \
                            SYNTAX_ERROR;                                                                          \
                       }

#define SYNTAX_ERROR  printf ("\nIn file %s: symbol %d: Syntax Error: There is no such function as %s.\n",         \
                               file_name, i, point_buff + i);                                                      \
                                                                                                                   \
                      assert (!"OK");                                                                              \


#define EXECUTE_ERROR  printf ("\nIn file %s: Execute Error: Code is not correct. PC is %d\n",                     \
                               file_name, cpu->pc);                                                                \
                                                                                                                   \
                       assert (!"OK");                                                                             \


#define ARYTHMETIC_ERROR  printf ("\nIn file %s: Arythmetic Error: You cannot divide by zero. PC is %d\n",         \
                               file_name, cpu->pc);                                                                \
                                                                                                                   \
                       assert (!"OK");


#define DEF_CMD(name, num, argc, proc_func, code)                                                                  \
                name##_CMD = num,
enum commands
{
    #include "Commands.h"
};

#undef DEF_CMD


enum errors_cpu
{
    NO_ERROR            = 0,
    TRANSLATION_ERROR   = 1,
    EXECUTION_ERROR     = 2,
    READING_ERROR       = 3,
};


#define DEF_REG(name, num)                                                                                           \
                name = num,
enum reg_names
{
    #include "Regs.h"
};

#undef DEF_REG

const size_t MAX_STRING_SIZE      = 30;
const char   IMPOSSIBLE_COMMAND   = -1;
const char   PUSH_MODE_REG        = 1;
const char   PUSH_MODE_VAL        = 2;
const char   IMPOSSIBLE_POS       = -2;
const char   IMPOSSIBLE_REG       = -1;
const size_t MAX_BUFF_SIZE        = 10000;
const size_t MAX_LABELS           = 100;
const size_t MAX_NAME             = 20;
const size_t FIRST                = 1;
const size_t SECOND               = 2;
const size_t MAX_CURRENT_COMMANDS = 20;
const size_t NUM_REGISTERS        = 4;



struct file_info
{
    size_t num_symbols;
    char*  point_buff;
};

struct CPU
{
    struct Stack_t* Stack;
    struct Stack_t* Stack_calls;
    char*           code;
    size_t          pc;

    double*         registers;
};


struct label
{
    char name[MAX_NAME];
    int number;
};

struct FileHeader
{
    char   signature[4];
    int    version;
};

struct file_info* File_Interactor         (const char* file_name);
size_t            Size_File_Counter       (const char* file_name);
size_t            Buff_Filler             (char* point_buff, const char* file_name, size_t file_size);
char*             Buff_Maker              (size_t file_size);
size_t            Strings_Counter         (char* point_buff, size_t num_symbols);

error_t           Translator              (char* buff_binary, struct file_info* buff_info, struct label* arr_labels, const char* file_name, const size_t mode, size_t offset);
char              Reg_Compare             (char* string_temp);
int               Processing_Commands     (size_t num, char* buff_binary, char* point_buff, int pos, size_t i, size_t* offset, struct label* arr_labels);
int               Processing_PUSHCommand  (char* point_buff, char* buff_binary, size_t* offset);
char              ModeCheck               (char* temp)
int               Processing_POPCommand   (char* point_buff, char* buff_binary, size_t* offset);
int               Processing_JMPCommand   (char* point_buff, char* buff_binary, size_t* offset, struct label* arr_labels);
void              Labels_Preprocessing    (struct label* arr_labels, size_t size_arr);
void              Processing_Label        (char* string_temp, char* pointer, struct label* arr_labels, size_t offset);
size_t            Find_Label              (struct label* arr_labels, char* string_temp);
void              Buff_To_File            (char* buff_binary, size_t offset);
size_t            Make_Signature          (char* buff_binary);

error_t           Reader_To_Buff          (const char* file_name, size_t num_symbols, char* buff);
error_t           Executor                (struct CPU* cpu, size_t num_symbols, const char* file_name);
void              Poison_Space            (double* begin, double* end);
int               Check_Signature         (char* pointer, size_t current_version);
