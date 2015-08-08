// snake.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define BOARD_WIDTH 60
#define BOARD_HEIGHT 25

#define BLANK_CELL ' '
#define WALL_CELL '#'
#define SNAKE_CELL '@'
#define FOOD_CELL '*'
#define ITEM_CELL 'I'

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

#define FOOD_COUNT 10
#define ITEM_COUNT 2
#define SLEEP_DELAY 50

char board[BOARD_WIDTH][BOARD_HEIGHT];

struct Snake {
  int x;
  int y;

  int headDirection;

  struct Snake *next;
  struct Snake *prev;
} *headSnake, *tailSnake;

int direction;
int isBackward;

//________________________________________________________________________helper function

int random(int min, int max)
{
  return min + int((max-min)*rand()/(RAND_MAX+1.0));
}

void gotoxy(int x, int y)
{
  COORD coord = {x, y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void putxy(int x, int y, char c)
{
  gotoxy(x,y);
  putchar(c);
}

void setCell(int x, int y, char cellElement)
{
  board[x][y] = cellElement;
  putxy(x, y, cellElement);
}

int checkCell(int x, int y, char elementToCheck)
{
  return board[x][y] == elementToCheck;
}

Snake * createNode(int x, int y, int headDirection, Snake *prev, Snake *next)
{
  Snake *snake = (Snake *)malloc(sizeof(Snake));
  
  snake->x = x;
  snake->y = y;
  snake->headDirection = headDirection;
  snake->prev = prev;
  snake->next = next;
  
  return snake;
}

void deleteNode(Snake *nodeToDelete)
{
  free(nodeToDelete);
}

void randomNewFood()
{
  int x = 0, y = 0;
  while(!checkCell(x, y, BLANK_CELL))
  {
    x = random(1, BOARD_WIDTH-1);
    y = random(1, BOARD_HEIGHT-1);  
  }

  setCell(x, y, FOOD_CELL);
}

void randomNewItem()
{ 
  int x = 0, y = 0;
  while(!checkCell(x, y, BLANK_CELL))
  {
    x = random(1, BOARD_WIDTH-1);
    y = random(1, BOARD_HEIGHT-1);  
  }

  setCell(x, y, ITEM_CELL);
}

//_______________________________________________________________________working function

void init()
{    
  for(int x = 0; x < BOARD_WIDTH; x++)
  {
    for(int y = 0; y < BOARD_HEIGHT; y++)
    {      
      if(x == 0 || x == BOARD_WIDTH-1) 
        setCell(x, y, WALL_CELL);
      else if(y == 0 || y == BOARD_HEIGHT-1) 
        setCell(x, y, WALL_CELL);
      else
        setCell(x, y, BLANK_CELL);
    }
  }
  
  direction = RIGHT;
  tailSnake = headSnake = createNode(2, 5, direction, 0, 0);
  setCell(headSnake->x, headSnake->y, SNAKE_CELL);

  for(int i = 1; i < 3; i++)
  {
    headSnake->next = createNode(2 + i, 5, direction, headSnake, 0);
    headSnake = headSnake->next;
    setCell(headSnake->x, headSnake->y, SNAKE_CELL);
  }

  for(int i = 0; i < FOOD_COUNT; i++)  
    randomNewFood();  

  for(int i = 0; i < ITEM_COUNT; i++)
    randomNewItem();
}

void userInput()
{
  if(kbhit())
  {
    int directionToMove = getch();
  
    switch (directionToMove){
      case 'w':
        if(direction != DOWN)
          direction = UP;
        break;
      case 's':
        if(direction != UP)
          direction = DOWN;
        break;
      case 'a':
        if(direction != RIGHT)
          direction = LEFT;
        break;
      case 'd':
        if(direction != LEFT)
          direction = RIGHT;
        break;
    }
  }
}

int moveSnake()
{ 
  switch(direction){
    case RIGHT:
      headSnake->next = createNode(headSnake->x + 1, headSnake->y, RIGHT, headSnake, 0);
      headSnake = headSnake->next;
      break;
    case LEFT:
      headSnake->next = createNode(headSnake->x - 1, headSnake->y, LEFT, headSnake, 0);
      headSnake = headSnake->next;
      break;
    case UP:
      headSnake->next = createNode(headSnake->x, headSnake->y - 1, UP, headSnake, 0);
      headSnake = headSnake->next;
      break;
    case DOWN:
      headSnake->next = createNode(headSnake->x, headSnake->y + 1, DOWN, headSnake, 0);
      headSnake = headSnake->next;
      break;    
  }
  
  if(checkCell(headSnake->x, headSnake->y, WALL_CELL) || 
    checkCell(headSnake->x, headSnake->y, SNAKE_CELL))
    return 0;

  if(checkCell(headSnake->x, headSnake->y, ITEM_CELL)) {
    randomNewItem();
  } else {
    
  }


  if(checkCell(headSnake->x, headSnake->y, FOOD_CELL))  {
    randomNewFood();
  } else {        
    setCell(tailSnake->x, tailSnake->y, BLANK_CELL);  
    tailSnake = tailSnake->next;
    deleteNode(tailSnake->prev);
    tailSnake->prev = 0;  
  }

  setCell(headSnake->x, headSnake->y, SNAKE_CELL);

  return 1;
}

void run()
{
  do
  {
    Sleep(SLEEP_DELAY);
    userInput();
  } while(moveSnake());
}

char selectedMenu()
{
  for(int x = 0; x < BOARD_WIDTH; x++)
    for(int y = 0; y < BOARD_HEIGHT; y++)   
      putxy(x, y, ' ');
  
  gotoxy(5, 5);
  puts("My Snake");

  gotoxy(5, 7);
  puts("type anykey for play snake");

  gotoxy(5, 9);
  puts("type 'ESC' for quit game");

  return getch();
}

void stop()
{
  gotoxy(10, 8);  
  puts("****************");
  gotoxy(10, 9);
  puts("*              *");
  gotoxy(10, 10);
  puts("*  Game Over!  *");  
  gotoxy(10, 11);
  puts("*              *");
  gotoxy(10, 12);
  puts("****************");

  getch();
}

int _tmain(int argc, _TCHAR* argv[])
{ 
  while(1){
    if(selectedMenu() == 0x1b) 
      break;

    init();
    run();
    stop();
  }
  
	return 0;
}