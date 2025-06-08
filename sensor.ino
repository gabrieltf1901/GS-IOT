#include <WiFi.h>            // Biblioteca Wi-Fi para ESP32
#include <HTTPClient.h>      // Para requisições HTTP
#include <DHT.h>             // Biblioteca para DHT22

#define DHTPIN 4             // Pino digital do DHT22
#define DHTTYPE DHT22        // Modelo do DHT
#define WATERPIN 34          // Pino analógico do sensor de nível

const char* ssid     = "Wokwi-GUEST";  // SSID do AP simulado
const char* password = "";             // Sem senha no Wokwi
const int   channel  = 6;              // Canal fixo para pular escaneamento
// Copie a Write API Key do canal ThingSpeak
String apiKey = "75OIQJF5M6Z0AMIW";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);         // Modo estação
  WiFi.persistent(false);      // Não gravar credenciais em flash
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password, channel);  // Conecta direto ao canal 6
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println(" Connected!");  // Confirmar conexão
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temp = dht.readTemperature();        // Leitura de temperatura
    float hum  = dht.readHumidity();           // Leitura de umidade
    int level  = analogRead(WATERPIN);         // Leitura de nível de água

    // Monta URL de atualização (HTTP GET)
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(temp) +
                 "&field2=" + String(hum) +
                 "&field3=" + String(level);

    HTTPClient http;
    http.begin(url);             // Inicializa requisição
    int httpCode = http.GET();   // Envia GET
    Serial.printf("Código HTTP: %d\n", httpCode);
    http.end();                  // Fecha conexão

    delay(20000);  // Intervalo mínimo de 15 s no plano gratuito :contentReference[oaicite:12]{index=12}
  }
}
