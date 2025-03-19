#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

// Blynk Authentication Token
#define BLYNK_AUTH "Y6YAEVFDmp0JEdZYsL3yEphkW-dudQri"

// WiFi Credentials
#define WIFI_SSID "SAMSUNG A15"
#define WIFI_PASS "123456789"

// PZEM-004T Serial Communication (D7 = RX, D8 = TX)
SoftwareSerial pzemSerial(D7, D8);
PZEM004Tv30 pzem(pzemSerial);

// Blynk Timer
BlynkTimer timer;

// Function to read PZEM data and send it to Blynk
void sendData() {
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();

    // Check if values are valid
    if (isnan(voltage) || isnan(current) || isnan(power) || isnan(energy)) {
        Serial.println("⚠️ Error reading PZEM data!");
        return;
    }

    // Print values to Serial Monitor
    Serial.println("=========================");
    Serial.print("🔋 Voltage: "); Serial.print(voltage); Serial.println(" V");
    Serial.print("⚡ Current: "); Serial.print(current); Serial.println(" A");
    Serial.print("💡 Power: "); Serial.print(power); Serial.println(" W");
    Serial.print("🔄 Energy: "); Serial.print(energy); Serial.println(" kWh");
    Serial.println("=========================");

    // Send data to Blynk App
    Blynk.virtualWrite(V0, voltage);  // V0 - Voltage
    Blynk.virtualWrite(V1, current);  // V1 - Current
    Blynk.virtualWrite(V2, power);    // V2 - Power
    Blynk.virtualWrite(V3, energy);   // V3 - Energy
}

void setup() {
    Serial.begin(115200);
    pzemSerial.begin(9600);

    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("🔄 Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\n✅ Connected to WiFi!");

    // Start Blynk
    Blynk.begin(BLYNK_AUTH, WIFI_SSID, WIFI_PASS, "blynk.cloud", 80);

    // Set a timer to send data every 5 seconds
    timer.setInterval(5000L, sendData);
}

void loop() {
    Blynk.run();
    timer.run();
}
