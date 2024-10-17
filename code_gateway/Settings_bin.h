String bin_id = "bin_QzJDmS3dke_SalleConf_2B61";
String floor_num =  "First_Floor";
String in_or_out =  "indoor";




#define pos_plastic 0
#define pos_metal 60
#define pos_other 180

#define pos_open 80
#define pos_closed 0

#define TRIG_PIN 5
#define ECHO_PIN 18

float fullness[3] = { 0, 0, 0 };      // {%plastic, %metal, %other}
const float original_distance = 10.0;


int servo_bin_pin = 13;  
int servo_lid_pin = 12;  

// Create a Servo object
Servo servo_bin;
Servo servo_lid;

const char* ssid = "Wifi_Samsung";
const char* password = "ocdm9308";

String str;
SX1278 radio = new Module(5, 35, 27, -1, SPI);
int transmissionState = RADIOLIB_ERR_NONE;
volatile bool receivedFlag = false;
#define LORA_DEVICE DEVICE_SX1278  //we need to define the device we are using
//LoRa module pins:
#define NSS 5
#define NRESET 27
#define DIO0 35
#define SCK 18
#define MISO 19
#define MOSI 23

std::string delimiter = "/";
std::string bin_id_;
std::string floor_bin;
std::string in_or_out_bin;
std::string per_plastic;
std::string per_metal;
std::string per_other;


#define API_KEY "AIzaSyArQ5XeGutcG-46kJVYQq1pa0mclb94zTM"
#define DATABASE_URL "https://the-smart-waste-segregator-default-rtdb.firebaseio.com/"
#define USER_EMAIL "mariem.benmehrez@insat.ucar.tn"
#define USER_PASSWORD "hack4Earth"

// Firebase Data and Auth objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
