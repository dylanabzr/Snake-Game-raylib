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

class GameStates{
public:
  GameStates(){
  for (int i = 0; i < squares_quantity; i++){
    square[i].posX = 100 + 2 * i;
    square[i].posY = 100 + 2 * i;
}
  }
  int lose {0};
  std::array <SnakeSquare, 1000> square;
  unsigned squares_quantity {20};
  Fruit fruit;
  int fruiteaten {0};
  int movementStateX = 1, movementStateY = 0;
  int points;
};

class CollisionANDFruit{
  public:
    static void fruitSpawn(GameStates &s){
      if (!s.fruiteaten){
        s.fruit.posX = GetRandomValue(20, 330);
        s.fruit.posY = GetRandomValue(20, 330);
        s.fruiteaten = 1;
      }
    }
    static int fruitCollision(SnakeSquare square, Fruit fruit){
      return 
        (square.posX + square.width >= fruit.posX &&
        square.posX <= fruit.posX + fruit.radius * 2 &&
        square.posY + square.height >= fruit.posY &&
        square.posY <= fruit.posY + fruit.radius * 2);
    }
    static void collision(GameStates &s){
      std::array <SnakeSquare, 1000> square = s.square; 
      if((square[0].posX > 350)||(square[0].posX < 0) || (square[0].posY > 350) || (square[0].posY < 0)){
        s.lose = 1;
        return;
      }
     for (int i = 1; i < s.squares_quantity; i++){ 
        if (square[0].posX == square[i].posX && square[0].posY == square[i].posY) {
          s.lose = 1;
          return;
        }
      }
      if (fruitCollision(square[0], s.fruit)){
          s.fruiteaten = 0;
          s.squares_quantity++;
      }
    }
};

class Movement{
  public:
    static void KeyInput(GameStates &s){
      if(!s.movementStateX){
        s.movementStateX = IsKeyDown(KEY_RIGHT) + IsKeyDown(KEY_D) - IsKeyDown(KEY_LEFT) - IsKeyDown(KEY_A); 
        s.movementStateY *= !s.movementStateX;
      }
      if(!s.movementStateY){
        s.movementStateY = IsKeyDown(KEY_DOWN) + IsKeyDown(KEY_S) - IsKeyDown(KEY_UP) - IsKeyDown(KEY_W);
        s.movementStateX *= !s.movementStateY;
      }  
    } 
    static void snakeMovement(GameStates &s){

      for (unsigned i = s.squares_quantity; i > 0; i--){
        s.square[i].posX = s.square[i-1].posX;
        s.square[i].posY = s.square[i-1].posY;
      }
      s.square[0].posX += 3 * s.movementStateX;
      s.square[0].posY += 3 * s.movementStateY;

      
    } 
};


class Graphics{
  public:
    static void gameUI(GameStates &s){ 
      CollisionANDFruit::fruitSpawn(s);
      Movement::snakeMovement(s);
      CollisionANDFruit::collision(s);
      ClearBackground(BLACK);
      BeginDrawing();
      s.points = s.squares_quantity - 20;
      DrawText(TextFormat("Score: %d", s.points), 10, 10, 10, GRAY);
      if(s.fruiteaten){
        DrawCircle(s.fruit.posX, s.fruit.posY, s.fruit.radius, RED);
      }
      for (int i = 0; i < s.squares_quantity; i++){
        DrawRectangle(s.square[i].posX, s.square[i].posY, s.square[0].width, s.square[0].height, RAYWHITE);
      }
      EndDrawing();

  }
};

class WindowLoop{
  private:
    static void windowLoop(){
      GameStates states;
      SetRandomSeed(std::time(nullptr));
      InitWindow(350, 350, "Snake");
      SetTargetFPS(60);
      while(!WindowShouldClose()){
        if (IsKeyDown(KEY_ESCAPE)){
          WindowShouldClose();
        }
        if(!states.lose){
          Movement::KeyInput(states);
          Graphics::gameUI(states);
          continue;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(TextFormat("\t\t\t\tYou died.\n\nYour final score was: %d", states.points), 20, 100, 24, LIGHTGRAY);
        DrawText("Press ESC to quit.\nPress Enter to play again." ,40, 180, 18, GRAY);
        EndDrawing();
        if (IsKeyDown(KEY_ENTER)){
          resetGame(states);
        }
          
      }
    CloseWindow();
    }
    static void resetGame(GameStates &s){
      s.lose = 0;
      s.points = 0;
      s.squares_quantity = 20;
      for (int i = 0; i < s.squares_quantity; i++){
        s.square[i].posX = 50 + 2 * i;
        s.square[i].posY = 150 + 2 * i;
      }
      s.movementStateX = 1; s.movementStateY = 0;

    }
  public:
    static void startGame(){
      windowLoop();
    }
  };

int main(){
  WindowLoop::startGame();
}
