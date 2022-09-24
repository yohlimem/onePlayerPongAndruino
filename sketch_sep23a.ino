#include "LedControl.h"

class Ball{
  private:
    bool isInside;

  public:

    double move[2] = {0, 0};
    double position[2] = {4, 4};
    double size = 1;

    Ball(double startVelocity[2]){
      move[0] = startVelocity[0];
      move[1] = startVelocity[1];
      size = 1;
    }
    Ball(double startVelocity[2], double startSize){
      move[0] = startVelocity[0];
      move[1] = startVelocity[1];
      size = startSize;
    }
    Ball(double startVelocity[2], double startSize, double startPostion[2]){
      move[0] = startVelocity[0];
      move[1] = startVelocity[1];
      position[0] = startPostion[0];
      position[1] = startPostion[1];
      size = startSize;
    }
    void ChangeVelocity(double velocity[2]){
      move[0] = velocity[0];
      move[1] = velocity[1];
      UpdatePostion();
    }
    void HitWall(bool up, bool right){
      if(right){
        move[0] = -move[0];
      }
      if(up){
        move[1] = -move[1];
      }
      // UpdatePostion();
    }
    void UpdatePostion(){
      position[0] += move[0];
      position[1] += move[1];

      if((position[0] >= 7 || position[1] >= 7 || position[0] <= 0 || position[1] <= 0) && isInside == false){
        bool isInside = true;
        HitWall(position[1] >= 7 || position[1] <= 0, position[0] >= 7 || position[0] <= 0);
      } else if((position[0] <= 7 || position[1] <= 7 || position[0] >= 0 || position[1] >= 0) && isInside == true){
        bool isInside = false;
      }
      delay(100);
    }

};


int DIN = 7;
int CS =  6;
int CLK = 5;

byte e[8]=     {0x7C,0x7C,0x60,0x7C,0x7C,0x60,0x7C,0x7C};
byte d[8]=     {0x78,0x7C,0x66,0x66,0x66,0x66,0x7C,0x78};
byte u[8]=     {0x66,0x66,0x66,0x66,0x66,0x66,0x7E,0x7E};
byte c[8]=     {0x7E,0x7E,0x60,0x60,0x60,0x60,0x7E,0x7E};
byte eight[8]= {0x7E,0x7E,0x66,0x7E,0x7E,0x66,0x7E,0x7E};
byte s[8]=     {0x7E,0x7C,0x60,0x7C,0x3E,0x06,0x3E,0x7E};
byte dot[8]=   {0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18};
byte o[8]=     {0x7E,0x7E,0x66,0x66,0x66,0x66,0x7E,0x7E};
byte m[8]=     {0xE7,0xFF,0xFF,0xDB,0xDB,0xDB,0xC3,0xC3};
byte amongUs[8] = {0x00,0x1E,0x3E,0x3E,0x1C,0x14,0x00,0x00};
byte test[8] = {0xFF,0xFF,0x22,0x22,0xFF,0xFF,0xFF,0xFF};
LedControl lc=LedControl(DIN,CLK,CS,0);

int movePaddle = 0;
int movePaddleAnalog = 0;

byte paddle[6][8] = {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0},{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,},{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,},{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,},{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,},{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,}};

double startMovingVec[2] = {1, 0.5d};

Ball ball = Ball(startMovingVec);



void setup(){
 lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
 lc.setIntensity(0,1);      // Set the brightness to maximum value
 lc.clearDisplay(0);         // and clear the display
 pinMode(0, INPUT);
 Serial.begin(9600);
// ShowAllPaddles();
}

void loop(){ 
  if(ball.position[1] == 7){
    GameOver();
    return;
  }
  RenderPaddle();
  RenderBall();
  
  // Serial.println(ball.move[0]);
  // Serial.println(ball.move[1]);
  ball.UpdatePostion();
  HandlePaddlePostion();

  // delay(700);


  delay(20);
  lc.clearDisplay(0);
}

void RenderPaddle(){
    movePaddle = analogRead(movePaddleAnalog);
    // Serial.println(movePaddle);

    movePaddle = map(movePaddle, 0, 1021, 1, 7);
    lc.setLed(0,6,movePaddle,true);
    lc.setLed(0,6,movePaddle + 1,true);
    lc.setLed(0,6,movePaddle - 1,true);
}

void HandlePaddlePostion(){
  if((ball.position[1] == 5 || ball.position[1] == 6) && (ball.position[0] == movePaddle || ball.position[0] == movePaddle - 1 || ball.position[0] == movePaddle + 1 || ball.position[0] + ball.move[0] == movePaddle )){
    ball.HitWall(true, false);
  }
}

void ShowAllPaddles(){
  for(int i = 0; i < sizeof(paddle); i++){
    // printByte(paddle[i]);
    delay(1000);

  }

}
void RenderBall(){
  lc.setLed(0, floor(ball.position[1]), floor(ball.position[0]), true);
  Serial.println("x:");
  Serial.println((ball.position[0]));
  Serial.println("y:");
  Serial.println(ball.position[1]);


}
void GameOver(){
  // while (true) {
    // lc.setRow(0, 0, B0000000000000000000000000000000000000000000000000000000000000000);
    // lc.setRow(0, 1, B0011100000010000010000100111110000111000010001000111110001111000);//g
    // lc.setRow(0, 2, B0100000000101000011001100100000001000100010001000100000001000100);
    // lc.setRow(0, 3, B0101110000101000011001100111100001000100001010000111100001000100);
    // lc.setRow(0, 4, B0100010001111100010110100100000001000100001010000100000001111000);
    // lc.setRow(0, 5, B0100010001000100010110100100000001000100000100000100000001001000);
    // lc.setRow(0, 6, B0011100001000100010000100111110000111000000100000111110001000100);
    // lc.setRow(0, 7, B00000000000000000000000.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.000000000000000000000000);
    // delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00111000);//a
    lc.setRow(0, 2, B01000000);
    lc.setRow(0, 3, B01011100);
    lc.setRow(0, 4, B01000100);
    lc.setRow(0, 5, B01000100);
    lc.setRow(0, 6, B00111000);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B01110000);//a
    lc.setRow(0, 2, B10000000);
    lc.setRow(0, 3, B10111000);
    lc.setRow(0, 4, B10001000);
    lc.setRow(0, 5, B10001000);
    lc.setRow(0, 6, B01110000);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B11100000);//a
    lc.setRow(0, 2, B00000000);
    lc.setRow(0, 3, B01110000);
    lc.setRow(0, 4, B00010001);
    lc.setRow(0, 5, B00010001);
    lc.setRow(0, 6, B11100001);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B11000000);//a
    lc.setRow(0, 2, B00000001);
    lc.setRow(0, 3, B11100001);
    lc.setRow(0, 4, B00100011);
    lc.setRow(0, 5, B00100010);
    lc.setRow(0, 6, B11000010);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B10000001);//a
    lc.setRow(0, 2, B00000010);
    lc.setRow(0, 3, B11000010);
    lc.setRow(0, 4, B01000111);
    lc.setRow(0, 5, B01000100);
    lc.setRow(0, 6, B10000100);
    lc.setRow(0, 7, B00000000);
    delay(100);
    // lc.setRow(0, 0, B00000000);
    // lc.setRow(0, 1, B10000001);//a
    // lc.setRow(0, 2, B00000010);
    // lc.setRow(0, 3, B11000010);
    // lc.setRow(0, 4, B01000111);
    // lc.setRow(0, 5, B01000100);
    // lc.setRow(0, 6, B10000100);
    // lc.setRow(0, 7, B00000000);
    // delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00000010);//a
    lc.setRow(0, 2, B00000101);
    lc.setRow(0, 3, B10000101);
    lc.setRow(0, 4, B10001111);
    lc.setRow(0, 5, B10001000);
    lc.setRow(0, 6, B00001000);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00000100);//a
    lc.setRow(0, 2, B00001010);
    lc.setRow(0, 3, B00001010);
    lc.setRow(0, 4, B00011111);
    lc.setRow(0, 5, B00010001);
    lc.setRow(0, 6, B00010001);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00010000);//a
    lc.setRow(0, 2, B00101000);
    lc.setRow(0, 3, B00101000);
    lc.setRow(0, 4, B01111100);
    lc.setRow(0, 5, B01000100);
    lc.setRow(0, 6, B01000100);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00100000);//a
    lc.setRow(0, 2, B01010000);
    lc.setRow(0, 3, B01010000);
    lc.setRow(0, 4, B11111000);
    lc.setRow(0, 5, B10001000);
    lc.setRow(0, 6, B10001000);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B01000001);//a
    lc.setRow(0, 2, B10100001);
    lc.setRow(0, 3, B10100001);
    lc.setRow(0, 4, B11110001);
    lc.setRow(0, 5, B00010001);
    lc.setRow(0, 6, B00010001);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B10000010);//a
    lc.setRow(0, 2, B01000011);
    lc.setRow(0, 3, B01000011);
    lc.setRow(0, 4, B11100010);
    lc.setRow(0, 5, B00100010);
    lc.setRow(0, 6, B00100010);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00010000);//a
    lc.setRow(0, 2, B00011001);
    lc.setRow(0, 3, B00011001);
    lc.setRow(0, 4, B00010110);
    lc.setRow(0, 5, B00010110);
    lc.setRow(0, 6, B00010000);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00100001);//a
    lc.setRow(0, 2, B00110011);
    lc.setRow(0, 3, B00110011);
    lc.setRow(0, 4, B00101101);
    lc.setRow(0, 5, B00101101);
    lc.setRow(0, 6, B00100001);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B10000100);//a
    lc.setRow(0, 2, B11001100);
    lc.setRow(0, 3, B11001100);
    lc.setRow(0, 4, B10110100);
    lc.setRow(0, 5, B10110100);
    lc.setRow(0, 6, B10000100);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00001001);//a
    lc.setRow(0, 2, B10011001);
    lc.setRow(0, 3, B10011001);
    lc.setRow(0, 4, B01101001);
    lc.setRow(0, 5, B01101001);
    lc.setRow(0, 6, B00001001);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00010011);//a
    lc.setRow(0, 2, B00110010);
    lc.setRow(0, 3, B00110011);
    lc.setRow(0, 4, B11010010);
    lc.setRow(0, 5, B11010010);
    lc.setRow(0, 6, B00010011);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00100111);//a
    lc.setRow(0, 2, B01100100);
    lc.setRow(0, 3, B01100111);
    lc.setRow(0, 4, B10100100);
    lc.setRow(0, 5, B10100100);
    lc.setRow(0, 6, B00100111);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B10011111);//a
    lc.setRow(0, 2, B10010000);
    lc.setRow(0, 3, B10011110);
    lc.setRow(0, 4, B10010000);
    lc.setRow(0, 5, B10010000);
    lc.setRow(0, 6, B10011111);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00111110);//a
    lc.setRow(0, 2, B00100000);
    lc.setRow(0, 3, B00111100);
    lc.setRow(0, 4, B00100000);
    lc.setRow(0, 5, B00100000);
    lc.setRow(0, 6, B00111110);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B0000000);
    lc.setRow(0, 1, B1111100);//a
    lc.setRow(0, 2, B1000000);
    lc.setRow(0, 3, B1111000);
    lc.setRow(0, 4, B1000000);
    lc.setRow(0, 5, B1000000);
    lc.setRow(0, 6, B1111100);
    lc.setRow(0, 7, B0000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B11110000);//a
    lc.setRow(0, 2, B00000001);
    lc.setRow(0, 3, B11100001);
    lc.setRow(0, 4, B00000001);
    lc.setRow(0, 5, B00000001);
    lc.setRow(0, 6, B11110000);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B11000011);//a
    lc.setRow(0, 2, B00000100);
    lc.setRow(0, 3, B10000100);
    lc.setRow(0, 4, B00000100);
    lc.setRow(0, 5, B00000100);
    lc.setRow(0, 6, B11000011);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00001110);//a
    lc.setRow(0, 2, B00010001);
    lc.setRow(0, 3, B00010001);
    lc.setRow(0, 4, B00010001);
    lc.setRow(0, 5, B00010001);
    lc.setRow(0, 6, B00001110);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00011100);//a
    lc.setRow(0, 2, B00100010);
    lc.setRow(0, 3, B00100010);
    lc.setRow(0, 4, B00100010);
    lc.setRow(0, 5, B00100010);
    lc.setRow(0, 6, B00011100);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00111000);//a
    lc.setRow(0, 2, B01000100);
    lc.setRow(0, 3, B01000100);
    lc.setRow(0, 4, B01000100);
    lc.setRow(0, 5, B01000100);
    lc.setRow(0, 6, B00111000);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B11100001);//a
    lc.setRow(0, 2, B00010001);
    lc.setRow(0, 3, B00010000);
    lc.setRow(0, 4, B00010000);
    lc.setRow(0, 5, B00010000);
    lc.setRow(0, 6, B11100000);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B0000000);
    lc.setRow(0, 1, B1000010);//a
    lc.setRow(0, 2, B0100010);
    lc.setRow(0, 3, B0100001);
    lc.setRow(0, 4, B0100001);
    lc.setRow(0, 5, B0100000);
    lc.setRow(0, 6, B1000000);
    lc.setRow(0, 7, B0000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00010001);//a
    lc.setRow(0, 2, B00010001);
    lc.setRow(0, 3, B00001010);
    lc.setRow(0, 4, B00001010);
    lc.setRow(0, 5, B00000100);
    lc.setRow(0, 6, B00000100);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B01000100);//a
    lc.setRow(0, 2, B01000100);
    lc.setRow(0, 3, B00101000);
    lc.setRow(0, 4, B00101000);
    lc.setRow(0, 5, B00010000);
    lc.setRow(0, 6, B00010000);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00010001);//a
    lc.setRow(0, 2, B00010001);
    lc.setRow(0, 3, B10100001);
    lc.setRow(0, 4, B10100001);
    lc.setRow(0, 5, B01000001);
    lc.setRow(0, 6, B01000001);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B01000111);//a
    lc.setRow(0, 2, B01000100);
    lc.setRow(0, 3, B10000111);
    lc.setRow(0, 4, B10000100);
    lc.setRow(0, 5, B00000100);
    lc.setRow(0, 6, B00000111);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00011111);//a
    lc.setRow(0, 2, B00010000);
    lc.setRow(0, 3, B00011110);
    lc.setRow(0, 4, B00010000);
    lc.setRow(0, 5, B00010000);
    lc.setRow(0, 6, B00011111);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00111110);//a
    lc.setRow(0, 2, B00100000);
    lc.setRow(0, 3, B00111100);
    lc.setRow(0, 4, B00100000);
    lc.setRow(0, 5, B00100000);
    lc.setRow(0, 6, B00111110);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B01111100);//a
    lc.setRow(0, 2, B01000000);
    lc.setRow(0, 3, B01111000);
    lc.setRow(0, 4, B01000000);
    lc.setRow(0, 5, B01000000);
    lc.setRow(0, 6, B01111100);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B11111000);//a
    lc.setRow(0, 2, B10000000);
    lc.setRow(0, 3, B11110000);
    lc.setRow(0, 4, B10000000);
    lc.setRow(0, 5, B10000000);
    lc.setRow(0, 6, B11111000);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B11100011);//a
    lc.setRow(0, 2, B00000010);
    lc.setRow(0, 3, B11000010);
    lc.setRow(0, 4, B00000011);
    lc.setRow(0, 5, B00000010);
    lc.setRow(0, 6, B11100010);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00011110);//a
    lc.setRow(0, 2, B00010001);
    lc.setRow(0, 3, B00010001);
    lc.setRow(0, 4, B00011110);
    lc.setRow(0, 5, B00010010);
    lc.setRow(0, 6, B00010001);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00111100);//a
    lc.setRow(0, 2, B00100010);
    lc.setRow(0, 3, B00100010);
    lc.setRow(0, 4, B00111100);
    lc.setRow(0, 5, B00100100);
    lc.setRow(0, 6, B00100010);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B01111000);//a
    lc.setRow(0, 2, B01000100);
    lc.setRow(0, 3, B01000100);
    lc.setRow(0, 4, B01111000);
    lc.setRow(0, 5, B01001000);
    lc.setRow(0, 6, B01000100);
    lc.setRow(0, 7, B00000000);
    delay(100);
    // lc.setRow(0, 0, B00000000);
    // lc.setRow(0, 1, B01111000);//a
    // lc.setRow(0, 2, B01000100);
    // lc.setRow(0, 3, B01000100);
    // lc.setRow(0, 4, B01111000);
    // lc.setRow(0, 5, B01001000);
    // lc.setRow(0, 6, B01000100);
    // lc.setRow(0, 7, B00000000);
    // delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B11110000);//a
    lc.setRow(0, 2, B10001001);
    lc.setRow(0, 3, B10001001);
    lc.setRow(0, 4, B11110001);
    lc.setRow(0, 5, B10010001);
    lc.setRow(0, 6, B10001000);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B11100001);//a
    lc.setRow(0, 2, B00010010);
    lc.setRow(0, 3, B00010010);
    lc.setRow(0, 4, B11100010);
    lc.setRow(0, 5, B00100010);
    lc.setRow(0, 6, B00010001);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B11000011);//a
    lc.setRow(0, 2, B00100100);
    lc.setRow(0, 3, B00100101);
    lc.setRow(0, 4, B11000100);
    lc.setRow(0, 5, B01000100);
    lc.setRow(0, 6, B00100011);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B0000000);
    lc.setRow(0, 1, B10000111);//a
    lc.setRow(0, 2, B01001000);
    lc.setRow(0, 3, B01001011);
    lc.setRow(0, 4, B10001000);
    lc.setRow(0, 5, B10001000);
    lc.setRow(0, 6, B01000111);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00001110);//a
    lc.setRow(0, 2, B10010000);
    lc.setRow(0, 3, B10010111);
    lc.setRow(0, 4, B00010001);
    lc.setRow(0, 5, B00010001);
    lc.setRow(0, 6, B10001110);
    lc.setRow(0, 7, B00000000);
    delay(100);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 1, B00011100);//a
    lc.setRow(0, 2, B00100000);
    lc.setRow(0, 3, B00101110);
    lc.setRow(0, 4, B00100010);
    lc.setRow(0, 5, B00100010);
    lc.setRow(0, 6, B00011100);
    lc.setRow(0, 7, B00000000);
  // // }
}



