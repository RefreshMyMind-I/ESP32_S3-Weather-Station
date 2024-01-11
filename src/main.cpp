#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_AHTX0.h>
#include "Adafruit_LTR390.h"
#include <GP2YDustSensor.h>
#include "SPI.h"
#include "TFT_eSPI.h"
#include <TFT_eWidget.h>

#define DARKER_GREY 0x18E3

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);
GraphWidget gr = GraphWidget(&tft); // Graph widget

// Traces are drawn on tft using graph instance
TraceWidget tr1 = TraceWidget(&gr); // Graph trace 1

const uint8_t SHARP_LED_PIN = 6; // Sharp Dust/particle sensor Led Pin
const uint8_t SHARP_VO_PIN = 11; // Sharp Dust/particle analog out pin used for reading
GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1010AU0F, SHARP_LED_PIN, SHARP_VO_PIN);

Adafruit_BMP085 bmp;

Adafruit_AHTX0 aht;

Adafruit_LTR390 ltr = Adafruit_LTR390();

void bmp_sensor();
void aht_sensor();
void ltr_sensor();
void dust_sensor();
void draw_graphi();
void graph();

void setup()
{
  Serial.begin(9600);
  bmp.begin();
  aht.begin();
  ltr.begin();

  ltr.setMode(LTR390_MODE_UVS);
  ltr.setGain(LTR390_GAIN_3);
  ltr.setResolution(LTR390_RESOLUTION_16BIT);
  ltr.setThresholds(100, 1000);
  ltr.configInterrupt(true, LTR390_MODE_UVS);
  dustSensor.begin();

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  graph();
}

void loop()
{

  aht_sensor();
  bmp_sensor();
  dust_sensor();
  draw_graphi();
}

void dust_sensor()
{
  int a;
  int b;
  b = map(dustSensor.getDustDensity(), 0, 10000, 30, 330);
  a = 30 - b;
  tft.drawSmoothArc(50, 170, 50, 40, 30, b, TFT_GREEN, TFT_BLACK);
  tft.drawSmoothArc(50, 170, 50, 40, 30 - a, 330, TFT_BLACK, TFT_BLACK);
  tft.setCursor(40, 160, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print(dustSensor.getDustDensity());
  // dustSensor.getRunningAverage()
  tft.setCursor(20, 220);
  tft.print("Dust Density");
}
void ltr_sensor()
{
  int e;
  int f;
  int j;
  int k;

  ltr.setMode(LTR390_MODE_UVS);
  e = map(ltr.readUVS(), 0, 300, 31, 330);
  f = 30 - e;
  tft.drawSmoothArc(140, 205, 25, 22, 30, e, TFT_GREEN, TFT_BLACK);
  tft.drawSmoothArc(140, 205, 25, 22, 30 - f, 330, TFT_BLACK, TFT_BLACK);
  tft.setCursor(135, 205, 1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print(ltr.readUVS());
  tft.setCursor(135, 233);
  tft.print("UV");

  ltr.setMode(LTR390_MODE_ALS);
  f = map(ltr.readALS(), 0, 100000, 31, 330);
  e = 30 - f;
  tft.drawSmoothArc(215, 205, 25, 22, 30, f, TFT_GREEN, TFT_BLACK);
  tft.drawSmoothArc(215, 205, 25, 22, 30 - e, 330, TFT_BLACK, TFT_BLACK);
  tft.setCursor(210, 205, 1);
  tft.fillRect(210, 205, 25, 25, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print(f);
  tft.setCursor(210, 232);
  tft.print("Lux");
}
void aht_sensor()
{
  int i;
  int y;
  int z;
  int x;

  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  i = map(temp.temperature, 0, 50, 30, 330);
  z = 30 - i;
  tft.drawSmoothArc(50, 50, 50, 40, 30, i, TFT_GREEN, TFT_BLACK);
  tft.drawSmoothArc(50, 50, 50, 40, 30 - z, 330, TFT_BLACK, TFT_BLACK);
  tft.setCursor(40, 40, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.print(temp.temperature);
  tft.setCursor(20, 100);
  tft.print("Temperature");

  y = map(humidity.relative_humidity, 0, 100, 30, 330);
  x = 30 - y;
  tft.drawSmoothArc(160, 50, 50, 40, 30, y, TFT_GREEN, TFT_BLACK);
  tft.drawSmoothArc(160, 50, 50, 40, 30 - x, 330, TFT_BLACK, TFT_BLACK);
  tft.setCursor(150, 40, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print(humidity.relative_humidity, 2);
  tft.setCursor(135, 100);
  tft.print("Humidity");
}
void bmp_sensor()
{
  int a;
  int b;
  a = map(bmp.readPressure() / 100, 0, 1000, 30, 330);
  b = 30 - a;
  tft.drawSmoothArc(270, 50, 50, 40, 30, a, TFT_GREEN, TFT_BLACK);
  tft.drawSmoothArc(270, 50, 50, 40, 30 - b, 330, TFT_BLACK, TFT_BLACK);
  tft.setCursor(260, 40, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print(bmp.readPressure() / 100);
  tft.setCursor(245, 100);
  tft.print("Pressure");
}

void graph()
{

  // Graph area is 200 pixels wide, 150 high, dark grey background
  gr.createGraph(180, 100, tft.color565(5, 5, 5));

  // x scale units is from 0 to 100, y scale units is -50 to 50
  gr.setGraphScale(0.0, 24.0, 930, 970);

  // X grid starts at 0 with lines every 10 x-scale units
  // Y grid starts at -50 with lines every 25 y-scale units
  // blue grid
  gr.setGraphGrid(0.0, 2.0, 0, 3.0, TFT_GREY);

  // Draw empty graph, top left corner at 40,10 on TFT
  gr.drawGraph(130, 120);

  // Draw the x axis scale
  tft.setTextDatum(TC_DATUM); // Top centre text datum
  tft.drawNumber(0, gr.getPointX(0.0), gr.getPointY(0) + 3);
  tft.drawNumber(12, gr.getPointX(12.0), gr.getPointY(0) + 3);
  tft.drawNumber(24, gr.getPointX(24.0), gr.getPointY(0) + 3);

  // Draw the y axis scale
  tft.setTextDatum(MR_DATUM); // Middle right text datum
  tft.drawNumber(930, gr.getPointX(0.0), gr.getPointY(930.0));
  tft.drawNumber(970, gr.getPointX(0.0), gr.getPointY(970.0));

  // Restart traces with new colours
  tr1.startTrace(TFT_GREEN);

  // Sample periodically
}

void draw_graphi()
{
  static uint32_t plotTime = millis();
  static float gx = 0.0, gy = 0.0;
  if (millis() - plotTime >= 864000ULL)
  {
    plotTime = millis();
    gx++;
    // Add a new point on each trace
    tr1.addPoint(gx, bmp.readPressure() / 100);

    // Create next plot point

    // If the end of the graph is reached start 2 new traces
    if (gx > 24.0)
    {
      gx = 0.0;

      // Draw empty graph at 40,10 on display
      gr.drawGraph(130, 120);
      // Start new trace
      tr1.startTrace(TFT_GREEN);
    }
  }
}