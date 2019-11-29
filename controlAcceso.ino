#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522
Servo servito;
void setup() {
  Serial.begin(9600); //Iniciamos La comunicacion serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC52
  Serial.println("Control de acceso:");
  servito.attach(8);
  servito.write(0);
}
byte ActualUID[4]; //almacenará el código del Tag leído
byte Usuario1[5] = {0xEB, 0xA4, 0x34, 0x5B,} ; //código del usuario 1
byte Usuario2[4] = {0xC1, 0x2F, 0xD6, 0x0E} ; //código del usuario 2
int contador = 1;
void loop() {
  // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent())
  {
    //Seleccionamos una tarjeta
    if ( mfrc522.PICC_ReadCardSerial())
    {
      // Enviamos serialemente su UID
      Serial.print(F("Card UID:"));
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        ActualUID[i] = mfrc522.uid.uidByte[i];
      }
      Serial.print("     ");
      //comparamos los UID para determinar si es uno de nuestros usuarios
      if (compareArray(ActualUID, Usuario1)) {
        if (contador == 11) {
          Serial.println("Acceso denegado, exceso de personas");
          servito.write(0);
        } else {
          Serial.println("Acceso concedido...");
          servito.write(90);
          delay(5000);
          servito.write(0);
          contador++;
        }
      } else {
        Serial.println("Usuario no registrado");
      }
      // Terminamos la lectura de la tarjeta tarjeta  actual
      mfrc522.PICC_HaltA();

    }
  }
}
//Función para comparar dos vectores
boolean compareArray(byte array1[], byte array2[]) {
  if (array1[0] != array2[0])return (false);
  if (array1[1] != array2[1])return (false);
  if (array1[2] != array2[2])return (false);
  if (array1[3] != array2[3])return (false);
  return (true);
}
