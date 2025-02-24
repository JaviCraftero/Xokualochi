#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "base64.h"
#include "esp_camera.h"

// Configuración WiFi
const char* ssid = "INFINITUM0ADD";
const char* password = "dYsNSA53cF";

// Configuración GitHub 
const char* githubToken = "TOKEN";
const char* githubRepo = "JaviCraftero/Xokualochi";
const char* filename = "foto.jpg"; 

// Configuración de la cámara (ESP32-CAM AI-Thinker)
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

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Desactiva detector de bajo voltaje
  Serial.begin(115200);

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
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Inicializar cámara
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Error inicializando cámara: 0x%x", err);
    return;
  }

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    captureAndUploadToGitHub();
  }
  delay(5000);  // Espera 5 segundos entre capturas
}

void captureAndUploadToGitHub() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Error en captura");
    return;
  }

  // Generar nombre único para evitar conflictos
  String timestamp = String(millis());
  String dynamicFilename = "fotos/foto_" + timestamp + ".jpg";  // Cambiar path
  
  // Codificar en Base64
  String imageBase64 = base64::encode(fb->buf, fb->len);
  imageBase64.replace("\n", "");
  
  // Crear payload
  String payload = 
    "{\"message\":\"Subida automática\","
    "\"content\":\"" + imageBase64 + "\"}";

  esp_camera_fb_return(fb);

  WiFiClientSecure client;
  client.setInsecure();

  if (client.connect("api.github.com", 443)) {
    String request = 
      "PUT /repos/" + String(githubRepo) + "/contents/" + dynamicFilename + " HTTP/1.1\r\n" +
      "Host: api.github.com\r\n" +
      "Authorization: Bearer " + String(githubToken) + "\r\n" +
      "User-Agent: ESP32-RanaMonitor (https://github.com/JaviCraftero)\r\n" +  // User-Agent mejorado
      "Content-Type: application/json\r\n" +
      "Content-Length: " + payload.length() + "\r\n\r\n" +
      payload;

    client.print(request);

    // Leer respuesta completa
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }

    String response = client.readString();
    Serial.println(response);
    
  } else {
    Serial.println("Error de conexión");
  }
  client.stop();
}