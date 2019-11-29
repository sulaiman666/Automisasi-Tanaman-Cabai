#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <DS3231.h>
#include <Wire.h>

//Inisiasi RTC
DS3231 rtc(SDA,SCL);
Time waktu;

//Trigger
unsigned long id = 0;

//Pemetaan pin sensor
#define sensor0 A0
#define sensor1 A1
#define sensor2 A2
#define sensor3 A3

//Pemetaan pin Relay
#define relay0 3
#define relay1 4
#define relay2 7
#define relay3 8

//Inisiasi layar LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Inisiasi NodeMcu
SoftwareSerial s(5,6);

void setup() {
  // put your setup code here, to run once:
  s.begin(9600);
  Serial.begin(9600);

  //Penentuan mode pin sensor
  pinMode(sensor0, INPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);

  //Pemetaan mode pinRelay
  pinMode(relay0, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  //Begin LCD
  lcd.begin();

  //Backlight LCD
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" Automisasi Tanaman");
  lcd.setCursor(0,1);
  lcd.print("       Cabai");
  lcd.setCursor(0,2);
  lcd.print("    Version Beta");
  lcd.setCursor(0,3);
  lcd.print("    Initializing");

  //Inisiasi Jam
  rtc.begin();
  //rtc.setDate(29, 11, 2019);     // Set tanggal 20 September 2016
  
  delay(5000);
}

void loop() {
  //jeda = millis();

  String pesan="";
  
  //LCD Greet
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Automisasi Tanaman");
  lcd.setCursor(0,1);
  lcd.print("       Cabai");
  lcd.setCursor(0,2);
  lcd.print("Memeriksa Kelembapan");
  lcd.setCursor(0,3);
  lcd.print("      Scanning");

  delay(5000);
  
  //Menghitung kelembapan tanah
  int kelembapan0 = map(analogRead(sensor0),0,1023,100,0);//Mendapatkan value pada sensor0
  int kelembapan1 = map(analogRead(sensor1),0,1023,100,0);//Mendapatkan value pada sensor1
  int kelembapan2 = map(analogRead(sensor2),0,1023,100,0);//Mendapatkan value pada sensor2
  int kelembapan3 = map(analogRead(sensor3),0,1023,100,0);//Mendapatkan value pada sensor3

  //Mengelompokkan berdasarkan sinar matahari
  int kelompok0 = (kelembapan0+kelembapan1)/2;
  Serial.println(kelompok0);
  int kelompok1 = (kelembapan2+kelembapan3)/2;
  Serial.println(kelompok1);

  //Conditioning berdasarkan kelompok
  if(kelompok1<60 && kelompok0<60){
    //Update Trigger
    id++;
    Serial.println(id+" 3");
    
    //Print LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Automisasi Tanaman");
    lcd.setCursor(0,1);
    lcd.print("      Cabai");
    lcd.setCursor(0,2);
    lcd.print("     Showering");
    lcd.setCursor(0,3);
    lcd.print("  Kelompok 1 dan 2");
  
    //Store kapan trigger terjadi
    waktu=rtc.getTime();

    //Store waktu
    uint8_t jam = waktu.hour;
    uint8_t menit = waktu.min;
    uint8_t detik = waktu.sec;
    
    //Store tanggal
    uint8_t hari = waktu.date;
    uint8_t bulan = waktu.mon;
    uint8_t tahun = waktu.year;

    //Menyalakan keran air
    digitalWrite(relay0, HIGH);
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);

    //Transfer trigger
    String index = String(id);
    
    //Transfer Sensor
    String sensor = "|" ;
    sensor += sensor0 ;
    sensor += "|" ;
    sensor += sensor1 ;
    sensor += "|" ;
    sensor += sensor2 ;
    sensor +="|" ;
    sensor+= sensor3;
    
    //Transfer Sensor Kelompok
    String kelompok = "|" ;
    kelompok += kelompok0 ;
    kelompok += "|" ;
    kelompok += kelompok1;
    
    //Transfer Waktu
    String waktu = "|" ;
    waktu += jam ;
    waktu += "|" ;
    waktu += menit; 
    waktu += "|" ;
    waktu += detik;

    //Transfer tanggal
    String tanggal = "|" ;
    tanggal += hari ;
    tanggal += "|" ;
    tanggal += bulan ;
    tanggal += "|" ;
    tanggal += tahun;
    tanggal += "|" ;

    //Keseluruhan
    pesan = index + sensor + kelompok + waktu + tanggal;
    Serial.println(pesan);

    for(int i = 0; i< pesan.length(); i++){
      s.write(pesan[i]);  
    }

//    if(s.available()>0){
//      s.write(pesan);
//    }

    sensor = "";
    kelompok = "";
    waktu = "";
    tanggal = "";
    index = "";
    
    delay(30000);

    //Mematikankeran air
    digitalWrite(relay0, LOW);
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);

    //Delay 30 menit untuk penyesuaian kelembapan

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Automisasi Tanaman");
    lcd.setCursor(0,1);
    lcd.print("       Cabai");
    lcd.setCursor(0,2);
    lcd.print("Memeriksa Kelembapan");
    lcd.setCursor(0,3);
    lcd.print("     Calibrating");
    
    delay(1800000);
    
  }
  else if(kelompok0<60){
    //Upload Trigger
    id++;
    Serial.println(id);
    
    //Print LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Automisasi Tanaman");
    lcd.setCursor(0,1);
    lcd.print("       Cabai");
    lcd.setCursor(0,2);
    lcd.print("     Showering");
    lcd.setCursor(0,3);
    lcd.print("    Kelompok 0");

    //Store kapan trigger terjadi
    waktu=rtc.getTime();

    //Store waktu
    uint8_t jam = waktu.hour;
    uint8_t menit = waktu.min;
    uint8_t detik = waktu.sec;
    
    //Store tanggal
    uint8_t hari = waktu.date;
    uint8_t bulan = waktu.mon;
    uint8_t tahun = waktu.year;

    //Menyalakan keran air
    digitalWrite(relay0, HIGH);
    digitalWrite(relay1, HIGH);

   //Transfer trigger
    String index = String(id);
    
    //Transfer Sensor
    String sensor = "|" ;
    sensor += sensor0 ;
    sensor += "|" ;
    sensor += sensor1 ;
    sensor += "|" ;
    sensor += sensor2 ;
    sensor +="|" ;
    sensor+= sensor3;
    
    //Transfer Sensor Kelompok
    String kelompok = "|" ;
    kelompok += kelompok0 ;
    kelompok += "|" ;
    kelompok += kelompok1;
    
    //Transfer Waktu
    String waktu = "|" ;
    waktu += jam ;
    waktu += "|" ;
    waktu += menit; 
    waktu += "|";
    waktu += detik;

    //Transfer tanggal
    String tanggal = "|" ;
    tanggal += hari ;
    tanggal += "|" ;
    tanggal += bulan ;
    tanggal += "|" ;
    tanggal += tahun;
    tanggal += "|" ;

    //Keseluruhan
    pesan = index + sensor + kelompok + waktu + tanggal;
    Serial.println(pesan);

    for(int i = 0; i< pesan.length(); i++){
      s.write(pesan[i]);  
    }

//    if(s.available()>0){
//      s.write(pesan);
//    }

    sensor = "";
    kelompok = "";
    waktu = "";
    tanggal = "";
    index = "";

    delay(30000);

    //Mematikankeran air
    digitalWrite(relay0, LOW);
    digitalWrite(relay1, LOW);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Automisasi Tanaman");
    lcd.setCursor(0,1);
    lcd.print("       Cabai");
    lcd.setCursor(0,2);
    lcd.print("Memeriksa Kelembapan");
    lcd.setCursor(0,3);
    lcd.print("     Calibrating");

     delay(1800000);
  
  }else if(kelompok1<60){
    //Update Trigger
    id++;
    Serial.println(id+" 2");
    
    //Print LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Automisasi Tanaman");
    lcd.setCursor(0,1);
    lcd.print("      Cabai");
    lcd.setCursor(0,2);
    lcd.print("     Showering");
    lcd.setCursor(0,3);
    lcd.print("    Kelompok 1");
  
    //Store kapan trigger terjadi
    waktu=rtc.getTime();

    //Store waktu
    uint8_t jam = waktu.hour;
    uint8_t menit = waktu.min;
    uint8_t detik = waktu.sec;
    
    //Store tanggal
    uint8_t hari = waktu.date;
    uint8_t bulan = waktu.mon;
    uint8_t tahun = waktu.year;

    //Menyalakan keran air
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);

    //Transfer trigger
    String index = String(id);
    
    //Transfer Sensor
    String sensor = "|" ;
    sensor += sensor0 ;
    sensor += "|" ;
    sensor += sensor1 ;
    sensor += "|" ;
    sensor += sensor2 ;
    sensor +="|" ;
    sensor+= sensor3;
    
    //Transfer Sensor Kelompok
    String kelompok = "|" ;
    kelompok += kelompok0 ;
    kelompok += "|" ;
    kelompok += kelompok1;
    
    //Transfer Waktu
    String waktu = "|" ;
    waktu += jam ;
    waktu += "|" ;
    waktu += menit; 
    waktu += "|" ;
    waktu += detik;

    //Transfer tanggal
    String tanggal = "|" ;
    tanggal += hari ;
    tanggal += "|" ;
    tanggal += bulan ;
    tanggal += "|" ;
    tanggal += tahun;
    tanggal += "|" ;

    //Keseluruhan
    pesan = index + sensor + kelompok + waktu + tanggal;
    Serial.println(pesan);

    for(int i = 0; i< pesan.length(); i++){
      s.write(pesan[i]);  
    }

//    if(s.available()>0){
//      s.write(pesan);
//    }

    sensor = "";
    kelompok = "";
    waktu = "";
    tanggal = "";
    index = "";
    
    delay(30000);

    //Mematikankeran air
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);

    //Calibrating    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Automisasi Tanaman");
    lcd.setCursor(0,1);
    lcd.print("       Cabai");
    lcd.setCursor(0,2);
    lcd.print("Memeriksa Kelembapan");
    lcd.setCursor(0,3);
    lcd.print("     Calibrating");

    //Delay 30 menit untuk penyesuaian kelembapan
    
    delay(1800000);
    
  }
}
