#include <WiFi.h>
#include <HTTPClient.h>

// Configuración de WiFi
const char* ssid = "Bandle";
const char* password = "YordlepeLudito1120";
const char* firebaseURL = "https://ranitas-test-default-rtdb.firebaseio.com/datos.json";

// Pines y variables
#define PIN_SALIDA 25   // DAC (0-255)
#define PIN_ENTRADA 34  // ADC (0-4095)

int salida = 0;  // Declaración correcta de variables globales
int entrada = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_ENTRADA, INPUT);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.print("Conectado. IP: ");
  Serial.println(WiFi.localIP());

}

void loop() {

  // Generar y leer valores (simulación)
  salida = random(0, 256);          // Valor DAC (0-255)
  dacWrite(PIN_SALIDA, salida);     // Escribir en DAC
  entrada = analogRead(PIN_ENTRADA); // Leer ADC (0-4095)


  if (WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(firebaseURL);
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"salida\":" + String(salida) + ",\"entrada\":" + String(entrada) + "}";
    int httpCode = http.PUT(payload);

    if (httpCode == 200) {
      Serial.println("Datos enviados a Firebase");
    } else {
      Serial.println("Error: " + String(httpCode));
    }
    http.end();
  }
  // Mostrar en Monitor Serie (opcional)
  Serial.print("Salida: ");
  Serial.print(salida);
  Serial.print("\tEntrada: ");
  Serial.println(entrada);

  delay(1000);  // Intervalo de actualización
}