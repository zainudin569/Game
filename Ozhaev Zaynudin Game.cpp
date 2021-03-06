
#include "TXLib.h"

const int Width = 1070;
const int Height = 650;
const int speed_racket = 20;
const int dt = 1;

struct Ball;
struct Key;
struct Pictures;
struct Coord;

void MoveBall();
void CollisionBall (Ball* plaer, Ball* ball);
void ScoreDraw (int score1, int score2);
void DrawBall_Pad (int* rotations, Pictures Photo, Ball ball1, Ball ball2, Ball ball3);

//---------------------------------------------------------------------------------

int main()
    {
    txCreateWindow (Width, Height);

    MoveBall();

    return 0;
    }

//---------------------------------------------------------------------------------

struct Key
    {
    int  key_left, key_right , key_up, key_down;

    void Control (Ball* ball, int* end_gam);
    };

//---------------------------------------------------------------------------------

struct Pictures
    {
    HDC Fon, Ball, Pad;
    };

//---------------------------------------------------------------------------------

struct Coord
    {
    int x, y;
    };

//---------------------------------------------------------------------------------

struct Ball
    {
    Coord position;     // position .x
    Coord vector_speed; // vector_speed .x

    int r;

    int plaer;

    void Physics (int* score1, int* score2);
    };

//---------------------------------------------------------------------------------

void MoveBall()
    {
    //srand(time(NULL));

    Ball ball1 = {{Width/2 + rand() % Width/2 ,  rand() % Height}, {0, 0}, 40 , 1};

    Ball ball2 = {{rand() % Width/2,  rand() % Height}, {0, 0}, 40, 2};

    Ball ball3 = {{Width/2, rand() % Height}, {-10 + rand() % 20, -10 + rand() % 20}, 25, 0};

    Key plaer1 = {VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN};
    Key plaer2 = {'A', 'D', 'W', 'S'};

    Pictures Photo = {txLoadImage ("Pictures\\Fon.bmp"), txLoadImage ("Pictures\\Ball.bmp"), txLoadImage ("Pictures\\Pad.bmp")};

    int score1 = 0, score2 = 0;
    int rotations = 0;
    int end_gam = 0;

    while (end_gam == 0)
        {
        txBegin();
        txSetFillColor (TX_BLACK);
        txClear();

        txBitBlt (txDC(), 0, 50, 0, 0, Photo .Fon);

        plaer1 .Control (&ball1, &end_gam);
        plaer2 .Control (&ball2, &end_gam);

        ball1 .Physics (&score1, &score2);
        ball2 .Physics (&score1, &score2);
        ball3 .Physics (&score1, &score2);

        CollisionBall (&ball1, &ball3);
        CollisionBall (&ball2, &ball3);

        DrawBall_Pad (&rotations, Photo, ball1, ball2, ball3);

        ball1 .vector_speed .x = 0;
        ball1 .vector_speed .y = 0;
        ball2 .vector_speed .x = 0;
        ball2 .vector_speed .y = 0;

        ScoreDraw (score1, score2);

        txEnd();
        txSleep (1);
        }

    txDeleteDC (Photo .Fon);
    txDeleteDC (Photo .Ball);
    txDeleteDC (Photo .Pad);
    }

//---------------------------------------------------------------------------------

void Key::Control (Ball* ball, int* end_gam)
    {
    if (txGetAsyncKeyState (key_left))  ball -> vector_speed .x = ball -> vector_speed .x - speed_racket;
    if (txGetAsyncKeyState (key_right)) ball -> vector_speed .x = ball -> vector_speed .x + speed_racket;
    if (txGetAsyncKeyState (key_up))    ball -> vector_speed .y = ball -> vector_speed .y - speed_racket;
    if (txGetAsyncKeyState (key_down))  ball -> vector_speed .y = ball -> vector_speed .y + speed_racket;

    if (txGetAsyncKeyState (VK_ESCAPE))
        {
        if (MessageBox (txWindow(), "?? ?????? ?? ?????", "?????",
             MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDYES)
            *end_gam = 1;
        }
    }

//---------------------------------------------------------------------------------

void Ball::Physics (int* score1, int* score2)
    {
    int W_limit   = Width;
    int W_limit_2 = 0;

    if (vector_speed .y >  15) vector_speed .y =  15;//??????????? ?? ???????? ????????
    if (vector_speed .x >  15) vector_speed .x =  15;
    if (vector_speed .y < -15) vector_speed .y = -15;
    if (vector_speed .x < -15) vector_speed .x = -15;

    position .x += vector_speed .x * dt;
    position .y += vector_speed .y * dt;

    if (plaer == 1) // ??????????? ??? plaer = 1
        {
        W_limit   = Width;
        W_limit_2 = Width/2;
        }

    if (plaer == 2) // ??????????? ??? plaer = 2
        {
        W_limit   = Width/2;
        W_limit_2 = 0;
        }

    if (position .x > W_limit - r)
        {
        vector_speed .x = - vector_speed .x;
        position .x  = W_limit - r;

        if (plaer == 0)
            {
            ++(*score1);
            position .x = Width/2; vector_speed .x = -10 + rand() % 20; vector_speed .y = -10 + rand() % 20;
            txPlaySound ("sounds/zvuk-krika-gol.wav");
            Sleep (200);
            }
        }

    if (position .y > Height - r)
        {
        vector_speed .y = - vector_speed .y;
        position .y  = Height - r;
        }

    if (position .x < W_limit_2 + r)
        {
        vector_speed .x = - vector_speed .x;
        position .x  = W_limit_2 + r;

        if ( plaer == 0)
            {
            ++(*score2);
            position .x = Width/2; vector_speed .x = -10 + rand() % 20; vector_speed .y = -10 + rand() % 20;
            txPlaySound ("sounds/zvuk-krika-gol.wav");
            Sleep (200);
            }
        }

    if (position .y < 50 + r)
        {
        vector_speed .y = - vector_speed .y;
        position .y  = 50 + r;
        }
    }

//---------------------------------------------------------------------------------

void CollisionBall (Ball* plaer, Ball* ball)
    {
     double Dx = ball -> position .x - plaer -> position .x; //??????? ????????????
     double Dy = ball -> position .y - plaer -> position .y; //??????? ????????????
     double hypotenuse = sqrt (Dx*Dx + Dy*Dy); if ((int)hypotenuse == 0) hypotenuse = 0.01; // ??????????
	 double sin = Dx/hypotenuse; //sin ???? ????????????
	 double cos = Dy/hypotenuse; //cos ???? ????????????

	 if (hypotenuse < (*ball) .r + (*plaer) .r) //???????? ????????????
        {
        txPlaySound ("sounds/Zvuk_Ball.wav");

        double Vn1 = plaer -> vector_speed .x*sin + plaer -> vector_speed .y*cos; //??????? ??????? ????????? ???1
        double Vn2 = ball  -> vector_speed .x*sin + ball  -> vector_speed .y*cos; //??????? ??????? ????????? ???2

        if ((int)(Vn1 - Vn2) == 0) Vn1 = Vn1 + 0.01;

        double dt_Collision = ((*ball) .r + (*plaer) .r - hypotenuse)/(Vn1 - Vn2); //???????? ?????????

        if (dt_Collision >  0.7) dt_Collision =  0.7; //??????????? ?? dt, ????? ??? ?? ?????????? ??????
        if (dt_Collision < -0.7) dt_Collision = -0.7;

        ball  -> position .x -= ROUND (ball  -> vector_speed .x*dt_Collision);
        ball  -> position .y -= ROUND (ball  -> vector_speed .y*dt_Collision);
        plaer -> position .x -= ROUND (plaer -> vector_speed .x*dt_Collision);
        plaer -> position .y -= ROUND (plaer -> vector_speed .y*dt_Collision);

        Dx = ball -> position .x - plaer -> position .x;
        Dy = ball -> position .y - plaer -> position .y;
        hypotenuse = sqrt (Dx*Dx + Dy*Dy); if ((int)hypotenuse == 0) hypotenuse = 0.01;
        sin = Dx/hypotenuse;
        cos = Dy/hypotenuse;

        Vn1 = plaer -> vector_speed .x*sin + plaer -> vector_speed .y*cos;
        Vn2 = ball  -> vector_speed .x*sin + ball  -> vector_speed .y*cos;

        double Vt2 = -ball -> vector_speed .x*cos + ball -> vector_speed .y*sin; //??????? ??????? ????????? ???2

        Vn2 = Vn1 - Vn2;

        ball -> vector_speed .x = ROUND (Vn2*sin - Vt2*cos);
        ball -> vector_speed .y = ROUND (Vn2*cos + Vt2*sin);

        (*ball)  .position .x +=ROUND ((*ball)  .vector_speed .x*dt);
        (*ball)  .position .y +=ROUND ((*ball)  .vector_speed .y*dt);
        (*plaer) .position .x +=ROUND ((*plaer) .vector_speed .x*dt);
        (*plaer) .position .y +=ROUND ((*plaer) .vector_speed .y*dt);
        //printf (" dt  = %f\n", dt);
        }

}

//---------------------------------------------------------------------------------

void ScoreDraw (int score1, int score2)
    {
    txSelectFont ("TimesNewRoman", 50);
    char str[12] = "";
    sprintf (str, "%d : %d", score1, score2);

    int textSizeX = txGetTextExtentX (str)/2 + 10,
        textSizeY = txGetTextExtentY (str);

    txSetColor ((TX_WHITE), 2);
    txSetFillColor (TX_BLACK);
    txRectangle (Width/2 - textSizeX, 2,
                 Width/2 + textSizeX, textSizeY);

    txSetTextAlign (TA_CENTER);
    txTextOut (txGetExtentX() / 2, 2, str);

    txSelectFont ("TimesNewRoman", 30);
    txSetTextAlign (TA_LEFT);
    txTextOut (10, 10, "Press key [Esc] to Pause");

    }

//---------------------------------------------------------------------------------

void DrawBall_Pad (int* rotations, Pictures Photo, Ball ball1, Ball ball2, Ball ball3)
    {
    int sizeBall_X = txGetExtentX (Photo .Ball)/8;
    int sizeBall_Y = txGetExtentY (Photo .Ball);
    int sizePad = txGetExtentX (Photo .Pad)/2;

    if (ball1 .position .y <= (Height + 50)/2)
        {
        txAlphaBlend (txDC(), ball1 .position .x - 55, ball1 .position .y - 60, sizePad, sizePad, Photo .Pad, sizePad, sizePad);
        }

        else
        {
        txAlphaBlend (txDC(), ball1 .position .x - 55, ball1 .position .y - 100, sizePad, sizePad, Photo .Pad, 0, sizePad);
        }

    if (ball2 .position .y <= (Height + 50)/2)
        {
        txAlphaBlend (txDC(), ball2  .position .x - 85, ball2 .position .y - 58, sizePad, sizePad, Photo .Pad, 0, 0);;
        }

        else
        {
        txAlphaBlend (txDC(), ball2 .position .x - 85, ball2 .position .y - 95, sizePad, sizePad, Photo .Pad, sizePad, 0);
        }

    txAlphaBlend (txDC(), ball3 .position .x - 25, ball3 .position .y - 25, sizeBall_Y, 0, Photo .Ball, sizeBall_X * (*rotations) + 4, 0);
    (*rotations) ++;
    if (*rotations >= 7) *rotations = 0;

    }
