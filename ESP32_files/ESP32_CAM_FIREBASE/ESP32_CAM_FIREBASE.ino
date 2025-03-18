#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <base64.h> // by Densaugeo

// Credenciales WiFi
const char* ssid = "Bandle";
const char* password = "YordlepeLudito1120";

// Firebase config
const char* function_url = "https://us-central1-ranitas-test.cloudfunctions.net/uploadImage"; // URL de Cloud Function
const char* firebaseURL = "https://ranitas-test-default-rtdb.firebaseio.com/Incu1.json";

// Sensores de temperatura
// DHT22
#define DHT1_PIN 12  
#define DHT2_PIN 13  
#define DHT3_PIN 2   
#define DHT_TYPE DHT22

DHT dht1(DHT1_PIN, DHT_TYPE);
DHT dht2(DHT2_PIN, DHT_TYPE);
DHT dht3(DHT3_PIN, DHT_TYPE);

#define ONE_WIRE_BUS 14
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Variables de temperatura
float temp1, hum1, temp2, hum2, temp3, hum3, tempDS;

// Pines de la cámara
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

#define LED_GPIO_NUM 4 // Flash

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Conectar WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("🌐 Conectando...");
  }
  Serial.println("✅ WiFi conectado");

  // Inicializar cámara
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Calidad de imagen
  if (psramFound()) {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 15;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 20;
    config.fb_count = 1;
  }
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("❌ Error inicializando cámara: 0x%x\n", err);
    return;
  }

  Serial.println("Iniciando sensores...");
  
  // Inicialización de los sensores
  sensors.begin();
  dht1.begin();
  dht2.begin();
  dht3.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    readSensors();
    captureAndUploadToFirebase();
  }
  delay(15000);  // ⏳ Esperar 15 segundos entre capturas
}

void captureAndUploadToFirebase() {
  Serial.println("📸 Tomando foto...");
  analogWrite(LED_GPIO_NUM, 180);  

  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("❌ Error en captura");
    analogWrite(LED_GPIO_NUM, 0);
    return;
  }

  Serial.printf("📏 Tamaño de la imagen: %d bytes\n", fb->len);
  Serial.println("📤 Subiendo imagen a Firebase...");
  String imageData = base64::encode(fb->buf, fb->len);

  HTTPClient http;
  http.begin(function_url);
  http.addHeader("Content-Type", "application/json"); // Se envia como JSON
  String payload = "{\"imageData\": \"" + imageData + "\"}";

  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    Serial.println("✅ Imagen subida con éxito");
    String response = http.getString();
    Serial.println("📄 Respuesta del servidor:");
    Serial.println(response);
  } else {
    Serial.printf("❌ Error al subir imagen: %s\n", http.errorToString(httpResponseCode).c_str());
  }
  analogWrite(LED_GPIO_NUM, 0);  

  http.end();
  esp_camera_fb_return(fb);
  digitalWrite(LED_GPIO_NUM, LOW);
}

void readSensors() {
  // Lectura del DS18B20
  sensors.requestTemperatures(); 
  tempDS = sensors.getTempCByIndex(0);
  
  if (tempDS == -127.00) {
    Serial.println("⚠️ Error: No se pudo leer la temperatura del DS18B20.");
    tempDS = 0; // Evita que el error afecte el promedio
  } else {
    Serial.print("🌡️ DS18B20: ");
    Serial.print(tempDS);
    Serial.println("°C");
  }

  // Lectura de los DHT22
  temp1 = dht1.readTemperature();
  hum1 = dht1.readHumidity();
  temp2 = dht2.readTemperature();
  hum2 = dht2.readHumidity();
  temp3 = dht3.readTemperature();
  hum3 = dht3.readHumidity();

  // Verificación de valores erróneos (NaN)
  int validReadings = 0; 
  float sumTemp = 0;  
  float sumHum = 0;

  if (!isnan(temp1)) { sumTemp += temp1; validReadings++; }
  if (!isnan(temp2)) { sumTemp += temp2; validReadings++; }
  if (!isnan(temp3)) { sumTemp += temp3; validReadings++; }
  if (tempDS != 0) { sumTemp += tempDS; validReadings++; } // Agrega el DS18B20 si es válido

  if (!isnan(hum1)) { sumHum += hum1; }
  if (!isnan(hum2)) { sumHum += hum2; }
  if (!isnan(hum3)) { sumHum += hum3; }

  // Cálculo del promedio
  float temp_prom = (validReadings > 0) ? (sumTemp / validReadings) : 0;
  float hum_prom  = (sumHum / 3); // 3 sensores de humedad, siempre sumamos los tres

  // Mostrar los datos de los DHT22
  Serial.print("🌡️ DHT22_1: "); Serial.print(temp1); Serial.print("°C  ");
  Serial.print("💧 Humedad: "); Serial.print(hum1); Serial.println("%");

  Serial.print("🌡️ DHT22_2: "); Serial.print(temp2); Serial.print("°C  ");
  Serial.print("💧 Humedad: "); Serial.print(hum2); Serial.println("%");

  Serial.print("🌡️ DHT22_3: "); Serial.print(temp3); Serial.print("°C  ");
  Serial.print("💧 Humedad: "); Serial.print(hum3); Serial.println("%");

  // Imprimir promedio de temperatura y humedad
  Serial.println("------------------------------------------------");
  Serial.print("📊 Promedio Temperatura (DHT22 + DS18B20): "); Serial.print(temp_prom); Serial.println("°C");
  Serial.print("📊 Promedio Humedad DHT22: "); Serial.print(hum_prom); Serial.println("%");
  Serial.println("------------------------------------------------");

  // Mandar datos a Firebase
  HTTPClient http;
  http.begin("https://ranitas-test-default-rtdb.firebaseio.com/Incu1.json");
  http.addHeader("Content-Type", "application/json");
  String payload = "{\"DS18\":" + String(tempDS) + ",\"Temp1\":" + String(temp1) + ",\"Temp2\":" + String(temp2) + ",\"Temp3\":" + String(temp3) + ",\"Hum1\":" + String(hum1) + ",\"Hum2\":" + String(hum2) + ",\"Hum3\":" + String(hum3) + ",\"PromT\":" + String(temp_prom) + ",\"PromH\":" + String(hum_prom) + "}";
  int httpCode = http.PUT(payload);

  if (httpCode == 200) {
    Serial.println("Datos enviados a Firebase");
    String response = http.getString();
    Serial.println("📄 Respuesta del servidor:");
    Serial.println(response);
  } else {
    Serial.println("Error: " + String(httpCode));
    String response = http.getString();
    Serial.println("📄 Respuesta del servidor:");
    Serial.println(response);
  }
  http.end();
  
}