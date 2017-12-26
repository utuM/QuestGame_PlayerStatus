#include "game_ui.h"

/**************************************/

extern TGraphics *oGraphics;
extern TQuest *oQuest;

VGfloat vgfStatFieldWidth;
VGfloat vgfImageBarHeight;

VGfloat vgfImageBias = ((uUserWidth / 4.0f) -
                       (2 * STATUSBAR_MARGIN_PX) -
                        DEFAULT_IMAGE_SIZE) / 2.0f;
VGfloat vgfLabelBias = ((uUserWidth / 4.0f) -
                       (2 * STATUSBAR_MARGIN_PX) -
                       DEFAULT_LABEL_WIDTH) / 2.0f;

/**************************************/

unsigned char DrawGreetingsSign()
{
    Image((uUserWidth - GREETINGS_IMAGE_SIZE) / 2.0f,
          (uUserHeight - GREETINGS_IMAGE_SIZE) / 2.0f,
          GREETINGS_IMAGE_SIZE, GREETINGS_IMAGE_SIZE,  "resource/greetings.jpg");
}

unsigned char BlackscreenDraw(bool isGreetings)
{
    Background(BACKGROUND_COLOR);
    if(isGreetings)
    {
        Fill(255, 255, 255, 1.0f);
        Text(0, oGraphics->GetWindowWidth() / 2.0f, "Hello there. Pass your RFID-card behind and wait for sum.", SansTypeface, 24.0f);
    }

    return FUNC_RESULT_OK;
}

unsigned char DrawLabelBar(unsigned char uIndex)
{
    if(uIndex >= STATS_AMOUNT)
    {
        std::cout << "ERROR DrawLabelBar(): unknown player status index (can be from 0 to 3)" << std::endl;
        return FUNC_RESULT_ERROR;
    }

    vgfStatFieldWidth = (VGfloat)(oGraphics->GetWindowWidth() / 4.0f);

    VGfloat vgfTempStartXPos = vgfStatFieldWidth * uIndex;
    VGfloat vgfTempHeight = STD_LABEL_HEIGHT_PX;
    VGfloat vgfTempStartYPos = (VGfloat)oGraphics->GetWindowHeight() - STATUSBAR_MARGIN_PX - vgfTempHeight;

    struct TEMP_POLY_COORDS
    {
        VGfloat x[STATUSBAR_POLY_PTS];
        VGfloat y[STATUSBAR_POLY_PTS];
    } stTempPolygon;

    VGfloat vgfTempWidth = vgfStatFieldWidth - 2 * STATUSBAR_MARGIN_PX;

    /* base rectangle of percents status bar */
    Fill(180, 180, 180, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX, vgfTempStartYPos, vgfTempWidth, vgfTempHeight);

    Fill(120, 120, 120, 1);
    stTempPolygon.x[0] = vgfTempStartXPos + STATUSBAR_MARGIN_PX;
    stTempPolygon.y[0] = vgfTempStartYPos + vgfTempHeight;

    stTempPolygon.x[1] = vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5;
    stTempPolygon.y[1] = vgfTempStartYPos + 75;

    stTempPolygon.x[2] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX - 5;
    stTempPolygon.y[2] = vgfTempStartYPos + 75;

    stTempPolygon.x[3] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX - 5;
    stTempPolygon.y[3] = vgfTempStartYPos + 5;

    stTempPolygon.x[4] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX;
    stTempPolygon.y[4] = vgfTempStartYPos;

    stTempPolygon.x[5] = vgfTempStartXPos + STATUSBAR_MARGIN_PX;
    stTempPolygon.y[5] = vgfTempStartYPos;

    Polygon(stTempPolygon.x, stTempPolygon.y, 6);

    //Fill(80, 80, 80, 1);
    //Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5.0f, vgfTempStartYPos + 5.0f, vgfTempWidth - STATUSBAR_MARGIN_PX, 70.0f);

    Fill(255, 255, 255, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5.0f, vgfTempStartYPos + 5.0f, vgfTempWidth - STATUSBAR_MARGIN_PX, 70.0f);

    return FUNC_RESULT_OK;
}

unsigned char FillLabelBar(unsigned char uIndex, char* sText)
{
    Fill(0, 0, 0, 1.0f);

    vgfStatFieldWidth = (VGfloat)(oGraphics->GetWindowWidth() / 4.0f);

    VGfloat vgfTempStartXPos = vgfStatFieldWidth * uIndex;
    VGfloat vgfTempHeight = STD_LABEL_HEIGHT_PX;
    VGfloat vgfTempStartYPos = (VGfloat)oGraphics->GetWindowHeight() - STATUSBAR_MARGIN_PX - vgfTempHeight;

    VGfloat vgfTempWidth = vgfStatFieldWidth - 2 * STATUSBAR_MARGIN_PX;

    Text(vgfTempStartXPos + vgfTempWidth / 4.0f, vgfTempStartYPos + STD_LABEL_HEIGHT_PX / 3.5f, sText, SansTypeface, STD_LABEL_HEIGHT_PX / 2.25f);

    return FUNC_RESULT_OK;
}

unsigned char DrawImageBar(unsigned char uIndex)
{
    if(uIndex >= STATS_AMOUNT)
    {
        std::cout << "ERROR DrawLabelBar(): unknown player status index (can be from 0 to 3)" << std::endl;
        return FUNC_RESULT_ERROR;
    }

    vgfStatFieldWidth = (VGfloat)(oGraphics->GetWindowWidth() / 4.0f);

    VGfloat vgfTempStartXPos = vgfStatFieldWidth * uIndex;
    VGfloat vgfTempHeight = (VGfloat)oGraphics->GetWindowHeight() - 4 * STATUSBAR_MARGIN_PX - STD_LABEL_HEIGHT_PX;//- STD_STATUS_HEIGHT_PX;
    vgfImageBarHeight = vgfTempHeight;

    VGfloat vgfTempStartYPos = STATUSBAR_MARGIN_PX;// + STD_STATUS_HEIGHT_PX;

    struct TEMP_POLY_COORDS
    {
        VGfloat x[STATUSBAR_POLY_PTS];
        VGfloat y[STATUSBAR_POLY_PTS];
    } stTempPolygon;

    VGfloat vgfTempWidth = vgfStatFieldWidth - 2 * STATUSBAR_MARGIN_PX;

    /* base rectangle of percents status bar */
    Fill(230, 230, 230, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX, vgfTempStartYPos, vgfTempWidth, vgfTempHeight);

    Fill(170, 170, 170, 1);
    stTempPolygon.x[0] = vgfTempStartXPos + STATUSBAR_MARGIN_PX;
    stTempPolygon.y[0] = vgfTempStartYPos + vgfTempHeight;

    stTempPolygon.x[1] = vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5;
    stTempPolygon.y[1] = vgfTempStartYPos + vgfTempHeight - 5;

    stTempPolygon.x[2] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX - 5;
    stTempPolygon.y[2] = vgfTempStartYPos + vgfTempHeight - 5;

    stTempPolygon.x[3] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX - 5;
    stTempPolygon.y[3] = vgfTempStartYPos + 5;

    stTempPolygon.x[4] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX;
    stTempPolygon.y[4] = vgfTempStartYPos;

    stTempPolygon.x[5] = vgfTempStartXPos + STATUSBAR_MARGIN_PX;
    stTempPolygon.y[5] = vgfTempStartYPos;

    Polygon(stTempPolygon.x, stTempPolygon.y, 6);

    Fill(80, 80, 80, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5.0f, vgfTempStartYPos + 5.0f, vgfTempWidth - STATUSBAR_MARGIN_PX, vgfTempHeight - STATUSBAR_MARGIN_PX);

    Fill(250, 250, 250, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX + 7.0f, vgfTempStartYPos + 5.0f, vgfTempWidth - STATUSBAR_MARGIN_PX - 2.0f, vgfTempHeight - STATUSBAR_MARGIN_PX - 2.0f);

    return FUNC_RESULT_OK;
}

unsigned char DrawImageDirectly()//unsigned char uIndex)
{
    Image(0, 0, uUserWidth, uUserHeight, "resource/background_status.jpg");
    /*VGfloat vgfXPos = (uUserWidth / 4.0f) * uIndex +
                       STATUSBAR_MARGIN_PX,
            vgfYPos = 80.0f;

    switch(uIndex)
    {
        case 0 :
        {
            Image(vgfXPos + vgfImageBias, vgfYPos, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE,
                                            "resource/health_icon.jpg");
            Image(vgfXPos + vgfLabelBias, uUserHeight - STD_LABEL_HEIGHT_PX - 5,
                                            DEFAULT_LABEL_WIDTH, DEFAULT_LABEL_HEIGHT,
                                            "resource/health_label.jpg");
            break;
        }
        case 1 :
        {
            Image(vgfXPos + vgfImageBias, vgfYPos, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE,
                                            "resource/shield_icon.jpg");
            Image(vgfXPos + vgfLabelBias, uUserHeight - STD_LABEL_HEIGHT_PX - 5,
                                            DEFAULT_LABEL_WIDTH, DEFAULT_LABEL_HEIGHT,
                                            "resource/shield_label.jpg");
            break;
        }
        case 2 :
        {
            Image(vgfXPos + vgfImageBias, vgfYPos, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE,
                                            "resource/food_icon.jpg");
            Image(vgfXPos + vgfLabelBias, uUserHeight - STD_LABEL_HEIGHT_PX - 5,
                                            DEFAULT_LABEL_WIDTH, DEFAULT_LABEL_HEIGHT,
                                            "resource/food_label.jpg");
            break;
        }
        case 3 :
        {
            Image(vgfXPos + vgfImageBias, vgfYPos, DEFAULT_IMAGE_SIZE, DEFAULT_IMAGE_SIZE,
                                            "resource/sleep_icon.jpg");
            Image(vgfXPos + vgfLabelBias, uUserHeight - STD_LABEL_HEIGHT_PX - 5,
                                            DEFAULT_LABEL_WIDTH, DEFAULT_LABEL_HEIGHT,
                                            "resource/sleep_label.jpg");
            break;
        }
    }*/
}

unsigned char DrawStatusBar(unsigned char uIndex)
{
    if(uIndex >= STATS_AMOUNT)
    {
        std::cout << "ERROR DrawStatusBar(): unknown player status index (can be from 0 to 3)" << std::endl;
        return FUNC_RESULT_ERROR;
    }

    vgfStatFieldWidth = (VGfloat)(oGraphics->GetWindowWidth() / 4.0f);
    VGfloat vgfTempStartXPos = vgfStatFieldWidth * uIndex;
    struct TEMP_POLY_COORDS
    {
        VGfloat x[STATUSBAR_POLY_PTS];
        VGfloat y[STATUSBAR_POLY_PTS];
    } stTempPolygon;

    VGfloat vgfTempWidth = vgfStatFieldWidth - 2 * STATUSBAR_MARGIN_PX;

    /* base rectangle of percents status bar */
    Fill(230, 230, 230, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX, STATUSBAR_MARGIN_PX, vgfTempWidth, 60.0f);

    Fill(170, 170, 170, 1);
    stTempPolygon.x[0] = vgfTempStartXPos + STATUSBAR_MARGIN_PX;
    stTempPolygon.y[0] = 70;

    stTempPolygon.x[1] = vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5;
    stTempPolygon.y[1] = 65;

    stTempPolygon.x[2] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX - 5;
    stTempPolygon.y[2] = 65;

    stTempPolygon.x[3] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX - 5;
    stTempPolygon.y[3] = 15;

    stTempPolygon.x[4] = vgfStatFieldWidth * (uIndex + 1) - STATUSBAR_MARGIN_PX;
    stTempPolygon.y[4] = 10;

    stTempPolygon.x[5] = vgfTempStartXPos + STATUSBAR_MARGIN_PX;
    stTempPolygon.y[5] = STATUSBAR_MARGIN_PX;

    Polygon(stTempPolygon.x, stTempPolygon.y, 6);

    Fill(80, 80, 80, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX + 5.0f, 15.0f, vgfTempWidth - STATUSBAR_MARGIN_PX, 50.0f);

    Fill(110, 110, 110, 1);
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX + 7.0f, 15.0f, vgfTempWidth - STATUSBAR_MARGIN_PX - 2.0f, 48.0f);

    return FUNC_RESULT_OK;
}

VGfloat vgfHeights[4] = { 106.0f, 291.0f, 473.0f, 650.0f };
unsigned char FillStatusBar(unsigned char uIndex)
{
    if(uIndex >= STATS_AMOUNT)
    {
        std::cout << "ERROR FillStatusBar(): unknown player status index (can be from 0 to 3)" << std::endl;
        return FUNC_RESULT_ERROR;
    }

    VGfloat stops[] = {
		0.0, 1.0, 1.0, 1.0, 1.0,
		0.5, 0.5, 0.5, 0.5, 1.0,
		1.0, 0.0, 0.0, 0.0, 1.0
	};
    unsigned int uColors[4];
    switch(uIndex)
    {
        case 0:
        {
            Fill(FIRST_STATUS_COLOR);
            break;
        }
        case 1:
        {
            Fill(SECOND_STATUS_COLOR);
            break;
        }

        case 2:
        {
            Fill(THIRD_STATUS_COLOR);
            break;
        }
        case 3:
        {
            Fill(FORTH_STATUS_COLOR);
            break;
        }
    }

    vgfStatFieldWidth = (VGfloat)(oGraphics->GetWindowWidth() / 4.0f);
    VGfloat vgfTempStartXPos = vgfStatFieldWidth * uIndex;
    VGfloat vgfTempWidth = vgfStatFieldWidth - 2 * STATUSBAR_MARGIN_PX;

    VGfloat vgfStatusPerc = (vgfImageBarHeight - STATUSBAR_MARGIN_PX - 2.0f) * oQuest->GetPlayerStat(uIndex) / 100.0f;
    Rect(vgfTempStartXPos + STATUSBAR_MARGIN_PX + 7, STATUSBAR_MARGIN_PX + 5, vgfTempWidth - STATUSBAR_MARGIN_PX - 2.0f, vgfStatusPerc);

    Fill(255, 255, 255, 1.0f);
    char sTempValue[5] = {0};
    sprintf(sTempValue, "%u%c", oQuest->GetPlayerStat(uIndex), '%');
    /*if(oQuest->GetPlayerStat(uIndex) < 100)
        Text(900.0f, uUserHeight * (0.25 * (3 - uIndex)) + 100.0f, sTempValue, SansTypeface, STD_STATUS_HEIGHT_PX / 2.0f);
    else*/
    Text(1000.0f, vgfHeights[3 - uIndex], sTempValue, SansTypeface, STD_STATUS_HEIGHT_PX - 10.0f);

    return FUNC_RESULT_OK;
}
