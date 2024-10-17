#include <SPI.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <RadioLib.h>
#include <ESP32Servo.h>
#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <Settings_bin.h>
#include <Firebase_ESP_Client.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
// Firebase includes
#include <addons/RTDBHelper.h>
#include <addons/TokenHelper.h>

void IRAM_ATTR setFlag(void) {
  receivedFlag = true;
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  servo_bin.attach(servo_bin_pin);
  servo_lid.attach(servo_lid_pin);

  SPI.begin(SCK, MISO, MOSI, NSS);
  connectToWiFi();



   // Firebase Initialization
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectNetwork(true);




  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.begin();
  radio.setFrequency(440.0);
  radio.setBandwidth(125.0);
  radio.setSpreadingFactor(12);
  radio.setCodingRate(5);
  radio.setGain(0);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
  radio.setPacketReceivedAction(setFlag);
  Serial.print(F("[SX1278] Starting to listen ... "));
  state = radio.startReceive();
  // Wait for the serial port to be available
  while (!Serial) {
    delay(10);
  }
  Serial.println("ESP32 is ready to receive");
}

int rotate_bin(char position) {
  switch (position) {
    case '0':
      servo_bin.write(pos_plastic);
      delay(1500);
      servo_lid.write(0);
      delay(1500);
      servo_lid.write(80);
      delay(1500);
      break;
    case '1':
      servo_bin.write(pos_metal);
      delay(1500);
      servo_lid.write(0);
      delay(1500);
      servo_lid.write(80);
      delay(1500);
      break;
    case '2':
      servo_bin.write(pos_other);
      delay(1500);
      servo_lid.write(0);
      delay(1500);
      servo_lid.write(80);
      delay(1500);
      break;
    default:
      break;
  }
  return 0;
}

int update_fullness_percentage(char position, float percentage) {
  switch (position) {
    case '0':
      fullness[0] = percentage;
      break;
    case '1':
      fullness[1] = percentage;
      break;
    case '2':
      fullness[2] = percentage;
      break;
    default:
      break;
  }
  return 0;
}

float getDistance() {
  // Send a 10us pulse to the TRIG_PIN
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

void update_percentage(String path, String percentage) {
  if (Firebase.RTDB.setString(&fbdo, path, percentage)) {
    Serial.println("percentage value set successfully!");
  } else {
    Serial.print("Failed to set string value, reason: ");
    Serial.println(fbdo.errorReason());
  }
}

void loop() {
  if (Serial.available() > 0) {
    // Read the incoming byte
    char incoming_byte = Serial.read();
    uint8_t pos = incoming_byte - '0';
    // Check the received value
    Serial.print(incoming_byte);
    Serial.println("****************************");
    rotate_bin(incoming_byte);
    // Measure the distance
    float current_distance = getDistance();
    float percentage = ((original_distance - current_distance) / original_distance) * 100.0;

    update_fullness_percentage(incoming_byte, percentage);
    String path_plastic = "/" + floor_num + "/" + in_or_out + "/" + bin_id + "/Plastic";
    update_percentage(path_plastic, String(fullness[0]));
    String path_metal = "/" + floor_num + "/" + in_or_out + "/" + bin_id + "/Metal";
    update_percentage(path_metal, String(fullness[1]));
    String path_other = "/" + floor_num + "/" + in_or_out + "/" + bin_id + "/Paper";
    update_percentage(path_other, String(fullness[2]));
  } else if (receivedFlag) {
    receivedFlag = false;
    int state = radio.readData(str);
    Serial.println(str);
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("[SX1278] Received packet!"));
      Serial.print(F("[SX1278] Data:\t\t"));
      Serial.println(str);
      std::string buffer(str.c_str());
      str = "";

      bin_id_ = buffer.substr(0, buffer.find(delimiter));
      buffer.erase(0, buffer.find(delimiter) + delimiter.length());

      floor_bin = buffer.substr(0, buffer.find(delimiter));
      buffer.erase(0, buffer.find(delimiter) + delimiter.length());

      in_or_out_bin = buffer.substr(0, buffer.find(delimiter));
      buffer.erase(0, buffer.find(delimiter) + delimiter.length());

      per_plastic = buffer.substr(0, buffer.find(delimiter));
      buffer.erase(0, buffer.find(delimiter) + delimiter.length());
      per_metal = buffer.substr(0, buffer.find(delimiter));
      buffer.erase(0, buffer.find(delimiter) + delimiter.length());
      per_other = buffer.substr(0, buffer.find(delimiter));
      buffer.erase(0, buffer.find(delimiter) + delimiter.length());

      std::string path_plastic = "/" + floor_bin + "/" + in_or_out_bin + "/" + bin_id_ + "/Plastic";
      String path_db_plastic = String(path_plastic.c_str());
      String per_plastic_db = String(per_plastic.c_str());
      update_percentage(path_db_plastic, per_plastic_db);

      std::string path_metal = "/" + floor_bin + "/" + in_or_out_bin + "/" + bin_id_ + "/Metal";
      String path_db_metal = String(path_metal.c_str());
      String per_metal_db = String(per_metal.c_str());

      update_percentage(path_db_metal, per_metal_db);

      std::string path_other = "/" + floor_bin + "/" + in_or_out_bin + "/" + bin_id_ + "/Paper";
      String path_db_other = String(path_other.c_str());
      String per_other_db = String(per_other.c_str());

      update_percentage(path_db_other, per_other_db);

    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      Serial.println(F("[SX1278] CRC error!"));
      Serial.println(F("this is the received string "));
      Serial.println(str);
    } else {
      Serial.print(F("[SX1278] Failed, code "));
      Serial.println(state);
    }
  }
}
