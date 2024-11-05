//Required libraries --> Must be installed!
#include <OOCSI.h>  //OOCSI
#include <WiFi.h>   //Add wifi capabilities

// Wi-Fi credentials
//If connecting to wifi gives issues, try a mobile hotspot
const char* ssid = "";            //Your wifi name
const char* password = "";  //Your wifi password

const char* OOCSIchannelName = "iterationThree";  //change the name of the channel you want to use!

//start oocsi
OOCSI oocsi = OOCSI();

const int buttonPin = 18;  //Digital pin
const int ledPin = 19;     //Digital pin
const int piezoPin = 32;   //Analog pin

int buttonState = 0;  //Initiate that the button is not pressed
int piezoValue = 0;   //Initiate that the piezo is not touched

String receivedMessage;   //OOCSI message will be assigned to this variable
String lastPiezoMessage;  //Last piezo message will be assigned to this variable

void setup() {
  //Begin serial for debugging purposes
  Serial.begin(9600);
  Serial.print("Serial connected!");

  //Set pinmodes
  pinMode(buttonPin, INPUT_PULLDOWN);
  pinMode(piezoPin, INPUT_PULLDOWN);
  pinMode(ledPin, OUTPUT);
  //Make sure LED is turned off
  digitalWrite(ledPin, LOW);


  //Setup wifi and OOCSI
  setupWifi();
  setupOOCSI();
}

void loop() {
  //Check the OOCSI channel. If a message is received, the processOOCSI() function will be called
  oocsi.check();

  //check if button is pressed
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Buttonstate is high");

    //send OOCSI message
    oocsi.newMessage(OOCSIchannelName);
    oocsi.addString("message", "Button pressed");
    oocsi.sendMessage();
    delay(150);
  }

  //Take the analog value from the piezo
  piezoValue = analogRead(piezoPin);
  Serial.println(piezoValue);

  //Pressing hard
  if (piezoValue >= 3000) {
    if (lastPiezoMessage != "High") {
      //send OOCSI message

      oocsi.newMessage(OOCSIchannelName);
      oocsi.addString("message", "High");
      oocsi.sendMessage();
      delay(150);
      lastPiezoMessage = "High";
    }
    //Pressing firmly
  } else if (piezoValue >= 1500) {
    if (lastPiezoMessage != "Medium") {
      //send OOCSI message

      oocsi.newMessage(OOCSIchannelName);
      oocsi.addString("message", "Medium");
      oocsi.sendMessage();
      delay(150);
      lastPiezoMessage = "Medium";
    }
    //Touching
  } else if (piezoValue >= 500) {
    if (lastPiezoMessage != "Low") {
      //send OOCSI message
      oocsi.newMessage(OOCSIchannelName);
      oocsi.addString("message", "Low");
      oocsi.sendMessage();
      delay(150);
      lastPiezoMessage = "Low";
    }
  }
  //If the piezo is not being touched, do not set value to low, as it would disturb results


  // In case wifi signal is lost
  if (WiFi.status() != WL_CONNECTED) {
    setupWifi();
    setupOOCSI();
  }

  delay(350);
}

void processOOCSI() {
  //Print incoming message
  oocsi.printMessage();

  //retrieve message
  receivedMessage = oocsi.getString("message", "error");

  //handle functionality
  if (receivedMessage == "LED_ON") {
    digitalWrite(ledPin, HIGH);
    Serial.println("set LED to on");
  } else if (receivedMessage == "LED_OFF") {
    digitalWrite(ledPin, LOW);
    Serial.println("set LED to off");
  }
  Serial.print(receivedMessage);
}

void setupOOCSI() {
  // Connect to OOCSI server
  oocsi.connect("ESP32_OOCSI", "oocsi.id.tue.nl", ssid, password, processOOCSI);  //Name of this oocsi sender/receiver (anything), name of oocsi server, wifi name, wifi password

  //Subsribe to an OOCSI channel. Change this name. OOCSI will continue to listen to messages sent in this channel
  oocsi.subscribe(OOCSIchannelName);  //change this OOCSI channel

  //Create and send a test message for debugging purposes
  oocsi.newMessage(OOCSIchannelName).addString("message", "ESP32 sending test message").sendMessage();
}

void setupWifi() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {  //As long as no wifi connection is established...
    delay(5000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.macAddress());
}
