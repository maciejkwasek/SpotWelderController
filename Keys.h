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

#ifndef KEYS_H_
#define KEYS_H_

#define KEYS_NUMKEYS            2

#define KEYS_KEY_SET				    0
#define KEYS_KEY_TRIG			      1

#define KEYS_PIN_SET            2
#define KEYS_PIN_TRIG           3

#define KEYS_EVENT_DOWN		      0x01
#define KEYS_EVENT_UP				    0x02
#define KEYS_EVENT_SHORTUP			0x04
#define KEYS_EVENT_HOLD				  0x08
#define KEYS_EVENT_REPEATED			0x10

void Keys_Init();
void Keys_Handler();
void Keys_Purge();
uint8_t Keys_GetKeyStatus(int8_t key, uint8_t mask);

#define Keys_IsDown(key)			  Keys_GetKeyStatus((key), KEYS_EVENT_DOWN)
#define Keys_IsUp(key)			    Keys_GetKeyStatus((key), KEYS_EVENT_UP)
#define Keys_IsShortUp(key)		  Keys_GetKeyStatus((key), KEYS_EVENT_SHORTUP)
#define Keys_IsHeld(key)			  Keys_GetKeyStatus((key), KEYS_EVENT_HOLD)
#define Keys_IsRepeated(key)	  Keys_GetKeyStatus((key), KEYS_EVENT_REPEATED)

#endif
