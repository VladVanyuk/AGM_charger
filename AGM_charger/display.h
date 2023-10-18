#include "includes.h"

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

void displaySetup()
{
    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(WHITE);
    display.display();
    display.clearDisplay();
}


void display_voltage(float voltage)
{
  display.fillRect(64, 0, 40, 10, BLACK);
  display.setTextSize(1);
  display.setCursor(64, 0);
  display.println(voltage);
  display.setCursor(94, 0);
  display.println("v");
  display.display();
}

void display_SOC_voltage()
{
  display_voltage(SOC_voltage);
}

void display_CCV_voltage()
{
  display_voltage(CCV_voltage); 
}

////////////////////////////////////////////


void print_stage_indicator(String stage)
{
  display.fillRect(100, 32, 28, 32, BLACK);
  display.setTextSize(1);
  display.setCursor(100, 32);
  display.println(stage);
  display.display();
}

void print_stage1()
{
  print_stage_indicator("st_1");
}

void print_stage2()
{
  print_stage_indicator("st_2");
}

void print_stage3()
{
  print_stage_indicator("st_3");
}

void print_stop_flag_indicator()
{
  print_stage_indicator("st_F");
}

///////////////////////////////


void charging()
{
  display.drawRoundRect(0, 0, 48, 10, 2, WHITE);
  display.display();

  for (uint8_t i = 2; i <= 44; i+7)
  {
    display.fillRect(i, 2, 5, 6, WHITE);
    display.display();
    delay(100);
  }

  display.fillRect(2, 2, 44, 6, BLACK);
  display.display();
}

void charged()
{
  display.drawRoundRect(0, 0, 48, 10, 2, WHITE);

  for (uint8_t i = 2; i <= 44; i+7)
  {
    display.fillRect(i, 2, 5, 6, WHITE);
  }
  
  display.display();
}

void no_bat()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("NO BATTERY");
  display.display();
}

void print_source_detector()
{
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.println("Source:");
  display.display();

  display.fillRect(42, 50, 40, 14, BLACK);
  display.setTextSize(1);
  display.setCursor(42, 50);
  display.print((flags.AC == true) ? "AC mains" : "Batt");
  
  display.display();
}
