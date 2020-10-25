DEF_CMD (HLT,  0, 0,
                     {
                     cpu->pc = num_symbols;
                     })

DEF_CMD (IN,   1, 0,
                     {
                     double temp = 0;
                     scanf ("%lg", &temp);
                     StackPush (cpu->Stack, temp);
                     })

DEF_CMD (OUT,  2, 0,
                     {
                     double temp = StackPop (cpu->Stack);
                     printf ("%lg\n", temp);
                     })


DEF_CMD (PUSH, 3, 2,
                     {
                     int mode = cpu->code[cpu->pc + 1];

                     if (mode == PUSH_MODE_REG)
                     {
                        int reg_num = cpu->code[cpu->pc + 2];
                        assert (!isnan (cpu->registers[reg_num]));
                        StackPush (cpu->Stack, cpu->registers[reg_num]);
                        cpu->pc += 2 * sizeof (char);
                     }

                     else if (mode == PUSH_MODE_VAL)
                     {
                        double value = *((double*)(cpu->code + cpu->pc + 2));
                        StackPush (cpu->Stack, value);
                        cpu->pc += (sizeof (char) + sizeof (double));
                     }

                     else
                     {
                        EXECUTE_ERROR
                     }

                     })

DEF_CMD (POP,  4, 1,
                     {
                     int reg_num = cpu->code[cpu->pc + 1];
                     if ( reg_num < 1 || reg_num > NUM_REGISTERS)
                     {
                        EXECUTE_ERROR
                     }
                     cpu->registers[reg_num] = StackPop (cpu->Stack);
                     cpu->pc += sizeof (char);
                     })

DEF_CMD (ADD,  5, 0,
                    {
                    StackPush (cpu->Stack, StackPop (cpu->Stack) + StackPop (cpu->Stack));
                    })

DEF_CMD (MUL,  6, 0,
                    {
                    StackPush (cpu->Stack, StackPop (cpu->Stack) * StackPop (cpu->Stack));
                    })

DEF_CMD (SUB,  7, 0,
                    {
                    double temp = StackPop (cpu->Stack);
                    StackPush (cpu->Stack, StackPop (cpu->Stack) - temp);
                    })

DEF_CMD (DIV,  8, 0,
                    {
                    double temp_1 = StackPop (cpu->Stack);
                    double temp_2 = StackPop (cpu->Stack);

                    if (temp_1 == 0)
                    {
                        ARYTHMETIC_ERROR
                    }

                    StackPush (cpu->Stack, temp_2 / temp_1);

                    })

DEF_CMD (POW,  9, 0,
                    {
                    double temp = StackPop (cpu->Stack);
                    StackPush (cpu->Stack, pow (StackPop (cpu->Stack), temp));
                    })

DEF_CMD (SIN, 10, 0,
                    {
                    StackPush (cpu->Stack, sin (StackPop (cpu->Stack)));
                    })

DEF_CMD (COS, 11, 0,
                    {
                    StackPush (cpu->Stack, cos (StackPop (cpu->Stack)));
                    })


