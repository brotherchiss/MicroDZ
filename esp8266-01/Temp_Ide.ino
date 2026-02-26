#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

/* ================== HARDWARE ================== */
#define DHTPIN   D4        // GPIO2
#define DHTTYPE  DHT22
DHT dht(DHTPIN, DHTTYPE);

/* ================== MQTT ================== */
const char* mqtt_server = "broker.hivemq.com";
const int   mqtt_port   = 1883;
const char* topic_data   = "sensor/esp8266_01/dht22";
const char* topic_status = "sensor/esp8266_01/status";

WiFiClient espClient;
PubSubClient client(espClient);
WiFiManager wm;

/* ================== MQTT RECONNECT ================== */
void reconnectMQTT() {
  static unsigned long lastTry = 0;
  if(client.connected()) return;
  if(millis() - lastTry < 5000) return;
  lastTry = millis();

  Serial.print("MQTT connect...");
  if(client.connect("ESP8266_DHT22_01")){
    Serial.println("OK");
    client.publish(topic_status,"ONLINE");
  } else {
    Serial.print("FAIL rc=");
    Serial.println(client.state());
  }
}

/* ================== PUBLISH DATA ================== */
void publishData(float temperature, float humidity) {
  DynamicJsonDocument doc(256);
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;

  char payload[128];
  serializeJson(doc, payload, sizeof(payload));

  if(client.connected()){
    client.publish(topic_data, payload);
    Serial.print("Publish: ");
    Serial.println(payload);
  } else {
    Serial.println("MQTT belum connect, data tidak dikirim");
  }
}

/* ================== SETUP ================== */
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n===== BOOT ESP8266 =====");

  /* ===== WiFiManager ===== */
  bool res = wm.autoConnect("ESP8266-Setup","12345678");
  if(!res){
    Serial.println("WiFi gagal, restart");
    ESP.restart();
  }
  Serial.print("WiFi OK, IP: ");
  Serial.println(WiFi.localIP());

  /* ===== MQTT ===== */
  client.setServer(mqtt_server, mqtt_port);
  Serial.println("MQTT siap");

  /* ===== SENSOR ===== */
  dht.begin();
  Serial.println("DHT22 siap");
}

/* ================== LOOP ================== */
unsigned long lastSend = 0;

void loop() {
  reconnectMQTT();
  client.loop();

  if(millis() - lastSend >= 5000){
    lastSend = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if(isnan(t) || isnan(h)){
      Serial.println("Gagal baca DHT22");
      return;
    }

    // ===== TAMPILKAN DI SERIAL =====
    Serial.printf("Temp: %.1f °C, Hum: %.1f %%\n", t, h);

    // ===== PUBLISH KE MQTT =====
    publishData(t, h);
  }
}
