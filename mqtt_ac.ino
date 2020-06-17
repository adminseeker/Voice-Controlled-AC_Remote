#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Kelvinator.h>
#include <PubSubClient.h> 

const char* ssid = "<wifi ssid>";  
const char* password = "wifi password"; 
const char* mqtt_server = "<mosquito server ip>";
const char* mqtt_topic = "test";
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* clientID = "aravind";

const uint16_t kIrLed = 4;  //D2
IRKelvinatorAC ac(kIrLed); 
String tempstring="26";
int tempint=26;

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); 

void ReceivedMessage(char* topic, byte* payload, unsigned int length) {

  payload[length]='\0';
  Serial.println((char *)payload);
  
  if (strcmp((char *)payload,"ac_on") == 0) {
    handle_ac_on(); 
  }
  else if (strcmp((char *)payload,"ac_off") == 0) {
    handle_ac_off();
  }
  else if((char)payload[2]=='m'){ //temp
    String sub;
    sub=(char *)payload;
    handle_set_temp(sub.substring(4,6));
   }
  else if (strcmp((char *)payload,"light_on") == 0) {
    handle_light_on();
  }
  else if (strcmp((char *)payload,"light_off") == 0) {
    handle_light_off();
  }
  else if((char)payload[2]=='i'){ //swing
    String sub;
    sub=(char *)payload;
    handle_swing(sub.substring(5,6),sub.substring(6,7));
   }
  else if((char)payload[0]=='x'){ // xfan
    String sub;
    sub=(char *)payload;
    handle_setxfan(sub.substring(4,5));
   }
    else if((char)payload[0]=='f'){ //fan
    String sub;
    sub=(char *)payload;
    handle_setxfan(sub.substring(3,4));
   }
}

bool Connect() {
  if (client.connect(clientID)) {
      client.subscribe(mqtt_topic);
      return true;
    }
    else {
      return false;
  }
}

void setup() {

  
  Serial.begin(9600);
  ac.begin();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  client.setCallback(ReceivedMessage);
  if (Connect()) {
    Serial.println("Connected Successfully to MQTT Broker!");  
  }
  else {
    Serial.println("Connection Failed!");
  }
}

void loop() {

  if (!client.connected()) {
    Connect();
  }
  client.loop();
  
}

void main_handler(){
    ac.on();
    ac.setLight(true);
    ac.setMode(kKelvinatorCool);
    ac.setTemp(tempint);
    delay(500);
}

void handle_ac_on() {
  main_handler();
  ac.send();
}

void handle_ac_off() {
  main_handler();
  ac.setSwingVertical(false);
  ac.setSwingHorizontal(false);
  ac.off();
  ac.send();
}


void handle_set_temp(String functemp){
    tempstring=functemp; 
    tempint=tempstring.toInt();
    main_handler();
    ac.send();
}

void handle_light_on(){
    main_handler();
    ac.setLight(true);
    ac.send();
}
void handle_light_off(){
    main_handler();
    ac.setLight(false);
    ac.send();
}
void handle_swing(String ver, String hor){
    main_handler();
    int vertical = ver.toInt();
    int horizontal=hor.toInt();
    if(vertical==1)
      ac.setSwingVertical(true);
    else if(vertical==0)
      ac.setSwingVertical(false);
    else if(horizontal==1)
      ac.setSwingHorizontal(true);
    else if(horizontal==0)
      ac.setSwingHorizontal(false);
     ac.send();
}
 
void handle_setxfan(String val){
  main_handler();
  int value=val.toInt();
  if(value==1)
    ac.setXFan(true);
   else if(value==0)
    ac.setXFan(false);  
   ac.send();
}

void handle_setfan(String val){
  main_handler();
  int value=val.toInt();
  ac.setFan(value);
  ac.send();
}
