#include <MD_MAX72xx.h>
#include <SPI.h>
#include "font.h"

#define MODULES_X 5
#define MODULES_Y 2
#define MODULES_COUNT MODULES_X*MODULES_Y
#define PIXELS_X MODULES_X*8
#define PIXELS_Y MODULES_Y*8
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 10
#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    9  // or SS

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
bool FrameBuffer[PIXELS_Y][PIXELS_X];

void clearBuffer()
{
  for (size_t i = 0; i < PIXELS_Y; i++)
    for (size_t j = 0; j < PIXELS_X; j++)
      FrameBuffer[i][j] = false;
}

void resetMatrix(void)
{
  mx.control(MD_MAX72XX::INTENSITY, 0);
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  mx.clear();
}

void redisplay()
{
  //mx.clear();
  mx.update(MD_MAX72XX::OFF);

  for (size_t i = 0; i < PIXELS_Y / 2; i++)
  {
    for (size_t j = 0; j < PIXELS_X; j++)
    {
      mx.setPoint(i, PIXELS_X - j - 1, FrameBuffer[i][j]);
    }
  }

  for (size_t i = PIXELS_Y / 2; i < PIXELS_Y; i++)
  {
    for (size_t j = 0; j < PIXELS_X; j++)
    {
      mx.setPoint(i - (PIXELS_Y / 2), PIXELS_X - j + 39, FrameBuffer[i][j]);
    }
  }

  mx.update(MD_MAX72XX::ON);
}

void draw8x16(size_t posY, size_t posX, uint8_t number)
{
  if (number > 9 || number < 0) number = 0;

  for (size_t i = 0; i < 16; i++)
  {
    for (size_t j = 0; j < 8; j++)
    {
      FrameBuffer[posY + i][posX + j] = pgm_read_byte(&(font8x16[number][i])) & (1 << (7 - j));
    }
  }
}

void setup()
{
  clearBuffer();
  mx.begin();
  resetMatrix();
}

void loop()
{
  draw8x16(0, 0, 0);
  draw8x16(0, 9, 1);
  draw8x16(0, 23, 2);
  draw8x16(0, 32, 3);
}
