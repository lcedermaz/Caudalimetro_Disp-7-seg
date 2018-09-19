//*********************** Driver max7219 ***********************//

#include "LedControl.h"
LedControl lc = LedControl(12, 11, 10, 1);

unsigned long delaytime = 250;

//*********************** Caudalimetro ***********************//

const int sensorPin = 2; 
const int measureInterval = 2500;
volatile int pulseConter;

// ---> Segun el tipo de caudalímetro seleccionar la constante

// YF-S201 
const float factorK = 7.5;

// FS300A
//const float factorK = 5.5;

// FS400A
//const float factorK = 3.5;

float volume = 0;
long t0 = 0;

//*********************** Multiplexado - (Solo arduino) ***********************// 

int digit1 = 9; //PWM Display pin 1
int digit2 = 10; //PWM Display pin 2
int digit3 = 11; //PWM Display pin 6
int digit4 = 12; //PWM Display pin 8

//Pin mapping from Arduino to the ATmega DIP28 if you need it
//http://www.arduino.cc/en/Hacking/PinMapping

int segA = 2; //Display pin 14
int segB = 3; //Display pin 16
int segC = 4; //Display pin 13
int segD = 5; //Display pin 3
int segE = 6; //Display pin 5
int segF = 7; //Display pin 11
int segG = 8; //Display pin 15


//************************//



void setup()
{
	Serial.begin(9600);
	attachInterrupt(digitalPinToInterrupt(1), ISRCountPulse, RISING); //Configuración inteerrupciones 
	t0 = millis();
	
//*********** Configuración Display ***********// 
 /* 
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  
  pinMode(13, OUTPUT);

  for (int thisReading = 0; thisReading < numReadings; thisReading++) { // PROMEDIO
    readings[thisReading] = 0;
    
*/

  } 

//*********** Configuración max7219 ***********// 

  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  
}

void loop()
{
	
	//--------  Caudalimetro -------- //
	
	// obtener frecuencia en Hz
	float frequency = GetFrequency();

	// calcular caudal L/min
	float flow_Lmin = frequency * factorK;
	SumVolume(flow_Lmin);

	Serial.print(" Caudal: ");
	Serial.print(flow_Lmin, 3);
	Serial.print(" (L/min)\tConsumo:");
	Serial.print(volume, 1);
	Serial.println(" (L)");

//----------- max 7219 -----------//	
	
// Acoplar funcion con el caudalímetro
	
}

//--------  Caudalimetro -------- //

void ISRCountPulse()
{
	pulseConter++;
}

float GetFrequency()
{
	pulseConter = 0;

	interrupts();
	delay(measureInterval);
	noInterrupts();

	return (float)pulseConter * 1000 / measureInterval;
}

void SumVolume(float dV)
{
	volume += dV / 60 * (millis() - t0) / 1000.0;
	t0 = millis();
}

/* float caudal ()  { // Funcion que devuelve el valor a mostrar en el display
    
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(SensorPIN);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) 
  {
   readIndex = 0;
  }
  average = total / numReadings;
  
float V = (average /1023.0 * Vcc);
float R = (Rc * V ) / (Vcc - V);
float logR = log(R);
float R_th = 1.0 / (A + B * logR + C * logR * logR * logR );
float kelvin = R_th - ((V*V)/(K * R));
float celsius = (kelvin-273.15);    // se puede corregir el offset restando o sumando la variable kelvin    
return celsius;*/
}  


void disp_something() // Muestra la palabra "COOL"
{
      lc.setRow(0,3,0x4E);
      lc.setDigit(0,2,0,false);
      lc.setDigit(0,1,0,false);
      lc.setRow(0,0,0x1E);
      delay(5*delaytime);
      lc.clearDisplay(0);
}

void moving_digits() // HAce un conteo hasta el número 6
{
  for(int i=0;i<8;i++)
  {
    lc.setDigit(0,i,i,false);
       delay(2*delaytime);
  }
  lc.clearDisplay(0);
  delay(delaytime);
}


//*********EJEMPLO PARA USAR DISPLAY 7 SEG, TOMADO DE LA PAGINA -> https://playground.arduino.cc/Main/LedControl#Seg7Control

void printNumber(int v) {
    int ones;
    int tens;
    int hundreds;
    boolean negative;	

    if(v < -999 || v > 999) 
       return;
    if(v<0) {
        negative=true;
        v=v*-1;
    }
    ones=v%10; // En esta parte se divide el valor, XXX, quedaría de ver el punto decimal
    v=v/10;
    tens=v%10;
    v=v/10;
    hundreds=v;	
  
    if(negative) {
       //print character '-' in the leftmost column	
       lc.setChar(0,3,'-',false);
    }
    else {
       //print a blank in the sign column
       lc.setChar(0,3,' ',false);
    }
    //Now print the number digit by digit
    lc.setDigit(0,2,(byte)hundreds,false);
    lc.setDigit(0,1,(byte)tens,false);
    lc.setDigit(0,0,(byte)ones,false);
}
