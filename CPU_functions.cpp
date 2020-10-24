#include "CPU.h"

int main ()
{
    Translator(File_Interactor("Input.txt"));

    return 0;
}

void Translator (const struct file_info buff_info)
{
    const char* arr_functions[NUM_FUNCTIONS] = {"hlt", "in", "out", "push", "add", "mul", "sub",
                                                "seg", "pow", "sin", "cos"};

    FILE* Output = fopen ("Code_format.txt", "a");

    size_t num_symbols = buff_info.num_symbols;
    char* point_buff   = buff_info.point_buff;

    for (size_t i = 0; i < num_symbols + 1; i++)
    {
        if (isdigit(*(point_buff + i)))
        {
            fprintf (Output, " %d", *(point_buff + i));
        }

        else if (*(point_buff + i) == '\0' && !isdigit(*(point_buff + i + 1)))
        {
            for (size_t func_num = 0; func_num < NUM_FUNCTIONS; func_num++)
            {
                printf("length = %d", strlen(point_buff + i + 1));
                if (strcmp(arr_functions[func_num], point_buff + i + 1) == 0)
                {
                    fprintf (Output, "\n%u", func_num);
                    printf  ("\n%u", func_num);
                    break;
                }

                if (func_num == NUM_FUNCTIONS - 1)
                {
                    printf ("You have put incorrect function. Please, check your program.");
                }
            }
        }
    }

}



//------------------------------------------------------------------------------
//! This function puts the text from the file to the buff.
//!
//! @param [out] file_name - Name of the file.
//!
//! @return Function returns structure file_info, which contains pointer to the
//!         buff with text and number of symbols.
//-------------------------------------------------------------------------------
struct file_info File_Interactor (const char* file_name)
{
    assert (file_name);

    size_t file_size   = Size_File_Counter (file_name) + 2;
    char*  point_buff  = Buff_Maker (file_size);

    assert (point_buff);

    size_t num_symbols = Buff_Filler (point_buff, file_name, file_size);

    assert (num_symbols > 0);

    size_t num_strings = Strings_Counter (point_buff, num_symbols);

    assert (num_strings > 0);

    struct file_info buff_info = { num_symbols, point_buff };

    return buff_info;
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



//--------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------
size_t Buff_Filler (char* point_buff, const char* file_name, size_t file_size)
{

    assert (point_buff);
    assert (file_name);

    FILE* file_pointer = fopen(file_name, "r");

    *(point_buff) = '\0';

    size_t num_symbols = fread (point_buff + 1, sizeof (char), file_size, file_pointer);

    *(point_buff + 1 + num_symbols) =  '\0';

    fclose (file_pointer);

    return num_symbols + 2;
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



//-------------------------------------------------------------------------------
//! This function counts '\n' and replaces them with '\0'.
//!
//! @param [out] point_buff  - Pointer to the buff with the text
//! @param [in]  num_symbols - Number of symbols in the text
//!
//! @return      num_strings - Number of strings (with empty strings)
//-------------------------------------------------------------------------------
size_t Strings_Counter (char* point_buff, size_t num_symbols)
{
    assert (point_buff != NULL);

    size_t num_strings = 1;

    for (char* pointer = point_buff; pointer < point_buff + num_symbols; pointer++)
        if (*pointer == '\n')
        {
            num_strings++;
            *pointer = '\0';
        }

    return num_strings;
}

