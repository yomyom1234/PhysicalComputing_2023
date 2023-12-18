#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Servo.h>
#define InDistanceTrig 4    // 내부 초음파 센서 송신
#define InDistanceEcho 3    // 내부 초음파 센서 수신
#define OutDistanceTrig 6   // 외부 초음파 센서 송신
#define OutDistanceEcho 5   // 외부 초음파 센서 수신
#define Gas A0               // 가스 센서
#define Moisture A1          // 습도 센서
#define MotorLeft 12       // 모터열기
#define MotorRight 11		// 모터닫기
bool isOpened = false;		//뚜껑 열렸는지??

DHT dht(Moisture, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo ServoLeft;
Servo ServoRight;

void setup() {
  lcd.init();
  pinMode(OutDistanceEcho, INPUT);
  pinMode(OutDistanceTrig, OUTPUT);
  pinMode(InDistanceEcho, INPUT);
  pinMode(InDistanceTrig, OUTPUT);
  ServoLeft.attach(MotorLeft);
  ServoRight.attach(MotorRight);

  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.begin(16, 2);
  lcd.print("Good Day!");

  Serial.begin(9600);
}

void loop() {
  // Checking outer sensor
  checkEntrance();
  // Checking inner sensor
  checkTrashHeight();
  // Checking garbage status
  checkGasMoisture();
}

// Function to measure distance using ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

// Open when outer sensor detect something
void checkEntrance() {
  int outDistance = getDistance(OutDistanceTrig, OutDistanceEcho);
  if (outDistance < 15 && isOpened == false)
  {
   	ServoLeft.write(170);
    ServoRight.write(-110);
    isOpened = true;
    Serial.println("Door opened");
  }
  if (outDistance > 15 && isOpened == true)
  {
   	ServoLeft.write(90);
    ServoRight.write(110);
   	isOpened = false; 
    Serial.println("Door closed");
  }
}
  
void checkTrashHeight() {
  int inDistance = getDistance(InDistanceTrig, InDistanceEcho);
  if (inDistance < 5)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Bin is full!!");
   Serial.println("Bin is full!!");
   delay(2000);
  } 
}
  
void checkGasMoisture() {
  int gasValue = analogRead(Gas);
  float moistureValue = dht.readHumidity();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas: ");
  lcd.print(gasValue);
  lcd.setCursor(0, 1);
  lcd.print("Moisture: ");
  lcd.print(moistureValue);
  delay(2000);  // Wait for 2000 milliseconds (2 seconds)
  if (gasValue > 300 || moistureValue > 300)
  {  
	  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Empty the");
    lcd.setCursor(0, 1);
    lcd.print("trashbin!!");
    delay(2000);
  }
}