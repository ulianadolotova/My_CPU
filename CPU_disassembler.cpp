#include "CPU.h"

int main ()
{
    const char* file_name = "Code_format.bin";

    size_t num_symbols = Size_File_Counter (file_name);

    char* buff_commands = Buff_Maker (MAX_BUFF_SIZE);

    error_t result_reading = Reader_To_Buff (file_name, cpu, num_symbols, buff_commands);
    assert (result_reading == NO_ERROR);

    error_t resulr_translate = error_t Translate_To_Text (
}
