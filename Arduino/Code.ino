//Libraries
#include <Wire.h>// Bibliothèque standart qui permet l'interaction avec les élements de l'Arduino - https://www.arduino.cc/en/reference/wire
#include <Adafruit_MPU6050.h>// Bibliothèque permettant l'interaction avec notre acceleromètre, le MPU-6050 - https://github.com/adafruit/Adafruit_MPU6050
#include <Adafruit_Sensor.h>//https://github.com/adafruit/Adafruit_Sensor


// Définition des constantes initiales mutables
float accX = 0;
bool key = false;
int nbr = 0;
int unite_time = 0;
float seuil = 2.0;


//Objects
Adafruit_MPU6050 mpu;
void setup() {
  
   //Initialisation du MPU-6050
   Serial.begin(9600);
   Serial.println(F("Initialize System"));
   if (!mpu.begin(0x68)) { // Change address if needed
      Serial.println("Failed to find MPU6050 chip");
      while (1) {
       delay(10);
     }
   }
   mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
   mpu.setGyroRange(MPU6050_RANGE_250_DEG);
   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}


void loop() {
  
   readMPU();

   // Initialisation des constantes du système
   const float m_structure = 240;
   const float m_TMD = 66;
   const float pi = 3.14;
   const float g = 9.81;

  // Calcul de la période puis de la longueur idéale du pendule effectué une fois que l'on a mesuré l'acceleration pendant 20 sec
  if (unite_time > 200){
    digitalWrite(5, HIGH);
    float N = nbr;
    float T = 2*20/N;
    float omega1_carre = 4*pi*pi/(T*T*(1 + m_TMD/m_structure));
    float longueur = g/omega1_carre;
    Serial.print("Nbr Chgmt Signes = ");
    Serial.print(nbr);
    Serial.print("\nPeriode =");
    Serial.print(T);
    Serial.print("\nLongueur Pendule = ");
    Serial.print(longueur);
    while(1);
  }

   // Permet de régler la fréquence d'échantillonage fe = 1/(delay/1000)
   delay(50); // décalage en ms entre deux mesures
}


void readMPU( ) { /* function readMPU */

   // Read accelerometer data
   sensors_event_t a, g, temp;
   mpu.getEvent(&a, &g, &temp);
   float accX2 = a.acceleration.x;
   float accY = a.acceleration.y;
   float accZ = a.acceleration.z;
   float norme_acc = sqrt(accX2*accX2 + accY*accY + accZ*accZ);
   
   // Print accelerometer data -> data reçu ensuite par PuTTy
   Serial.print(unite_time/10.0);
   Serial.print(";");
   Serial.print(accX2);
   Serial.print(";");
   Serial.print(accY);
   Serial.print(";");
   Serial.print(accZ);
   Serial.print(";");
   Serial.print(norme_acc);
   Serial.print("\n");


  if (norme_acc > seuil){
    key = true;
  }

  // Permet de compter le nombre de changements de signes et donc ensuite la période
  if (key = true){
    if (accX2 * accX < 0){
      nbr++;
    }
    unite_time++;
    accX = accX2;
  }

   
}
