#include <WiFi.h>
#include <HTTPClient.h>
#include <Fuzzy.h>
#include <time.h>
#include <ArduinoJson.h>

// Credenciales WiFi
const char* ssid = "Bandle";
const char* password = "YordlepeLudito1120";

// Firebase y APIs
const char* firebaseURL = "https://ranitas-test-default-rtdb.firebaseio.com/Incu1.json";
const char* sunriseAPI = "https://api.sunrise-sunset.org/json?lat=19.432608&lng=-99.133209&formatted=0";

// Pines de relés
#define RELAY_CALEFACTOR 15
#define RELAY_ESPERSOR 2
#define RELAY_PH 4
#define RELAY_LUZ 16
#define RELAY_CASCADA 17

// Variables globales
float PromT = 0.0, PromH = 0.0, pH = 0.0;
float lastPromT = -1.0, lastPromH = -1.0, lastPH = -1.0;
time_t sunriseTime = 0;
time_t lightOnTime = 0;
time_t lightOffTime = 0;
bool dataValid = false;
bool rainSimulated = false;
unsigned long lastFirebaseRequest = 0;
const unsigned long firebaseInterval = 300000; // 5 minutos

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

    // Inicializar todos los relés en LOW
    digitalWrite(RELAY_CALEFACTOR, LOW);
    digitalWrite(RELAY_ESPERSOR, LOW);
    digitalWrite(RELAY_PH, LOW);
    digitalWrite(RELAY_LUZ, LOW);
    digitalWrite(RELAY_CASCADA, LOW);

    // Configurar fuzzy
    setupFuzzy();

    // Configurar zona horaria (UTC-6 para CDMX)
    configTime(-6 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    Serial.println("⏰ Configurando hora...");
    while (time(nullptr) < 24 * 3600) {
        delay(500);
    }

    // Obtener hora del amanecer
    getSunriseTime();

    // Obtener datos iniciales de Firebase
    getFirebaseData();
}

void loop() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);

    // Actualizar amanecer a medianoche
    if (timeinfo->tm_hour == 0 && timeinfo->tm_min == 0 && timeinfo->tm_sec < 10) {
        getSunriseTime();
    }

    if (!dataValid) {
        Serial.println("⚠️ Esperando datos válidos de Firebase...");
        getFirebaseData();
        delay(1000);
        return;
    }

    // Actualizar datos de Firebase cada 5 minutos
    if (millis() - lastFirebaseRequest >= firebaseInterval) {
        lastFirebaseRequest = millis();
        getFirebaseData();
    }

    // Ejecutar lógica difusa si hay cambios
    if (dataValid && (PromT != lastPromT || PromH != lastPromH || pH != lastPH)) {
        lastPromT = PromT;
        lastPromH = PromH;
        lastPH = pH;
        Serial.println("🔄 Datos cambiaron, ejecutando fuzzy...");
        applyFuzzyLogic();
    }

    // Controlar subsistemas
    controlLight();
    controlHumedad();
    controlWaterfall(); // Para oxigenación del agua
}

void getFirebaseData() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(firebaseURL);
        int httpCode = http.GET();

        if (httpCode == 200) {
            String payload = http.getString();
            Serial.println("📥 Datos recibidos de Firebase:");
            Serial.println(payload);

            // Parsear JSON con ArduinoJson
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);

            if (doc.containsKey("PromT") && doc.containsKey("PromH") && doc.containsKey("pH")) {
                PromT = doc["PromT"].as<float>();
                PromH = doc["PromH"].as<float>();
                pH = doc["pH"].as<float>();

                Serial.printf("🌡️ PromT: %.1f°C\n", PromT);
                Serial.printf("💧 PromH: %.1f%%\n", PromH);
                Serial.printf("📏 pH: %.1f\n", pH);

                dataValid = true;
            } else {
                Serial.println("❌ Error: Datos incompletos en Firebase");
                dataValid = false;
            }
        } else {
            Serial.printf("❌ Error HTTP: %d\n", httpCode);
            dataValid = false;
        }
        http.end();
    } else {
        Serial.println("⚠️ WiFi desconectado. Reconectando...");
        WiFi.begin(ssid, password);
        delay(5000);
    }
}

void getSunriseTime() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(sunriseAPI);
        int httpCode = http.GET();

        if (httpCode == 200) {
            String payload = http.getString();
            Serial.println("🌅 Datos de amanecer recibidos:");

            DynamicJsonDocument doc(2048);
            deserializeJson(doc, payload);

            if (doc["status"] == "OK") {
                String sunriseStr = doc["results"]["sunrise"].as<String>();
                struct tm tm;
                strptime(sunriseStr.c_str(), "%Y-%m-%dT%H:%M:%S%z", &tm);
                time_t utcSunrise = mktime(&tm);

                // Ajustar a zona horaria (UTC-6)
                sunriseTime = utcSunrise - (6 * 3600);
                
                // Ajustar para horario de verano si es necesario
                struct tm* local = localtime(&sunriseTime);
                if (local->tm_isdst > 0) {
                    sunriseTime += 3600;
                }

                // Calcular horario de luz (7 horas de luz a partir de 1 hora después del amanecer)
                lightOnTime = sunriseTime + 3600;
                lightOffTime = lightOnTime + 25200; // 7 horas = 25200 segundos

                Serial.printf("🌅 Amanecer: %s", ctime(&sunriseTime));
                Serial.printf("💡 Luz ON: %s", ctime(&lightOnTime));
                Serial.printf("💡 Luz OFF: %s", ctime(&lightOffTime));
            }
        } else {
            Serial.printf("❌ Error al obtener amanecer: %d\n", httpCode);
        }
        http.end();
    }
}

void controlLight() {
    time_t now = time(nullptr);
    static bool lightState = false;

    bool shouldBeOn = (now >= lightOnTime && now < lightOffTime);
    
    if (shouldBeOn && !lightState) {
        digitalWrite(RELAY_LUZ, HIGH);
        lightState = true;
        Serial.println("💡 Luz encendida");
    } else if (!shouldBeOn && lightState) {
        digitalWrite(RELAY_LUZ, LOW);
        lightState = false;
        Serial.println("💡 Luz apagada");
    }
}

void controlHumedad() {
    static unsigned long lastRainTime = 0;
    const unsigned long rainInterval = 3600000; // 1 hora

    // Pre-reproducción (<70%) o reproducción (70-90%)
    if (PromH < 70 && !rainSimulated) {
        // Simular lluvia por 30 segundos
        digitalWrite(RELAY_ESPERSOR, HIGH);
        Serial.println("🌧️ Activando aspersores para simular lluvia");
        delay(30000);
        digitalWrite(RELAY_ESPERSOR, LOW);
        rainSimulated = true;
        lastRainTime = millis();
    } 
    // Si la humedad es adecuada, resetear flag de lluvia
    else if (PromH >= 70 && PromH <= 90) {
        rainSimulated = false;
    }
    // Si ha pasado 1 hora y aún necesitamos más humedad
    else if (PromH < 90 && millis() - lastRainTime >= rainInterval) {
        rainSimulated = false;
    }
}

void controlWaterfall() {
    // Activar cascada 10 minutos cada hora para oxigenación
    static unsigned long lastWaterfallOn = 0;
    const unsigned long waterfallInterval = 3600000; // 1 hora
    const unsigned long waterfallDuration = 600000;  // 10 minutos

    if (millis() - lastWaterfallOn >= waterfallInterval) {
        digitalWrite(RELAY_CASCADA, HIGH);
        Serial.println("💦 Activando cascada para oxigenación");
        delay(waterfallDuration);
        digitalWrite(RELAY_CASCADA, LOW);
        lastWaterfallOn = millis();
    }
}

void setupFuzzy() {
    
    // 1. Entrada: Temperatura (ºC)
    FuzzyInput* temperature = new FuzzyInput(1);
    
    FuzzySet* tempCold = new FuzzySet(15, 15, 18, 21);      // Frío
    FuzzySet* tempIdeal = new FuzzySet(20, 22, 24, 26);      // Ideal
    FuzzySet* tempHot = new FuzzySet(25, 28, 30, 30);        // Caliente
    
    temperature->addFuzzySet(tempCold);
    temperature->addFuzzySet(tempIdeal);
    temperature->addFuzzySet(tempHot);
    fuzzy.addFuzzyInput(temperature);

    // 2. Entrada: Humedad (%)
    FuzzyInput* humidity = new FuzzyInput(2);
    
    FuzzySet* humDry = new FuzzySet(50, 50, 60, 70);         // Seco
    FuzzySet* humIdeal = new FuzzySet(65, 75, 75, 85);       // Ideal
    FuzzySet* humWet = new FuzzySet(80, 90, 100, 100);       // Húmedo
    
    humidity->addFuzzySet(humDry);
    humidity->addFuzzySet(humIdeal);
    humidity->addFuzzySet(humWet);
    fuzzy.addFuzzyInput(humidity);

    // 3. Entrada: pH
    FuzzyInput* phInput = new FuzzyInput(3);
    
    FuzzySet* phAcidic = new FuzzySet(5.5, 5.5, 6.0, 6.5);   // Ácido
    FuzzySet* phNeutral = new FuzzySet(6.3, 6.8, 7.2, 7.7);  // Neutral
    FuzzySet* phBasic = new FuzzySet(7.5, 8.0, 8.5, 8.5);    // Básico
    
    phInput->addFuzzySet(phAcidic);
    phInput->addFuzzySet(phNeutral);
    phInput->addFuzzySet(phBasic);
    fuzzy.addFuzzyInput(phInput);

    // 1. Salida: Calefactor (0-100%)
    FuzzyOutput* heater = new FuzzyOutput(1);
    
    FuzzySet* heaterOff = new FuzzySet(0, 0, 20, 40);        // Apagado
    FuzzySet* heaterLow = new FuzzySet(30, 50, 50, 70);      // Baja potencia
    FuzzySet* heaterHigh = new FuzzySet(60, 80, 100, 100);   // Alta potencia
    
    heater->addFuzzySet(heaterOff);
    heater->addFuzzySet(heaterLow);
    heater->addFuzzySet(heaterHigh);
    fuzzy.addFuzzyOutput(heater);

    // 2. Salida: Aspersor (0-100%)
    FuzzyOutput* sprinkler = new FuzzyOutput(2);
    
    FuzzySet* sprinklerOff = new FuzzySet(0, 0, 20, 40);     // Apagado
    FuzzySet* sprinklerLow = new FuzzySet(30, 50, 50, 70);   // Riego bajo
    FuzzySet* sprinklerHigh = new FuzzySet(60, 80, 100, 100);// Riego alto
    
    sprinkler->addFuzzySet(sprinklerOff);
    sprinkler->addFuzzySet(sprinklerLow);
    sprinkler->addFuzzySet(sprinklerHigh);
    fuzzy.addFuzzyOutput(sprinkler);

    // 3. Salida: Control de pH (0-100%)
    FuzzyOutput* phControl = new FuzzyOutput(3);
    
    FuzzySet* phDecrease = new FuzzySet(0, 0, 20, 40);       // Reducir pH (CO2)
    FuzzySet* phMaintain = new FuzzySet(30, 50, 50, 70);     // Mantener
    FuzzySet* phIncrease = new FuzzySet(60, 80, 100, 100);   // Aumentar pH (alcalino)
    
    phControl->addFuzzySet(phDecrease);
    phControl->addFuzzySet(phMaintain);
    phControl->addFuzzySet(phIncrease);
    fuzzy.addFuzzyOutput(phControl);

    // Reglas difusas

    // Reglas para temperatura
    // Si temperatura es fría ENTONCES calefactor alta
    FuzzyRuleAntecedent* ifTempCold = new FuzzyRuleAntecedent();
    ifTempCold->joinSingle(tempCold);
    FuzzyRuleConsequent* thenHeaterHigh = new FuzzyRuleConsequent();
    thenHeaterHigh->addOutput(heaterHigh);
    fuzzy.addFuzzyRule(new FuzzyRule(1, ifTempCold, thenHeaterHigh));

    // Si temperatura es ideal ENTONCES calefactor baja
    FuzzyRuleAntecedent* ifTempIdeal = new FuzzyRuleAntecedent();
    ifTempIdeal->joinSingle(tempIdeal);
    FuzzyRuleConsequent* thenHeaterLow = new FuzzyRuleConsequent();
    thenHeaterLow->addOutput(heaterLow);
    fuzzy.addFuzzyRule(new FuzzyRule(2, ifTempIdeal, thenHeaterLow));

    // Si temperatura es caliente ENTONCES calefactor apagado
    FuzzyRuleAntecedent* ifTempHot = new FuzzyRuleAntecedent();
    ifTempHot->joinSingle(tempHot);
    FuzzyRuleConsequent* thenHeaterOff = new FuzzyRuleConsequent();
    thenHeaterOff->addOutput(heaterOff);
    fuzzy.addFuzzyRule(new FuzzyRule(3, ifTempHot, thenHeaterOff));

    // Reglas para humedad
    // Si humedad es baja ENTONCES aspersor alto
    FuzzyRuleAntecedent* ifHumDry = new FuzzyRuleAntecedent();
    ifHumDry->joinSingle(humDry);
    FuzzyRuleConsequent* thenSprinklerHigh = new FuzzyRuleConsequent();
    thenSprinklerHigh->addOutput(sprinklerHigh);
    fuzzy.addFuzzyRule(new FuzzyRule(4, ifHumDry, thenSprinklerHigh));

    // Si humedad es ideal ENTONCES aspersor bajo
    FuzzyRuleAntecedent* ifHumIdeal = new FuzzyRuleAntecedent();
    ifHumIdeal->joinSingle(humIdeal);
    FuzzyRuleConsequent* thenSprinklerLow = new FuzzyRuleConsequent();
    thenSprinklerLow->addOutput(sprinklerLow);
    fuzzy.addFuzzyRule(new FuzzyRule(5, ifHumIdeal, thenSprinklerLow));

    // Si humedad es alta ENTONCES aspersor apagado
    FuzzyRuleAntecedent* ifHumWet = new FuzzyRuleAntecedent();
    ifHumWet->joinSingle(humWet);
    FuzzyRuleConsequent* thenSprinklerOff = new FuzzyRuleConsequent();
    thenSprinklerOff->addOutput(sprinklerOff);
    fuzzy.addFuzzyRule(new FuzzyRule(6, ifHumWet, thenSprinklerOff));

    // Reglas para pH
    // Si pH es ácido ENTONCES aumentar pH
    FuzzyRuleAntecedent* ifPhAcidic = new FuzzyRuleAntecedent();
    ifPhAcidic->joinSingle(phAcidic);
    FuzzyRuleConsequent* thenPhIncrease = new FuzzyRuleConsequent();
    thenPhIncrease->addOutput(phIncrease);
    fuzzy.addFuzzyRule(new FuzzyRule(7, ifPhAcidic, thenPhIncrease));

    // Si pH es neutral ENTONCES mantener
    FuzzyRuleAntecedent* ifPhNeutral = new FuzzyRuleAntecedent();
    ifPhNeutral->joinSingle(phNeutral);
    FuzzyRuleConsequent* thenPhMaintain = new FuzzyRuleConsequent();
    thenPhMaintain->addOutput(phMaintain);
    fuzzy.addFuzzyRule(new FuzzyRule(8, ifPhNeutral, thenPhMaintain));

    // Si pH es básico ENTONCES reducir pH
    FuzzyRuleAntecedent* ifPhBasic = new FuzzyRuleAntecedent();
    ifPhBasic->joinSingle(phBasic);
    FuzzyRuleConsequent* thenPhDecrease = new FuzzyRuleConsequent();
    thenPhDecrease->addOutput(phDecrease);
    fuzzy.addFuzzyRule(new FuzzyRule(9, ifPhBasic, thenPhDecrease));
}

void applyFuzzyLogic() {
    // Establecer entradas
    fuzzy.setInput(1, PromT);
    fuzzy.setInput(2, PromH);
    fuzzy.setInput(3, pH);
    
    // Ejecutar lógica difusa
    fuzzy.fuzzify();
    
    // Obtener salidas
    float heaterOutput = fuzzy.defuzzify(1);
    float sprinklerOutput = fuzzy.defuzzify(2);
    float phControlOutput = fuzzy.defuzzify(3);
    
    // Controlar relés con las salidas difusas
    digitalWrite(RELAY_CALEFACTOR, heaterOutput > 70 ? HIGH : (heaterOutput < 30 ? LOW : LOW));
    digitalWrite(RELAY_ESPERSOR, sprinklerOutput > 70 ? HIGH : (sprinklerOutput < 30 ? LOW : LOW));
    digitalWrite(RELAY_PH, phControlOutput > 70 ? HIGH : (phControlOutput < 30 ? LOW : digitalRead(RELAY_PH)));
    
    Serial.printf("🔥 Calefacción: %.1f%%\n", heaterOutput);
    Serial.printf("💦 Aspersor: %.1f%%\n", sprinklerOutput);
    Serial.printf("⚗️ Control pH: %.1f%%\n", phControlOutput);
}