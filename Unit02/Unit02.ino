#include "cred.h" //Don't be a tosser
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <AceButton.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned char FLASH[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x0e, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0xfd, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1f, 0xf8, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1f, 0xf8, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x1f, 0xf8, 0x20,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0f, 0xf0, 0x10,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0f, 0xf0, 0x08,
    0xe0, 0xcf, 0x07, 0x9f, 0xf1, 0xff, 0x1f, 0xe6, 0x03, 0x80, 0x1e, 0x00, 0x40, 0x07, 0xe0, 0x02,
    0xe0, 0xcf, 0x87, 0x9f, 0xf9, 0xff, 0x9f, 0xe6, 0x03, 0x80, 0x3e, 0x00, 0xc0, 0x07, 0xe0, 0x03,
    0xe0, 0xcf, 0x8f, 0x9c, 0x19, 0x83, 0x98, 0x06, 0x03, 0x80, 0x3f, 0x00, 0xe0, 0x07, 0xe0, 0x07,
    0xe0, 0xcd, 0xcd, 0x9f, 0xf9, 0x83, 0x9f, 0xe7, 0x03, 0x80, 0x73, 0x00, 0xfc, 0x03, 0xc0, 0x3f,
    0xe0, 0xcc, 0xdd, 0x9f, 0xf9, 0xff, 0x1f, 0xe6, 0x03, 0x80, 0x73, 0x80, 0x7f, 0x03, 0xc0, 0xfe,
    0xe0, 0xcc, 0xf9, 0x98, 0x19, 0xff, 0x18, 0x06, 0x03, 0x80, 0xff, 0x80, 0x7f, 0xe1, 0x87, 0xfe,
    0xff, 0xcc, 0x79, 0x9f, 0xf9, 0x83, 0x9f, 0xe7, 0xfb, 0xfc, 0xff, 0xc0, 0x7f, 0xf9, 0x9f, 0xfe,
    0x7f, 0xcc, 0x71, 0x9f, 0xf9, 0x83, 0x9f, 0xf7, 0xfb, 0xfd, 0xc0, 0xc0, 0x7f, 0xff, 0xff, 0xfe,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf9, 0x9f, 0xfe,
    0x00, 0x00, 0x00, 0x03, 0xcf, 0x3e, 0x79, 0xe7, 0xc6, 0x78, 0x7c, 0x80, 0x7f, 0xe1, 0x87, 0xfe,
    0x00, 0x00, 0x00, 0x04, 0x58, 0xa2, 0x8b, 0x14, 0x45, 0x11, 0x44, 0xc8, 0x7f, 0x03, 0xc0, 0xfe,
    0x00, 0x00, 0x00, 0x04, 0x10, 0xb2, 0xca, 0x16, 0x49, 0x11, 0x44, 0xa8, 0xfc, 0x03, 0xc0, 0x3f,
    0x00, 0x00, 0x00, 0x04, 0x58, 0xa2, 0xc3, 0x14, 0x4d, 0x92, 0x44, 0x98, 0xf0, 0x03, 0xe0, 0x0f,
    0x00, 0x00, 0x00, 0x03, 0x8f, 0x00, 0x00, 0xe4, 0x40, 0x00, 0x30, 0x00, 0xc0, 0x07, 0xe0, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x07, 0xe0, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0f, 0xf0, 0x08,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0f, 0xf0, 0x10,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x1f, 0xf8, 0x20,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1f, 0xf8, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1f, 0xf8, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0xfd, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x1e, 0x00};

#define NTPServer "asia.pool.ntp.org"
#define TimeZone 6           // GMT +6
#define UpdateInterval 60000 // in milisec

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTPServer, (TimeZone * 60 * 60), UpdateInterval);

String weekDays[7] = {"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};

using namespace ace_button;
bool fetch_blynk_state = true;

#define RelayPin1 D5
#define RelayPin2 D6
#define RelayPin3 D7
#define RelayPin4 D8

#define SwitchPin1 1
#define SwitchPin2 3
#define SwitchPin3 D3
#define SwitchPin4 10

#define uplinkLED D0
// uplink status indicator LED
#define opsLED D4
// operations status indicator LED
#define VPIN_BUTTON_1 V0
#define VPIN_BUTTON_2 V1
#define VPIN_BUTTON_3 V2
#define VPIN_BUTTON_4 V3

bool toggleState_1 = LOW;
bool toggleState_2 = LOW;
bool toggleState_3 = LOW;
bool toggleState_4 = LOW;

int wifiFlag = 0;

char auth[] = BLYNK_AUTH_TOKEN;

ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);
ButtonConfig config3;
AceButton button3(&config3);
ButtonConfig config4;
AceButton button4(&config4);

void handleEvent1(AceButton *, uint8_t, uint8_t);
void handleEvent2(AceButton *, uint8_t, uint8_t);
void handleEvent3(AceButton *, uint8_t, uint8_t);
void handleEvent4(AceButton *, uint8_t, uint8_t);

BlynkTimer timer;

BLYNK_WRITE(VPIN_BUTTON_1)
{
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, !toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2)
{
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, !toggleState_2);
}

BLYNK_WRITE(VPIN_BUTTON_3)
{
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, !toggleState_3);
}

BLYNK_WRITE(VPIN_BUTTON_4)
{
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, !toggleState_4);
}

void checkBlynkStatus()
{

  bool isconnected = Blynk.connected();
  if (isconnected == false)
  {
    wifiFlag = 1;
    Serial.println("Blynk Not Connected");
    digitalWrite(uplinkLED, HIGH);
  }
  if (isconnected == true)
  {
    wifiFlag = 0;
    if (!fetch_blynk_state)
    {
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    }
    digitalWrite(uplinkLED, LOW);
    Serial.println("Blynk Connected");
    timeClient.update();
    // If it works,please don't touch it~
  }
}

BLYNK_CONNECTED()
{
  // Request the latest state from the server
  if (fetch_blynk_state)
  {
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
    Blynk.syncVirtual(VPIN_BUTTON_3);
    Blynk.syncVirtual(VPIN_BUTTON_4);
  }
}

void setup()
{
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.drawBitmap(0, 0, FLASH, 128, 32, WHITE);
  display.display();

  pinMode(opsLED, OUTPUT);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(uplinkLED, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);
  digitalWrite(RelayPin3, !toggleState_3);
  digitalWrite(RelayPin4, !toggleState_4);

  digitalWrite(uplinkLED, HIGH);

  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  config3.setEventHandler(button3Handler);
  config4.setEventHandler(button4Handler);

  button1.init(SwitchPin1);
  button2.init(SwitchPin2);
  button3.init(SwitchPin3);
  button4.init(SwitchPin4);

  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus);
  Blynk.config(auth);
  delay(1000);

  if (!fetch_blynk_state)
  {
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
  }
}

void loop()
{
  digitalWrite(opsLED, HIGH);

  Blynk.run();
  timer.run();

  String weekDay = weekDays[timeClient.getDay()];
  /*
  Size 1 Config ( 2104 ) : 0,8,16,24
  Size 2 Config ( 1002 ) : 0,18
  */

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(timeClient.getFormattedTime());
  display.setCursor(0, 18);
  display.println(weekDay);
  display.display();

  button1.check();
  button2.check();
  button3.check();
  button4.check();

  digitalWrite(opsLED, LOW);
}

void button1Handler(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  Serial.println("EVENT1");
  switch (eventType)
  {
  case AceButton::kEventReleased:
    Serial.println("kEventReleased");
    digitalWrite(RelayPin1, toggleState_1);
    toggleState_1 = !toggleState_1;
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    break;
  }
}
void button2Handler(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  Serial.println("EVENT2");
  switch (eventType)
  {
  case AceButton::kEventReleased:
    Serial.println("kEventReleased");
    digitalWrite(RelayPin2, toggleState_2);
    toggleState_2 = !toggleState_2;
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    break;
  }
}
void button3Handler(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  Serial.println("EVENT3");
  switch (eventType)
  {
  case AceButton::kEventReleased:
    Serial.println("kEventReleased");
    digitalWrite(RelayPin3, toggleState_3);
    toggleState_3 = !toggleState_3;
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    break;
  }
}
void button4Handler(AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  Serial.println("EVENT4");
  switch (eventType)
  {
  case AceButton::kEventReleased:
    Serial.println("kEventReleased");
    digitalWrite(RelayPin4, toggleState_4);
    toggleState_4 = !toggleState_4;
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    break;
  }
}