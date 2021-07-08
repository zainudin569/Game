#include "TXLib.h"
const int Width = 1070;
const int Height = 650;
struct Ball;
struct Key;
struct Pictures;

void MoveBall();
void CollisionBall (Ball* plaer, Ball* ball);
void ScoreDraw (int score1, int score2);
void DrawBall_Pad(int* rotations, Pictures Photo, Ball ball1, Ball ball2, Ball ball3);

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

    void Control (Ball* ball, int* F4_Col);
    };

//---------------------------------------------------------------------------------

struct Pictures
    {
    HDC Fon, Ball, Pad;
    };

//---------------------------------------------------------------------------------

struct Ball
    {
    int  x, y , vx, vy;

    int r;

    int plaer;

    COLORREF Color;
    COLORREF FillColor;

    void Physics (int* score1, int* score2, int dt);
    //void Drow();
    };

//---------------------------------------------------------------------------------

void MoveBall()
    {
    //srand(time(NULL));

    Ball ball1 = { Width/2 + rand() % Width/2 ,  rand() % Height, 0, 0,
                   40,
                   1,
                   RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200),
                   RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200) };

    Ball ball2 = { rand() % Width/2,  rand() % Height, 0, 0,
                   40,
                   2,
                   RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200),
                   RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200) };

    Ball ball3 = { Width/2,  rand() % Height, -10 + rand() % 20, -10 + rand() % 20,
                   25,
                   0,
                   RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200),
                   RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200) };

    Key plaer1 = { VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN };
    Key plaer2 = { 'A', 'D', 'W', 'S' };

    Pictures Photo  = {txLoadImage ("Pictures\\Fon.bmp"), txLoadImage ("Pictures\\Ball.bmp"), txLoadImage ("Pictures\\Pad.bmp")};

    int dt = 1;
    int F4_Col = 0;
    int score1 = 0, score2 = 0;
    int rotations = 0;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txBegin();
        txSetFillColor (TX_BLACK);
        txClear();

        /*if (score1 == 5 or score2 == 5)
            {
            ball3 .x = 145;
            ball3 .y = 155;
            ball3 .r = 15;
            HDC Map1 = txLoadImage ("Pictures\\Map1.bmp");
            HDC Map2 = txLoadImage ("Pictures\\Map2.bmp");

            while (!txGetAsyncKeyState ('Q'))
                {
                txBegin();
                txSetFillColor (TX_BLACK);
                txClear();

                txBitBlt (txDC(), 0, 50, 0, 0, Map2);

                plaer1 .Control (&ball3, &F4_Col);

                int a = ball3 .x, b = ball3 .y;

                ball3 .Physics (&score1, &score2, dt);

                COLORREF color = txGetPixel (ball3 .x, ball3 .y, Map2);

                if(color != RGB (0, 255, 0))
                    {
                    ball3 .x = a;
                    ball3 .y = b;
                    }

                //txBitBlt (txDC(), 0, 50, 0, 0, Map1);

                ball3.Drow();

                ball3 .vx = 0;
                ball3 .vy = 0;

                printf ("In color():  %06x\n", color);
                //printf ("In col1or():  %d\n", color);

                txEnd();
                txSleep (1);
                }

            txDeleteDC (Map1);
            txDeleteDC (Map2);

            //Maze (&ball3, plaer1, &F4_Col);
            score1 = 0, score2 = 0;
            ball3 .r = 25;
            }*/

        txBitBlt (txDC(), 0, 50, 0, 0, Photo .Fon);

        //printf ("In collor():  %06x\n", collor);
        //printf ("In CraziBall(): score1 = %d and score2 = %d\n", score1, score2);

        plaer1 .Control (&ball1, &F4_Col);
        plaer2 .Control (&ball2, &F4_Col);

        ball1 .Physics (&score1, &score2, dt);
        ball2 .Physics (&score1, &score2, dt);
        ball3 .Physics (&score1, &score2, dt);

        CollisionBall (&ball1, &ball3);//проверка столкновения
        CollisionBall (&ball2, &ball3);

        DrawBall_Pad(&rotations, Photo, ball1, ball2, ball3);

        ball1 .vx = 0;
        ball1 .vy = 0;
        ball2 .vx = 0;
        ball2 .vy = 0;

        //ball1.Drow();
        //ball2.Drow();
        //ball3.Drow();

        ScoreDraw (score1, score2);

        txEnd();
        txSleep (1);
        }

    txDeleteDC (Photo .Fon);
    txDeleteDC (Photo .Ball);
    txDeleteDC (Photo .Pad);
    }

//---------------------------------------------------------------------------------

/*void Ball::Drow()
    {
    txSetColor ((Color), 2);
    txSetFillColor (FillColor);

    txCircle (x, y, r);
    txLine (x, y, x + vx*5, y + vy*5);
    txCircle (x + vx*5, y + vy*5, 3);

    if ( plaer == 1 or plaer == 2 )
        {
        vx = 0;
        vy = 0;
        }
    } */

//---------------------------------------------------------------------------------

void Key::Control (Ball* ball, int* F4_Col)
    {
    if (txGetAsyncKeyState (key_left))  (*ball) .vx = (*ball) .vx - 15;
    if (txGetAsyncKeyState (key_right)) (*ball) .vx = (*ball) .vx + 15;
    if (txGetAsyncKeyState (key_up))    (*ball) .vy = (*ball) .vy - 15;;
    if (txGetAsyncKeyState (key_down))  (*ball) .vy = (*ball) .vy + 15;;

    if (txGetAsyncKeyState (VK_SPACE)) (*ball) .vy = (*ball) .vx = 0;

    if (txGetAsyncKeyState (VK_F1))
        {
        (*ball) .Color = TX_LIGHTRED;
        (*ball) .FillColor = TX_RED;
        *F4_Col = 0;
        }

    if (txGetAsyncKeyState (VK_F2))
        {
        (*ball) .Color = TX_LIGHTBLUE;
        (*ball) .FillColor = TX_BLUE;
        *F4_Col = 0;
        }

    if (txGetAsyncKeyState (VK_F3))
        {
        (*ball) .Color = TX_LIGHTGREEN;
        (*ball) .FillColor = TX_GREEN;
        *F4_Col = 0;
        }

    if (txGetAsyncKeyState (VK_F4))
        {
        *F4_Col = 1;
        }

    if (*F4_Col == 1)
        {
        (*ball) .Color     = RGB((*ball) .x,   (*ball) .y,   150);
        (*ball) .FillColor = RGB((*ball) .x/2, (*ball) .y/2, 150);
        }
    }

//---------------------------------------------------------------------------------

void Ball::Physics (int* score1, int* score2, int dt)
    {
    int W_limit   = Width;
    int W_limit_2 = 0;

    if (vy >  15) vy =  15;//ограничение на скорость движения
    if (vx >  15) vx =  15;
    if (vy < -15) vy = -15;
    if (vx < -15) vx = -15;

    (*this) .x += (*this) .vx * dt;
    (*this) .y += (*this) .vy * dt;

    if (plaer == 1) // ограничения для plaer = 1
        {
        W_limit   = Width;
        W_limit_2 = Width/2;
        }

    if (plaer == 2) // ограничения для plaer = 2
        {
        W_limit   = Width/2;
        W_limit_2 = 0;
        }

    if (x > W_limit - r)
        {
        vx = - vx;
        x  = W_limit - r;

        if (plaer == 0)
            {
            ++(*score1);
            x = Width/2; vx = -10 + rand() % 20; vy = -10 + rand() % 20;
            txPlaySound ("sounds/zvuk-krika-gol.wav");
            Sleep (200);
            }
        }

    if (y > Height - r)
        {
        vy = - vy;
        y  = Height - r;
        }

    if (x < W_limit_2 + r)
        {
        vx = - vx;
        x  = W_limit_2 + r;

        if ( plaer == 0)
            {
            ++(*score2);
            x = Width/2; vx = -10 + rand() % 20; vy = -10 + rand() % 20;
            txPlaySound ("sounds/zvuk-krika-gol.wav");
            Sleep (200);
            }
        }

    if (y < 50 + r)
        {
        vy = - vy;
        y  = 50 + r;
        }
    }

//---------------------------------------------------------------------------------

void CollisionBall (Ball* plaer, Ball* ball)
    {
     double Dx = (*ball) .x - (*plaer) .x; //стороны треугольника
     double Dy = (*ball) .y - (*plaer) .y; //стороны треугольника
     double d = sqrt (Dx*Dx + Dy*Dy); if (d == 0) d = 0.01; // гипотенуза
	 double sin = Dx/d; //sin угла треугольника
	 double cos = Dy/d; //cos угла треугольника

	 if (d < (*ball) .r + (*plaer) .r) //проверка столкновения
        {
        txPlaySound ("sounds/Zvuk_Ball.wav");

        double Vn1 = (*plaer) .vx*sin + (*plaer) .vy*cos; //поворот системы координат шар1
        double Vn2 = (*ball)  .vx*sin + (*ball)  .vy*cos; //поворот системы координат шар2

        if ((Vn1 - Vn2) == 0) Vn1 = Vn1 + 0.01;

        double dt = ((*ball) .r + (*plaer) .r - d)/(Vn1 - Vn2); //удаление залипания

        if (dt >  0.7) dt =  0.7; //ограничение на dt, чтобы мяч не отскакивал далеко
        if (dt < -0.7) dt = -0.7;

        (*ball)  .x -= ROUND ((*ball)  .vx*dt);
        (*ball)  .y -= ROUND ((*ball)  .vy*dt);
        (*plaer) .x -= ROUND ((*plaer) .vx*dt);
        (*plaer) .y -= ROUND ((*plaer) .vy*dt);

        Dx = (*ball) .x - (*plaer) .x;
        Dy = (*ball) .y - (*plaer) .y;
        d = sqrt (Dx*Dx + Dy*Dy); if (d == 0) d = 0.01;
        sin = Dx/d;
        cos = Dy/d;

        Vn1 = (*plaer) .vx*sin + (*plaer) .vy*cos;
        Vn2 = (*ball)  .vx*sin + (*ball)  .vy*cos;

        double Vt2 = -(*ball) .vx*cos + (*ball) .vy*sin; //поворот системы координат шар2

        Vn2 = Vn1 - Vn2;

        (*ball)  .vx = ROUND (Vn2*sin - Vt2*cos);
        (*ball)  .vy = ROUND (Vn2*cos + Vt2*sin);

        (*ball)  .x +=ROUND ((*ball)  .vx*dt);
        (*ball)  .y +=ROUND ((*ball)  .vy*dt);
        (*plaer) .x +=ROUND ((*plaer) .vx*dt);
        (*plaer) .y +=ROUND ((*plaer) .vy*dt);
        //printf (" dt  = %f\n", dt);
        //txSleep (3000);
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
    }

//---------------------------------------------------------------------------------

void DrawBall_Pad (int* rotations, Pictures Photo, Ball ball1, Ball ball2, Ball ball3)
    {
    int sizeBall_X = txGetExtentX (Photo .Ball)/8;
    int sizeBall_Y = txGetExtentY (Photo .Ball);
    int sizePad = 150;

    if (ball1 .y <= (Height + 50)/2)
        {
        txAlphaBlend (txDC(), ball1 .x - 55, ball1 .y - 60, sizePad, sizePad, Photo .Pad, sizePad, sizePad);
        }

        else
        {
        txAlphaBlend (txDC(), ball1 .x - 55, ball1 .y - 100, sizePad, sizePad, Photo .Pad, 0, sizePad);
        }

    if (ball2 .y <= (Height + 50)/2)
        {
        txAlphaBlend (txDC(), ball2 .x - 85, ball2 .y - 58, sizePad, sizePad, Photo .Pad, 0, 0);;
        }

        else
        {
        txAlphaBlend (txDC(), ball2 .x - 85, ball2 .y - 95, sizePad, sizePad, Photo .Pad, sizePad, 0);
        }

    txAlphaBlend (txDC(), ball3 .x - 25, ball3 .y - 25, sizeBall_Y, 0, Photo .Ball, sizeBall_X * (*rotations) + 4, 0);
    (*rotations) ++;
    if (*rotations >= 7) *rotations = 0;

    }
