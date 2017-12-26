#ifndef GAME_DEFINES_H
#define GAME_DEFINES_H

/**************************************/

#include <ctime>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**************************************/

#include "VG/openvg.h"
#include "VG/vgu.h"

#include "fontinfo.h"
#include "game_ui.h"
#include "shapes.h"
#include "wiringPi.h"

/**************************************/

#define     YES     true
#define     NO      false

#define     FUNC_RESULT_OK       0x00
#define     FUNC_RESULT_ERROR    0xFF

#define     MODE_INIT           0x00
#define     MODE_GREETINGS      0x01
#define     MODE_GREETS_IMP      5
#define     MODE_GAMING         0x02
#define     MODE_WIN_IMP         5
#define     MODE_WAITING        0x04
#define     MODE_WAIT_IMP        6

#define     WIN_OPACITY_INC      20

#define     REPEATS_AMOUNT       1

#define     SLEEP_MICROSECS     4000
#define     SLEEP_DEVIATION       0

#define     DRAW_SLEEPING_MS    100000

#define     STATS_AMOUNT          4
#define     STAT_MAX_PERS        100
#define     STAT_MIN_PERS         50

#define     BACKGROUND_COLOR      (unsigned int)0, (unsigned int)0, (unsigned int)0

#define     GPIO_READ_TRYOUT    1

#define     WPI_GPIO4_PIN       4
#define     WPI_GPIO5_PIN       5
#define     WPI_GPIO6_PIN       6

#define     WPI_GPIO12_PIN      12
#define     WPI_GPIO13_PIN      13
#define     WPI_GPIO16_PIN      16
#define     WPI_GPIO17_PIN      17
#define     WPI_GPIO18_PIN      18
#define     WPI_GPIO19_PIN      19
#define     WPI_GPIO20_PIN      20
#define     WPI_GPIO21_PIN      21
#define     WPI_GPIO22_PIN      22
#define     WPI_GPIO23_PIN      23
#define     WPI_GPIO24_PIN      24
#define     WPI_GPIO25_PIN      25
#define     WPI_GPIO26_PIN      26
#define     WPI_GPIO27_PIN      27

/**************************************/

const unsigned short uUserWidth = 1280;
const unsigned short uUserHeight = 800;

const uint16_t PAUSE_LENGTH = 250;

/**************************************/

using namespace std;

class TGraphics
{
    private:
        bool isFullscreen;
        int iWinXBias;
        int iWinYBias;
        int iWinHeight;
        int iWinWidth;

    public:
        TGraphics(bool _isFullscreen, int _iWinXBias,
                  int _iWinYBias, int _iWinWidth,
                  int _iWinHeight);
        ~TGraphics();

        int GetWindowWidth(void)    { return iWinWidth; }
        int GetWindowHeight(void)    { return iWinHeight; }

        unsigned char InitializeWindow(void);
        unsigned char StartDrawing(void);
        unsigned char EndDrawing(void);
};

class TQuest
{
    private:
        unsigned char uPlayerStats[STATS_AMOUNT];
        char iBadStatIndex;

        unsigned char uMode;
        unsigned char uRisingEdges;
        clock_t uEdgeDuration;

        unsigned int uSleepTime;
        unsigned char uRepeats;
        int iUsingPin;
        pthread_t pPinLevelThread;

    public:
        TQuest();
        ~TQuest();

        unsigned char GetPlayerStat(unsigned char uIndex) { if(uIndex >= STATS_AMOUNT) return 0; return uPlayerStats[uIndex]; }
        unsigned int GetSleepTime(void) { return uSleepTime; }
        unsigned char GetMode(void) { return uMode; }
        unsigned char GetRisingEdges(void) { return uRisingEdges; }
        int GetUsingPin(void) { return iUsingPin; }
        unsigned char GetRepeats(void) { return uRepeats; }
        clock_t GetEdgeDuration(void) { return uEdgeDuration; }
        pthread_t* GetThreadHandler(void) { return &pPinLevelThread; }

        static void* StaticCheckPinState(void* ptrValue);
        void CheckPinState();

        void SetMode(unsigned char uValue) { uMode = uValue; }
        void SetRepeats(unsigned char uValue) { uRepeats = uValue; }
        void SetMinPlayerStat(char iIndex) { uPlayerStats[iIndex] = STAT_MIN_PERS; }
        void SetMaxPlayerStat(char iIndex) { uPlayerStats[iIndex] = STAT_MAX_PERS; }
        void SetEdgeDuration(clock_t uDuration) { uEdgeDuration = uDuration; }
        void SetThread(void);

        void StartThread(void);
};

#endif
