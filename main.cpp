#include <iostream>
#include <ctime>
#include "graphics.h"
#define WIDTH 800
#define HEIGHT 600
using namespace std;

struct Button
{
   int x1,y1,x2,y2;
   int info;
   bool visible = 0;
   bool mark = 0;
}btMenu[3],polePlayer[10][10],polePC[10][10],btInGame;




void drawMenu();
void drawButton(Button bt);
bool checkButton(Button bt);
void menu();

void setShips(Button pole[][10]);
void setPlayerShips();
bool findPlace(int size, int x, int y, int dir, Button pole[][10]);
bool canBePut(int x, int y, int dir, Button pole[][10]);

void drawGame();
void game();
void init();
bool action(Button pole[][10],int x,int y);
bool actionPC();
int searchWounded();
bool target(int x, int y);

bool killShip(Button pole[][10],int x, int y);
void selectArea(Button pole[][10],int x, int y);
void winner(bool flag);
void about();
int checkWin();

int main()
{
   initwindow(WIDTH,HEIGHT);
   init();
   menu();
}

void init() // инициализация игры
{
   srand(time(NULL));
   int d = 70;  // размер клетки
   for(int i = 0; i < 3; i++)
   {
      btMenu[i].x1 = WIDTH/3;
      btMenu[i].x2 = (2*WIDTH)/3;
      btMenu[i].y1 = 200+i*d;
      btMenu[i].y2 = 260+i*d;
   }

   d = 30;      // размер клетки поля
   
   for(int i = 0; i < 10; i++)
      for(int j = 0; j < 10; j++)
      {
         polePlayer[i][j].x1 = 50+i*d;
         polePlayer[i][j].x2 = 50+(i+1)*d;
         polePlayer[i][j].y1 = 50+j*d;
         polePlayer[i][j].y2 = 50+(j+1)*d;
         polePlayer[i][j].visible = 1;
         
         polePC[i][j].x1 = 450+i*d;
         polePC[i][j].x2 = 450+(i+1)*d;
         polePC[i][j].y1 = 50+j*d;
         polePC[i][j].y2 = 50+(j+1)*d;
      }
   
   btInGame.x1 = (2*WIDTH)/5;
   btInGame.x2 = (3*WIDTH)/5;
   btInGame.y1 = 500;
   btInGame.y2 = 550;
}
void menu() // меню
{
   while(1)
   {
      drawMenu();
      delay(500);
      while(!mousebuttons());
      
      if(checkButton(btMenu[0]))
         game();
      else if(checkButton(btMenu[1]))
         about();
      else if(checkButton(btMenu[2]))
         break;
   }
}
void drawMenu() // отрисовать меню
{
   setfillstyle(SOLID_FILL,WHITE);
   setcolor(BLACK);
   setbkcolor(LIGHTGRAY);
   cleardevice();
   setbkcolor(WHITE);
   
   settextjustify(1,1);
   
   string name[] = {"Начать игру","Об игре","Выход"};
   
   for(int i = 0; i < sizeof(btMenu)/sizeof(Button); i++)
   {
      drawButton(btMenu[i]);
      outtextxy((btMenu[i].x1+btMenu[i].x2)/2,
                    (btMenu[i].y1+btMenu[i].y2)/2, name[i].c_str());
   }
}
void drawButton(Button bt) // отрисовать кнопку
{
   setfillstyle(SOLID_FILL,WHITE);
   setcolor(BLACK);
   bar(bt.x1,bt.y1,bt.x2,bt.y2);
   rectangle(bt.x1,bt.y1,bt.x2,bt.y2);
   if(bt.visible || bt.mark)
   {
      if(bt.mark)
      {
         setfillstyle(SOLID_FILL,LIGHTBLUE);
         bar(bt.x1,bt.y1,bt.x2,bt.y2);
         rectangle(bt.x1,bt.y1,bt.x2,bt.y2);
      }
      if(bt.info)
      {
         setfillstyle(SOLID_FILL,BLUE);
         fillellipse((bt.x1+bt.x2)/2,(bt.y1+bt.y2)/2, (bt.x2-bt.x1)/2, (bt.y2-bt.y1)/2);
         setfillstyle(SOLID_FILL,WHITE);
      }
   }
}
bool checkButton(Button bt) // проверка нажатия на поле
{
   int x = mousex();
   int y = mousey();
   return (x > bt.x1 && x < bt.x2 && y > bt.y1 && y < bt.y2);
}
void drawGame() // отрисовать игровое поле
{
   setbkcolor(LIGHTGRAY);
   cleardevice();
   for(int i = 0; i < 10; i++)
      for(int j = 0; j < 10; j++)
      {
         drawButton(polePC[i][j]);
         drawButton(polePlayer[i][j]);
      }
      
   string a = "A";
   int c = 1;
   for(int i = 0; i < 10; i++)
   {
      outtextxy(polePlayer[i][0].x1+12,polePlayer[i][0].y1-12,a.c_str());
      outtextxy(polePC[i][0].x1+12,polePC[i][0].y1-12,a.c_str());
      
      outtextxy(polePlayer[0][i].x1-12,polePlayer[0][i].y2-10,to_string(c).c_str());
      outtextxy(polePC[0][i].x1-12,polePC[0][i].y2-10,to_string(c).c_str());
      a[0]++;
      c++;
   }
   setbkcolor(WHITE);
   drawButton(btInGame);
   outtextxy((btInGame.x1+btInGame.x2)/2,
                  (btInGame.y1+btInGame.y2)/2, "Вернуться к меню");
   
}
void setShips(Button pole[][10]) // установить корабли на выбранное поле
{
   for(int i = 0; i < 10; i++)
      for(int j = 0; j < 10; j++)
      {
         pole[i][j].info = 0;
         pole[i][j].mark = 0;
      }
   int x,y,dir;
   for(int size = 4; size > 0; size--)
      for(int count = 4-size; count >= 0; count--)
         do
         {
            x = rand()%10;
            y = rand()%10;
            dir = rand()%2;
         }while(!findPlace(size,x,y,dir,pole));
}
bool findPlace(int size, int x, int y, int dir, Button pole[][10]) // найти место под корабль и поставить его
{
   if(size == 0)
      return true;
   if(x < 0 || x > 9 || y < 0 || y > 9)
      return false;
   if(!canBePut(x,y,dir,pole))
      return false;
   
   if(findPlace(size-1, x+dir, y + !dir, dir,pole))
   {
      pole[x][y].info = 1;
      return true;
   }
   return false;
}

bool canBePut(int x, int y, int dir, Button pole[][10]) // проверка на то, можно ли поставить корабль
{
   if(pole[x][y].info)
      return false;
   if(x > 0)
   {
      if(pole[x-1][y].info)
         return false;
      if(y > 0 && pole[x-1][y-1].info)
         return false;
      if(y < 9 && pole[x-1][y+1].info)
         return false;
   }
   if(x < 9)
   {
      if(pole[x+1][y].info)
         return false;
      if(y > 0 && pole[x+1][y-1].info)
         return false;
      if(y < 9 && pole[x+1][y+1].info)
         return false;
   }
   
   if(y > 0 && pole[x][y-1].info)
      return false;
   if(y < 9 && pole[x][y+1].info)
      return false;
   
   return true;
}
void game() // сам процесс игры
{
   setShips(polePC);
   setShips(polePlayer);
   drawGame();
   bool flag = rand()%2;
   int win;
   while(1)
   {
      if(flag)
      {
         while(!mousebuttons());
         if(checkButton(btInGame))
            return;
         for(int i = 0; i < 10; i++)
            for(int j = 0; j < 10; j++)
               if(checkButton(polePC[i][j]) && polePC[i][j].mark == 0)
                  if(!action(polePC,i,j))
                     flag = !flag;
      }
      else
      {
         if(!actionPC())
            flag = !flag;
      }
      win = checkWin();
      if(win)
      {
         winner(win-1);
         return;
      }
      delay(200);
   }
}
bool action(Button pole[][10],int x,int y) // атака на выбранную клетку
{
   pole[x][y].mark = 1;
   drawButton(pole[x][y]);
   if(pole[x][y].info != 0)
   {
      if(killShip(pole,x,y))
         selectArea(pole,x,y);
      return true;
   }
   return false;
}
bool actionPC() // действие компьютера
{
   int x,y;
   int hit = searchWounded(); 
   if(hit < 0)
   {
      do
      {
         x = rand()%10;
         y = rand()%10;
      }while(polePlayer[x][y].mark != 0);
      return action(polePlayer,x,y);
   }
   return hit;
}
int searchWounded() // поиск подбитого корабля и попытка его атаковать
{
   for(int i = 0; i < 10; i++)
      for(int j = 0; j < 10; j++)
         if(target(i,j))
         {
            if(i > 0 && i < 9 && polePlayer[i+1][j].mark && polePlayer[i+1][j].info && polePlayer[i-1][j].mark == 0)
               return action(polePlayer,i-1,j);
            if(i > 0 && i < 9 && polePlayer[i-1][j].mark && polePlayer[i-1][j].info && polePlayer[i+1][j].mark == 0)
               return action(polePlayer,i+1,j);
            
            if(j > 0 && j < 9 && polePlayer[i][j+1].mark && polePlayer[i][j+1].info && polePlayer[i][j-1].mark == 0)
               return action(polePlayer,i,j-1);
            if(j > 0 && j < 9 && polePlayer[i][j-1].mark && polePlayer[i][j-1].info && polePlayer[i][j+1].mark == 0)
               return action(polePlayer,i,j+1);
            
            //если не определили направление корабля бьем наугад рядом
            while(1)
            {
               if(rand()%2)
               {
                  if(rand()%2)
                  {
                     if(i > 0 && polePlayer[i-1][j].mark == 0)
                        return action(polePlayer,i-1,j);
                  }
                  else
                  {
                     if(i < 9 && polePlayer[i+1][j].mark == 0)
                        return action(polePlayer,i+1,j);
                  }
               }
               else
               {
                  if(rand()%2)
                  {
                     if(j > 0 && polePlayer[i][j-1].mark == 0)
                        return action(polePlayer,i,j-1);
                  }
                  else
                  {
                     if(j < 9 && polePlayer[i][j+1].mark == 0)
                        return action(polePlayer,i,j+1);
                  }
               }
            }
         }
   return -1;
}
bool target(int x, int y) //если это часть корабля
{
   if(polePlayer[x][y].mark == 0 || polePlayer[x][y].info == 0)
      return false;
   
   if(x > 0)
      if(polePlayer[x-1][y].mark == 0)
         return true;
      
   if(x < 9)
      if(polePlayer[x+1][y].mark == 0)
         return true;
      
   if(y > 0)
      if(polePlayer[x][y-1].mark == 0)
         return true;
      
   if(y < 9)
      if(polePlayer[x][y+1].mark == 0)
         return true;
   return false;
}

bool killShip(Button pole[][10],int x, int y) // убит ли корабль
{
   if(x < 0 || y < 0 || x >= 10 || y >= 10)
      return true;
   if(pole[x][y].info == 0)
      return true;
   if(pole[x][y].info < 0)
      return true;
   if(pole[x][y].mark == 1 && pole[x][y].info > 0)
   {
      pole[x][y].info = -pole[x][y].info;
      bool kill = killShip(pole,x+1,y) && killShip(pole,x-1,y);
      kill = kill && killShip(pole,x,y+1) && killShip(pole,x,y-1);
      pole[x][y].info = -pole[x][y].info;
      return kill;
   }
   return false;
}
void selectArea(Button pole[][10],int x, int y) // выделить арену вокруг убитого корабля
{
   if(x < 0 || y < 0 || x >= 10 || y >= 10)
      return;
   if(pole[x][y].info > 0)
   {
      for(int i = -1; i <= 1; i++)
         for(int j = -1; j <= 1; j++)
            if(x+i >= 0 && y+j >= 0 && x+i < 10 && y+j < 10)
            {
               pole[x+i][y+j].mark = 1;
               drawButton(pole[x+i][y+j]);
            }
      pole[x][y].info = -pole[x][y].info;
      selectArea(pole,x+1,y);
      selectArea(pole,x-1,y);
      selectArea(pole,x,y+1);
      selectArea(pole,x,y-1);
      pole[x][y].info = -pole[x][y].info;
   }
}

int checkWin() // проверка победы
{
   bool winPlayer = 1;
   bool winPC = 1;
   
   for(int i = 0; i < 10 && winPlayer; i++)
      for(int j = 0; j < 10 && winPlayer; j++)
         if(polePC[i][j].info && !polePC[i][j].mark)
            winPlayer = 0;
   if(winPlayer)
      return 1;
   
   for(int i = 0; i < 10 && winPC; i++)
      for(int j = 0; j < 10 && winPC; j++)
         if(polePlayer[i][j].info && !polePlayer[i][j].mark)
            winPC = 0;
   if(winPC)
      return 2;
   return 0;
}
void winner(bool flag) // вывести победителя
{
   setfillstyle(SOLID_FILL,WHITE);
   bar(WIDTH/3, 80, (2*WIDTH)/3, 320);
   rectangle(WIDTH/3, 80, (2*WIDTH)/3, 320);
   if(flag)
      outtextxy(WIDTH/2, 210,"Вы проиграли(");
   else
      outtextxy(WIDTH/2, 210,"Вы победили!");
   while(1)
   {
      while(!mousebuttons());
      if(checkButton(btInGame))
         return;
   }
}
void about()
{
   setfillstyle(SOLID_FILL,WHITE);
   bar(WIDTH/7, 80, (6*WIDTH)/7, 400);
   rectangle(WIDTH/7, 80, (6*WIDTH)/7, 400);
   outtextxy(WIDTH/2, 210,"О программе:");
   outtextxy(WIDTH/2, 240,"Данная программа реализует морской бой в режиме Человек vs Компьютер.");
   outtextxy(WIDTH/2, 300,"Автор:");
   outtextxy(WIDTH/2, 330,"Юнусов Артем Ильгизович. ЕТ-114.");
   drawButton(btInGame);
   outtextxy((btInGame.x1+btInGame.x2)/2,
                  (btInGame.y1+btInGame.y2)/2, "Вернуться к меню");
   while(1)
   {
      while(!mousebuttons());
      if(checkButton(btInGame))
         return;
   }
}