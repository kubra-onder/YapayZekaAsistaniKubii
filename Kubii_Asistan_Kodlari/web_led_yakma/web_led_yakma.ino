#include <WiFi.h>
#include <WebServer.h>

// --- AYARLAR ---
const char* ssid = "Harama el uzatma";         // Burayı değiştir!
const char* password = "tvare123";   // Burayı değiştir!

WebServer server(80); // Sunucuyu 80 portunda başlat
const int ledPin = 2; // ESP32 üzerindeki mavi LED (GPIO 2)

// --- WEB SAYFASI TASARIMI ---
void handleRoot() {
  String html = "<html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{text-align:center; font-family:sans-serif; background:#121212; color:white; padding-top:50px;}";
  html += ".btn{display:inline-block; padding:15px 30px; margin:10px; font-size:20px; color:white; border:none; border-radius:10px; cursor:pointer; text-decoration:none;}";
  html += ".on{background: #27ae60;} .off{background: #c0392b;}</style></head><body>";
  html += "<h1>KANKI'NIN KONTROL PANELI</h1>";
  html += "<a href='/ledOn' class='btn on'>LED'İ YAK</a><br>";
  html += "<a href='/ledOff' class='btn off'>LED'İ SÖNDÜR</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// --- KOMUTLAR ---
void handleLedOn() {
  digitalWrite(ledPin, HIGH);
  server.sendHeader("Location", "/"); // Butona basınca sayfada kal
  server.send(303);
}

void handleLedOff() {
  digitalWrite(ledPin, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  // Wi-Fi Bağlantısı
  WiFi.begin(ssid, password);
  Serial.print("Baglaniyor...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWi-Fi Baglandi!");
  Serial.print("IP Adresi: ");
  Serial.println(WiFi.localIP()); // Bu adresi tarayıcıya yazacağız

  // Sayfa Yönlendirmeleri
  server.on("/", handleRoot);
  server.on("/ledOn", handleLedOn);
  server.on("/ledOff", handleLedOff);
  server.begin();
}

void loop() {
  server.handleClient(); // Gelen istekleri dinle
}