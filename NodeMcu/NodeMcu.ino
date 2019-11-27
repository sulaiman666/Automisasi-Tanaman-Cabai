#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

SoftwareSerial s(D6,D5);

//Variabel untuk wifi
const char* ssid = "wifi";
const char* password = "1234567";


//API KEY Thingspeak
const char* apiKey = "Q6MDDGTS2RDX2W1I";
const char* resource = "/update?api_key=";

//Thing Speak API server
const char* server = "api.thinkspeak.com"



void setup() {
  Serial.begin(115200);
  s.begin(115200);

  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);

  //Inisiasi wifi
  initWifi();
}

void loop() {
  //Pembacaan data dari UNO
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid()) return;

  //Get Trigger
  unsigned long id = root["id"];

  //Get Sensor
  int sensor0 = root["sensor0"];
  int sensor1 = root["sensor1"];
  int sensor2 = root["sensor2"];
  int sensor3 = root["sensor3"];

  //Get Kelompok
  int kelompok0 = root["kelompok0"];
  int kelompok1 = root["kelompok1"];

  //Get waktu
  int jam=root["jam"];
  int menit=root["menit"];
  int detik=root["detik"];

  //Menggabungkan Waktu
  String waktu = "";
  waktu += String(jam);
  waktu += ":";
  waktu += String(menit);
  waktu += ":";
  waktu += String(detik); 

  //Get tanggal
  int hari=root["hari"];
  int bulan=root["bulan"];
  int tahun=root["tahun"];

  //Menggabungkan Tanggal
  String tanggal = "";
  tanggal += String(hari);
  tanggal += ".";
  tanggal += String(bulan);
  tanggal += ".";
  tanggal += String(tahun); 

  unsigned long trigger = 0;

  if(trigger != id){
    //Fungsi upload
    WiFiClient client;
    int retries 5;
  
    while(!!!client.connect(server, 80) && (retries-- > )) {
      digitalWrite(D0, LOW);
    }
    if(!!!client.connected()) {
      digitalWrite(D0, HIGH);
    }
  
    client.print(String("GET ") + resource + apiKey + "&field1=" + sensor0 + "&field2=" + sensor1 + "&field3=" + sensor2 + "&field4=" + sensor3 + "&field5=" + kelompok0 + "&field6=" + kelompok1 + "&field7=" + tanggal + "&field8=" + waktu +
                    " HTTP/1.1\r\n" +
                    "Host: " + server + "\r\n" + 
                    "Connection: close\r\n\r\n");
    int timeout = 5 * 10; // 5 seconds             
    while(!!!client.available() && (timeout-- > )){
      delay(100);
    }
    if(!!!client.available()) {
       digitalWrite(D0, LOW);
    }
  
    while(client.available()){
      digitalWrite(D0, HIGH)
    }
  
    Serial.println("\nclosing connection");
    client.stop();

    trigger=id;
  }
}

void initWifi(){
  WiFi.begin(ssid,password);

  int timeout = 10 * 4; //10 detik

  while(WiFi.status() != WL_CONNECTED && (timeout-- > )){
    delay(250);  
  }

  if(WiFi.status() != WL_CONNECTED) {
     digitalWrite(D0, HIGH);
  }
}
