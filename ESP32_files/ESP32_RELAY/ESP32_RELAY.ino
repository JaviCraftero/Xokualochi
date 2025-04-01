#include <WiFi.h>
#include <HTTPClient.h>
#include <Fuzzy.h>
#include <time.h>

// Credenciales WiFi
const char* ssid = "Bandle";
const char* password = "YordlepeLudito1120";

// Firebase y Sunrise-Sunset API
const char* firebaseURL = "https://ranitas-test-default-rtdb.firebaseio.com/Incu1.json";
const char* sunriseAPI = "https://api.sunrise-sunset.org/json?lat=19.432608&lng=-99.133209&formatted=0"; // Coordenadas de CDMX

// Relés
#define RELAY_CALEFACTOR 15
#define RELAY_ESPERSOR 2
#define RELAY_PH 4
#define RELAY_LUZ 16
#define RELAY_CASCADA 17

// Variables globales
float PromT = 0.0, PromH = 0.0, pH = 0.0; // Últimos valores obtenidos
float lastPromT = -1.0, lastPromH = -1.0, lastPH = -1.0; // Valores anteriores para comparación
time_t sunriseTime = 0;                  // Hora del amanecer en segundos desde epoch
time_t lightOffTime = 0;                 // Hora para apagar la luz
bool dataValid = false;                  // Indica si los datos de Firebase son válidos
unsigned long lastFirebaseRequest = 0;   // Tiempo de la última solicitud a Firebase
const unsigned long firebaseInterval = 300000; // 5 minutos en milisegundos

Fuzzy fuzzy;

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

    // Configurar pines de relés
    pinMode(RELAY_CALEFACTOR, OUTPUT);
    pinMode(RELAY_ESPERSOR, OUTPUT);
    pinMode(RELAY_PH, OUTPUT);
    pinMode(RELAY_LUZ, OUTPUT);
    pinMode(RELAY_CASCADA, OUTPUT);

    // Configurar fuzzy
    setupFuzzy();

    // Configurar zona horaria para México (CST)
    configTime(-6 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    // Obtener la hora del amanecer
    getSunriseTime();

    // Obtener datos iniciales de Firebase
    getFirebaseData();
}

void loop() {
    time_t now = time(nullptr);
    struct tm* localTime = localtime(&now);

    // Ejecutar consulta de la hora del amanecer a la medianoche (UTC-6)
    if (localTime->tm_hour == 0 && localTime->tm_min == 0 && localTime->tm_sec == 0) {
        getSunriseTime();
    }

    if(!dataValid) {
        Serial.println("⚠️ Esperando datos válidos de Firebase...");
        getFirebaseData(); // Intentar obtener datos de Firebase
        delay(1000); // Esperar un segundo antes de volver a intentar
        return;
    }

    // Obtener datos de Firebase cada 5 minutos
    if (millis() - lastFirebaseRequest >= firebaseInterval) {
        lastFirebaseRequest = millis();
        getFirebaseData();
    }

    // Evaluar fuzzy solo si los datos han cambiado
    if (dataValid && (PromT != lastPromT || PromH != lastPromH || pH != lastPH)) {
        lastPromT = PromT;
        lastPromH = PromH;
        lastPH = pH;

        Serial.println("🔄 Datos cambiaron, ejecutando fuzzy...");
        applyFuzzyLogic();
    }

    // Controlar la luz según la hora
    controlLight();
}

void getFirebaseData() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(firebaseURL);
        int httpCode = http.GET();

        if (httpCode == 200) { // Solicitud exitosa
            String payload = http.getString();
            Serial.println("📥 Datos recibidos de Firebase:");
            Serial.println(payload);

            // Parsear JSON manualmente
            int promTIndex = payload.indexOf("\"PromT\":");
            int promHIndex = payload.indexOf("\"PromH\":");
            int pHIndex = payload.indexOf("\"pH\":");

            if (promTIndex != -1 && promHIndex != -1 && pHIndex != -1) {
                PromT = payload.substring(promTIndex + 8, payload.indexOf(",", promTIndex)).toFloat();
                PromH = payload.substring(promHIndex + 8, payload.indexOf(",", promHIndex)).toFloat();
                pH = payload.substring(pHIndex + 5, payload.indexOf("}", pHIndex)).toFloat();

                Serial.print("🌡️ PromT: "); Serial.println(PromT);
                Serial.print("💧 PromH: "); Serial.println(PromH);
                Serial.print("📏 pH: "); Serial.println(pH);

                dataValid = true; // Datos válidos
            } else {
                Serial.println("❌ Error: Datos incompletos en Firebase.");
                dataValid = false;
            }
        } else {
            Serial.print("❌ Error al obtener datos de Firebase. Código HTTP: ");
            Serial.println(httpCode);
            dataValid = false;
        }
        http.end();
    } else {
        Serial.println("⚠️ WiFi desconectado. Intentando reconectar...");
        WiFi.begin(ssid, password);
    }
}

void getSunriseTime() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(sunriseAPI);
        int httpCode = http.GET();

        if (httpCode == 200) { // Solicitud exitosa
            String payload = http.getString();
            Serial.println("📥 Datos recibidos de Sunrise API:");
            Serial.println(payload);

            // Parsear JSON manualmente para obtener la hora del amanecer
            int sunriseIndex = payload.indexOf("\"sunrise\":\"");
            if (sunriseIndex != -1) {
                String sunriseStr = payload.substring(sunriseIndex + 11, payload.indexOf("\"", sunriseIndex + 11));
                struct tm tm;
                strptime(sunriseStr.c_str(), "%Y-%m-%dT%H:%M:%S", &tm);
                time_t utcSunriseTime = mktime(&tm);

                // Convertir UTC a hora central de México (CST)
                time_t localSunriseTime = utcSunriseTime - (6 * 3600); // UTC-6
                struct tm* localTime = localtime(&localSunriseTime);

                // Ajustar si es horario de verano (DST)
                if (localTime->tm_isdst > 0) {
                    localSunriseTime += 3600; // UTC-5 durante horario de verano
                }

                sunriseTime = localSunriseTime;

                Serial.print("🌅 Hora del amanecer (CST): ");
                Serial.println(ctime(&sunriseTime));

                // Calcular la hora para apagar la luz (8 horas después del amanecer)
                lightOffTime = sunriseTime + (8 * 3600);
                Serial.print("💡 Hora para apagar la luz (CST): ");
                Serial.println(ctime(&lightOffTime));
            }
        } else {
            Serial.print("❌ Error al obtener datos de Sunrise API. Código HTTP: ");
            Serial.println(httpCode);
        }
        http.end();
    }
}

void controlLight() {
    static bool lightState = LOW; // Estado actual de la luz (inicialmente apagada)
    time_t now = time(nullptr);

    if (now >= sunriseTime && now < lightOffTime) {
        if (lightState == LOW) { // Solo imprimir si la luz estaba apagada
            digitalWrite(RELAY_LUZ, HIGH); // Encender la luz
            lightState = HIGH;
            Serial.println("💡 Luz encendida");
        }
    } else {
        if (lightState == HIGH) { // Solo imprimir si la luz estaba encendida
            digitalWrite(RELAY_LUZ, LOW); // Apagar la luz
            lightState = LOW;
            Serial.println("💡 Luz apagada");
        }
    }
}

void applyFuzzyLogic() {
    // Configurar entradas difusas
    fuzzy.setInput(1, PromT);
    fuzzy.setInput(2, PromH);
    fuzzy.setInput(3, pH);

    // Ejecutar lógica difusa
    fuzzy.fuzzify();

    // Obtener salidas difusas
    float calefaccion = fuzzy.defuzzify(1);
    float aspersor = fuzzy.defuzzify(2);
    float phControl = fuzzy.defuzzify(3);

    // Imprimir resultados
    Serial.print("🔥 Calefacción: "); Serial.println(calefaccion);
    Serial.print("💦 Aspersor: "); Serial.println(aspersor);
    Serial.print("⚗️ Control de pH: "); Serial.println(phControl);

    // Controlar relés según las salidas
    digitalWrite(RELAY_CALEFACTOR, calefaccion > 50 ? HIGH : LOW);
    digitalWrite(RELAY_ESPERSOR, aspersor > 50 ? HIGH : LOW);
    digitalWrite(RELAY_PH, phControl > 50 ? HIGH : LOW);
}

void setupFuzzy() {
    // Configurar entradas difusas
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

    FuzzyInput* ph = new FuzzyInput(3);
    FuzzySet* phAcido = new FuzzySet(5.5, 6.0, 6.0, 6.5);
    FuzzySet* phNeutral = new FuzzySet(6.4, 7.0, 7.0, 7.5);
    FuzzySet* phBasico = new FuzzySet(7.4, 8.0, 8.0, 8.5);
    ph->addFuzzySet(phAcido);
    ph->addFuzzySet(phNeutral);
    ph->addFuzzySet(phBasico);
    fuzzy.addFuzzyInput(ph);

    // Configurar salidas difusas
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

    FuzzyOutput* phControl = new FuzzyOutput(3);
    FuzzySet* phReducir = new FuzzySet(0, 50, 50, 100);
    FuzzySet* phMantener = new FuzzySet(100, 150, 150, 200);
    phControl->addFuzzySet(phReducir);
    phControl->addFuzzySet(phMantener);
    fuzzy.addFuzzyOutput(phControl);

    // Configurar reglas difusas
    FuzzyRuleAntecedent* reglaTempBaja = new FuzzyRuleAntecedent();
    reglaTempBaja->joinSingle(tempBaja);
    FuzzyRuleConsequent* actCalefactor = new FuzzyRuleConsequent();
    actCalefactor->addOutput(calAlta);
    fuzzy.addFuzzyRule(new FuzzyRule(1, reglaTempBaja, actCalefactor));

    FuzzyRuleAntecedent* reglaHumBaja = new FuzzyRuleAntecedent();
    reglaHumBaja->joinSingle(humBaja);
    FuzzyRuleConsequent* actAspersor = new FuzzyRuleConsequent();
    actAspersor->addOutput(aspEncendido);
    fuzzy.addFuzzyRule(new FuzzyRule(2, reglaHumBaja, actAspersor));

    FuzzyRuleAntecedent* reglaPhBasico = new FuzzyRuleAntecedent();
    reglaPhBasico->joinSingle(phBasico);
    FuzzyRuleConsequent* actPhControl = new FuzzyRuleConsequent();
    actPhControl->addOutput(phReducir);
    fuzzy.addFuzzyRule(new FuzzyRule(3, reglaPhBasico, actPhControl));
}