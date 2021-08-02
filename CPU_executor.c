#include "CPU.h"

int main ()
{
    // Вынести в отдельную функцию - CPU* cpu = CPU_Construct(...)
    CPU* cpu = (CPU*) calloc (1, sizeof (CPU));

    assert (cpu);

    cpu->code      = Buff_Maker (MAX_BUFF_SIZE);
    cpu->registers = (double*) Buff_Maker ((NUM_REGISTERS + 1) * sizeof (double));
    cpu->pc        = 0;
    cpu->RAM       = (double*) Buff_Maker ((RAM_SIZE) * sizeof (double));
    for (int i = 0; i < RAM_SIZE; i++)
    {
    cpu->RAM[i] = i;
    }

    //------------------------------------------------------------
    Poison_Space (cpu->registers, cpu->registers + NUM_REGISTERS);

    const char* file_name = "Code_format.bin";

    size_t num_symbols = Size_File_Counter (file_name);

    error_t result_reading = Reader_To_Buff (file_name, num_symbols, cpu->code);
    assert (result_reading == NO_ERROR);

    size_t current_version = 7;
    cpu->pc = Check_Signature (cpu->code, current_version);

    assert (cpu->pc > 0);

    error_t result_execute = Executor (cpu, num_symbols, file_name);
    assert (result_execute == NO_ERROR);

    return 0;
}




error_t Reader_To_Buff (const char* file_name, size_t num_symbols, char* buff)
{
    FILE* Code = fopen (file_name, "rb");

    int amount = fread (buff, 1, num_symbols, Code);

    fclose (Code);

    return (amount > 0) ? NO_ERROR : READING_ERROR;
}


error_t Executor (struct CPU* cpu, size_t num_symbols, const char* file_name)
{
    cpu->Stack       = StackConstruct (num_symbols);
    cpu->Stack_calls = StackConstruct (MAX_CURRENT_COMMANDS);

    #define DEF_CMD(name, num, argc, proc_func, code)  \
    case name##_CMD: code break;                       \

    while (cpu->pc < num_symbols)
    {
        switch ((int) cpu->code[cpu->pc])
        {
        #include "Commands.h"
        }

    }

    #undef DEF_CMD

    StackDestruct (cpu->Stack);

    return (cpu->pc == num_symbols) ? NO_ERROR : EXECUTION_ERROR;
}


int Check_Signature (char* pointer, size_t current_version)
{
    if (*((int*)(pointer + 4)) != current_version)
    {
        printf ("%d\n", *((int*)(pointer + 4)));
        printf ("\nNot correct version. Current version is %d.\n", current_version);
        assert (!"OK");
    }

    return sizeof (FileHeader);
}


void Poison_Space (double* begin, double* end)
{
    assert (begin);
    assert (end);

    size_t lenth = end - begin;

    for (size_t i = 0; i < lenth; i++)
        *(begin + i) = NAN;
}



char* Buff_Maker (size_t file_size)
{
    char*  point_buff  = (char*) calloc (file_size, sizeof (char));
    return point_buff;
}



size_t Size_File_Counter (const char* file_name)
{
    assert (file_name);

    size_t file_size = 0;
    struct stat buff;

    stat (file_name, &buff);

    file_size = buff.st_size;

    return file_size;
}
