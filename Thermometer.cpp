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

#include "Thermometer.h"

/*------------------------------------------------------------------------------------------
 * MACROS
 *------------------------------------------------------------------------------------------*/

#define TO_KELVIN(x)    ((x)+273.15)
#define TO_CELSIUS(x)   ((x)-273.15)

/*------------------------------------------------------------------------------------------
 * CONSTANTS
 *------------------------------------------------------------------------------------------*/

static const float vref=5.00;

/*------------------------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 *------------------------------------------------------------------------------------------*/

/*
 * 
 * 
 */
float Thermometer_GetTemp()
{
    float rt, t, vrt; 

    // Voltage
    vrt = (vref / 1023.00) * analogRead(THERMOMETER_PIN);

    // Resistance
    rt = vrt / ((VCC-vrt) / R);

    // Temperature
    t = (1 / ((log(rt / R0) / B) + (1 / TO_KELVIN(T0))));

    return TO_CELSIUS(t);
}
