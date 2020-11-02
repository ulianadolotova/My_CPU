DEF_CMD (HLT, 0, 0, IMPOSSIBLE_POS,
                                    {
                                    cpu->pc = num_symbols;
                                    })

DEF_CMD (IN,   1, 0, IMPOSSIBLE_POS,
                                    {
                                    double temp = 0;
                                    scanf ("%lg", &temp);
                                    StackPush (cpu->Stack, temp);
                                    cpu->pc += sizeof (char);
                                    })

DEF_CMD (OUT,  2, 0, IMPOSSIBLE_POS,
                                    {
                                    double temp = StackPop (cpu->Stack);
                                    printf ("\n%lg\n", temp);
                                    cpu->pc += sizeof (char);
                                    })


DEF_CMD (PUSH, 3, 2, Processing_PUSHCommand (point_buff  + i + pos, buff_binary + *offset, offset),
                     {
                     int mode = cpu->code[cpu->pc + 1];

                     if (mode == PUSH_MODE_REG)
                     {
                        int reg_num = cpu->code[cpu->pc + 2];
                        assert (!isnan (cpu->registers[reg_num]));
                        StackPush (cpu->Stack, cpu->registers[reg_num]);
                        cpu->pc += 3 * sizeof (char);
                     }

                     else if (mode == PUSH_MODE_VAL)
                     {
                        double value = *((double*)(cpu->code + cpu->pc + 2));
                        StackPush (cpu->Stack, value);
                        cpu->pc += (2 * sizeof (char) + sizeof (double));
                     }

                     else
                     {
                        EXECUTE_ERROR
                     }

                     })

DEF_CMD (POP,  4, 1, Processing_POPCommand  (point_buff + i + pos, buff_binary + *offset, offset),
                     {
                     int reg_num = cpu->code[cpu->pc + 1];
                     if ( reg_num < 1 || reg_num > NUM_REGISTERS)
                     {
                        EXECUTE_ERROR
                     }
                     cpu->registers[reg_num] = StackPop (cpu->Stack);
                     cpu->pc += 2 * sizeof (char);
                     })

DEF_CMD (ADD,  5, 0, IMPOSSIBLE_POS,
                                    {
                                    StackPush (cpu->Stack, StackPop (cpu->Stack) + StackPop (cpu->Stack));
                                    cpu->pc += sizeof (char);
                                    })

DEF_CMD (MUL,  6, 0, IMPOSSIBLE_POS,
                                    {
                                    StackPush (cpu->Stack, StackPop (cpu->Stack) * StackPop (cpu->Stack));
                                    cpu->pc += sizeof (char);
                                    })

DEF_CMD (SUB,  7, 0, IMPOSSIBLE_POS,
                                    {
                                    double temp = StackPop (cpu->Stack);
                                    StackPush (cpu->Stack, StackPop (cpu->Stack) - temp);
                                    cpu->pc += sizeof (char);
                                    })

DEF_CMD (DIV,  8, 0, IMPOSSIBLE_POS,
                                    {
                                    double temp_1 = StackPop (cpu->Stack);
                                    double temp_2 = StackPop (cpu->Stack);

                                    if (temp_1 == 0)
                                    {
                                        ARYTHMETIC_ERROR
                                    }

                                    StackPush (cpu->Stack, temp_2 / temp_1);
                                    cpu->pc += sizeof (char);

                                    })

DEF_CMD (POW,  9, 0, IMPOSSIBLE_POS,
                                    {
                                    double temp = StackPop (cpu->Stack);
                                    StackPush (cpu->Stack, pow (StackPop (cpu->Stack), temp));
                                    cpu->pc += sizeof (char);
                                    })

DEF_CMD (SIN, 10, 0, IMPOSSIBLE_POS,
                                    {
                                    StackPush (cpu->Stack, sin (StackPop (cpu->Stack)));
                                    cpu->pc += sizeof (char);
                                    })

DEF_CMD (COS, 11, 0, IMPOSSIBLE_POS,
                                    {
                                    StackPush (cpu->Stack, cos (StackPop (cpu->Stack)));
                                    cpu->pc += sizeof (char);
                                    })

DEF_CMD (JMP, 12, 1, Processing_JMPCommand  (point_buff + i + pos, buff_binary + *offset, offset, arr_labels),
                    {
                    cpu->pc = *(int*)(cpu->code + cpu->pc + 1);
                    })

DEF_CMD (JE,  13, 1, Processing_JMPCommand  (point_buff + i + pos, buff_binary + *offset, offset, arr_labels),
                    {
                    double last  = StackPop (cpu->Stack);
                    double first = StackPop (cpu->Stack);

                    if (last == first)
                        cpu->pc = *(int*)(cpu->code + cpu->pc + 1);
                    else
                        cpu->pc += sizeof (char) + sizeof (int);
                    })

DEF_CMD (JNE, 14, 1, Processing_JMPCommand  (point_buff + i + pos, buff_binary + *offset, offset, arr_labels),
                    {
                    double last  = StackPop (cpu->Stack);
                    double first = StackPop (cpu->Stack);

                    if (last != first)
                        cpu->pc = *(int*)(cpu->code + cpu->pc + 1);
                    else
                        cpu->pc += sizeof (char) + sizeof (int);
                    })

DEF_CMD (JA,  15, 1, Processing_JMPCommand  (point_buff + i + pos, buff_binary + *offset, offset, arr_labels),
                    {
                    double last  = StackPop (cpu->Stack);
                    double first = StackPop (cpu->Stack);

                    if (first > last)
                        cpu->pc = *(int*)(cpu->code + cpu->pc + 1);
                    else
                        cpu->pc += sizeof (char) + sizeof (int);
                    })

DEF_CMD (JAE, 16, 1, Processing_JMPCommand  (point_buff + i + pos, buff_binary + *offset, offset, arr_labels),
                    {
                    double last  = StackPop (cpu->Stack);
                    double first = StackPop (cpu->Stack);

                    if (first >= last)
                        cpu->pc = *(int*)(cpu->code + cpu->pc + 1);
                    else
                        cpu->pc += sizeof (char) + sizeof (int);
                    })

DEF_CMD (JB,  17, 1, Processing_JMPCommand  (point_buff + i + pos, buff_binary + *offset, offset, arr_labels),
                    {
                    double last  = StackPop (cpu->Stack);
                    double first = StackPop (cpu->Stack);

                    if (first < last)
                        cpu->pc = *(int*)(cpu->code + cpu->pc + 1);
                    else
                        cpu->pc += sizeof (char) + sizeof (int);
                    })

DEF_CMD (JBE, 18, 1, Processing_JMPCommand  (point_buff + i + pos, buff_binary + *offset, offset, arr_labels),
                    {
                    double last  = StackPop (cpu->Stack);
                    double first = StackPop (cpu->Stack);

                    if (first <= last)
                        cpu->pc = *(int*)(cpu->code + cpu->pc + 1);
                    else
                        cpu->pc += sizeof (char) + sizeof (int);
                    })

DEF_CMD (CALL, 19, 1, Processing_JMPCommand  (point_buff + i + pos, buff_binary + *offset, offset, arr_labels),
                    {
                    StackPush (cpu->Stack_calls, cpu->pc + sizeof (char) + sizeof (int));
                    cpu->pc = cpu->code[cpu->pc + sizeof (char)];
                    })

DEF_CMD (RET, 20, 0, IMPOSSIBLE_POS,
                                    {
                                    cpu->pc = StackPop (cpu->Stack_calls);
                                    })

DEF_CMD (START, 21, 0, IMPOSSIBLE_POS,
                    {
                    cpu->pc = *(int*)(cpu->code + cpu->pc + 1);
                    })
