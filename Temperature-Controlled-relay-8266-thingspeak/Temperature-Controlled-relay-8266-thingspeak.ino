#include <DHT.h>  // Including library for DHT
#include <ESP8266WiFi.h>

String apiKey = "OG1WE9AGMND9OJ0X";   
const char *ssid = "IT solutions";   
const char *pass = "Python@786";
const char* server = "api.thingspeak.com";

#define DHTPIN 2  // DHT sensor pin
#define RELAY_PIN 5  // Relay connected to GPIO 5 (D1)

DHT dht(DHTPIN, DHT11);
WiFiClient client;

void setup() 
{
    Serial.begin(115200);
    delay(1000);
    dht.begin();

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); // Ensure relay is off initially

    Serial.println("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
}

void loop() 
{
    float h = dht.readHumidity();
    float t = dht.readTemperature();      

    if (isnan(h) || isnan(t)) 
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    } 

    // Relay Control Logic
    if (t > 31.5) {
        digitalWrite(RELAY_PIN, LOW); // Turn relay ON
        Serial.println("Relay ON - Temperature exceeded 30°C");
    } else {
        digitalWrite(RELAY_PIN, HIGH); // Turn relay OFF
        Serial.println("Relay OFF - Temperature below threshold");
    }

    if (client.connect(server, 80))  
    {                              
        String postStr = apiKey;
        postStr += "&field1=";
        postStr += String(t);
        postStr += "&field2=";
        postStr += String(h);
        postStr += "\r\n\r\n";

        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);

        Serial.print("Temperature: ");
        Serial.print(t);
        Serial.print("°C, Humidity: ");
        Serial.print(h);
        Serial.println("% - Data sent to ThingSpeak.");
    }
    client.stop(); 
    Serial.println("Waiting...");

    delay(1000);
}
