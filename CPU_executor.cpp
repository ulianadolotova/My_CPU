#include "CPU.h"

int main ()
{

    CPU* cpu = (CPU*) calloc (1, sizeof (CPU));

    assert (cpu);

    cpu->code      = Buff_Maker (MAX_BUFF_SIZE);
    cpu->registers = (double*) Buff_Maker ((NUM_REGISTERS + 1) * sizeof (double));
    cpu->pc        = 0;

    Poison_Space (cpu->registers, cpu->registers + NUM_REGISTERS + 1);

    const char* file_name = "Code_format.bin";

    size_t num_symbols = Size_File_Counter (file_name);

    Reader_To_Code (file_name, cpu, num_symbols);
    Executor       (cpu, num_symbols, file_name);

    return 0;
}




void Reader_To_Code (const char* file_name, struct CPU* cpu, size_t num_symbols)
{
    FILE* Code = fopen (file_name, "rb");
    fread (cpu->code, 1, num_symbols, Code);
    fclose (Code);
}


void Executor (struct CPU* cpu, size_t num_symbols, const char* file_name)
{
    cpu->Stack = StackConstruct (num_symbols);
    #define DEF_CMD(name, num, argc, code)  \
    case name##_CMD: code break;            \

    while (cpu->pc < num_symbols)
    {
        Stack_Dump(cpu->Stack);
        switch ((int) cpu->code[cpu->pc])
        {
        #include "Commands.h"
        }
        cpu->pc++;

    }

    #undef DEF_CMD

    StackDestruct (cpu->Stack);
}


//-----------------------------------------------------------------------------------
//! This functions puts NaN in the elements of array at half-interval [begin, end)
//!
//! @param [out] begin Pointer to the begin
//! @param [out] end   Pointer to the end
//------------------------------------------------------------------------------------
void Poison_Space (double* begin, double* end)
{
    assert (begin);
    assert (end);

    size_t lenth = end - begin;

    for (size_t i = 0; i < lenth; i++)
            *(begin + i) = NAN;
}




//-------------------------------------------------------------------------------
//! This function creates buffer for the text from the file.
//!
//! @param [in] file_size - Size of the file.
//!
//! @return Function returns pointer to the buff.
//-------------------------------------------------------------------------------
char* Buff_Maker (size_t file_size)
{
    char*  point_buff  = (char*) calloc (file_size, sizeof (char));
    return point_buff;
}




//----------------------------------------------------------------------
//! This function looks for the size of the file.
//!
//! @param [out] file_name - Name of the file
//!
//! @return file_size      - Size of the file
//----------------------------------------------------------------------
size_t Size_File_Counter (const char* file_name)
{
    assert (file_name);

    size_t file_size = 0;
    struct stat buff;

    stat (file_name, &buff);

    file_size = buff.st_size;

    return file_size;
}
