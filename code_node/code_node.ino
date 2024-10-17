#include <SPI.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <RadioLib.h>
#include <ESP32Servo.h>
#include <Settings_bin.h>


void setFlag(void) {
  // we sent a packet, set the flag
  transmittedFlag = true;
}

void transmitData(const char* data) {
  transmissionState = radio.startTransmit(data);
  if (transmittedFlag) {
    transmittedFlag = false;
    if (transmissionState == RADIOLIB_ERR_NONE) {
      // packet was successfully sent
      Serial.println(F("transmission finished:"));
      Serial.print("str= ");
      Serial.println(data);
    } else {
      Serial.print(F("failed, code "));
      Serial.println(transmissionState);
    }
  }
}


void setup() {
  Serial.begin(115200);
  servo_bin.attach(servo_bin_pin);
  servo_lid.attach(servo_lid_pin);
  servo_bin.write(0);
  delay(1500);
  servo_lid.write(80);

  SPI.begin(SCK, MISO, MOSI, NSS);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
  radio.setPacketSentAction(setFlag);
  radio.setFrequency(440.0);
  radio.setBandwidth(125.0);
  radio.setSpreadingFactor(12);
  radio.setCodingRate(5);
  radio.setOutputPower(20);
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

void data_to_transmit() {

  String transString = bin_id + "/" + floor_num + "/" + in_or_out + "/" + String(fullness[0]) + "/" + String(fullness[1]) + "/" + String(fullness[2]) + "/";
  delay(3000);
  const char* dataToTransmit1 = transString.c_str();
  Serial.print("The data of the percenatges to tansmit is :");
  Serial.println(String(dataToTransmit1));
  transmitData(dataToTransmit1);
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
    Serial.println(current_distance);
    float percentage = ((original_distance - current_distance) / original_distance) * 100.0;

    update_fullness_percentage(incoming_byte, percentage);
    data_to_transmit();
  }
}
