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

#include "Welder.h"

/*------------------------------------------------------------------------------------------
 * STATIC FUNCTIONS
 *------------------------------------------------------------------------------------------*/

/*
 * 
 * 
 */
static bool waitForZero()
{
    uint32_t tout = 500;
    while(digitalRead(WELDER_SYNC_PIN)==LOW && tout)
    {
        tout--;
        delay(1);
    }

    return tout;
}

/*------------------------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 *------------------------------------------------------------------------------------------*/

/*
 * 
 * 
 */
void Welder_Init()
{
    pinMode(WELDER_TRIGGER_PIN, OUTPUT);
    digitalWrite(WELDER_TRIGGER_PIN, LOW);

    pinMode(WELDER_SYNC_PIN, INPUT);
    digitalWrite(WELDER_SYNC_PIN, HIGH);
}

/*
 * 
 * 
 */
bool Welder_DoSpot(uint32_t pulse1, uint32_t pulse2, uint32_t gap)
{
    bool result = false;
    
    if(waitForZero())
    {
        result = true;

        // Wait for max
        delay(5);

        digitalWrite(WELDER_TRIGGER_PIN, HIGH);
        delay(pulse1);
        digitalWrite(WELDER_TRIGGER_PIN, LOW);
    
        delay(gap);
    
        if(pulse2)
        {
            if(waitForZero())
            {
                // Wait for max
                delay(5);
              
                digitalWrite(WELDER_TRIGGER_PIN, HIGH);
                delay(pulse2);
                digitalWrite(WELDER_TRIGGER_PIN, LOW);
            }
            else
            {
                result = false;
            }
        }
    }

    return result;
}
