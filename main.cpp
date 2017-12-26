#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**************************************/

#include "include/game_defines.h"
#include "include/game_ui.h"

#include "wiringPi.h"

/**************************************/

using namespace std;

TGraphics *oGraphics;
TQuest *oQuest;

bool needToBeFinished = NO;
pthread_t pInputThread;

// player status labels
/*char* sLabels[4] =
{
    "Health", //абвг
    "Defence",
    "Food",
    "Sleep"
};*/

char sTemp[4] = {0};

FILE *fsTestFile;
char sTestInfo[256] = {0};

/**************************************/

void* InputCycle(void* ptrEmpty)
{
    //while(YES)
    //{
        char s[3];
        fgets(s, 2, stdin);
        finish();

        needToBeFinished = YES;

        //usleep(oQuest->GetSleepTime());
    //}
}

/* MAIN PROJECT FUNCTION */
int main()
{
    oGraphics = new TGraphics(YES, 0, 0, 800, 480);
    oQuest = new TQuest();


    //fsTestFile = fopen("debug.txt", "wt");
    // initializes OpenVG graphics library; draws window in
    // fullscreen mode if oQuest object field 'isFullscreen'
    // defines by 'true', otherwise - uses biases on X and Y
    // axes and user-defined window size
    oGraphics->InitializeWindow();

    // configures RaspberryPi functional library
    wiringPiSetup();
    // configures GPIOs state detection
    wiringPiSetupGpio();

    // configure GPIOs checking thread
    oQuest->SetThread();
    // start this thread
    //oQuest->StartThread();
    pthread_create(&pInputThread, NULL, InputCycle, NULL);
    // main game cycle
    while(YES)
    {
        // check game mode:
        // MODE_GREETINGS - first loading display that allow
        // to start the game after RFID-card was swing behind
        // sensor;
        // MODE_GAMING - main mode that draws general information
        // on screen;
        // MODE_WAITING - low-powered mode that arrives after
        // defined time-out time; can be changed to MODE_GREETINGS

        // configures library to allow drawing inside created
        // window
        oGraphics->StartDrawing();
        switch(oQuest->GetMode())
        {
        case MODE_GREETINGS :
            {
                BlackscreenDraw(NO);
                DrawGreetingsSign();

                break;
            }
        case MODE_GAMING :
            {
                DrawImageDirectly();
                for(unsigned char i = 0; i < STATS_AMOUNT; i ++)
                {
                    //DrawLabelBar(i);

                    //DrawImageBar(i);

                    FillStatusBar(i);
                }

                break;
            }
        case MODE_WAITING :
            {
                BlackscreenDraw(NO);

                break;
            }
        default :
            {
                break;
            }
        }

        //fprintf(fsTestFile, "Mode: %u   Impulses:%u   Duration:%u%c",
        //                        oQuest->GetMode(), oQuest->GetRisingEdges(),
        //                        (unsigned long)oQuest->GetEdgeDuration(), (char)0x0A);

        sprintf(sTemp, "%.2X", (int)oQuest->GetMode());

        std::cout << sTemp << "; " << (int)oQuest->GetRisingEdges() <<
                               "; " << (int)oQuest->GetPlayerStat(0) <<
                               "; " << (int)oQuest->GetPlayerStat(1) <<
                               "; " << (int)oQuest->GetPlayerStat(2) <<
                               "; " << (int)oQuest->GetPlayerStat(3) << std::endl;
        oGraphics->EndDrawing();

        usleep(DRAW_SLEEPING_MS);

        if(needToBeFinished)
            break;
    }

    //fclose(fsTestFile);

	return 0;
}
