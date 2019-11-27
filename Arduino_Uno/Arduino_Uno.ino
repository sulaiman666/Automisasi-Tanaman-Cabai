#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <DS3231.h>
#include <Wire.h>

//Inisiasi RTC
DS3231 rtc(SDA,SCL);
Time waktu;
unsigned long jeda = 0;

//Pemetaan pin sensor
#define sensor0 A0
#define sensor1 A1
#define sensor2 A2
#define sensor3 A3

//Pemetaan pin Relay
#define relay0 D0
#define relay1 D1

//Inisiasi layar LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Inisiasi NodeMcu
SoftwareSerial s(5,6);

void setup() {
  // put your setup code here, to run once:
  s.begin(115200);
  Serial.begin(115200);

  //Penentuan mode pin sensor
  pinMode(sensor0, INPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);

  //Pemetaan mode pinRelay
  pinMode(relay0, OUTPUT);
  pinMode(relay1, OUTPUT);

  //Begin LCD
  lcd.begin();

  //Backlight LCD
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" Automisasi Tanaman");
  lcd.setCursor(1,0);
  lcd.print("       Cabai");
  lcd.setCursor(2,0);
  lcd.print("    Version Beta");
  lcd.setCursor(3,0);
  lcd.print("    Initializing");

  //Inisiasi Jam
  rtc.begin();

  //Konfigurasi jam
  rtc.setDOW(SELASA);       //Set hari
  rtc.setTime(00, 47, 00);  //Set jam
  rtc.setDate(27, 11, 2019);//Set Tanggal

  jeda = millis();
  while(millis() < jeda + 5000){
    //Waiting
  }
}

void loop() {
  //LCD Greet
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Automisasi Tanaman");
  lcd.setCursor(1,0);
  lcd.print("       Cabai");
  lcd.setCursor(2,0);
  lcd.print("Memeriksa Kelembapan");
  lcd.setCursor(3,0);
  lcd.print("      Scanning");

  jeda = millis();
  while(millis() < jeda + 5000){
    //Waiting
  }
  
  //Menghitung kelembapan tanah
  int kelembapan0 = map(analogRead(sensor0),0,1023,100,0);//Mendapatkan value pada sensor0
  int kelembapan1 = map(analogRead(sensor1),0,1023,100,0);//Mendapatkan value pada sensor1
  int kelembapan2 = map(analogRead(sensor2),0,1023,100,0);//Mendapatkan value pada sensor2
  int kelembapan3 = map(analogRead(sensor3),0,1023,100,0);//Mendapatkan value pada sensor3

  //Mengelompokkan berdasarkan sinar matahari
  int kelompok0 = (kelembapan0+kelembapan1)/2;
  int kelompok1 = (kelembapan2+kelembapan3)/2;

  //Conditioning berdasarkan kelompok
  if(kelompok0<60){
    //Print LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Automisasi Tanaman");
    lcd.setCursor(1,0);
    lcd.print("       Cabai");
    lcd.setCursor(2,0);
    lcd.print("     Showering");
    lcd.setCursor(3,0);
    lcd.print("    Kelompok 0");
    
    //Menyalakan keran air
    digitalWrite(relay0, HIGH);

    //Store kapan trigger terjadi
    waktu=rtc.getTime();
    int jam = waktu.hour;
    int menit = waktu.min;
    int detik = waktu.sec

    String tanggal = rtc.getDOWStr(FORMAT_SHORT);

    //Upload ke NodeMcu

    //Delay 30 detik
    while(millis() < jeda + 30000){
        //wait
    }

    //Mematikankeran air
    digitalWrite(relay0, LOW);

    //Delay 30 menit untuk penyesuaian kelembapan
    while(millis() < jeda + 1800000){
        //wait
    }  
  }else if(kelompok1<60){
    //Print LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Automisasi Tanaman");
    lcd.setCursor(1,0);
    lcd.print("       Cabai");
    lcd.setCursor(2,0);
    lcd.print("     Showering");
    lcd.setCursor(3,0);
    lcd.print("    Kelompok 1");
    
    //Menyalakan keran air
    digitalWrite(relay1, HIGH);

    //Store kapan trigger terjadi
    waktu=rtc.getTime();
    int jam = waktu.hour;
    int menit = waktu.min;
    int detik = waktu.sec

    String tanggal = rtc.getDOWStr(FORMAT_SHORT);

    //Upload ke NodeMcu

    //Delay 30 detik
    while(millis() < jeda + 30000){
        //wait
    }

    //Mematikankeran air
    digitalWrite(relay1, LOW);

    //Delay 30 menit untuk penyesuaian kelembapan
    while(millis() < jeda + 1800000){
        //wait
    }
  }
}
