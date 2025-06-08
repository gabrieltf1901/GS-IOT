# NOME: GABRIEL TORRES FERNANDES - RM 553635

# LINK VIDEO APRESENTAÇÃO --> https://youtu.be/ZOOu20TTGpQ
# LINK PROJETO WOKWI --> https://wokwi.com/projects/433203722729355265
# LINK THINGSPEAK --> https://thingspeak.mathworks.com/channels/2983904

## Projeto IoT: Monitoramento de Abrigos em Tempo Real

### Descrição da Solução

Este projeto fornece um protótipo de sistema IoT para monitoramento de abrigos em cenários de desastres naturais. Utilizando um ESP32 simulado no Wokwi com sensores virtuais DHT22 (temperatura e umidade) e um potenciômetro (nível de água), os dados são enviados a cada 20 segundos para um canal no ThingSpeak via HTTP/JSON. O ThingSpeak atua como gateway e dashboard, exibindo gráficos em tempo real e permitindo análise embutida via MATLAB Analysis para detecção de anomalias.

### Requisitos

* Conta gratuita no ThingSpeak
* Internet (para simular no Wokwi)
* Navegador moderno (Chrome, Firefox, Edge)
* IDE Arduino (opcional, apenas se quiser rodar localmente)

### Instruções de Configuração

1. **Clonar repositório**

```bash
git clone <URL-do-repo>
cd <nome-do-repo>
```

2. **Configurar ThingSpeak**

   * Acesse [https://thingspeak.com](https://thingspeak.com) e crie uma conta.
   * Em **Channels → New Channel**, defina:

     * Name: `AbrigosSensor`
     * Fields: `field1=temperatura`, `field2=umidade`, `field3=nivel_agua`
   * Copie a **Write API Key**.

3. **Atualizar credenciais no sketch**

   * No arquivo `sketch/monitor.ino`, substitua:

     ```cpp
     const char* ssid     = "Wokwi-GUEST";  
     const char* password = "";             
     String apiKey = "SUA_WRITE_API_KEY";
     ```
   * Cole sua Write API Key.

4. **Abrir no Wokwi**

   * Acesse [https://wokwi.com](https://wokwi.com) e importe `diagram.json`.
   * Verifique o circuito (ESP32, DHT22, potenciômetro).

5. **Iniciar simulação**

   * Clique em **Start Simulation**.
   * Abra o **Serial Monitor** para ver logs da conexão WiFi e envios HTTP.

### Executando Localmente (Opcional)

1. **Abrir Arduino IDE**

   * Adicione o ESP32 no gerenciador de placas (URLs adicionais).
   * Instale bibliotecas: `WiFi.h`, `HTTPClient.h`, `DHT.h`.
2. **Configurar portas**

   * Conecte seu ESP32 real ao PC.
   * Selecione porta correta em **Ferramentas > Porta**.
3. **Carregar sketch**

   * Carregue `sketch/monitor.ino`.
   * Abra Serial Monitor e acompanhe.

### Testes da Aplicação

1. **Verificar conexão WiFi**: no console deve mostrar “Connected!”.
2. **Checar HTTP Code**: deve retornar `200` a cada envio.
3. **Visualizar Dashboard**: No ThingSpeak, abra seu canal e observe os gráficos de temperatura, umidade e nível de água.
4. **Ajustar valores**: No Wokwi, clique nos sensores e altere valores para ver atualização em tempo real.

### Código-Fonte (monitor.ino)

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define WATERPIN 34

const char* ssid     = "Wokwi-GUEST";  
const char* password = "";            
String apiKey = "SUA_WRITE_API_KEY";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  WiFi.begin(ssid, password, 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temp = dht.readTemperature();
    float hum  = dht.readHumidity();
    int level  = analogRead(WATERPIN);

    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(temp) +
                 "&field2=" + String(hum) +
                 "&field3=" + String(level);

    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    Serial.printf("HTTP Code: %d\n", httpCode);
    http.end();

    delay(20000);
  }
}
```

### Links Úteis

* Wokwi Docs: [https://docs.wokwi.com](https://docs.wokwi.com)
* ThingSpeak API: [https://docs.thingspeak.com/docs/](https://docs.thingspeak.com/docs/)
