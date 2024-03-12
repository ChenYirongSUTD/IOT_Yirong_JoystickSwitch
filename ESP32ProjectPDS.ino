#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
/* 1. Define the WiFi credentials */
#define WIFI_SSID "ask me for password"
#define WIFI_PASSWORD "03052000"
/* 2. Define the API Key */
#define API_KEY "AIzaSyC-xaggNSsar4qXgnMv13vqGRm6LWu_Y78"
#define DATABASE_URL "https://pds-project-d5863-default-rtdb.asia-southeast1.firebasedatabase.app/" 
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
// Define Firebase Data object
/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "yirong_chen@mymail.sutd.edu.sg"
#define USER_PASSWORD "passhair1"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

TaskHandle_t firebaseTask; //test
// Define button and LED pins
#define buttonPin 3
#define ledR 2
#define ledF 9
#define ledL 7
bool ledOn = false;
bool buttonPressed = false;  // Keeps track of current button state
#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
unsigned long sendDataPrevMillis = 0;
unsigned long sendDataPrevMillis2 = 0;
int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int lgetValue = 0;
int rgetValue = 0;
int fgetValue = 0;
int count = 0;
bool leftBumped = false;
bool rightBumped = false;
bool frontBumped = false;
bool currentButtonState = false;
int readcount = 0;

void setup()
{
  buttonPressed = false;
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledR, OUTPUT);
  pinMode(ledF, OUTPUT);
  pinMode(ledL, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);
  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
}

void loop() {  
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 100 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    currentButtonState = digitalRead(buttonPin);
    xValue = analogRead(VRX_PIN);
    yValue = analogRead(VRY_PIN);
    // Detect button press (transition from high to low)
    if (currentButtonState == LOW && !buttonPressed) {
      buttonPressed = true;
      Firebase.set(fbdo, "Analog/l", 0);
      Firebase.set(fbdo, "Analog/r", 0);
      Firebase.set(fbdo, "Analog/f", 0);
      Serial.println("Button pressed! Off everything.");
    }
    // Detect button release (transition from low to high)
    if (currentButtonState == HIGH && buttonPressed) {
      buttonPressed = false;
    }
    //READING from RTDB constantly every 1 sec
    if (count == 12){
      if (Firebase.getInt(fbdo, "/Analog/l")) {
        lgetValue = fbdo.intData();
      }
      if (Firebase.getInt(fbdo, "/Analog/r")) {
        rgetValue = fbdo.intData();
      }
      if (Firebase.getInt(fbdo, "/Analog/f")) {
        fgetValue = fbdo.intData();
      }
      count = 0; //reset counter for every 12 * 100ms 
      Serial.println("===READING!===");
      readcount = readcount + 1; //keeping track of how many times the database has been read
      Serial.print(readcount);
    }
    // Toggle LED state
    if(leftBumped || rightBumped || frontBumped){
      if ( (xValue > 1000) && (xValue < 3600) ){
        if (leftBumped){   
          if (lgetValue == 1){
            Firebase.set(fbdo, "Analog/l", 0);
          }
          else{
            Firebase.set(fbdo, "Analog/l", 1);      
          }
          Serial.println("Joystick bumped to the left!.");
        }
        if (rightBumped){
          if (rgetValue == 1){
            Firebase.set(fbdo, "Analog/r", 0);
          }
          else{
            Firebase.set(fbdo, "Analog/r", 1);      
          }
          Serial.println("Joystick bumped to the right!.");
        }
      } 
      if ( (yValue > 1000) && (yValue < 3600) ){
        if (frontBumped){
          if (fgetValue == 1){
            Firebase.set(fbdo, "Analog/f", 0);
          }
          else{
            Firebase.set(fbdo, "Analog/f", 1);      
          }
          Serial.println("Joystick bumped to the front!.");
        }
      }
    }
    //Act based on retrieved RTDB values
    digitalWrite(ledL, lgetValue == 1 ? HIGH : LOW);
    digitalWrite(ledR, rgetValue == 1 ? HIGH : LOW);
    digitalWrite(ledF, fgetValue == 1 ? HIGH : LOW);
    //reading analog values of joystick
    leftBumped = (xValue < 2000) ? true : false;
    rightBumped = (xValue > 3600) ? true : false;
    frontBumped = (yValue < 2000) ? true : false;
    Serial.print(".");
    count = count + 1;
  }
}



