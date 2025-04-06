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
#include <ArduinoJson.h> 
#include <time.h> // Biblioteca para manejar la hora

// Credenciales WiFi
const char* ssid = "Bandle";
const char* password = "YordlepeLudito1120";



// Firebase config
const char* function_url = "https://us-central1-ranitas-test.cloudfunctions.net/uploadImage"; // URL de Cloud Function
const char* firebaseURL = "https://ranitas-test-default-rtdb.firebaseio.com/Incu1.json";

// Sensores de temperatura
// DHT22
#define DHT1_PIN 2
#define DHT2_PIN 13  
#define DHT3_PIN 15  
#define DHT_TYPE DHT22

DHT dht1(DHT1_PIN, DHT_TYPE);
DHT dht2(DHT2_PIN, DHT_TYPE);
DHT dht3(DHT3_PIN, DHT_TYPE);

#define ONE_WIRE_BUS 14
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Sensor de pH
#define PH_SENSOR_PIN 12 // Pin analógico para el sensor de pH

float phValue = 0.0; // Variable para almacenar el valor de pH

// Variables de temperatura
float temp1, hum1, temp2, hum2, temp3, hum3, tempDS, temp_prom, hum_prom;

// Variables de tiempo

unsigned long lastFirebaseSendTime = 0;
unsigned long lastImageSendTime = 0;
unsigned long lastSensorReadTime = 0;

const unsigned long firebaseInterval = 30 * 60 * 1000; // 30 minutos
const unsigned long imageInterval = 5 * 60 * 1000; // 10 minutos
const unsigned long sensorReadInterval = 30 * 1000; // 30 segundos

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

  // Conectar a WiFi para sincronizar la hora
  Serial.println("🌐 Conectando a WiFi para sincronizar la hora...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi conectado");

  // Configurar el servidor NTP
  configTime(-6 * 3600, 0, "pool.ntp.org", "time.nist.gov"); // UTC-6 para México (ajusta según tu zona horaria)
  Serial.println("⏳ Sincronizando con el servidor NTP...");
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("❌ Error al obtener la hora");
    return;
  }
  Serial.println("✅ Hora sincronizada");

  // Mostrar la hora sincronizada
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);
  Serial.print("🕒 Hora sincronizada: ");
  Serial.println(timestamp);

  // Apagar WiFi para ahorrar energía
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("🌐 WiFi apagado después de sincronizar la hora");

  // Inicializar otros componentes (sensores, cámara, etc.)
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
  unsigned long currentMillis = millis();

  if (currentMillis - lastSensorReadTime >= sensorReadInterval) {
    lastSensorReadTime = currentMillis;
    readSensors();

    // Verificar si los valores están fuera de rango
    if (phValue < 6.5 || phValue > 7.5) {
      Serial.println("⚠️ pH fuera de rango. Enviando datos a Firebase...");
      sendToFirebase();
    }

    if (temp_prom < 21 || temp_prom > 28) {
      Serial.println("⚠️ Temperatura fuera de rango. Enviando datos a Firebase...");
      sendToFirebase();
    }

    if (hum_prom < 70 || hum_prom > 80) {
      Serial.println("⚠️ Humedad fuera de rango. Enviando datos a Firebase...");
      sendToFirebase();
    }
  }
    
  if (currentMillis - lastFirebaseSendTime >= firebaseInterval) {
    lastFirebaseSendTime = currentMillis;
    sendToFirebase();
  }
  if (currentMillis - lastImageSendTime >= imageInterval) {
    lastImageSendTime = currentMillis;
    captureAndUploadToFirebase();
}
}

void captureAndUploadToFirebase() {
  // Verificar conexión WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("🌐 Conectando a WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\n✅ WiFi conectado");
  }

  // Capturar imagen
  Serial.println("📸 Tomando foto...");
  analogWrite(LED_GPIO_NUM, 180); // Encender el flash

  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("❌ Error en captura");
    analogWrite(LED_GPIO_NUM, 0); // Apagar el flash
    return;
  }

  Serial.printf("📏 Tamaño de la imagen: %d bytes\n", fb->len);
  Serial.println("📤 Subiendo imagen a Firebase...");
  String imageData = base64::encode(fb->buf, fb->len);

  // Subir imagen a Firebase
  HTTPClient http;
  http.begin(function_url);
  http.addHeader("Content-Type", "application/json"); // Se envía como JSON
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

  // Liberar recursos
  analogWrite(LED_GPIO_NUM, 0); // Apagar el flash
  http.end();
  esp_camera_fb_return(fb);

  // Desconectar WiFi para ahorrar energía
  WiFi.disconnect(true);
  Serial.println("🌐 WiFi desconectado");
}

void readSensors() {
  float analogValue = analogRead(PH_SENSOR_PIN); // Leer el valor analógico
  float voltage = (analogValue / 4094.0) * 3.3; // Convertir a rango de pH (0-14)
  phValue = (-0.853*pow(voltage,2)) + (0.047*voltage) + 10.304;
  Serial.print("⚡ Valor de voltaje: ");
  Serial.println(voltage);
  Serial.print("📏 Valor de pH: ");
  Serial.println(phValue);

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
  temp_prom = (validReadings > 0) ? (sumTemp / validReadings) : -1; // Usar -1 como indicador de error
  hum_prom  = (sumHum > 0) ? (sumHum / 3) : -1; // Usar -1 si no hay lecturas válidas

  if (temp_prom == -1 || hum_prom == -1) {
    Serial.println("⚠️ Error: No se pudieron calcular los promedios.");
    return;
  }

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

}

void sendToFirebase() {
  // Verificar conexión WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("🌐 Conectando a WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\n✅ WiFi conectado");
  }

  // Obtener la hora actual
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("❌ Error al obtener la hora");
    return;
  }

  // Formatear el timestamp
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", &timeinfo); // Reemplazar ':' por '-'
  Serial.print("🕒 Timestamp: ");
  Serial.println(timestamp);

  // Crear el objeto JSON
  StaticJsonDocument<512> doc;
  doc["DS18"] = tempDS;
  doc["Temp1"] = temp1;
  doc["Temp2"] = temp2;
  doc["Temp3"] = temp3;
  doc["Hum1"] = hum1;
  doc["Hum2"] = hum2;
  doc["Hum3"] = hum3;
  doc["PromT"] = temp_prom;
  doc["PromH"] = hum_prom;
  doc["pH"] = phValue;
  doc["timestamp"] = timestamp; // Agregar el timestamp al JSON

  // Serializar el objeto JSON a un string
  String payload;
  serializeJson(doc, payload);

  // **Payload 1: Sobrescribir datos en un nodo fijo**
  HTTPClient http;
  http.begin("https://ranitas-test-default-rtdb.firebaseio.com/Incu1.json"); // Nodo fijo
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.PUT(payload);

  if (httpCode == 200) {
    Serial.println("✅ Datos sobrescritos en Firebase");
    String response = http.getString();
    Serial.println("📄 Respuesta del servidor:");
    Serial.println(response);
  } else {
    Serial.println("❌ Error al sobrescribir datos: " + String(httpCode));
    String response = http.getString();
    Serial.println("📄 Respuesta del servidor:");
    Serial.println(response);
  }
  http.end();

  // **Payload 2: Guardar datos con un timestamp único**
  char firebasePath[128];
  snprintf(firebasePath, sizeof(firebasePath), "/Incu1/%s.json", timestamp); // Usar el timestamp corregido
  String firebaseTimestampURL = "https://ranitas-test-default-rtdb.firebaseio.com" + String(firebasePath);

  http.begin(firebaseTimestampURL);
  http.begin(String(firebaseURL) + firebasePath); // Nodo único basado en timestamp
  http.addHeader("Content-Type", "application/json");
  httpCode = http.PUT(payload);

  if (httpCode == 200) {
    Serial.println("✅ Datos guardados con timestamp en Firebase");
    String response = http.getString();
    Serial.println("📄 Respuesta del servidor:");
    Serial.println(response);
  } else {
    Serial.println("❌ Error al guardar datos con timestamp: " + String(httpCode));
    String response = http.getString();
    Serial.println("📄 Respuesta del servidor:");
    Serial.println(response);
  }
  http.end();

  // Desconectar WiFi para ahorrar energía
  WiFi.disconnect(true);
  Serial.println("🌐 WiFi desconectado");
}
