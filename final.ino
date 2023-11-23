#include <Keypad.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "VTR-2094477";
const char* password = "p3dSqGpqhyjk";

String url1 = "http://64.227.1.109/api/station/get_report_retire?access_key=This_is_the_first_key&locker="; //Agregar numero de casillero
String url2 = "http://64.227.1.109/api/station/get_report_deposit?access_key=This_is_the_first_key&locker="; //Agregar numero de casillero
String url3 = "http://64.227.1.109/api/station/get_update?access_key=This_is_the_first_key&state=";

int iterador = 0;

int WEIGHT_PIN1 = 23;
int WEIGHT_PIN2 = 22;
int WEIGHT_PIN3 = 21;
Servo servo1;
Servo servo2;
Servo servo3;
int servoPin1 = 12;
int servoPin2 = 14;
int servoPin3 = 27;
int led1 = 25;
int led2 = 33;
int led3 = 32;

const byte ROWS = 4;
const byte COLS = 4;
String password1;
String password2;
String password3 = "";
String input = "";

int correct = 0;
int open_door = 0;

int weightstate1 = 0;
int weightstate2 = 0;
int weightstate3 = 0;

int locker_state1 = 0;
int locker_state2 = 0;
int locker_state3 = 0;


char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte colPins[ROWS] = {16, 4, 0, 2};
byte rowPins[COLS] = {19, 18, 5, 17};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(WEIGHT_PIN1, INPUT);
  pinMode(WEIGHT_PIN2, INPUT);
  pinMode(WEIGHT_PIN3, INPUT);
  Serial.begin(9600);   // Initialise the serial monitor
  servo1.attach(servoPin1, 1000, 2000); 
  servo2.attach(servoPin2, 1000, 2000); 
  servo3.attach(servoPin3, 1000, 2000); 

  servo1.write(180);
  servo2.write(180);
  servo3.write(180);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if(millis() >= 30000*iterador){
    HTTPClient http;
    http.begin(url3 + String(digitalRead(WEIGHT_PIN1)) + "," + String(digitalRead(WEIGHT_PIN2)) + "," + String(digitalRead(WEIGHT_PIN3)));

    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      if(payload.substring(14, 20).toInt()){
        password1 = payload.substring(14, 20);
        Serial.println(password1);
      }
      if(payload.substring(35, 41).toInt()){
        password2 = payload.substring(35, 41);
        Serial.println(password2);
      }
      if(payload.substring(56, 62).toInt()){
        password3 = payload.substring(56, 62);
        Serial.println(password3);
      }
    } else {
      Serial.println("HTTP Request failed");
    }
    Serial.println("Clave1: "+password1);
    Serial.println("Clave2: "+password2);
    Serial.println("Clave3: "+password3);
    http.end();
    iterador++;
  }

  // Read the pushed button
  char button = customKeypad.getKey();
  // Serial.println("A" + password1 + password2 + password3);
  if (button) {
    if (button == '#'){
      input.remove(input.length()-1);
      Serial.println(input);
      if(open_door == 1){
        if(!locker_state1 && weightstate1 == 1){
          Serial.println("Puerta 1 cerrada y paquete depositado");
          HTTPClient http;
          http.begin(url2 + "1");
          int httpCode = http.GET();
          if (httpCode > 0) {
            String payload = http.getString();
            if(payload.substring(13, 19).toInt()){
              password1 = payload.substring(13, 19);
              Serial.println(password1);
            }
          } else {
          Serial.println("HTTP Request failed");
          }
          http.end();
          servo1.write(180);          
          digitalWrite(led1, LOW);
          locker_state1 = 1;
        } else {
          Serial.println("Puerta 1 cerrada y paquete extraido");
          HTTPClient http;
          http.begin(url1 + "1");
          int httpCode = http.GET();
          if (httpCode > 0) {
            String payload = http.getString();
            Serial.println(payload);
          } else {
          Serial.println("HTTP Request failed");
          }
          http.end();
          servo1.write(180);
          digitalWrite(led1, LOW);
          locker_state1 = 0;
        }
      }
      if(open_door == 2){
        if(!locker_state2 && weightstate2 == 1){
          Serial.println("Puerta 2 cerrada y paquete depositado");
          HTTPClient http;
          http.begin(url2 + "2");
          int httpCode = http.GET();
          if (httpCode > 0) {
            String payload = http.getString();
            if(payload.substring(13, 19).toInt()){
              password2 = payload.substring(13, 19);
              Serial.println(password2);
            }
          } else {
          Serial.println("HTTP Request failed");
          }
          http.end();
          servo2.write(180);
          digitalWrite(led2, LOW);
          locker_state2 = 1;
        } else {
          Serial.println("Puerta 2 cerrada y paquete extraido");
          HTTPClient http;
          http.begin(url1 + "2");
          int httpCode = http.GET();
          if (httpCode > 0) {
            String payload = http.getString();
            Serial.println(payload);
          } else {
          Serial.println("HTTP Request failed");
          }
          http.end();
          servo2.write(180);
          digitalWrite(led2, LOW);
          locker_state2 = 0;
        }
      }
      if(open_door == 3){
        if(!locker_state3 && weightstate3 == 1){
          Serial.println("Puerta 3 cerrada y paquete depositado");
          HTTPClient http;
          http.begin(url2 + "3");
          int httpCode = http.GET();
          if (httpCode > 0) {
            String payload = http.getString();
            if(payload.substring(13, 19).toInt()){
              password3 = payload.substring(13, 19);
              Serial.println(password3);
            }
          } else {
          Serial.println("HTTP Request failed");
          }
          http.end();
          servo3.write(180);
          digitalWrite(led3, LOW);
          locker_state3 = 1;
        } else {
          Serial.println("Puerta 3 cerrada y paquete extraido");
          HTTPClient http;
          http.begin(url1 + "3");
          int httpCode = http.GET();
          if (httpCode > 0) {
            String payload = http.getString();
            Serial.println(payload);
          } else {
          Serial.println("HTTP Request failed");
          }
          http.end();
          servo3.write(180);
          digitalWrite(led3, LOW);
          locker_state3 = 0;
        }
      }
      open_door = 0;
    }
    else if (button == '*'){
      Serial.println("Digito borrado");
      input.remove(input.length() - 1);
      Serial.println(input);
    }
    else{
      input += String(button);
      Serial.println(button);
      Serial.println(input);
      if(input.length() == 6){
        if(input == password1){
          Serial.println("Correct1");
          correct = 1;
        }
        else if(input == password2){
          Serial.println("Correct2");
          correct = 2;
        }
        else if(input == password3){
          Serial.println("Correct3");
          correct = 3;
          Serial.println(correct);
        }
        else{
          Serial.println("Wrong");
          input = "";
          correct = 0;
        }
      }
    }
  }
  if(correct){
    open_door = correct;
    input = String("");
  }

  if(correct){
    Serial.println(correct);
    if(correct == 1){
      digitalWrite(led1, HIGH);  // turn the LED on (HIGH is the voltage level) 
      // servo1.writeMicroseconds(1000);
      servo1.write(0);
    }
    else if(correct == 2){
      digitalWrite(led2, HIGH);  // turn the LED on (HIGH is the voltage level)
      // servo2.writeMicroseconds(1000);
      servo2.write(0);
    }
    else if(correct == 3){
      digitalWrite(led3, HIGH);  // turn the LED on (HIGH is the voltage level) 
      // servo3.writeMicroseconds(1000);
      servo3.write(0);
    }
    correct = 0;
  }
  if(open_door){
    if(open_door == 1){
      weightstate1 = digitalRead(WEIGHT_PIN1);
      Serial.println(weightstate1);
    }
    else if(open_door == 2){
      weightstate2 = digitalRead(WEIGHT_PIN2);
      Serial.println(weightstate2);
    }
    else if(open_door == 3){
      weightstate3 = digitalRead(WEIGHT_PIN3);
      Serial.println(weightstate3);
    }
  }
}
