#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

const char* ssid = "Harama el uzatma";
const char* password = "tvare123";

AsyncWebServer server(80);
const int ledPin = 2;
bool ledState = false; // LED durumunu takip et

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html lang="tr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
  <title>Asistan</title>
  <style>
    body { background-color: #010409; color: white; font-family: sans-serif; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; overflow: hidden; }
    #assistant-core {
      width: 150px; height: 150px;
      background: radial-gradient(circle, #ff00ff 0%, #a000a0 70%, #600060 100%);
      border-radius: 50%;
      box-shadow: 0 0 50px 20px rgba(255, 0, 255, 0.4);
      cursor: pointer;
      transition: transform 0.1s;
      animation: pulse 2s infinite ease-in-out;
    }
    #assistant-core:active { transform: scale(0.95); }
    @keyframes pulse {
      0% { transform: scale(1); box-shadow: 0 0 50px 20px rgba(255, 0, 255, 0.4); }
      50% { transform: scale(1.05); box-shadow: 0 0 70px 30px rgba(255, 0, 255, 0.6); }
      100% { transform: scale(1); box-shadow: 0 0 50px 20px rgba(255, 0, 255, 0.4); }
    }
    #info { position: fixed; bottom: 20px; font-size: 12px; color: #444; }
  </style>
</head>
<body>
  <div id="assistant-core" onclick="toggleLED()"></div>
  <div id="info">LED: <span id="status">Kapalı</span></div>
  <script>
    function toggleLED() {
      fetch('/toggle');
    }
    // WebSocket ile durum takibi (Gelecek için hazırlık)
    var gateway = `ws://${window.location.hostname}/ws`;
    var websocket;
    function initWebSocket() {
      websocket = new WebSocket(gateway);
      websocket.onmessage = onMessage;
    }
    function onMessage(event) {
      document.getElementById('status').innerText = (event.data == "on" ? "Açık" : "Kapalı");
    }
    window.onload = initWebSocket;
  </script>
</body></html>)rawliteral";

// WebSocket sunucusu
AsyncWebSocket ws("/ws");

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nIP: " + WiFi.localIP().toString());

  ws.onEvent([](AsyncWebSocket *s, AsyncWebSocketClient *c, AwsEventType t, void *a, uint8_t *d, size_t l){
    if (t == WS_EVT_CONNECT) c->text(ledState ? "on" : "off");
  });
  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *r){ r->send_P(200, "text/html", index_html); });
  
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *r){
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    ws.textAll(ledState ? "on" : "off");
    r->send(200, "text/plain", "OK");
  });

  server.begin();
}
void loop() { ws.cleanupClients(); }