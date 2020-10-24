#include "CPU.h"

int main ()
{
    #ifdef IN_CMD
        printf("\n\nall is ok\n\n");
    #endif

    const char* file_name = "Input.txt";
    Translator  (File_Interactor (file_name), file_name);

    return 0;
}



//----------------------------------------------------------------------------------------------
//! This function translates text from the buff into the commands_language
//!                                             and puts it to the bin file.
//!
//! @param [in]  buff_info - Information about buff
//! @param [out] file_name - Name of the file, from where was text copied
//-----------------------------------------------------------------------------------------------
void Translator (const struct file_info buff_info, const char* file_name)
{
    FILE* Output = fopen ("Code_format.txt", "w+");

    assert (Output);

    size_t num_symbols = buff_info.num_symbols;
    char*  point_buff  = buff_info.point_buff;

    assert (point_buff);

    size_t i = 1;

    while (i < num_symbols)
    {
        char string_temp[MAX_STRING_SIZE] = {};

        int pos = IMPOSSIBLE_POS ;

        sscanf (point_buff + i, "%s%n", string_temp, &pos);

        if (pos == IMPOSSIBLE_POS  || pos > MAX_STRING_SIZE)
        {
            SYNTAX_ERROR
        }

        char num_command = Command_Compare (string_temp);

        if (num_command == IMPOSSIBLE_COMMAND)
        {
            SYNTAX_ERROR
        }

        fprintf (Output, "%hd", num_command);

        if (num_command == PUSH_CMD)
        {
            int pos_temp = Processing_PushCommand (point_buff  + i + (pos + 1), Output);

            if (pos_temp == IMPOSSIBLE_POS)
            {
                SYNTAX_ERROR
            }

            pos += pos_temp;
        }

        if (num_command == POP_CMD)
        {
            int pos_temp = Processing_PopCommand (point_buff, Output);

            if (pos_temp == IMPOSSIBLE_POS)
            {
                SYNTAX_ERROR
            }

            pos += pos_temp;
        }

        i += pos + 1;
    }

    fclose (Output);
}



//------------------------------------------------------------------------------
//! This function puts the text from the file to the buff.
//!
//! @param [out] file_name - Name of the file.
//!
//! @return Function returns structure file_info, which contains pointer to the
//!         buff with text and number of symbols and number of strings.
//-------------------------------------------------------------------------------
struct file_info File_Interactor (const char* file_name)
{
    assert (file_name);

    size_t file_size   = Size_File_Counter (file_name) + 2;
    char*  point_buff  = Buff_Maker (file_size);

    assert (point_buff);

    size_t num_symbols = Buff_Filler (point_buff, file_name, file_size);

    assert (num_symbols > 0);

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



//----------------------------------------------------------------------------------
//! This function looks for the right command in array of possible commands.
//!
//! @param [out] string_temp - Array, which contains name of the command
//!
//! @return Function returns number of the command or IMPOSSSIBLE_command
//!         if command was not found in array.
//-----------------------------------------------------------------------------------
char Command_Compare (char* string_temp)
{
    const char* arr_commands[NUM_COMMANDS] = {"hlt", "in", "out", "push", "pop", "add", "mul", "sub",
                                                "seg", "pow", "sin", "cos"};


    for (char i = 0; i < NUM_COMMANDS; i++)
    {
        assert (i >= 0);

        if (strcmp (arr_commands[i], string_temp) == 0)
            return i;
    }

    return IMPOSSIBLE_COMMAND;
}



//----------------------------------------------------------------------------------
//! This function looks for the right register in array of possible registers.
//!
//! @param [out] string_temp - Array, which contains name of the register
//!
//! @return Function returns number of the register or IMPOSSSIBLE_REG
//!         if register was not found in array.
//-----------------------------------------------------------------------------------
char Reg_Compare (char* string_temp)
{
    if (strcmp (string_temp, "rax") == 0)
        return RAX;

    if (strcmp (string_temp, "rbx") == 0)
        return RBX;

    if (strcmp (string_temp, "rcx") == 0)
        return RCX;

    if (strcmp (string_temp, "rdx") == 0)
        return RDX;

    return IMPOSSIBLE_REG;
}



//------------------------------------------------------------------------------------
//! This function reads symbols after PUSH command. If the symbols are correct,
//! it translate them and puts them into the file.
//!
//! @param [out] point_buff - Array with the text
//! @param [out] Output     - File, where to put symbols
//!
//! @return Function returns offset in the buff or IMPOSSIBLE_POS,
//!         if the symbols were incorrect.
//--------------------------------------------------------------------------------------
int Processing_PushCommand (char* point_buff, FILE* Output)
{
    int pos      = IMPOSSIBLE_POS;
    double value = 0;

    char string_temp[MAX_STRING_SIZE] = {};

    sscanf (point_buff, "%lg%n", &value, &pos);

    if (pos == IMPOSSIBLE_POS)
    {
        sscanf (point_buff, "%s%n", string_temp, &pos);

        char reg_name = Reg_Compare (string_temp);

        if (reg_name == IMPOSSIBLE_REG)

            return IMPOSSIBLE_POS;

        else
        {
            fprintf (Output, "%hd", PUSH_MODE_REG);
            fprintf (Output, "%hd", reg_name);
            return pos + 1;
        }
    }

    else
    {
        fprintf (Output, "%hd",  PUSH_MODE_VAL);
        fprintf (Output, "%lg", value);
        return pos + 1;
    }
}



//------------------------------------------------------------------------------------
//! This function reads symbols after POP command. If the symbols are correct,
//! it translate them and puts them into the file.
//!
//! @param [out] point_buff - Array with the text
//! @param [out] Output     - File, where to put symbols
//!
//! @return Function returns offset in the buff or IMPOSSIBLE_POS,
//!         if the symbols were incorrect.
//--------------------------------------------------------------------------------------
int Processing_PopCommand (char* point_buff, FILE* Output)
{
    int pos = IMPOSSIBLE_POS;

    char string_temp[MAX_STRING_SIZE] = {};

    sscanf (point_buff, "%s%n", string_temp, &pos);

    char reg_name = Reg_Compare (string_temp);

    if (reg_name == IMPOSSIBLE_REG)

        return IMPOSSIBLE_POS;

    else
    {
        fprintf (Output, "%hd", reg_name);
        return pos + 1;
    }

}
