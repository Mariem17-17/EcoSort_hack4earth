String bin_id = "bin_CUkjAc65EL_bureau_directeur";
String floor_num =  "First_Floor";
String in_or_out =  "outdoor";


WMSANFSDLBKSXXNWEOQMSNRKEL6DG36V




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


SX1278 radio = new Module(5, 35, 27, -1, SPI);
int transmissionState = RADIOLIB_ERR_NONE;
volatile bool transmittedFlag = false;
#define LORA_DEVICE DEVICE_SX1278  //we need to define the device we are using
//LoRa module pins:
#define NSS 5
#define NRESET 27
#define DIO0 35
#define SCK 18
#define MISO 19
#define MOSI 23
