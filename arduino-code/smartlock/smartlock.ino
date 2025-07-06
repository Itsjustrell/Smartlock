#define BLYNK_TEMPLATE_ID "INPUT_YOUR_TEMPLATE_ID_HERE"
#define BLYNK_TEMPLATE_NAME "INPUT_YOUR_TEMPLATE_NAME_HERE"
#define BLYNK_AUTH_TOKEN "INPUT_YOUR_BLYNK_AUTH_TOKEN_HERE"

#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin setup
#define SS_PIN        21
#define RST_PIN       22
#define SERVO_PIN     25
#define I2C_SDA       4
#define I2C_SCL       5

#define BLYNK_LED_VPIN    V1
#define BLYNK_BUTTON_VPIN V2

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2); 

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "INPUT_YOUR_WIFI_SSID_HERE";
char pass[] = "INPUT_YOUR_WIFI_PASSWORD_HERE";

const char* serverName = "INPUT_YOUR_SERVER_NAME_HERE"; //Example "http://192.168.61.59:3000/check-uid"

void unlockAndLock() {
  myServo.write(180); 
  Blynk.virtualWrite(BLYNK_LED_VPIN, 255); 
  lcd.setCursor(0, 1);
  lcd.print("Status: OPEN     ");
  delay(4000);
  myServo.write(0); 
  Blynk.virtualWrite(BLYNK_LED_VPIN, 0);  
  Blynk.virtualWrite(BLYNK_BUTTON_VPIN, 0);
  lcd.setCursor(0, 1);
  lcd.print("Status: LOCKED   ");
}

BLYNK_WRITE(BLYNK_BUTTON_VPIN) {
  int pinValue = param.asInt();
  if (pinValue == 1) {
    Serial.println("🔓 Manual unlock via Blynk");
    lcd.setCursor(0, 0);
    lcd.print("Manual Unlock");
    unlockAndLock();
    lcd.setCursor(0, 0);
    lcd.print("Waiting RFID...");
  }
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Welcome");
  lcd.setCursor(0, 1);
  lcd.print("Waiting RFID...");

  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2400);
  myServo.write(0);

  WiFi.begin(ssid, pass);
  Blynk.begin(auth, ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("✅ WiFi Connected");
}

void loop() {
  Blynk.run();

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String uidStr = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidStr += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidStr.toLowerCase();

  Serial.println("🎴 UID: " + uidStr);
  lcd.setCursor(0, 0);
  lcd.print("UID:            ");
  lcd.setCursor(5, 0);
  lcd.print(uidStr.substring(0, 11)); 

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> json;
    json["uid"] = uidStr;
    String requestBody;
    serializeJson(json, requestBody);

    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode == 200) {
      String response = http.getString();
      Serial.println("📩 Response: " + response);

      StaticJsonDocument<200> resJson;
      DeserializationError error = deserializeJson(resJson, response);

      if (!error) {
        bool access = resJson["access"];
        if (access) {
          Serial.println("✅ Access Granted");
          lcd.setCursor(0, 1);
          lcd.print("Access: GRANTED ");
          unlockAndLock();
        } else {
          Serial.println("❌ Access Denied");
          lcd.setCursor(0, 1);
          lcd.print("Access: DENIED  ");
          delay(3000);
        }
      } else {
        Serial.println("⚠️ JSON parse error!");
        lcd.setCursor(0, 1);
        lcd.print("Parse error!    ");
        delay(3000);
      }

    } else {
      Serial.println("❌ HTTP Error: " + String(httpResponseCode));
      lcd.setCursor(0, 1);
      lcd.print("HTTP Error: " + String(httpResponseCode));
      delay(3000);
    }

    http.end();
  } else {
    lcd.setCursor(0, 1);
    lcd.print("WiFi not conn.  ");
    delay(3000);
  }

  lcd.setCursor(0, 0);
  lcd.print("Waiting RFID... ");
  lcd.setCursor(0, 1);
  lcd.print("                ");

  delay(1000);
}