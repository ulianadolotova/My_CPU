DEF_CMD (HLT,  0, 0,
                     {

                     })

DEF_CMD (IN,   1, 0,
                     {

                     })

DEF_CMD (OUT,  2, 0,
                     {

                     })


DEF_CMD (PUSH, 3, 2,
                     {
                     StackPush  (&Stack, code[pc + 1]);
                     })

DEF_CMD (POP,  4, 1,
                     {
                     StackPop  (&Stack, code[pc + 1]);
                     })

DEF_CMD (ADD,  5, 0,
                    {

                    })

DEF_CMD (MUL,  6, 0,
                    {

                    })

DEF_CMD (SUB,  7, 0,
                    {

                    })

DEF_CMD (DIV,  8, 0,
                    {

                    })

DEF_CMD (POW,  9, 0,
                    {

                    })

DEF_CMD (SIN, 10, 0,
                    {

                    })

DEF_CMD (COS, 11, 0,
                    {

                    })


