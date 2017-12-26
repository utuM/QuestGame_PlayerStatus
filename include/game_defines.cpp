#include "game_defines.h"

/**************************************/

extern bool needToBeFinished;
extern TQuest *oQuest;

//#define __DEBUG__

/**************************************/

bool IsAboveSix = false;

TGraphics::TGraphics(bool _isFullscreen, int _iWinXBias, int _iWinYBias,
                     int _iWinWidth, int _iWinHeight)
{
    isFullscreen = _isFullscreen;
    iWinXBias = _iWinXBias;
    iWinYBias = _iWinYBias;
    iWinWidth = _iWinWidth;
    iWinHeight = _iWinHeight;
}

unsigned char TGraphics::InitializeWindow()
{
    if(isFullscreen)
    {
        init(&iWinWidth, &iWinHeight);
    }
    else
    {
        initWindowSize(iWinXBias, iWinYBias, iWinWidth, iWinHeight);
        init(&iWinWidth, &iWinHeight);
    }

    return FUNC_RESULT_OK;
}

unsigned char TGraphics::StartDrawing()
{
    Start(iWinWidth, iWinHeight);
    Background(BACKGROUND_COLOR);

    return FUNC_RESULT_OK;
}

unsigned char TGraphics::EndDrawing()
{
    End();
    char s[3];
    //fgets(s, 2, stdin);
    //finish();

    return FUNC_RESULT_OK;
}

/**************************************/

TQuest::TQuest()
{
    //srand(time(NULL));
    //unsigned char uTempIndex = (unsigned char)(rand() % STATS_AMOUNT);
    for(unsigned char i = 0; i < STATS_AMOUNT; i ++)
    {
        //if(i == uTempIndex)
        //{
        //    uPlayerStats[i] = STAT_MIN_PERS;
        //}
        //else
        //{
            uPlayerStats[i] = STAT_MAX_PERS;
        //}
    }

    iBadStatIndex = -1;
    //iBadStatIndex = 2;
    uSleepTime = SLEEP_MICROSECS + SLEEP_DEVIATION;

    uEdgeDuration = 0;

    uMode = MODE_WAITING;
    uRisingEdges = MODE_WAIT_IMP;
    //uMode = MODE_GAMING;
    //uRisingEdges = MODE_GREETINGS;
    uPlayerStats[iBadStatIndex] = STAT_MIN_PERS;

    uRepeats = 0;
    iUsingPin = WPI_GPIO19_PIN;

    pPinLevelThread = NULL;
}

void TQuest::SetThread()
{
    //if(pPinLevelThread == NULL)
        (void)pthread_create(&pPinLevelThread, NULL, StaticCheckPinState, this);
}

void TQuest::StartThread()
{
     (void)pthread_join(pPinLevelThread, NULL);
}

/*
 * @brief   Checking GPIO state by its number; major
 *          checking cycle
 * @param   ptrGpio Pointer to GPIO number that need
 *                  to be checked
 */
void* TQuest::StaticCheckPinState(void* ptrValue)
{
    ((TQuest*) ptrValue)->CheckPinState();
}

void TQuest::CheckPinState()
{
    bool isFirstArrived = NO;
    bool isSuccessful;

    unsigned char uCurrImpulses = 0;
    unsigned char uTryings = 0;

    uint32_t cStartTick = 0,
              cEndTick = 0,
              cPauseTick = 0,
              cFlagTick = 0;
    // infinite cycle
    while(YES)
    {
        // Major flag that breaks this cycle
        if(needToBeFinished)
            break;

        if(millis() - cFlagTick >= 2000)
        {
            cFlagTick = false;
        }


        // Pause measurement
        if((millis() - cPauseTick) > PAUSE_LENGTH && uCurrImpulses > 0)
        {
            //isSuccessful = YES;
                //uTryings = 0;

            if(uCurrImpulses >= 6 && !IsAboveSix)
            {
                IsAboveSix = true;
                cFlagTick = millis();
            }



            // check if we got some amount of impulses
            // in a row
            switch(uCurrImpulses)
            {
            case 1: case 2: case 3: case 4:
                {
                    //if(uRisingEdges == MODE_GREETS_IMP && GetMode() == MODE_GREETINGS)
                    //{
                    for(unsigned char i = 0; i < STATS_AMOUNT; i ++)
                    {
                        SetMaxPlayerStat(i);
                    }

                    iBadStatIndex = (char)(uCurrImpulses - 1);
                    SetMinPlayerStat(iBadStatIndex);

                    uMode = MODE_GAMING;

                    isSuccessful = YES;
                    /*}
                    else
                    {
                        isSuccessful = NO;
                    }*/

                    break;
                }
            /*case MODE_WIN_IMP :
                {
                    if(uRisingEdges >= 1 && uRisingEdges <= 4)
                    {
                        SetMaxPlayerStat(iBadStatIndex);
                        //}
                        //else
                        //{
                        //    isSuccessful = NO;
                        //}

                        break;
                    }
                }*/
            case MODE_WIN_IMP :
                {
                    if(uRisingEdges >= 1 && uRisingEdges <= 4)
                    {
                        SetMaxPlayerStat(iBadStatIndex);
                        iBadStatIndex = -1;

                        isSuccessful = YES;
                    }
                    else if(GetMode() == MODE_GREETINGS)
                    {
                        SetMode(MODE_GAMING);
                        iBadStatIndex = -1;

                        for(unsigned char i = 0; i < STATS_AMOUNT; i ++)
                        {
                            SetMaxPlayerStat(i);
                        }

                        isSuccessful = YES;
                    }
                    else if(GetMode() == MODE_WAITING)
                    {
                        iBadStatIndex = -1;

                        SetMode(MODE_GREETINGS);

                        isSuccessful = YES;
                    }
                    else
                        isSuccessful = NO;

                    break;
                }
            case MODE_WAIT_IMP :
                {
                    //if(uRisingEdges == MODE_WIN_IMP)
                    uMode = MODE_WAITING;
                    isSuccessful = YES;
                    //else
                    //    isSuccessful = NO;

                    break;
                }
            default :
                {
                    isSuccessful = NO;
                    break;
                }
            }

            if(isSuccessful)
            {
                uRisingEdges = uCurrImpulses;
                //SetEdgeDuration(ctStopTick - ctStartTick);

                //ctStopTick = ctStartTick = 0;
            }
            uCurrImpulses = 0;
            isSuccessful = NO;
        }

        // if GPIO state is 1 (impulse is up-leveled)
        if(digitalRead(iUsingPin))
        {
            // Measurement start flag
            if(!isFirstArrived)
            {
                cStartTick = millis();
                isFirstArrived = YES;
            }
            // increase rising edges amount
            //uCurrImpulses ++;
            //uTryings = 0;
        }
        // if GPIO state is 0 (impulse is down-leveled)
        else
        {
            if(isFirstArrived)
            {
                cEndTick = millis() - cStartTick;
                cPauseTick = millis();

                isFirstArrived = NO;

                if(cEndTick >= 95 && cEndTick <= 105)
                {
                    ++ uCurrImpulses;
                }

                //cEndTick = 0;
                /*if(uTryings < GPIO_READ_TRYOUT)
                {
                    uTryings ++;
                }*/
            }
        }

        // wait time between 2 neighbour front edges of
        // impulses ('uSleepTime' defined in the 'main'
        // function
        usleep(uSleepTime);
#ifdef __DEBUG__
        std:cout << " [ " << cEndTick <<
                     "; " << millis() - cPauseTick <<
                     "; " << (int)uCurrImpulses <<
                     "; " << (int)oQuest->GetRisingEdges() <<
                     "; " << (int)GetMode() << " ] " <<
                     " [" << (int)(IsAboveSix == true ? 1 : 0) << "] " << std::endl;
#endif // __DEBUG__
    }
}
