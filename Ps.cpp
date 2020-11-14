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
#include <EEPROM.h>

#include "Ps.h"

/*------------------------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 *------------------------------------------------------------------------------------------*/

/*
 * 
 * 
 */
bool Ps_Load(uint16_t addr, uint8_t* buf, uint16_t len)
{
    bool result = false; 
    uint32_t cs = 0;

    for(int i=0; i<len; i++)
    {
        buf[i] = EEPROM.read(addr++);
        cs += buf[i];
    }

    uint32_t csFromPs = EEPROM.read(addr++);
    csFromPs |= ((uint32_t)EEPROM.read(addr++))<<8;
    csFromPs |= ((uint32_t)EEPROM.read(addr++))<<16;
    csFromPs |= ((uint32_t)EEPROM.read(addr++))<<24;
    
    if(cs == csFromPs)
    {
        result = true;
    }

    return result;
}

/*
 * 
 * 
 */
void Ps_Save(uint16_t addr, uint8_t* buf, uint16_t len)
{
    uint32_t cs = 0;

    for(int i=0; i<len; i++)
    {
        EEPROM.write(addr++, buf[i]);
        cs += buf[i];
    }

    EEPROM.write(addr++, cs & 0x000000ff);
    EEPROM.write(addr++, (cs & 0x0000ff00) >> 8);
    EEPROM.write(addr++, (cs & 0x00ff0000) >> 16);
    EEPROM.write(addr++, (cs & 0xff000000) >> 24);
}
