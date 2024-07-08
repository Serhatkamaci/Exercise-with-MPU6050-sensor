#include <Servo.h> // Servo motorları kontrol etmek için kullanılmaktadır.
#include <Wire.h> // I2C haberleşme işlemleri için kullanılmaktadır.
#include <Adafruit_MPU6050.h> // MPU6050 sensör özellikleri için kullanılmaktadır.
#include <Adafruit_Sensor.h> // Sensör verilerini okumak ve işlemek için kullanılmaktadır.
#include <SD.h> // SD karta erişim sağlamak için kullanıldı.
#include <Adafruit_SSD1306.h> // Oled ekranı ile iletişim kurmak için çağrıldı.
#include <Adafruit_GFX.h> // Ekran cihazlarını yönetmek için kullanılır.

Servo servoMotor1; // servoMotor1 nesnesi oluşturduk.
int servoMin1 = 0;  // Servo motorun minimum pozisyonunu belirledik. 
int servoMax1 = 180; // Servo motorun maksimum pozisyonunu belirledik.
Servo servoMotor2; // servoMotor2 nesnesi oluşturduk.
int servoMin2 = 0;   // Servo motorun minimum pozisyonunu belirledik.
int servoMax2 = 180; // Servo motorun maksimum pozisyonunu belirledik.

const int chipSelect = 53; // SD kart ile iletişim kurulurken hangi pine bağlandığını belirtmek için kullanıldı.

Adafruit_MPU6050 mpu; // MPU6050 sensörü için bir nesne oluşturduk.

#define SCREEN_WIDTH 128  // Oled ekranının genişliğini ayarladık.
#define SCREEN_HEIGHT 64 // Oled ekranının yüksekliğini ayarladık.

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Oled nesnemizi oluşturuduk.


void initializeSDcard(){ //SD karta bağlanma fonksiyonu.
  Serial.println("SD karta bağlanılıyor..."); // Console ekranına "SD karta bağlanılıyor..." yazdım.
  if (!SD.begin(chipSelect)) { // Pin ile bağlantı aşamasını başlatmaya çalışıyor.
    Serial.println("SD karta bağlantı işlemi başarısız!"); // Console ekranına "SD karta bağlantı işlemi başarısız!" yazdım.
    return;
  }
  Serial.println("SD karta bağlantı işlemi başarılı"); // Console ekranına "SD karta bağlantı işlemi başarılı" yazdım.
}


void setup() {

  Serial.begin(115200); // Arduino ile seri iletişimi başlattık.
  servoMotor1.attach(9); // Arduino'nun 9.pinine servo motor1'i bağladık.
  servoMotor2.attach(8); // Arduino'nun 8.pinine servo motor2'yi bağladık.

  while (!Serial) // Seri bağlantının başlamasını bekledik.
    delay(10); // 10 milisaniye beklettik.

  Serial.println("Seri bağlantı başladı");//Console ekranına "Seri bağlantının başladı" yazdık.

  // Oled ekranının başlatılmasını denetler.
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 oled başarısız!")); // Console ekranına "SSD1306 oled başarısız!" yazdık.
    while (1);
  }
  Serial.println(F("SSD1306 oled başarılı")); // Console ekranına "SSD1306 oled başarılı" yazdık.
  delay(10); // 10 milisaniye beklettik.

  initializeSDcard(); // SD karta bağlanma fonksiyonunu çağırdık.
  
  if (!mpu.begin())  // MPU6050 sensörünün başlatmaya çalışır.
  {
      Serial.println("MPU6050 sensörü bulunamadı!"); // Console ekranına "MPU6050 sensörü bulunamadı!" yazdık.
      while (1) {
        delay(3000); // 3000 milisaniye beklettik.
    }
  }

  Serial.println("MPU6050 sensörü bulundu\n"); // Console ekranına "MPU6050 sensörü bulundu" yazdık.

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); // İvmeölçer aralığını ayarladık.
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); // Jiroskop aralığını ayarladık. 
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ); // Filtre bant genişliğini ayarladık.

  Serial.println("\n\n"); // Console ekranında 2 satır aşağı geçtik.
  delay(100); // 100 milisaniye beklettik.
}

void loop() {
  Serial.println();
  sensors_event_t a, g, temp; // ivme, açısal hız, sıcaklık değişkenlerini tanımladık.
  mpu.getEvent(&a, &g, &temp); // ivme, açısal hız, sıcaklık değerlerini okuduk.

  Serial.println("SD karta yazılan ölçümler:\n"); // Console ekranına "SD karta yazılan ölçümler:" yazdık.
  Serial.println("Accelerometer – m/s^2"); // Console ekranına "Accelerometer – m/s^2" yazdık.
  Serial.print(a.acceleration.x); // Console ekranına x eksenindeki ivme değerini yazdık.
  Serial.print(", "); // Console ekranına "," yazdık. 
  Serial.print(a.acceleration.y); // Console ekranına y eksenindeki ivme değerini yazdık.
  Serial.print(", "); // Console ekranına "," yazdık. 
  Serial.println(a.acceleration.z); // Console ekranına z eksenindeki ivme değerini yazdık.

  Serial.println("Gyroscope – rps"); // Console ekranına "Gyroscope – rps" yazdık.
  Serial.print(g.gyro.x); // Console ekranına Gyroscope'un x eksenindeki değerini yazdık.
  Serial.print(", "); // Console ekranına "," yazdık. 
  int servoPos1 = map(g.gyro.x, -4.3, +4.3, servoMin1, servoMax1); // Servo motorun yeni açı değeri belirlendi.
  servoMotor1.write(servoPos1); // Servo motor yeni pozisyona geçti.
  Serial.print(g.gyro.y); // Console ekranına Gyroscope'un y eksenindeki değerini yazdık.
  Serial.print(", "); // Console ekranına "," yazdık. 
  int servoPos2 = map(g.gyro.y, -4.3, +4.3, servoMin2, servoMax2);  // Servo motorun yeni açı değeri belirlendi.
  servoMotor2.write(servoPos2); // Servo motor yeni pozisyona geçti.
  Serial.println(g.gyro.z); // Console ekranına Gyroscope'un z eksenindeki değerini yazdık.

  Serial.println("Temperature"); // Console ekranına "Temperature" yazdık.
  Serial.print(temp.temperature); // Console ekranına sıcaklık değerini yazdık.
  Serial.println(" degC"); // Console ekranına " degC" yazdık.

  oled.clearDisplay(); // Oled ekranının içeriğini temizledik.

  oled.setTextSize(1); // Metnin boyutunu belirledik
  oled.setTextColor(WHITE); // Metnin rengini belirledik.
  oled.setCursor(0, 2); // Satır olarak 0 sürun olarak 2'den yazmaya başlamasını belirledik.
  oled.println("Accelerometer-m/s^2");  // Oled ekranına "Accelerometer – m/s^2" yazdık.
  oled.print(a.acceleration.x); // Oled ekranına x eksenindeki ivme değerini yazdık.
  oled.print(", "); // Oled ekranına "," yazdık.
  oled.print(a.acceleration.y); // Oled ekranına y eksenindeki ivme değerini yazdık.
  oled.print(", "); // Oled ekranına "," yazdık.
  oled.println(a.acceleration.z); // Oled ekranına z eksenindeki ivme değerini yazdık.
  oled.println("Gyroscope-rps"); // Oled ekranına "Gyroscope-rps" yazdık.
  oled.print(g.gyro.x); // Oled ekranına Gyroscope'un x eksenindeki değerini yazdık.
  oled.print(", "); // Oled ekranına "," yazdık.
  oled.print(g.gyro.y); // Oled ekranına Gyroscope'un y eksenindeki değerini yazdık.
  oled.print(", "); // Oled ekranına "," yazdık.
  oled.println(g.gyro.z);  // Oled ekranına Gyroscope'un z eksenindeki değerini yazdık.
  oled.println("Temperature");  // Oled ekranına "Temperature" yazdık.
  oled.print(temp.temperature); // Oled ekranına sıcaklık değerini yazdık.
  oled.println(" degC"); // Oled ekranına " degC" yazdık.

  oled.display(); // Oled ekranında göstermek için kullanılır. 

  // SD kart yazma işlemleri.
  File dataFile = SD.open("mpu6050.txt", FILE_WRITE); // Dosyaya yazma modunda açar.

  // Dosyaya yazma işlemleri gerçekleştirir.
  if (dataFile) {
    dataFile.println("Accelerometer-m/s^2");  // Dosyaya "Accelerometer-m/s^2" yazdık.
    dataFile.print(a.acceleration.x); // Dosyaya x eksenindeki ivme değerini yazdık.
    dataFile.print(", "); // Dosyaya "," yazdık.
    dataFile.print(a.acceleration.y); // Dosyaya y eksenindeki ivme değerini yazdık.
    dataFile.print(", "); // Dosyaya "," yazdık.
    dataFile.println(a.acceleration.z); // Dosyaya z eksenindeki ivme değerini yazdık.
    dataFile.println("Gyroscope-rps"); // Dosyaya "Gyroscope-rps" yazdık.
    dataFile.print(g.gyro.x); // Dosyaya Gyroscope'un x eksenindeki değerini yazdık.
    dataFile.print(", "); // Dosyaya "," yazdık.
    dataFile.print(g.gyro.y); // Dosyaya Gyroscope'un y eksenindeki değerini yazdık.
    dataFile.print(", "); // Dosyaya "," yazdık.
    dataFile.println(g.gyro.z);  // Dosyaya Gyroscope'un z eksenindeki değerini yazdık.
    dataFile.println("Temperature");  // Dosyaya "Temperature" yazdık.
    dataFile.print(temp.temperature); // Dosyaya sıcaklık değerini yazdık.
    dataFile.println(" degC"); // Dosyaya " degC" yazdık.
    dataFile.close(); // Dosyayı kapattık.
    Serial.println("SD karta yazma işlemleri başarılı."); // Console ekranına "SD karta yazma işlemleri başarılı." yazdık.
  }
  else {
    Serial.println("SD karta yazma işlemleri başarısız!"); // Console ekranına "SD karta yazma işlemleri başarısız!" yazdık.
  }
  Serial.println(); // Console ekranında bir satır aşağı geçtik.
  delay(20); // 20 milisaniye beklettik.
}