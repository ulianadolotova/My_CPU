#include "CPU.h"

int main ()
{
    struct CPU My_CPU
    {
        StackConstruct(STACK_BEGIN_SIZE);
        NULL;

        NAN;
        NAN;
        NAN;
        NAN;
    };

    CPU* pointer_cpu = &My_CPU;

    assert (pointer_cpu);

    const char* file_name = "Code_format.bin";

    Make_Signature (pointer_cpu);
    Reader_To_Code (file_name, pointer_cpu);
    Executor       (pointer_cpu);

    return 0;
}



//
void Reader_To_Code (const char* file_name, struct CPU* pointer_cpu)
{
    size_t file_size = Size_File_Counter (file_name);

    pointer_cpu->code = Buff_Maker (file_size);

    Make_Signature (pointer_cpu->code);

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



//---------------------------------------------------------------------------------
void Make_Signature (char* pointer)
{



}





