/*Gabriel Gómez, Jonathan Montoya, Kévin Caldas
 Institución: Francisco de Miranda IED 
 Docente: William Suarez
 Curso:1101 - Año 2022
 Materias: Diseño tecnológico/Metodología de la investigación/Biotecnología ambiental
 */

 //Htas.ino
 
/*Librería de LCD (LyquidCristal), Incluyendo los siguientes comandos para trabajar en la librería dijital de Arduino:
Función: lcd
Ordenes:
-begin
-SetCursor
-Print
-Clear
*/
#include <LiquidCrystal.h>
/*Declaración de pines seriales en el area digital de Arduino
Pines Digitales usados:
-2
-3
-4
-5
-11
-12 */
int rs = 12;
int e = 11;
int d4 = 5;
int d5 = 4;
int d6 = 3;
int d7 = 2;
int numero = 0;
int dTime = 500;
/*Se declara en la función de la librería "LiquidCrystal lcd" los pines a usar que se tradcen en la tarjeta*/
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

/*DHT22
 * Se declara la librería de DHT y se define el tipo "Type" el cual es "DHT22" por usar el sensor del mismo nombre
*/
#include <DHT.h>
#include <DHT_U.h>
#define Type DHT22
/*Se declaran la variable en el pin digital 8 */
/*Librería de DHT, Incluyendo los siguientes comandos para trabajar en la librería dijital de Arduino:
Función: DHT
Ordenes:
-readHumidity() //Humedad (porcentaje)
-readTemperature() //Temperatura (Celcius)
-readTemperature(true) //Temperatura (Farenheit)
*/
int dhtPin = 8;
DHT HT (dhtPin, Type);
/*Se declaran la variable de humedad [int(Número entero)] y temperatura [float(Número décimal)]*/
int humidity;
float tempC;
float tempF;

//YL-69
int SensorHPin = A0;
/*Se declaran la variable de humedad del suelo al sensor analógico*/

//Ph-1959
float calibration_value = 21.34;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;
/*Se declara los valores numéricos:
calibration_value=21.34
phval=0
avgal(variable sin valor asignado)
buffer_arr (con un arco de 10 valores asignados)
temp=no asignado*/
void setup() {  //codigo de arranque, solo se ejecuta una vez
  
  lcd.begin(16, 2);
  Serial.begin(9600);
  HT.begin();
  lcd.setCursor(3,0);
  lcd.print("Htas1.5.1");
   lcd.setCursor(0,1);
   lcd.print("FM IED      ");
   lcd.print("2022");
  delay(9600);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HS: Suelo");
  lcd.setCursor(0,1);
  lcd.print("HA: Ambiente");
  delay(9600);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ph: Acidez");
  lcd.setCursor(0,1);
  lcd.print("T: Temperatura");
   delay(9600);
  lcd.clear();
  /*código de solo LCD arrancando con el nombre del prototipo, el colegio, el año y los comentarios de
  HS:Humedad del suelo
  HA:Humedad relativa
  Ph:Acidez
  T: Temperatura*/
  }

void loop() {
  //DHT22
  // Código en bucle, se repite en bucle después de terminar el código de arranque
  humidity = HT.readHumidity(); //porcenaje humedad
  tempC = HT.readTemperature(); //Valor temperatura
  tempF = HT.readTemperature(true); //Valor temperatura

  
  //YL-69
  Serial.print("Humedad:");
    //Crear un valor al sensor de humerdad
int humedad = analogRead(SensorHPin); /* El programa realiza una lectura constante del sensor y            
                                                                                      posterior lo escribe en el puerto serie */ 
  Serial.print(humedad);

//Condicional
if (humedad>=1015)  /* Agreagamos una condicion de funcionamiento en base a las lecturas*/
{
digitalWrite(7, LOW); /* Si se cumple la condicion la salida se desactiva, estado bajo */
}
else
{
digitalWrite(7, HIGH); /* Si no se cumple la condicion la salida se activa, estado alto */
}

int Suelo = analogRead(SensorHPin);           //asignar los valores//
Suelo = constrain (Suelo, 0, 1015);             /* Rango de medición */
Suelo = map (Suelo, 0, 1015, 100, 0);        /* Valor porcentual de 0-100*/


 //PH-1959
 //Bucle repetitivo
  for (int i = 0; i < 10; i++) //Condición de suma y declaración de var "i"
  {
    buffer_arr [i] = analogRead (A1); //Lectura del puerto analógico a la var "i"
    delay(30);
    }
    for (int i = 0; i < 9; i++) //Condición bucle var "i"
    {
      for (int j = i + 1; j < 10; j++) //Condición en bucle con suma y declaración a var "j"
      {
        if(buffer_arr[i] > buffer_arr [j]) //Condicional de la var "i" y la var "j"
        {
          temp = buffer_arr [i];  //Ejecución de temperatura
          buffer_arr [i] = buffer_arr [j];
          buffer_arr [j] = temp;  //Ciclo de repetición
          
        }
      }
    }     
    avgval = 0;
    for (int i = 2; i < 8; i++)
    avgval += buffer_arr [i];
    float volt = (float) avgval * 5.0 / 1024 / 6;
    float ph_act = -5.70 * volt + calibration_value; //Calibración al valor de Ph del sensor


    //Puerto serial
Serial.print(" / " );
Serial.print(Suelo);
Serial.print("% / " );
  Serial.print(" / Humedad Relativa: ");
  Serial.print(humidity);
  Serial.print("% / Temperatura: ");
  Serial.print(tempC);
  Serial.print("ºC / ");
  Serial.print(tempF);
  Serial.println("ºF / ");
  Serial.print("Ph:");
  Serial.print(ph_act);
/*código de la pantalla serial en bucle con los valores de, Temperatura C°, Temperatura F°, Humedad del suelo
 humedad relativa y Ácidez del suelo
  HS:Humedad del suelo
  HA:Humedad relativa
  Ph:Acidez
  T: Temperatura*/


 //Puerto LCD
  lcd.setCursor(0, 0);
  lcd.print("HS:");
  lcd.print(Suelo);
  lcd.print("%");
  lcd.print("   HA:");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Ph:");
  lcd.print(ph_act);
  lcd.print(" T:");
  lcd.print(tempC);
  lcd.print("C");
 /*código de solo LCD en bucle con los valores de, Temperatura, Humedad del suelo
 humedad relativa y Ácidez del suelo
  HS:Humedad del suelo
  HA:Humedad relativa
  Ph:Acidez
  T: Temperatura*/
  delay(dTime);
  lcd.clear();
  }
