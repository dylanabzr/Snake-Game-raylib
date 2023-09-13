#include <raylib.h>
#include <unistd.h> 
#include <cstdlib>
#include <ctime>

struct SnakeSquare{
  int width = 35, height = 35;
  int posX, posY;
};

struct Fruit{
  int radius = 10;
  int posX, posY;
};

class Game{
public:
  Game(){
  for (int i = 0; i < squares_quantity; i++){
    square[i].posX = 100 + 2 * i;
    square[i].posY = 100 + 2 * i;
}
  }
protected:
  int lose = 0;
  SnakeSquare square[210];
  unsigned char squares_quantity = 20;
};

class CollisionANDFruit : public Game{
  protected:
    Fruit fruit;
    int fruiteaten = 0;
    void fruitSpawn(){
      if (!fruiteaten){
      SetRandomSeed(square[0].posX + squares_quantity + std::time(nullptr));
      fruit.posX = GetRandomValue(20, 430);
      SetRandomSeed(square[0].posY + squares_quantity + std::time(nullptr));
      fruit.posY = GetRandomValue(20, 430);
      fruiteaten = 1;
      }
    }
    int fruitCollision(SnakeSquare square, Fruit fruit){
      return (square.posX + square.width >= fruit.posX &&
        square.posX <= fruit.posX + fruit.radius * 2 &&
        square.posY + square.height >= fruit.posY &&
        square.posY <= fruit.posY + fruit.radius * 2);
    }
    void collision(){
      if((square[0].posX > 450)||(square[0].posX < 0) || (square[0].posY > 450) || (square[0].posY < 0))
        lose = 1;
     for (int i = 1; i < squares_quantity; i++) 
        if (square[0].posX == square[i].posX && square[0].posY == square[i].posY) {
          lose = 1;
          break;
        }
      for(int i = 0; i < squares_quantity; i++){
        if (fruitCollision(square[i], fruit)){
          fruiteaten = 0;
          squares_quantity++;
        }
      }
    }
};

class Movement : public CollisionANDFruit{
  protected:
    void KeyInput(){
      if(!movementStateX){
        movementStateX = (1 * IsKeyDown(KEY_RIGHT)) + (-1 * IsKeyDown(KEY_LEFT)); 
        movementStateY *= !movementStateX;
      }
      if(!movementStateY){
        movementStateY = (1 * IsKeyDown(KEY_DOWN)) + (-1 * IsKeyDown(KEY_UP));
        movementStateX *= !movementStateY;
      }  
    } 
    int movementStateX = 1, movementStateY = 0;
    void snakeMovement(){
      //usleep(150000);
      for (unsigned char i = squares_quantity; i > 0; i--){
        square[i].posX = square[i-1].posX;
        square[i].posY = square[i-1].posY;
      }
      square[0].posX += 3 * movementStateX;
      square[0].posY += 3 * movementStateY;

      
    } 
};


class Graphics : public Movement{
  protected:
    void gameUI(){
      ClearBackground(BLACK); 
      fruitSpawn();
      snakeMovement();
      collision();
      BeginDrawing();
      for (int i = 0; i < squares_quantity; i++){
        DrawRectangle(square[i].posX, square[i].posY, square[0].width, square[0].height, RAYWHITE);
      }
      DrawCircle(fruit.posX, fruit.posY, fruit.radius, RED);
      EndDrawing();
      ClearBackground(BLACK); 

  }
};

class WindowLoop : public Graphics{
  public:
    void windowLoop(){
      InitWindow(450, 450, "Snake");
      SetTargetFPS(60);
      while(!WindowShouldClose()){
        if (IsKeyDown(KEY_ESCAPE))
          break;
        if(!lose){
          KeyInput();
          gameUI();
        }
        else{
          BeginDrawing();
          ClearBackground(BLACK);
          DrawText("You Lost. Press ESC to quit.", 50, 100, 20, GRAY);
          EndDrawing();
      }
    }
    CloseWindow();
  }
};

int main(){
  WindowLoop jogo;
  jogo.windowLoop();
}

