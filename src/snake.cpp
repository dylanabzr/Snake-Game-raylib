#include <raylib.h>
#include <unistd.h> 
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <array>

struct SnakeSquare{
  int width = 25, height = 25;
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
  int lose {0};
  std::array <SnakeSquare, 1000> square;
  unsigned squares_quantity {20};
};

class CollisionANDFruit : public Game{
  protected:
    Fruit fruit;
    int fruiteaten {0};
    void fruitSpawn(){
      if (!fruiteaten){
      SetRandomSeed(square[0].posX + squares_quantity + std::time(nullptr));
      fruit.posX = GetRandomValue(20, 330);
      SetRandomSeed(square[0].posY + squares_quantity + std::time(nullptr));
      fruit.posY = GetRandomValue(20, 330);
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
      if((square[0].posX > 350)||(square[0].posX < 0) || (square[0].posY > 350) || (square[0].posY < 0)){
        lose = 1;
      }
     for (int i = 1; i < squares_quantity; i++){ 
        if (square[0].posX == square[i].posX && square[0].posY == square[i].posY) {
          lose = 1;
          break;
        }
      }
      if (fruitCollision(square[0], fruit)){
          fruiteaten = 0;
          squares_quantity++;
        }
    }
};

class Movement : public CollisionANDFruit{
  protected:
    void KeyInput(){
      if(!movementStateX){
        movementStateX = IsKeyDown(KEY_RIGHT) + IsKeyDown(KEY_D) - IsKeyDown(KEY_LEFT) - IsKeyDown(KEY_A); 
        movementStateY *= !movementStateX;
      }
      if(!movementStateY){
        movementStateY = IsKeyDown(KEY_DOWN) + IsKeyDown(KEY_S) - IsKeyDown(KEY_UP) - IsKeyDown(KEY_W);
        movementStateX *= !movementStateY;
      }  
    } 
    int movementStateX = 1, movementStateY = 0;
    void snakeMovement(){

      for (unsigned i = squares_quantity; i > 0; i--){
        square[i].posX = square[i-1].posX;
        square[i].posY = square[i-1].posY;
      }
      square[0].posX += 3 * movementStateX;
      square[0].posY += 3 * movementStateY;

      
    } 
};


class Graphics : public Movement{
  protected:
    int points;
    void gameUI(){ 
      fruitSpawn();
      snakeMovement();
      collision();
      ClearBackground(BLACK);
      BeginDrawing();
      points = squares_quantity - 20;
      DrawText(TextFormat("Score: %d", points), 10, 10, 10, GRAY);
      if(fruiteaten){
        DrawCircle(fruit.posX, fruit.posY, fruit.radius, RED);
      }
      for (int i = 0; i < squares_quantity; i++){
        DrawRectangle(square[i].posX, square[i].posY, square[0].width, square[0].height, RAYWHITE);
      }
      EndDrawing();

  }
};

class WindowLoop : public Graphics{
  private:
    void windowLoop(){
      InitWindow(350, 350, "Snake");
      SetTargetFPS(60);
      while(!WindowShouldClose()){
        if (IsKeyDown(KEY_ESCAPE)){
          WindowShouldClose();
        }
        if(!lose){
          KeyInput();
          gameUI();
        }
        else{
          BeginDrawing();
          ClearBackground(BLACK);
          DrawText(TextFormat("        You died.\n\nYour final score was: %d", points), 20, 100, 24, LIGHTGRAY);
          DrawText("Press ESC to quit.\nPress Enter to play again." ,40, 180, 18, GRAY);
          EndDrawing();
          if (IsKeyDown(KEY_ENTER)){
            resetGame();
        }
        }
          
      }
    CloseWindow();
    }
    void resetGame(){
      lose = 0;
      points = 0;
      squares_quantity = 20;
      for (int i = 0; i < squares_quantity; i++){
        square[i].posX = 50 + 2 * i;
        square[i].posY = 150 + 2 * i;
      }
      movementStateX = 1; movementStateY = 0;

    }
  public:
    void startGame(){
      windowLoop();
  }
  };

int main(){
  WindowLoop jogo;
  jogo.startGame();
}
