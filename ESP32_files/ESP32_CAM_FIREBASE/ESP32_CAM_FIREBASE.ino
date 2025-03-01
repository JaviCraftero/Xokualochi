#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <base64.h> // by Densaugeo


// Credenciales WiFi
const char* ssid = "Bandle";
const char* password = "YordlepeLudito1120";

// Firebase config
const char* function_url = "https://us-central1-ranitas-test.cloudfunctions.net/uploadImage"; // URL de Cloud Function



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

#define LED_GPIO_NUM 4 //Flash

void setup() {
  Serial.begin(115200);

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
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
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
  http.addHeader("Content-Type", "application/json"); //Se envia como JSON
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