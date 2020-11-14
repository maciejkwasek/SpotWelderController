/*
 * MIT License
 *
 * Copyright (c) 2020 maciejkwasek
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <Arduino.h>
#include <stdint.h>

#include "Keys.h"

/*------------------------------------------------------------------------------------------
 * TYPES
 *------------------------------------------------------------------------------------------*/
typedef struct KeyState_s
{
    uint8_t pin;
    uint8_t cnt;
    uint8_t flags;
} KeyState_t;

/*------------------------------------------------------------------------------------------
 * CONSTANTS
 *------------------------------------------------------------------------------------------*/
static const uint8_t holdTime=50;
static const uint8_t repeatTime=5;
static const uint8_t shortTime=20;

/*------------------------------------------------------------------------------------------
 * VARIABLES
 *------------------------------------------------------------------------------------------*/
static KeyState_t keys[KEYS_NUMKEYS] =
{
    {KEYS_PIN_SET, 0, 0},
    {KEYS_PIN_TRIG, 0, 0},
};

/*------------------------------------------------------------------------------------------
 * STATIC FUNCTIONS
 *------------------------------------------------------------------------------------------*/

/*
 * 
 * 
 */
static void hwInit()
{
    uint8_t i;
    for(i=0; i<KEYS_NUMKEYS; i++)
    {
        KeyState_t *k = &keys[i];
        pinMode(k->pin, INPUT); 
        digitalWrite(k->pin, OUTPUT);
    }
}

/*
 * 
 * 
 */
static void keyProcess(uint8_t key)
{
    if(key < KEYS_NUMKEYS)
    {
        uint8_t tmp;
        KeyState_t *k = &keys[key];

        if(digitalRead(k->pin) == HIGH)
        {
            tmp = k->cnt;
            if(tmp>1)
            {
                if(tmp<shortTime)
                {
                    k->flags |= KEYS_EVENT_SHORTUP | KEYS_EVENT_UP;
                }
                else
                {
                    k->flags |= KEYS_EVENT_UP;
                }
            }

            k->cnt=0;
        }
        else
        {
            tmp = k->cnt;
            if(tmp>0)
            {
                if(tmp==1)
                {
                    k->flags |= KEYS_EVENT_DOWN;
                }
                else
                {
                    if(tmp==holdTime)
                    {
                        k->flags |= KEYS_EVENT_HOLD;
                    }
                    else
                    {
                        if(tmp>=(holdTime+repeatTime))
                        {
                            k->flags |= KEYS_EVENT_REPEATED;
                            k->cnt = holdTime;
                        }
                    }
                }
            }

            k->cnt++;
        }
    }
}

/*------------------------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 *------------------------------------------------------------------------------------------*/

/*
 * 
 * 
 */
void Keys_Init()
{
    hwInit();	
}

/*
 * 
 * 
 */
void Keys_Handler()
{
    uint8_t i;
    for(i=0; i<KEYS_NUMKEYS; i++)
    {
        keyProcess(i);
    }
}

/*
 * 
 * 
 */
void Keys_Purge()
{
    uint8_t i;
    for(i=0; i<KEYS_NUMKEYS; i++)
    {
        keys[i].flags = 0;
    }
}

/*
 * 
 * 
 */
uint8_t Keys_GetKeyStatus(int8_t key, uint8_t mask)
{
    uint8_t tmp = keys[key].flags & mask;
    keys[key].flags &= ~mask;
    return tmp;
}
