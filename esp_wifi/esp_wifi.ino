#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//Wifi name
#define WLAN_SSID       "10h37"
//Wifi password
#define WLAN_PASS       "12345678"

//setup Adafruit
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
//fill your username                   
#define AIO_USERNAME    "Giaqui1403"
//fill your key
#define AIO_KEY         "aio_gdaB15TjbFqhdJeLhoVPND8ouA9r"

//setup MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//setup publish
Adafruit_MQTT_Publish light_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/light");
Adafruit_MQTT_Subscribe light_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light", MQTT_QOS_1);

Adafruit_MQTT_Publish power_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/power");
Adafruit_MQTT_Subscribe power_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/power", MQTT_QOS_1);

Adafruit_MQTT_Publish temp_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp");
Adafruit_MQTT_Subscribe temp_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/temp", MQTT_QOS_1);

Adafruit_MQTT_Publish game_info_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/game-info");
Adafruit_MQTT_Subscribe game_info_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/game-info", MQTT_QOS_1);

int led_counter = 0;
int led_status = HIGH;

void lightcallback(char* value, uint16_t len){
  if(value[0] == '0') Serial.print('a');
  if(value[0] == '1') Serial.print('A');
}

void setup() {
  // put your setup code here, to run once:
  //set pin 2,5 as OUTPUT
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  //set busy pin HIGH
  digitalWrite(5, HIGH);

  Serial.begin(115200);

  //connect Wifi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //subscribe light feed
  light_sub.setCallback(lightcallback);
  mqtt.subscribe(&light_sub); 
  mqtt.subscribe(&power_sub); 
  mqtt.subscribe(&temp_sub); 
  mqtt.subscribe(&game_info_sub); 


  //connect MQTT
  while (mqtt.connect() != 0) { 
    mqtt.disconnect();
    delay(500);
  }

  //finish setup, set busy pin LOW
  digitalWrite(5, LOW);
}

String parts[5];
void devideString(String inputString)
{
  int numberOfParts = 0; // Số lượng phần được tách
  int startPos = 0; // Vị trí bắt đầu tìm ký tự phân cách

  for (int i = 0; i < inputString.length(); i++) {
    if (inputString.charAt(i) == '#') {
      parts[numberOfParts] = inputString.substring(startPos, i);
      startPos = i + 1;
      numberOfParts++;
    }
  }
  // Xử lý phần cuối cùng
  if (startPos < inputString.length()) {
    parts[numberOfParts] = inputString.substring(startPos);
    numberOfParts++; 
  }
}
void erasePart() 
{
  parts[0] = "";
  parts[1] = "";
  parts[2] = "";
  parts[3] = "";
  parts[4] = "";
}

void loop() {
  // put your main code here, to run repeatedly:

  //receive packet
  mqtt.processPackets(1);
  
  //read serial
  if(Serial.available()){
    char msg = Serial.read();
    String revStr = "";
    revStr += msg;
    while (Serial.available()) { // Kiểm tra nếu có dữ liệu trong cổng UART
      char incomingByte = Serial.read(); // Đọc dữ liệu từ cổng UART
      revStr += incomingByte; // Nối dữ liệu vào chuỗi
      delay(2); // Đợi để nhận các byte tiếp theo (tùy thuộc vào tốc độ baudrate)
    }
    devideString(revStr);
    if(parts[0] == "SENSOR")
    {
      int temp = parts[1].toInt();
      temp_pub.publish(temp);
      int light = parts[2].toInt();
      light_pub.publish(light);
      int power = parts[3].toInt();
      power_pub.publish(power);

    }
    erasePart();
    // to do
  }

  
  
  delay(10);
}
