#include "CPU.h"

int main ()
{
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
    size_t num_symbols = buff_info.num_symbols;
    char*  point_buff  = buff_info.point_buff;

    assert (point_buff);

    size_t i = 0;

    char string_temp[MAX_STRING_SIZE] = {};
    char num_command  = IMPOSSIBLE_COMMAND;

    char*  buff_binary = Buff_Maker (MAX_BUFF_SIZE);

    size_t offset = 0;

    #define DEF_CMD(name, num, argc, code)                                                                   \
    if (strcmp(string_temp, #name) == 0)                                                                     \
    {                                                                                                        \
        if (pos == IMPOSSIBLE_POS  || pos > MAX_STRING_SIZE)                                                 \
        {                                                                                                    \
            SYNTAX_ERROR                                                                                     \
        }                                                                                                    \
                                                                                                             \
        buff_binary[offset] = (char) num;                                                                    \
        offset += sizeof (char);                                                                             \
                                                                                                             \
        if (num == PUSH_CMD)                                                                                 \
        {                                                                                                    \
            int pos_temp = Processing_PushCommand (point_buff  + i + pos, buff_binary + offset, &offset);    \
                                                                                                             \
            if (pos_temp == IMPOSSIBLE_POS)                                                                  \
            {                                                                                                \
                SYNTAX_ERROR                                                                                 \
            }                                                                                                \
                                                                                                             \
            pos += pos_temp;                                                                                 \
        }                                                                                                    \
                                                                                                             \
        if (num == POP_CMD)                                                                                  \
        {                                                                                                    \
            int pos_temp = Processing_PopCommand (point_buff + i + pos, buff_binary + offset, &offset);      \
                                                                                                             \
            if (pos_temp == IMPOSSIBLE_POS)                                                                  \
            {                                                                                                \
                SYNTAX_ERROR                                                                                 \
            }                                                                                                \
                                                                                                             \
            pos += pos_temp;                                                                                 \
        }                                                                                                    \
                                                                                                             \
        i += pos;                                                                                            \
    }                                                                                                        \
    else                                                                                                     \

    while (i < num_symbols)
    {

        int pos = IMPOSSIBLE_POS ;

        sscanf (point_buff + i, "%s%n", string_temp, &pos);

        #include "Commands.h"
        {
        /*else*/ SYNTAX_ERROR
        }

    }

    #undef DEF_CMD

    FILE* Output = fopen ("Code_format.bin", "wb+");

    assert (Output);

    fwrite (buff_binary, offset, 1, Output);

    fclose (Output);

    free   (buff_binary);
    free   (point_buff);
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
    if (strcmp (string_temp, "RAX") == 0)
        return RAX;

    else if (strcmp (string_temp, "RBX") == 0)
        return RBX;

    else if (strcmp (string_temp, "RCX") == 0)
        return RCX;

    else if (strcmp (string_temp, "RDX") == 0)
        return RDX;

    return IMPOSSIBLE_REG;
}



int Processing_PushCommand (char* point_buff, char* buff_binary, size_t* offset)
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
            *(buff_binary) = PUSH_MODE_REG;
            //printf ("%d\n", PUSH_MODE_REG);
            *(offset) += sizeof (char);

            *(buff_binary + sizeof (char)) = reg_name;
            //printf ("%d\n", reg_name);
            *(offset) += sizeof (char);
            return pos;
        }
    }

    else
    {
        *(buff_binary) = PUSH_MODE_VAL;
        //printf ("%d\n", PUSH_MODE_VAL);
        *(offset) += sizeof (char);

        *((double*) (buff_binary + sizeof (char))) = value;
        //printf ("%lg\n", value);
        *(offset) += sizeof (double);

        return pos;
    }
}



int Processing_PopCommand (char* point_buff, char* buff_binary, size_t* offset)
{
    int pos = IMPOSSIBLE_POS;

    char string_temp[MAX_STRING_SIZE] = {};

    sscanf (point_buff, "%s%n", string_temp, &pos);

    char reg_name = Reg_Compare (string_temp);

    if (reg_name == IMPOSSIBLE_REG)

        return IMPOSSIBLE_POS;

    else
    {
        *(buff_binary) = reg_name;
        //printf ("%d\n", reg_name);
        *(offset) += sizeof (char);
        return pos;
    }

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
//---------------------------------------------------------------------------------
size_t Buff_Filler (char* point_buff, const char* file_name, size_t file_size)
{

    assert (point_buff);
    assert (file_name);

    FILE* file_pointer = fopen(file_name, "r");

    size_t num_symbols = fread (point_buff, sizeof (char), file_size, file_pointer);

    fclose (file_pointer);

    return num_symbols;
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
