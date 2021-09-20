#include <SoftwareSerial.h>

SoftwareSerial ESPserial(2, 3); // RX | TX
String AP = "";                 // AP NAME
String PASS = "";               // AP PASSWORD
int countTimeCommand = 0;
boolean found = false;
String API = "ZKT750WGBDFDB438"; // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field2";

const int trigPin = 6;
const int echoPin = 7;

float distance;

void setup()
{
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(echoPin, INPUT);

    Serial.begin(9600); // communication with the host computer
    // Start the software serial for communication with the ESP8266
    ESPserial.begin(9600);
    Serial.println("");
    Serial.println("Remember to to set Both NL & CR in the serial monitor.");
    Serial.println("Ready");
    Serial.println("");
    sendCommand("AT", 5, "OK");
    sendCommand("AT+CWMODE=1", 5, "OK");
    sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
}

void loop()
{
    distance = getDistance();
    String getData = "GET /update?api_key=" + API + "&" + field + "=" + String(distance);
    sendCommand("AT+CIPMUX=1", 5, "OK");
    sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
    sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
    ESPserial.println(getData);
    delay(50);
    sendCommand("AT+CIPCLOSE=0", 5, "OK");
    delay(50);
}

void sendCommand(String command, int maxTime, char expectedResult[])
{
    while (countTimeCommand < maxTime)
    {
        ESPserial.println(command);         //at+cipsend
        if (ESPserial.find(expectedResult)) //ok
        {
            found = true;
            break;
        }
        countTimeCommand++;
    }

    countTimeCommand = 0;
    found = false;
}

float getDistance()
{
    // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Calculating the distance
    float distance = pulseIn(echoPin, HIGH) * 0.0343 / 2;
    Serial.println(distance);
    return distance; // Speed of sound wave divided by 2 (go and back)
}
