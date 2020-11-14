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

#include <TimerOne.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Keys.h"
#include "Welder.h"
#include "Version.h"
#include "Ps.h"
#include "Thermometer.h"

/*------------------------------------------------------------------------------------------
 * MACROS
 *------------------------------------------------------------------------------------------*/
 
#define ELEMS(x)      (sizeof(x)/sizeof(x[0]))

/*------------------------------------------------------------------------------------------
 * TYPES
 *------------------------------------------------------------------------------------------*/
typedef void (*OnHeldCallback_t)(void);
typedef bool (*OnUpCallback_t)(uint8_t);
typedef void (*ScreenPainterCallback_t)(void);

typedef struct ScreenHandler_s
{
    OnHeldCallback_t onHeld;
    OnUpCallback_t onUp;
} ScreenHandler_t;

typedef struct ScreenParam_s
{
    const char* const caption;
    const char* const units;
    int32_t* const var;
    bool isEditing;

    uint32_t minVal;
    uint32_t maxVal;
} ScreenParam_t;

typedef struct Ps_s
{
    uint32_t pulse1Width;
    uint32_t pulse2Width;
    uint32_t pulseDelay;
    uint32_t totalSpots;
} Ps_t;

/*------------------------------------------------------------------------------------------
 * PROTOTYPES
 *------------------------------------------------------------------------------------------*/
static void paintSettingsScreen();
static void paintStatusScreen();

static void onHeldSettingsScreen();
static void onHeldStatusScreen();

static bool onUpSettingsScreen(uint8_t tmp);

/*------------------------------------------------------------------------------------------
 * CONSTANTS
 *------------------------------------------------------------------------------------------*/
static const char pulse1WidthCaption[] PROGMEM =  "Pulse 1: ";
static const char pulse2WidthCaption[] PROGMEM =  "Pulse 2: ";
static const char pulseDelayCaption[] PROGMEM =   "GapTime: ";

static const char currentSpotsCaption[] PROGMEM = "Curr. Spots: ";
static const char totalSpotsCaption[] PROGMEM =   "Total spots: ";
static const char transfTempCaption[] PROGMEM =   "Transf temp: ";

static const char unitsCaption[] PROGMEM = " ms";
static const char emptyCaption[] PROGMEM = "";
static const char celciusCaption[] PROGMEM = " C";

/*------------------------------------------------------------------------------------------
 * VARIABLES
 *------------------------------------------------------------------------------------------*/
static Adafruit_SSD1306 oled(128, 32, &Wire, -1);

static int32_t pulse1Width = 80;
static int32_t pulse2Width = 80;
static int32_t pulseDelay = 200;

static int32_t currentSpots = 0;
static int32_t totalSpots = 0;
static int32_t transfTemp = 0;

static bool editingFlag;
static bool waitForPressed;
static bool repaintFlag;

static uint8_t editingParamIdx;
static uint8_t currentScreen = 0;

static ScreenParam_t settingsParams[] = 
{
    {pulse1WidthCaption, unitsCaption, &pulse1Width, false, 15, 1000},
    {pulseDelayCaption, unitsCaption, &pulseDelay, false, 100, 5000},
    {pulse2WidthCaption, unitsCaption, &pulse2Width, false, 0, 1000}
};

static ScreenParam_t statusParams[] = 
{
    {currentSpotsCaption, emptyCaption, &currentSpots, false},
    {totalSpotsCaption, emptyCaption, &totalSpots, false},
    {transfTempCaption, celciusCaption, &transfTemp, false}
};

static const ScreenHandler_t screenHandlers[] =
{
    {onHeldSettingsScreen, onUpSettingsScreen},
    {onHeldStatusScreen, NULL} 
};

static const ScreenPainterCallback_t screenPainters[] =
{
    paintSettingsScreen,
    paintStatusScreen
};

/*------------------------------------------------------------------------------------------
 * STATIC FUNCTIONS
 *------------------------------------------------------------------------------------------*/

/*
 * 
 * 
 */
static void saveToPs()
{
    Ps_t ps;
   
    ps.pulse1Width = pulse1Width;
    ps.pulse2Width = pulse2Width;
    ps.pulseDelay = pulseDelay;
    ps.totalSpots = totalSpots;

    Ps_Save(0, (uint8_t*)&ps, sizeof(ps));
}

/*
 * 
 * 
 */
static void loadFromPs()
{
    Ps_t ps;
    
    if(Ps_Load(0, (uint8_t*)&ps, sizeof(ps)))
    {
        pulse1Width = ps.pulse1Width;
        pulse2Width = ps.pulse2Width;
        pulseDelay = ps.pulseDelay;
        totalSpots = ps.totalSpots;

        Serial.println(F("Ps_Load success"));
    }
}
 
/*
 * 
 * 
 */
static void paintWelcomeScreen()
{
    oled.clearDisplay();

    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(5, 0); 
    oled.print(F("SpotWelder"));

    oled.setTextSize(1.2);
    oled.setCursor(54, 20); 
    oled.print(F(VERSION));  

    oled.display();
}

/*
 * 
 * 
 */
static void paintParams(ScreenParam_t* p, uint8_t n)
{
    char buffer[16];

    oled.clearDisplay();
    
    oled.setTextSize(1);

    for(int i=0; i<n; i++)
    {
      oled.setCursor(0, i*10);

      strncpy_P(buffer, p[i].caption, sizeof(buffer));      
      oled.print(buffer);

      if(p[i].isEditing)
      {
          oled.print(F("<"));
          oled.print(*(p[i].var));
          oled.print(F(">"));
      }
      else
      {
          oled.print(*(p[i].var));
      }

      strncpy_P(buffer, p[i].units, sizeof(buffer));      
      oled.print(buffer); 
    }
    
    oled.display();  
}

/*
 * 
 * 
 */
static void paintSettingsScreen()
{   
    paintParams(settingsParams, ELEMS(settingsParams));
}

/*
 * 
 * 
 */
static void paintStatusScreen()
{   
    paintParams(statusParams, ELEMS(statusParams));
}

/*
 * 
 * 
 */
static void timerOverflow(void)
{
    Keys_Handler();  
}

/*
 * 
 * 
 */
static void initTimer(void)
{
    Timer1.initialize(40000);
    Timer1.attachInterrupt(timerOverflow);
}

/*
 * 
 * 
 */
static void onHeldSettingsScreen()
{
    if(!editingFlag)
    {
        editingFlag = true;
        settingsParams[editingParamIdx].isEditing = true;
    }
    else
    {
        settingsParams[editingParamIdx].isEditing = false;
        editingParamIdx++;
            
        if(editingParamIdx < ELEMS(settingsParams))
        {
            settingsParams[editingParamIdx].isEditing = true;    
        }
        else
        {
            editingFlag = false;
            editingParamIdx = 0; 

            // Save settings to Eeprom
            saveToPs();
        }
    }
}

/*
 * 
 * 
 */
static bool onUpSettingsScreen(uint8_t tmp)
{
     bool result = false;

     if(editingFlag)
     {
          result = true;
              
         if(tmp == (KEYS_EVENT_SHORTUP | KEYS_EVENT_UP))
         {
             uint32_t tmp;
             if(*(settingsParams[editingParamIdx].var) < 100)
             {
                 tmp = 5;
             }
             else if(*(settingsParams[editingParamIdx].var) < 1000)
             {
                 tmp = 10;  
             }
             else
             {
                 tmp = 100;
             }
             
             *(settingsParams[editingParamIdx].var) += tmp;

             if(*(settingsParams[editingParamIdx].var) > settingsParams[editingParamIdx].maxVal)
             {
                 *(settingsParams[editingParamIdx].var) = settingsParams[editingParamIdx].maxVal;
             }
         }
         else
         if(tmp == KEYS_EVENT_UP)
         {
             *(settingsParams[editingParamIdx].var) = settingsParams[editingParamIdx].minVal;
         }
     }
     
     return result;
}

/*
 * 
 * 
 */
static void onHeldStatusScreen()
{
    currentSpots = 0;
    totalSpots = 0;
}

/*
 * 
 * 
 */
static void nextScreen()
{
    currentScreen++;
    if(currentScreen==2)
    {
        currentScreen=0;
    }
    repaintFlag = true;  
}

/*
 * 
 * 
 */
static void handleScreen(OnHeldCallback_t onHeld, OnUpCallback_t onUp)
{
    if(Keys_IsHeld(KEYS_KEY_SET))
    {
        if(onHeld)
        {
            onHeld();
        }

        waitForPressed = true;
        Keys_IsDown(KEYS_KEY_SET);
        
        repaintFlag = true;
    }

    if(Keys_IsDown(KEYS_KEY_SET))
    {
        if(waitForPressed)
        {
            waitForPressed = false;
            Keys_GetKeyStatus(KEYS_KEY_SET, KEYS_EVENT_SHORTUP | KEYS_EVENT_UP);
        }
    }

    if(!waitForPressed)
    {
        uint8_t tmp = Keys_GetKeyStatus(KEYS_KEY_SET, KEYS_EVENT_SHORTUP | KEYS_EVENT_UP);
        
        if(tmp & (KEYS_EVENT_SHORTUP | KEYS_EVENT_UP))
        {
            if(onUp)
            {
                if(!onUp(tmp))
                {
                    nextScreen();
                }
            }
            else
            {
                nextScreen();
            }
        }
    }  
}

/*
 * 
 * 
 */
static void handleTrigKey()
{
    if(Keys_IsDown(KEYS_KEY_TRIG))
    {
        if(!editingFlag)
        {
            if(Welder_DoSpot(pulse1Width, pulse2Width, pulseDelay))
            {
                currentSpots++;
                totalSpots++;
        
                if(currentSpots >= 1000000)
                {
                    currentSpots = 0;
                }
        
                if(totalSpots >= 1000000)
                {
                    totalSpots = 0;
                }
        
                if(currentScreen == 1)
                {
                    repaintFlag = true;
                }
        
                // Save totalSpost to Eeprom
                saveToPs();
            }
            else
            {
                // Zero sync error
                // paintError();
            }
        }
    }
}

/*
 *
 *
 */
static void handleThermometer()
{
    static uint8_t cycleCounter;

    cycleCounter++;
    if(cycleCounter >= 20)
    {
        cycleCounter = 0;
        transfTemp = Thermometer_GetTemp();

        if(currentScreen == 1)
        {
            repaintFlag = true;
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
void setup()
{
    Serial.begin(115200);
    Serial.println(F("SpotWelder started"));

    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("Oled init failed!"));
        while(1);
    }

    loadFromPs();

    paintWelcomeScreen();
    delay(3000);
    paintSettingsScreen();

    Welder_Init();
    Keys_Init();
    initTimer();
}

/*
 *
 *
 */
void loop()
{
    delay(10);

    handleScreen(screenHandlers[currentScreen].onHeld, screenHandlers[currentScreen].onUp);
    handleTrigKey();
    handleThermometer();

    if(repaintFlag)
    {
        screenPainters[currentScreen]();
    }
}
