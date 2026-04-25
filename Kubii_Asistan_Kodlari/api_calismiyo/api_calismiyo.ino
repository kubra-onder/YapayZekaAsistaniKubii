#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "index.h" // Arayüz dosyanın adı aynı olmalı

// --- AYARLARIN (Buraları doldur kanki) ---
const char* ssid = "Harama el uzatma"; 
const char* password = "tvare123";
String apiKey = "AIzaSyAgjbEKVsDvv1ggLhByKN5Mg1FVnJJzvzo"; // Boşluk bırakma!

// --- EKRAN AYARLARI ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
// Ekran adresi genelde 0x3C'dir, çalışmazsa 0x3D dene.
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WebServer server(80);

// Ekrana Yazdırma Fonksiyonu
void ekranaYaz(String baslik, String mesaj) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(baslik);
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  display.setCursor(0, 18);
  display.println(mesaj.substring(0, 70)); 
  display.display();
}

// Gemini API Fonksiyonu (Düzeltilmiş URL ve Güvenlik)
String GeminiSor(String soru) {
  if (WiFi.status() != WL_CONNECTED) return "Wi-Fi Yok!";

  HTTPClient http;
  // GÜNCEL URL: 404 hatasını önlemek için v1beta ve model ismi kontrol edildi.
  String url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + apiKey;

  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  // JSON Gövdesi76
  String jsonInput = "{\"contents\":[{\"parts\":[{\"text\":\"Senin adın KUBİİ. Kanki diye hitap et. Cevabın 2 cümleyi geçmesin. Soru: " + soru + "\"}]}]}";

  int httpCode = http.POST(jsonInput);
  String payload = http.getString();
  
  if (httpCode == 200) {
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, payload);
    String cevap = doc["candidates"][0]["content"]["parts"][0]["text"];
    http.end();
    return cevap;
  } else {
    String hataMesaji = "Hata: " + String(httpCode);
    http.end();
    return hataMesaji;
  }
}

void handleChat() {
  if (server.hasArg("msg")) {
    String userMsg = server.arg("msg");
    ekranaYaz("KUBII", "Dusunuyor...");
    
    String botReply = GeminiSor(userMsg);
    
    ekranaYaz("KUBII", botReply);
    server.send(200, "text/plain", botReply);
  }
}

void setup() {
  Serial.begin(115200);
  
  // EKRAN BAŞLATMA: Pinleri (21, 22) burada zorla tanımlıyoruz
  Wire.begin(21, 22); 
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Ekran Hatasi!"));
  }
  
  display.clearDisplay();
  ekranaYaz("KUBII", "Sistem Aciliyor...");

  // WIFI BAĞLANTISI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nIP: " + WiFi.localIP().toString());
  ekranaYaz("BAGLANDI", WiFi.localIP().toString());

  // WEB SERVER
  server.on("/", []() { server.send(200, "text/html", index_html); });
  server.on("/chat", handleChat);
  server.begin();
}

void loop() {
  server.handleClient();
}