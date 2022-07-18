/*
https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/SimpleWiFiServer/SimpleWiFiServer.ino
 
 */

#include <WiFi.h>
#include <String.h>

const char* ssid = "1102 Signal";
const char* password =  "1102development";

WiFiServer server(80);

int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 50;
unsigned long currentMillis = 0; 

//Index Finger
const int indexA = 25;
const int indexB = 26;
unsigned long indexTimer = 0;

//Middle Finger
const int middleA = 32;
const int middleB = 33;
unsigned long middleTimer = 0;

//Ring Finger
const int ringA = 18;
const int ringB = 19;
unsigned long ringTimer = 0;

//Pinky
const int pinkyA = 22;
const int pinkyB = 23;
unsigned long pinkyTimer = 0;

int speed = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(21, OUTPUT);

    delay(10);
    WiFi.begin(ssid, password);

    currentMillis = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          if (ledState == LOW){
            ledState = HIGH;
          }
          else {
            ledState = LOW;
          }
          digitalWrite(21, ledState);
        }
    }
    digitalWrite(21, HIGH);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();

}

int value = 0;

void loop(){
  WiFiClient client = server.available();
  currentMillis = millis();

   // Turn off after some time
  if (currentMillis - indexTimer > 10) {
    haptic_vibrate(indexA, indexB, "OFF");
  }
  if (currentMillis - middleTimer > 10) {
    haptic_vibrate(middleA, middleB, "OFF");
  }
  if (currentMillis - ringTimer > 10) {
    haptic_vibrate(ringA, ringB, "OFF");
  }
  if (currentMillis - pinkyTimer > 10) {
    haptic_vibrate(pinkyA, pinkyB, "OFF");
  }

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);



        //Read user request
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        //Get input
        if (currentLine.endsWith("GET /finger2")) {
          indexTimer = currentMillis;
          haptic_vibrate(indexA, indexB, "ON");
        }
        if (currentLine.endsWith("GET /finger3")) {
          middleTimer = currentMillis;
          haptic_vibrate(middleA, middleB, "ON");
        }
        if (currentLine.endsWith("GET /finger4")) {
          ringTimer = currentMillis;
          haptic_vibrate(ringA, ringB, "ON");
        }
        if (currentLine.endsWith("GET /finger5")) {
          pinkyTimer = currentMillis;
          haptic_vibrate(pinkyA, pinkyB, "ON");
        }

        
      }
    }
    // close the connection:
    client.stop();
  }
}

void haptic_vibrate(int A, int B, String status){
  if(status == "ON"){
    speed = 200;
  }
  else {
    speed = 0;
  }
  analogWrite(A, 0);
  analogWrite(B, speed);
}
