#define BLYNK_TEMPLATE_ID "TMPLUCQskaiC"
#define BLYNK_TEMPLATE_NAME "Template1"
#define BLYNK_AUTH_TOKEN "TiH1a2RGMyWOmi24zaqPFwCrinT2SweW"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266_SSL.h>

char ssid[] = "gg2";
char pass[] = "19931125";

BlynkTimer timer;

#include <TridentTD_LineNotify.h>
#define LINE_TOKEN "YSVmbwmLbulaSvwQSHUflGKxgGwAOHDVmQpwXcvgkte"

void sendSeconds() {
  Blynk.virtualWrite(V2, millis() / 1000);
}

WidgetLED led1(V4);
void blinkLedWidget() {
  if (led1.getValue()) {
    led1.off();
    // Serial.println("LED on V1: off");
    digitalWrite(16, 1);
  } else {
    led1.on();
    // Serial.println("LED on V1: on");
    digitalWrite(16, 0);
  }
}

BLYNK_WRITE(V0) {
  int pinValue0 = param.asInt();
  if (pinValue0 == 1) {
    digitalWrite(2, 0);
    LINE.notify("pinValue0 == 1");
    Serial.println("pinValue0 == 1");
  } else {
    digitalWrite(2, 1);
  }
}

void setup() {
  // Debug console
  Serial.begin(115200);


  pinMode(16, OUTPUT);
  digitalWrite(16, 1);

  pinMode(2, OUTPUT);
  digitalWrite(2, 1);
  LINE.setToken(LINE_TOKEN);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 443);
  timer.setInterval(1000L, sendSeconds);
  timer.setInterval(2000L, blinkLedWidget);
}

void loop() {
  Blynk.run();
  timer.run();
}
