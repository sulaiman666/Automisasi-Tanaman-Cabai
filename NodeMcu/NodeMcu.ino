#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

SoftwareSerial s(D6,D5);

//Variabel untuk wifi
const char* ssid = "OnePlus";
const char* password = "Oneplusone";


//API KEY Thingspeak
const char* apiKey = "Q6MDDGTS2RDX2W1I";
const char* resource = "/update?api_key=";

//Thing Speak API server
const char* server = "api.thingspeak.com";

//Variabel global
String trigger = 0;
String id=0;  



void setup() {
  Serial.begin(9600);
  s.begin(9600);

  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);

  Serial.println("Initializing");

  //Inisiasi wifi
  WiFi.begin(ssid,password);

  int timeout = 10 * 4; //10 detik

  while(WiFi.status() != WL_CONNECTED && (timeout-- > 0)){
    Serial.print(".");
    delay(250);  
  }

  if(WiFi.status() != WL_CONNECTED) {
     digitalWrite(D0, HIGH);
     Serial.print("Not Connected");
  }
  Serial.println("Connected");
}

void loop() {
  Serial.println(id);

  //Get Trigger
  if(s.available()>0){
    id=s.readStringUntil('|');
    if(strcmp(id, trigger)!=0){
      //Sensor value
      String sensor0=s.readStringUntil('|');
      String sensor1=s.readStringUntil('|');
      String sensor2=s.readStringUntil('|');
      String sensor3=s.readStringUntil('|');

      //Kelompok value
      String kelompok0=s.readStringUntil('|');
      String kelompok1=s.readStringUntil('|');

      //Waktu value
      String jam=s.readStringUntil('|');
      String menit=s.readStringUntil('|');
      String detik=s.readStringUntil('|');

      //Tanggal value
      String hari=s.readStringUntil('|');
      String bulan=s.readStringUntil('|');
      String tahun=s.readStringUntil('|');

      //Fungsi upload
      WiFiClient client;
      int retries = 5;
  
      while(!!!client.connect(server, 80) && (retries-- > 0)) {
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
      while(!!!client.available() && (timeout-- > 0)){
        delay(100);
      }
      if(!!!client.available()) {
        Serial.println("Nggak Connect");
        digitalWrite(D0, LOW);
      }
  
      while(client.available()){
        digitalWrite(D0, HIGH);
      }
  
      Serial.println("\nclosing connection");
      client.stop();

      trigger=id;
    }
  }

    
}
