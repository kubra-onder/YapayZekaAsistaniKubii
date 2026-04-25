#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> // Paketleri açmak için gerekli kütüphane

// --- AYARLAR ---
const char* ssid = "Haramaeluzatma";      // Kendi Wi-Fi adını yaz
const char* password = "tvare123"; // Kendi Wi-Fi şifreni yaz
const int ledPin = 2;                // ESP32 üzerindeki mavi/yeşil LED pini

// Amasya koordinatlarına ayarlı Hava Durumu API'si
const String url = "http://api.open-meteo.com/v1/forecast?latitude=40.65&longitude=35.83&current_weather=true";

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); 

  // Wi-Fi Başlatma
  WiFi.begin(ssid, password);
  Serial.print("KUBII internete baglaniyor");

  // Bağlanana kadar LED hızlı yanıp söner
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
    Serial.print(".");
  }

  Serial.println("\nBaglanti Basarili! KUBII artik cevrimici.");
  digitalWrite(ledPin, HIGH); // Bağlanınca sabit yansın
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url); 
    
    int httpCode = http.GET(); 
    
    if (httpCode > 0) {
      String payload = http.getString();
      
      // JSON formatındaki veriyi parçalıyoruz
      StaticJsonDocument<512> doc;
      deserializeJson(doc, payload);
      
      float derece = doc["current_weather"]["temperature"];
      
      Serial.println("\n--- KUBII BILGI EKRANI ---");
      Serial.print("Amasya Hava Durumu: ");
      Serial.print(derece);
      Serial.println(" Derece");

      // --- AKILLI UYARI SİSTEMİ ---
      if (derece < 5.0) {
        Serial.println("UYARI: Hava cok soguk! Don tehlikesi var.");
        // LED'i 5 kez flaşör gibi yak söndür (Görsel uyarı)
        for(int i = 0; i < 5; i++) {
          digitalWrite(ledPin, LOW);  delay(150);
          digitalWrite(ledPin, HIGH); delay(150);
        }
      } else {
        digitalWrite(ledPin, HIGH); // Hava iyiyse sabit yansın
      }
      Serial.println("--------------------------");
      
    } else {
      Serial.println("Veri cekme hatasi!");
    }
    http.end(); 
  }
  
  // 30 saniyede bir veriyi tazele
  delay(30000); 
}