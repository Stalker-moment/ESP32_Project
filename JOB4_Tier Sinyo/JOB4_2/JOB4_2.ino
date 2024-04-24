#include <Arduino.h>

// Memasukkan header file tergantung pada platform yang digunakan (ESP32 atau ESP8266)
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

// Memasukkan library Firebase ESP Client
#include <Firebase_ESP_Client.h>

// Menyertakan informasi proses pembuatan token
#include "addons/TokenHelper.h"
// Menyertakan informasi pencetakan payload RTDB dan fungsi bantu lainnya
#include "addons/RTDBHelper.h"

// Masukkan SSID dan password Wi-Fi Anda
#define WIFI_SSID "p"
#define WIFI_PASSWORD "malah nanya"

// Masukkan API Key proyek Firebase Anda
#define API_KEY "AIzaSyB_uhQudEEnifCy8XUORCGqzGIbiYweeWI"

// Masukkan URL Realtime Database Firebase Anda
#define DATABASE_URL "https://esp32-led-6af07-default-rtdb.asia-southeast1.firebasedatabase.app"

// Mendefinisikan objek Data Firebase
FirebaseData fbdo;

// Objek autentikasi Firebase
FirebaseAuth auth;

// Konfigurasi Firebase
FirebaseConfig config;

// Beberapa variabel penting
String sValue, sValue2;
bool signupOK = false;
int led1 = 12;
int led2 = 14;

void setup() {
  // Memulai komunikasi serial dengan kecepatan 115200 bps
  Serial.begin(115200);
  
  // Mengatur pin LED sebagai output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  // Menghubungkan ke jaringan Wi-Fi menggunakan kredensial yang telah ditentukan
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan ke Wi-Fi");
  
  // Tunggu sampai terhubung ke jaringan Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Terhubung dengan IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Mengisi kunci API (diperlukan) */
  config.api_key = API_KEY;

  /* Mengisi URL Realtime Database Firebase (diperlukan) */
  config.database_url = DATABASE_URL;

  /* Mendaftar ke layanan Firebase */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Berhasil mendaftar ke Firebase");
    signupOK = true;
  }
  else {
    Serial.printf("Gagal mendaftar ke Firebase: %s\n", config.signer.signupError.message.c_str());
  }

  /* Mengisi fungsi panggilan balik untuk tugas pembuatan token yang berjalan lama */
  config.token_status_callback = tokenStatusCallback; // Lihat addons/TokenHelper.h

  // Memulai koneksi ke Firebase dengan konfigurasi yang telah ditentukan
  Firebase.begin(&config, &auth);
  
  // Membuat koneksi ulang ke jaringan Wi-Fi jika terputus
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Memeriksa apakah koneksi ke Firebase sudah siap dan proses pendaftaran berhasil
  if (Firebase.ready() && signupOK ) {
    // Menerima status LED1 dari Firebase
    if (Firebase.RTDB.getString(&fbdo, "LED/LED1")) {
      // Memeriksa apakah data yang diterima adalah string
      if (fbdo.dataType() == "string") {
        // Menerima nilai string dan mengubahnya menjadi integer
        sValue = fbdo.stringData();
        int value_led1 = sValue.toInt();
        Serial.println(value_led1);
        
        // Mengendalikan LED berdasarkan nilai yang diterima
        if (value_led1 == 1){
          digitalWrite(led1, HIGH);
        } else {
          digitalWrite(led1, LOW);
        }
      }
    }
    else {
      // Menampilkan pesan kesalahan jika gagal menerima data dari Firebase
      Serial.println("Gagal menerima data dari Firebase: " + fbdo.errorReason());
    }
    
    // Menerima status LED2 dari Firebase
    if (Firebase.RTDB.getString(&fbdo, "LED/LED2")) {
      // Memeriksa apakah data yang diterima adalah string
      if (fbdo.dataType() == "string") {
        // Menerima nilai string dan mengubahnya menjadi integer
        sValue2 = fbdo.stringData();
        int value_led2 = sValue2.toInt();
        Serial.println(value_led2);
        
        // Mengendalikan LED berdasarkan nilai yang diterima
        if (value_led2 == 1){
          digitalWrite(led2, HIGH);
        } else {
          digitalWrite(led2, LOW);
        }
      }
    }
    else {
      // Menampilkan pesan kesalahan jika gagal menerima data dari Firebase
      Serial.println("Gagal menerima data dari Firebase: " + fbdo.errorReason());
    }
  }
}
