#define BLYNK_TEMPLATE_ID "TMPL6BMoY0sbk"
#define BLYNK_DEVICE_NAME "SmartFarm"
#define BLYNK_AUTH_TOKEN "QWJTb8O1h1fzwhQa-RwZ4wIipN1T2NxY"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "10000";
char pass[] = "13131131313113";

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <TridentTD_LineNotify.h>
#define LINE_TOKEN "1cNUdJIJhJW8s5IZqiqiIdP1YxjYpiC0v4lufBq7HEK"
String tempe = "";
String humid = "";
String soil = "";
String waterLevel = "";

#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 17  //dht11

DHT dht(DHTPIN, DHTTYPE, 11);  // 11 works fine for ESP8266
String humidity = "";
String tempf = "";

unsigned long previousMillis = 0;  // will store last temp was read
const long interval = 150;         // interval at which to read sensor

#define yl69_pin 32
int val = 0;
int water = 0;
int val_ = 0;
int water_ = 0;
int LCD_switch = 0;
int Water_switch = 0;

int value8 = 0;
int value9 = 0;

int water4_time = 0;
int water5_time = 0;

BlynkTimer timer;


BLYNK_WRITE(V8) {
  value8 = param.asInt();
  Serial.println("V8=" + value8);
  if (value8 == 1) {
    Serial.println(value8);
    digitalWrite(4, HIGH);
  } else {
    Serial.println(value8);
    digitalWrite(4, LOW);
  }
}

BLYNK_WRITE(V9) {
  value9 = param.asInt();
  Serial.println("V9=" + value9);
  if (value9 == 1) {
    Serial.println(value9);
    digitalWrite(5, HIGH);
  } else {
    Serial.println(value9);
    digitalWrite(5, LOW);
  }
}

void Auto_Water() {
  if (Water_switch == 0) {
    Water_switch = 1;
    if (val_ <= 700)  //最滿1300
    {
      // Serial.println("water4 OPEN !!!!!");
      digitalWrite(4, HIGH);
      water4_time = millis();
    }
  } else {
    Water_switch = 0;
    if (water_ <= 700)  //最滿1300
    {
      // Serial.println("water5 OPEN !!!!!");
      digitalWrite(5, HIGH);
      water5_time = millis();
    }
  }
}

void Close_Water() {
  if (value8 == 0) {
    if (millis() - water4_time >= 800) {
      digitalWrite(4, LOW);
    } else {
      // Serial.println(millis() - water4_time);
    }
  }
  if (value9 == 0) {
    if (millis() - water5_time >= 800) {
      digitalWrite(5, LOW);
    } else {
      // Serial.println(millis() - water5_time);
    }
  }
}

void Read_Sensor() {
  // unsigned long currentMillis = millis();
  // if (currentMillis - previousMillis >= interval) {
  // 將最後讀取感測值的時間紀錄下來
  // previousMillis = currentMillis;
  // 讀取溫度大約 250 微秒!
  float humidity_, tempf_;
  humidity_ = dht.readHumidity();       // 讀取濕度(百分比)
  tempf_ = dht.readTemperature(true);  // 讀取溫度(華氏)
  // 檢查兩個值是否為空值
  if (isnan(humidity_) || isnan(tempf_)) {
    Serial.println("DHT sensor 讀不到!");
    return;
  } else {
    humidity = String((int)(round(humidity_)));
    tempf = String((int)(round((tempf_ - 32) * 5 / 9)));
  }
  // }

  humid = "濕度:" + humidity + "％";
  tempe = "溫度:" + tempf + "℃";
  Serial.println(humid);
  Serial.println(tempe);

  water_ = (analogRead(33));
  water = round(map(water_, 0, 1800, 0, 45));
  waterLevel = "水箱水位:" + String((int)water) + "mm";
  // Serial.println("水箱水位:" + String((int)water) + "mm");

  val_ = (4095 - analogRead(yl69_pin));
  val = round(map(val_, 0, 2500, 0, 100));
  soil = "土壤濕度:" + String((int)val) + "％";
  // Serial.println("土壤濕度:" + String((int)val) + "％");

  // void LCD_monitor() {
  if (LCD_switch == 0) {
    Serial.println("LCD_switch to 1");
    LCD_switch = 1;
    lcd.init(); //初始化LCD
    lcd.backlight(); //開啟背光
    lcd.setCursor(0, 0);  //設定游標位置 (字,行)
    lcd.print("Hum :");   //Relative Humidity 相對濕度簡寫
    lcd.setCursor(6, 0);
    lcd.print(humidity);
    lcd.setCursor(12, 0);
    lcd.print("%");
    lcd.setCursor(0, 1);  //設定游標位置 (字,行)
    lcd.print("Temp:");
    lcd.setCursor(6, 1);
    lcd.print(tempf);
    lcd.setCursor(12, 1);
    lcd.print((char)223);  //用特殊字元顯示符號的"度"
    lcd.setCursor(13, 1);
    lcd.print("C");
  } else {
    Serial.println("LCD_switch to 0");
    LCD_switch = 0;
    lcd.clear();
    lcd.setCursor(0, 0);  //設定游標位置 (字,行)
    lcd.print("Soil:");
    lcd.setCursor(6, 0);
    lcd.print(val);
    lcd.setCursor(12, 0);
    lcd.print("%");
    lcd.setCursor(0, 1);  //設定游標位置 (字,行)
    lcd.print("Wlev:");
    lcd.setCursor(6, 1);
    lcd.print(water);
    lcd.setCursor(12, 1);
    lcd.print("mm");
  }
  // }
  //void Send_Blynk() {
  Blynk.virtualWrite(V6, tempf);
  Blynk.virtualWrite(V5, humidity);
  Blynk.virtualWrite(V1, val);
  Blynk.virtualWrite(V7, water);
  //}
  //void Send_Line() {
  LINE.setToken(LINE_TOKEN);
  LINE.notify("\n" + tempe + "\n" + humid + "\n" + soil + "\n" + waterLevel);
  //}
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(5000L, Auto_Water);
  // timer.setInterval(500L, Close_Water);
  timer.setInterval(7000L, Read_Sensor);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  dht.begin();
  Wire.begin();
  lcd.begin(16, 2);
}

void loop() {
  Blynk.run();
  timer.run();
  Close_Water();
}
