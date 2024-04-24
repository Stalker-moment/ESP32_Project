#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT11

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

// Inisialisasi sensor DHT dengan pin yang ditentukan dan tipe sensor DHT yang digunakan
DHT dht(DHTPIN, DHTTYPE);

// Menyertakan informasi proses pembuatan token Firebase
#include "addons/TokenHelper.h"
// Menyertakan informasi pencetakan payload RTDB dan fungsi bantu lainnya
#include "addons/RTDBHelper.h"

// Masukkan SSID dan password Wi-Fi Anda
#define WIFI_SSID "p"
#define WIFI_PASSWORD "malah nanya"

// Masukkan API Key proyek Firebase Anda
#define API_KEY "AIzaSyA-mNxoYku17-J9BeLVpQGmyr1nNlQlBGk"

// Masukkan URL dari Realtime Database (RTDB) Firebase Anda
#define DATABASE_URL "https://sensor-dht-11-86299-default-rtdb.asia-southeast1.firebasedatabase.app" 

// Objek FirebaseData untuk berinteraksi dengan Firebase
FirebaseData fbdo;

// Objek autentikasi Firebase
FirebaseAuth auth;

// Konfigurasi Firebase
FirebaseConfig config;

// Variabel flag untuk menandai apakah proses pendaftaran berhasil
bool signupOK = false;

void setup(){
  // Konfigurasi pin sensor DHT sebagai input
  pinMode(DHTPIN, INPUT);
  
  // Memulai sensor DHT
  dht.begin();
  
  // Memulai komunikasi serial dengan kecepatan 115200 bps
  Serial.begin(115200);
  
  // Menghubungkan ke jaringan Wi-Fi menggunakan kredensial yang telah ditentukan
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan ke Wi-Fi");
  
  // Tunggu sampai terhubung ke jaringan Wi-Fi
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Terhubung dengan IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Mengassign kunci API Firebase (diperlukan) */
  config.api_key = API_KEY;

  /* Mengassign URL RTDB Firebase (diperlukan) */
  config.database_url = DATABASE_URL;

  /* Mendaftar ke layanan Firebase */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Berhasil mendaftar ke Firebase");
    signupOK = true;
  }
  else{
    Serial.printf("Gagal mendaftar ke Firebase: %s\n", config.signer.signupError.message.c_str());
  }

  /* Mengassign fungsi panggilan balik untuk tugas pembuatan token yang berjalan lama */
  config.token_status_callback = tokenStatusCallback; // Lihat addons/TokenHelper.h
  
  // Memulai koneksi ke Firebase dengan konfigurasi yang telah ditentukan
  Firebase.begin(&config, &auth);
  
  // Membuat koneksi ulang ke jaringan Wi-Fi jika terputus
  Firebase.reconnectWiFi(true);
}

void loop(){
  // Delay untuk menghindari pembacaan yang terlalu sering
  delay(1000);
  
  // Membaca kelembapan dari sensor DHT
  float h = dht.readHumidity();

  // Membaca suhu dari sensor DHT
  float t = dht.readTemperature();
  
  // Memeriksa apakah koneksi ke Firebase sudah siap dan proses pendaftaran berhasil
  if (Firebase.ready() && signupOK ) {
    
    // Menyimpan nilai kelembapan ke Realtime Database Firebase
    if (Firebase.RTDB.setString(&fbdo, "DHT_11/kelembapan",h)){
       Serial.print("Kelembapan berhasil disimpan: ");
       Serial.println(h);
      
    }
    else {
      Serial.println("Gagal menyimpan kelembapan");
      Serial.println("Alasan: " + fbdo.errorReason());
    }
    
    // Menyimpan nilai suhu ke Realtime Database Firebase
    if (Firebase.RTDB.setString(&fbdo, "DHT_11/suhu", t)){
       Serial.print("Suhu berhasil disimpan: ");
       Serial.println(t);
    }
    else {
      Serial.println("Gagal menyimpan suhu");
      Serial.println("Alasan: " + fbdo.errorReason());
    }
  }
  
  // Menampilkan separator untuk membedakan setiap iterasi dalam serial monitor
  Serial.println("______________________________");
}
