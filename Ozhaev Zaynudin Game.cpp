#include "TXLib.h"
const int W = 1070;
const int H = 650;
struct Ball;
struct Key;

void MoveBall();

void CollisionBall(Ball* ball_p, Ball* ball_b);

void ScoreDraw (int score1, int score2);

//void bilo_stolknov (double xA, double yA, double xB, double yB, double rA, double rB)
//double Distansce (double x1, double y1, double x2, double y2);

//---------------------------------------------------------------------------------

int main()
    {
    txCreateWindow (W, H);

    MoveBall();

    return 0;
    }

//---------------------------------------------------------------------------------

struct Key
    {
    int  key_left, key_right , key_up, key_down;

    void Control(Ball* ball, int* F4_Col, int dt);
    };

struct Ball
    {
    int  x, y , vx, vy;

    int r;

    COLORREF Color;
    COLORREF FillColor;

    void Physics(int* score1, int* score2, int dt);
    void Drow();
    };

//---------------------------------------------------------------------------------

void MoveBall()
    {
    //srand(time(NULL));

    Ball ball1 = {W/2 + rand() % W/2 ,  rand() % H, 0, 0, 25,
                 RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200),
                 RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200) };

    Ball ball2 = { rand() % W/2,  rand() % H, 0, 0, 25,
                 RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200),
                 RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200) };

    Ball ball3 = { rand() % W,  rand() % H, 1 + rand() % 9, 1 + rand() % 9, 40,
                 RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200),
                 RGB(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200) };

    Key plaer1 = { VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN };
    Key plaer2 = { 'A', 'D', 'W', 'S' };

    int dt = 1;
    int F4_Col = 0;
    int score1 = 0, score2 = 0;

    HDC Fon  = txLoadImage ("Pictures\\Fon.bmp");

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {

        txBegin ();
        txSetFillColor (TX_BLACK);
        txClear ();

        txBitBlt  (txDC(), 0, 50, 0, 0, Fon);

        ball1.Drow();
        ball2.Drow();
        ball3.Drow();

        //printf ("In CraziBall(): x  = %d and y  = %d\n", x2, y2);
        //printf ("In CraziBall(): vx = %d and vy = %d\n", vx2, vy2);

        ball1 .Physics(&score1, &score2, dt);
        ball2 .Physics(&score1, &score2, dt);
        ball3 .Physics(&score1, &score2, dt);

        CollisionBall(&ball1, &ball3);//проверка столкновения
        CollisionBall(&ball2, &ball3);

        plaer1 .Control(&ball1, &F4_Col, dt);
        plaer2 .Control(&ball2, &F4_Col, dt);

        ScoreDraw (score1, score2);

        txEnd ();
        txSleep (1);
        }

    txDeleteDC(Fon);
    }

//---------------------------------------------------------------------------------

void Ball::Drow()
    {
    txSetColor ((Color), 2);
    txSetFillColor (FillColor);

    txCircle (x, y, r);
    //txLine (x, y, x + vx*5, y + vy*5);
    //txCircle (x + vx*5, y + vy*5, 3);
    }

//---------------------------------------------------------------------------------

void Key::Control(Ball* ball, int* F4_Col, int dt)
        {
        if (txGetAsyncKeyState (key_left))  (*ball) .vx = (*ball) .vx - 5;
        if (txGetAsyncKeyState (key_right)) (*ball) .vx = (*ball) .vx + 5;
        if (txGetAsyncKeyState (key_up))    (*ball) .vy = (*ball) .vy - 5;;
        if (txGetAsyncKeyState (key_down))  (*ball) .vy = (*ball) .vy + 5;;

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
        /*PhysicsBall(ball, dt);
        (*ball) .vx = 0;
        (*ball) .vy = 0;*/
        }

//---------------------------------------------------------------------------------

void Ball::Physics(int* score1, int* score2, int dt)
    {
    if (vy >  15) vy =  15;//ограничение на скорость движения
    if (vx >  15) vx =  15;
    if (vy < -15) vy = -15;
    if (vx < -15) vx = -15;

    (*this) .x += (*this) .vx * dt;
    (*this) .y += (*this) .vy * dt;

    if (x > W - r)
        {
        vx =   - vx;
        x  = W - r;
        ++(*score1);
        }

    if (y  > H - r)
        {
        vy =   - vy;
        y  = H - r;
        }

    if (x  < 0 + r)
        {
        vx =   - vx;
        x  = 0 + r;
        ++(*score2);
        }

    if (y  < 50 + r)
        {
        vy =   - vy;
        y  = 50 + r;
        }
    }

//---------------------------------------------------------------------------------

void CollisionBall (Ball* ball_p, Ball* ball_b)
{
    int Dx = (*ball_p) .x - (*ball_b) .x; // стороны треугольника
    int Dy = (*ball_p) .y - (*ball_b) .y; // стороны треугольника
    double d = sqrt(Dx*Dx + Dy*Dy); if (d == 0) d = 0.01; //гипотенуза
    double sin = Dx/d; // sin угла треугольника
    double cos = Dy/d; // cos угла треугольника

    if (d < (*ball_p) .r + (*ball_b) .r) //проверка столкновения
        {
        txPlaySound ("sounds/Zvuk_Ball.wav");

        //++(*score);

        double Vn1 = (*ball_p) .vx*sin + (*ball_p) .vy*cos; //поворот системы координат шар1
        double Vn2 = (*ball_b) .vx*sin + (*ball_b) .vy*cos; //поворот системы координат шар2

        if ((Vn1 - Vn2) == 0) Vn1 = Vn1 + 0.01;

        double dt = ((*ball_p) .r + (*ball_b) .r - d)/(Vn1 - Vn2); // удаление залипания

        if (dt >  0.6) dt =  0.6;   // ограничение на dt, чтоб мяч не отскакивал далеко
        if (dt < -0.6) dt = -0.6;

        (*ball_p) .x = ROUND((*ball_p) .x - (*ball_p) .vx*dt);
        (*ball_p) .y = ROUND((*ball_p) .y - (*ball_p) .vy*dt);
        (*ball_b) .x = ROUND((*ball_b) .x - (*ball_b) .vx*dt);
        (*ball_b) .y = ROUND((*ball_b) .y - (*ball_b) .vy*dt);

        Dx = (*ball_p) .x - (*ball_b) .x;
        Dy = (*ball_p) .y - (*ball_b) .y;
        d = sqrt(Dx*Dx + Dy*Dy); if (d == 0) d = 0.01;
        sin = Dx/d; // sin
        cos = Dy/d; // cos
        Vn1 = (*ball_p) .vx*sin + (*ball_p) .vy*cos;
        Vn2 = (*ball_b) .vx*sin + (*ball_b) .vy*cos;

        double Vt2 = -(*ball_b) .vx*cos + (*ball_b) .vy*sin; //поворот системы координат шар2

        Vn2 = Vn1 - Vn2;

        (*ball_b) .vx = ROUND(Vn2*sin - Vt2*cos); //обратный поворот системы координат шар2
        (*ball_b) .vy = ROUND(Vn2*cos + Vt2*sin); //обратный поворот системы координат шар2

        (*ball_p) .x = ROUND((*ball_p) .x + (*ball_p) .vx*dt);
        (*ball_p) .y = ROUND((*ball_p) .y + (*ball_p) .vy*dt);
        (*ball_b) .x = ROUND((*ball_b) .x + (*ball_b) .vx*dt);
        (*ball_b) .y = ROUND((*ball_b) .y + (*ball_b) .vy*dt);
        //printf (" dt  = %f\n", dt);
        //txSleep (3000);
        }

     /*double Dx = (*ball_b).x-(*ball_p).x;
     double Dy = (*ball_b).y-(*ball_p).y;
     double d = sqrt(Dx*Dx+Dy*Dy);      if (d==0) d=0.01;                 // во избежании деления на ноль
	 double ax = Dx/d;
	 double ay = Dy/d;

	 if (d < (*ball_b).r+(*ball_p).r)
	 {
		 double Vn1 = (*ball_p).vx*ax + (*ball_p).vy*ay;
         double Vn2 = (*ball_b).vx*ax + (*ball_b).vy*ay;

		 if ((Vn1 - Vn2) == 0) Vn1 = Vn1 + 0.01;

		 double dt=((*ball_b).r+(*ball_p).r-d)/(Vn1-Vn2);

	                 if (dt>0.6)  dt= 0.6;   // ограничение на dt, чтоб мяч не улетал
                     if (dt<-0.6) dt=-0.6; // усли проникновение сильное было


		 (*ball_b).x-=(*ball_b).vx*dt;
		 (*ball_b).y-=(*ball_b).vy*dt;
		 (*ball_p).x-=(*ball_p).vx*dt;
		 (*ball_p).y-=(*ball_p).vy*dt;



	////////основная часть, находим новые скорости после столкновения
		  Dx = (*ball_b).x-(*ball_p).x;
		  Dy = (*ball_b).y-(*ball_p).y;
	      d = sqrt(Dx*Dx+Dy*Dy);  if (d==0) d=0.01;
  		  ax = Dx/d;
		  ay = Dy/d;


		  Vn1=(*ball_p).vx*ax+(*ball_p).vy*ay;
          Vn2=(*ball_b).vx*ax+(*ball_b).vy*ay;

   double Vt2= -(*ball_b).vx*ay+(*ball_b).vy*ax;

          Vn2 = Vn1-Vn2;

          (*ball_b).vx=Vn2*ax-Vt2*ay;
		  (*ball_b).vy=Vn2*ay+Vt2*ax;
    ///////


		 (*ball_b).x+=(*ball_b).vx*dt;
		 (*ball_b).y+=(*ball_b).vy*dt;
		 (*ball_p).x+=(*ball_p).vx*dt;
		 (*ball_p).y+=(*ball_p).vy*dt;
	 }*/

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
    txRectangle (W/2 - textSizeX, 2,
                 W/2 + textSizeX, textSizeY);

    txSetTextAlign (TA_CENTER);
    txTextOut (txGetExtentX() / 2, 2, str);
    }


