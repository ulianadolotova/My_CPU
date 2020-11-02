#include "CPU.h"

int main (int argc, const char* argv[])
{
    const char* file_name = argv[1];

    struct file_info* buff_info = File_Interactor (file_name);

    struct label* arr_labels = (label*) Buff_Maker (sizeof (label) * MAX_LABELS);
    assert (arr_labels);

    Labels_Preprocessing (arr_labels, MAX_LABELS);

    char* buff_binary = Buff_Maker (MAX_BUFF_SIZE);
    assert (buff_binary);

    size_t offset = Make_Signature (buff_binary);
    buff_binary[offset] = START_CMD;
    offset += sizeof(char) + sizeof(int);

    assert (offset > 0);

    error_t result = Translator (buff_binary, buff_info, arr_labels, file_name, FIRST,  offset);
    assert (result == NO_ERROR);

    result         = Translator (buff_binary, buff_info, arr_labels, file_name, SECOND, offset);
    assert (result == NO_ERROR);

    free (buff_binary);
    free (arr_labels);
    free (buff_info);

    return 0;
}



error_t Translator (char* buff_binary, struct file_info* buff_info, struct label* arr_labels, const char* file_name, const size_t mode, size_t offset)
{
    assert (file_name);
    assert (buff_binary);
    assert (buff_info);
    assert (arr_labels);

    size_t num_symbols = buff_info->num_symbols;
    char*  point_buff  = buff_info->point_buff;

    assert (point_buff);

    size_t i = 0;

    char string_temp[MAX_STRING_SIZE] = {};
    char num_command = IMPOSSIBLE_COMMAND;

    #define DEF_CMD(name, num, argc, proc_func, code)                                                               \
    if (strcmpi(string_temp, #name) == 0)                                                                           \
    {                                                                                                               \
        if (name##_CMD == START_CMD)                                                                                \
        {                                                                                                           \
            *(int*)(buff_binary + sizeof (FileHeader) + sizeof(char)) = offset;                                     \
        }                                                                                                           \
        else                                                                                                        \
        {                                                                                                           \
            CHECK_POS(pos)                                                                                          \
            buff_binary[offset] = (char) num;                                                                       \
            offset += sizeof (char);                                                                                \
                                                                                                                    \
            if (argc > 0)                                                                                           \
            {                                                                                                       \
                int pos_temp = Processing_Commands (num, buff_binary, point_buff, pos, i, &offset, arr_labels);     \
                CHECK_POS(pos)                                                                                      \
                pos += pos_temp;                                                                                    \
            }                                                                                                       \
        }                                                                                                           \
        i += pos;                                                                                                   \
    }                                                                                                               \
    else                                                                                                            \

    while (i < num_symbols)
    {
        int pos = IMPOSSIBLE_POS;
        sscanf (point_buff + i, "%s%n", string_temp, &pos);

        char* pointer = strchr (string_temp, ':');
        if (pointer)
        {
            if (mode == SECOND)
            {
                i += pos;
                continue;
            }
            Processing_Label (string_temp, pointer, arr_labels, offset);
            i += pos;
        }
        else
        #include "Commands.h"
        {
        /*else*/ SYNTAX_ERROR
        }
    }

    #undef DEF_CMD

    if (mode == SECOND)
    {
        Buff_To_File (buff_binary, offset);
        free (point_buff);
    }

    return (i == num_symbols) ? NO_ERROR : TRANSLATION_ERROR;
}



size_t Make_Signature (char* buff_binary)
{
    struct FileHeader my_sign = {"USD", 7};

    size_t size_signature = sizeof (my_sign);

    memcpy (buff_binary, &my_sign, size_signature);

    return size_signature;
}


char Reg_Compare (char* string_temp)
{
    assert (string_temp);

    #define DEF_REG(name, num)                         \
    if (strcmpi (string_temp, #name) == 0)             \
        return num;                                    \
    else

    #include "Regs.h"

    #undef DEF_REG

    /*else*/ return IMPOSSIBLE_REG;
}



int Processing_Commands (size_t num_c, char* buff_binary, char* point_buff, int pos, size_t i, size_t* offset, struct label* arr_labels)
{
    assert (buff_binary);
    assert (point_buff);

    int pos_temp = IMPOSSIBLE_POS;

    #define DEF_CMD(name, num, argc, proc_func, code)       \
    if (num_c == num)                                       \
    {                                                       \
        return proc_func;                                   \
    }                                                       \
    else                                                    \


    #include "Commands.h"
    /*else*/ return pos_temp;

    #undef DEF_CMD
}



int Processing_PUSHCommand (char* point_buff, char* buff_binary, size_t* offset)
{
    assert (point_buff);
    assert (buff_binary);
    assert (offset);

    //char mode = 0;
    int pos      = IMPOSSIBLE_POS;
    double value = 0;
    char string_temp[MAX_STRING_SIZE] = {};

    sscanf (point_buff, "%lg%n", &value, &pos);

    if (pos == IMPOSSIBLE_POS)
    {
        sscanf (point_buff, "%s%n", string_temp, &pos);
        //mode = ModeCheck (temp);
        *(buff_binary) = mode;
        *(offset) += sizeof (char);

        /*if ((mode & 4)/4)         // Simply skips first '['
            string_temp++;

        if (!(mode & 1) && ((mode & 2)/2) && ((mode & 4)/4))    // Deleting last ']' if it was
        {
            int len = strlen (string_temp);
            string_temp[len - 1] = '\0';
        }

        char* string_val_temp = strchr(string_temp, '+');       // If 2 args
        if (string_val_temp)
        {
            *string_val_temp = '\0';
            string_val_temp++;

            ArgMake (buff_binary,     string_temp, offset);
            ArgMake (buff_binary, string_val_temp, offset);
        }
        else
        {
            ArgMake (buff_binary,     string_temp, offset);
        }

        */
        char reg_name = Reg_Compare (string_temp);

        if (reg_name == IMPOSSIBLE_REG)
            return      IMPOSSIBLE_POS;
        else
        {
            *(buff_binary + sizeof (char)) = reg_name;
            *(offset) += sizeof (char);
            return pos;
        }

    }
    else
    { //      imid   reg   mem
        //mode = 1*1 + 0*2 + 0*4;
        *(buff_binary) = mode;
        *(offset) += sizeof (char);

        //printf("\n%lg\n", value);
        *((double*) (buff_binary + sizeof (char))) = value;
        *(offset) += sizeof (double);
        return pos;

    }
    //return pos;
}
/*
void ArgMake (char* temp, size_t* offset)
{
    if (strpbrk(temp, "1234567890"))
    {
        *((double*) (buff_binary + sizeof (char))) = atof(temp);
        *(offset) += sizeof (double);
    }
    else
    {
        char reg_name = Reg_Compare (string_temp);

        if (reg_name == IMPOSSIBLE_REG)
        {
            printf("IMPOSSIBLE_REG");
            assert (!"OK");
        }
        else
        {
            *(buff_binary + sizeof (char)) = reg_name;
            *(offset) += sizeof (char);
            return pos;
        }
    }
}


char ModeCheck (char* temp)
{
    int imid = 0;
    int reg = 0;
    int mem = 0;

    if (temp[0] == '[')
        mem = 1;
    if (strpbrk(temp, "+"))
    {
        imid = 1;
        reg = 1;
    }
    else if (strpbrk(temp, "1234567890"))
    {
        imid = 1;
    }
    else
    {
        reg = 1;
    }

    char mode = imid*1 + reg*2 + mem*4;
    return mode;
}
*/


int Processing_POPCommand (char* point_buff, char* buff_binary, size_t* offset)
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
        *(offset) += sizeof (char);
        return pos;
    }
}



int Processing_JMPCommand (char* point_buff, char* buff_binary, size_t* offset, struct label* arr_labels)
{
    assert (point_buff);
    assert (buff_binary);
    assert (offset);

    int pos   = IMPOSSIBLE_POS;
    char string_temp[MAX_STRING_SIZE] = {};

    sscanf (point_buff, "%s%n", string_temp, &pos);

    if (string_temp[0] != ':')
    {
        printf ("\nNot correct label: %s\n", string_temp);
        assert (!"OK");
    }

    if (strspn(string_temp + 1, "1234567890") == strlen(string_temp + 1))
    {
        int value = atoi (string_temp + 1);

        if (value <= 0)
        {
            printf ("\nNot correct label: %s\n", string_temp);
            assert (!"OK");
        }

        *((int*) buff_binary) = arr_labels[value].number;
        *(offset) += sizeof (int);
    }
    else
    {
        size_t index = Find_Label (arr_labels, string_temp + 1);

        if (index < MAX_LABELS)
        {
            *((int*) buff_binary) = arr_labels[index].number;
        }
        else
            *((int*) buff_binary) = -1;

        *(offset) += sizeof (int);
    }
    return pos;
}



void Processing_Label (char* string_temp, char* pointer, struct label* arr_labels, size_t offset)
{
    assert (string_temp);
    assert (pointer);

    static size_t free_label = 1;

    int value = 0;
    *pointer  = '\0';

    if (strspn(string_temp, "1234567890") == strlen(string_temp))
    {
        value = atoi (string_temp);

        if (value <= 0)
        {
            printf ("\nNot correct label: %s\n", string_temp);
            assert (!"OK");
        }
        if (value < free_label)
        {
            assert (free_label < MAX_LABELS);
            strcpy (arr_labels[free_label].name, arr_labels[value].name);
            arr_labels[free_label].number = arr_labels[value].number;
            strcpy (arr_labels[value].name, "");

            while (arr_labels[free_label].number != -1)
                free_label++;
        }
        arr_labels[value].number = offset;
    }
    else
    {
        assert(strcpy (arr_labels[free_label].name, string_temp));
        arr_labels[free_label].number = offset;

        while (arr_labels[free_label].number != -1)
            free_label++;
    }
}


void Labels_Preprocessing (struct label* arr_labels, size_t size_arr)
{
    for (size_t i = 0; i < size_arr; i++)
    {
        arr_labels[i].number = -1;
    }

}



size_t Find_Label (struct label* arr_labels, char* string_temp)
{
    for (size_t i = 1; i < MAX_LABELS; i++)
    {
        if (strcmp(arr_labels[i].name, string_temp) == 0)
        {
            return i;
        }
    }

    return MAX_LABELS + 1;
}



void Buff_To_File (char* buff_binary, size_t offset)
{
    assert (buff_binary);

    FILE* Output = fopen ("Code_format.bin", "wb+");

    assert (Output);

    fwrite (buff_binary, offset, 1, Output);
    fclose (Output);
}



struct file_info* File_Interactor (const char* file_name)
{
    assert (file_name);

    size_t file_size   = Size_File_Counter (file_name) + 2;
    char*  point_buff  = Buff_Maker (file_size);

    assert (point_buff);

    size_t num_symbols = Buff_Filler (point_buff, file_name, file_size);

    assert (num_symbols > 0);

    struct file_info* buff_info = (file_info*) calloc (1, sizeof (buff_info));
    *(buff_info) = { num_symbols, point_buff };

    return buff_info;
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



size_t Buff_Filler (char* point_buff, const char* file_name, size_t file_size)
{

    assert (point_buff);
    assert (file_name);

    FILE* file_pointer = fopen(file_name, "r");

    size_t num_symbols = fread (point_buff, sizeof (char), file_size, file_pointer);

    fclose (file_pointer);

    return num_symbols;
}



char* Buff_Maker (size_t file_size)
{
    char*  point_buff  = (char*) calloc (file_size, sizeof (char));
    return point_buff;
}

