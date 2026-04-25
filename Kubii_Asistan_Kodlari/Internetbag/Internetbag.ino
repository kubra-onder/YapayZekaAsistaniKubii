#include <WiFi.h>

// --- KENDİ BİLGİLERİNİ YAZ ---
const char* ssid = "Harama el uzatma";         
const char* password = "tvare123";   

// Beyaz kabloyu taktığın D2 pini
const int ledPin = 2; 

void setup() {
  // 1. ADIM: Bilgisayarla konuşma hızını 115200 yapıyoruz
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println("\nSistem baslatildi...");
  WiFi.begin(ssid, password);

  // 2. ADIM: Bağlanana kadar LED yanıp sönsün (Avantaj: Durumu gözle takip edersin)
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(ledPin, !digitalRead(ledPin)); 
  }

  // 3. ADIM: Bağlantı başarılı!
  Serial.println("\nBasarili! Internet baglandi.");
  Serial.print("IP Adresiniz: ");
  Serial.println(WiFi.localIP());
  
  // Bağlanınca LED'i sabit yak
  digitalWrite(ledPin, HIGH); 
}

void loop() {
  // 4. ADIM: İnternet koparsa LED'i söndür (Avantaj: Hata takibi)
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);
  }
}
