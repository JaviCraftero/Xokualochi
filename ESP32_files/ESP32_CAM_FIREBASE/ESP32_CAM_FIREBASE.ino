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
#include <Fuzzy.h>

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

//Fuzzy
Fuzzy fuzzy;

void setup() {
  Serial.begin(115200);
  delay(1000);

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

  // Inicialización de los pines de salida
  pinMode(CALEFACTOR, OUTPUT);
  pinMode(ASPERSOR, OUTPUT);
  // pinMode(phRegulador, OUTPUT);
  setupFuzzy();
}

void loop() {
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

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("🌐 Conectando...");
  }
  Serial.println("✅ WiFi conectado");

  if (WiFi.status() == WL_CONNECTED) {
    readSensors();
    captureAndUploadToFirebase();
    WiFi.disconnect(true);
  }
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
  temp_prom = (validReadings > 0) ? (sumTemp / validReadings) : 0;
  hum_prom  = (sumHum / 3); // 3 sensores de humedad, siempre sumamos los tres

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
  String payload = "{\"DS18\":" + String(tempDS) + ",\"Temp1\":" + String(temp1) + ",\"Temp2\":" + String(temp2) + ",\"Temp3\":" + String(temp3) + ",\"Hum1\":" + String(hum1) + ",\"Hum2\":" + String(hum2) + ",\"Hum3\":" + String(hum3) + ",\"PromT\":" + String(temp_prom) + ",\"PromH\":" + String(hum_prom) + ",\"pH\":" + String(phValue) + "}";
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

void setupFuzzy() {
    // Definir variables de entrada
    FuzzyInput* temp = new FuzzyInput(1);
    FuzzySet* tempBaja = new FuzzySet(15, 20, 20, 22);
    FuzzySet* tempMedia = new FuzzySet(21, 23, 23, 25);
    FuzzySet* tempAlta = new FuzzySet(24, 26, 27, 30);
    temp->addFuzzySet(tempBaja);
    temp->addFuzzySet(tempMedia);
    temp->addFuzzySet(tempAlta);
    fuzzy.addFuzzyInput(temp);

    FuzzyInput* hum = new FuzzyInput(2);
    FuzzySet* humBaja = new FuzzySet(50, 60, 60, 70);
    FuzzySet* humMedia = new FuzzySet(69, 75, 75, 85);
    FuzzySet* humAlta = new FuzzySet(84, 90, 90, 100);
    hum->addFuzzySet(humBaja);
    hum->addFuzzySet(humMedia);
    hum->addFuzzySet(humAlta);
    fuzzy.addFuzzyInput(hum);

    // 📌 FUTURA IMPLEMENTACIÓN: Agregar entrada de pH
    // FuzzyInput* ph = new FuzzyInput(3);
    // FuzzySet* phAcido = new FuzzySet(5.5, 6.0, 6.0, 6.5);
    // FuzzySet* phNeutral = new FuzzySet(6.4, 7.0, 7.0, 7.5);
    // FuzzySet* phBasico = new FuzzySet(7.4, 8.0, 8.0, 8.5);
    // ph->addFuzzySet(phAcido);
    // ph->addFuzzySet(phNeutral);
    // ph->addFuzzySet(phBasico);
    // fuzzy.addFuzzyInput(ph);

    // Definir variables de salida
    FuzzyOutput* calefaccion = new FuzzyOutput(1);
    FuzzySet* calBaja = new FuzzySet(0, 50, 50, 100);
    FuzzySet* calAlta = new FuzzySet(100, 150, 150, 200);
    calefaccion->addFuzzySet(calBaja);
    calefaccion->addFuzzySet(calAlta);
    fuzzy.addFuzzyOutput(calefaccion);

    FuzzyOutput* aspersor = new FuzzyOutput(2);
    FuzzySet* aspApagado = new FuzzySet(0, 0, 0, 50);
    FuzzySet* aspEncendido = new FuzzySet(50, 100, 100, 150);
    aspersor->addFuzzySet(aspApagado);
    aspersor->addFuzzySet(aspEncendido);
    fuzzy.addFuzzyOutput(aspersor);

    // 📌 FUTURA IMPLEMENTACIÓN: Agregar salida de control de pH
    // FuzzyOutput* phControl = new FuzzyOutput(3);
    // FuzzySet* phReducir = new FuzzySet(0, 50, 50, 100);
    // FuzzySet* phMantener = new FuzzySet(100, 150, 150, 200);
    // phControl->addFuzzySet(phReducir);
    // phControl->addFuzzySet(phMantener);
    // fuzzy.addFuzzyOutput(phControl);

    // Definir reglas difusas
    FuzzyRuleAntecedent* reglaTempBaja = new FuzzyRuleAntecedent();
    reglaTempBaja->joinSingle(tempBaja);
    FuzzyRuleConsequent* consCalefaccion = new FuzzyRuleConsequent();
    consCalefaccion->addOutput(calAlta);
    fuzzy.addFuzzyRule(new FuzzyRule(1, reglaTempBaja, consCalefaccion));

    FuzzyRuleAntecedent* reglaHumBaja = new FuzzyRuleAntecedent();
    reglaHumBaja->joinSingle(humBaja);
    FuzzyRuleConsequent* consAspersor = new FuzzyRuleConsequent();
    consAspersor->addOutput(aspEncendido);
    fuzzy.addFuzzyRule(new FuzzyRule(2, reglaHumBaja, consAspersor));

    // 📌 FUTURA IMPLEMENTACIÓN: Reglas para el control de pH
    // FuzzyRuleAntecedent* reglaPhAlto = new FuzzyRuleAntecedent();
    // reglaPhAlto->joinSingle(phBasico);
    // FuzzyRuleConsequent* consPhControl = new FuzzyRuleConsequent();
    // consPhControl->addOutput(phReducir);
    // fuzzy.addFuzzyRule(new FuzzyRule(3, reglaPhAlto, consPhControl));
}
void evaluateFuzzy(){
  Serial.println("Evaluando fuzzy...");
  Serial.print("Temperatura: "); Serial.println(temp_prom);
  Serial.print("Humedad: "); Serial.println(hum_prom);
  // Serial.print("PH: "); Serial.println(ph);

  fuzzy.setInput(1, temp_prom);
  fuzzy.setInput(2, hum_prom);
  // fuzzy.setInput(3, ph);

  int calefaccion = fuzzy.defuzzify(1);
  int aspersor = fuzzy.defuzzify(2);
  // int phControl = fuzzy.defuzzify(3);

  digitalWrite(CALEFACTOR, calefaccion > CALEFACTOR_THRESHOLD ? HIGH : LOW);
  digitalWrite(ASPERSOR, aspersor > ASPERSOR_THRESHOLD ? HIGH : LOW);
  // digitalWrite(phRegulador, phControl > PH_THRESHOLD ? HIGH : LOW);
}