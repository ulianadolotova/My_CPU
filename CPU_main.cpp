#include "CPU.h"


struct arr_info File_Interactor (const char* file_name)
{
    assert (file_name);

    size_t file_size   = Size_File_Counter (file_name) + 2;

    char*  point_buff  = (char*) calloc (file_size, sizeof(char));


    assert (point_buff);


    size_t num_symbols = Buff_Filler (point_buff, file_name, file_size);

    size_t num_strings = Strings_Counter (point_buff, num_symbols);

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



//-------------------------------------------------------------------------
//! This function fills the buffer with text
//! from the file. Also it replaces '\n' with '\0'.
//!
//! @param [out] point_buff   - Pointer to the buffer
//! @param [out] file_name    - Name of the file
//! @param [in]  size_file    - Size of the file
//!
//! @return      num_symbols  - Number of the symbols in the file
//!
//! @note Function pushes '\0' to the first element and '\0' to the last.
//------------------------------------------------------------------------
size_t Buff_Filler (char* point_buff, const char* file_name, size_t file_size)
{

    assert (point_buff);
    assert (file_name);


    FILE* file_pointer = fopen(file_name, "r");


    *(point_buff) = '\0';

    size_t num_symbols = fread (point_buff + 1, sizeof(char), file_size, file_pointer);

    *(point_buff + 1 + num_symbols) =  '\0';


    fclose (file_pointer);


    return num_symbols;
}
