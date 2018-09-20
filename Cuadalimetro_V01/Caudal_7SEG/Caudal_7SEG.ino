#include "LedControl.h" //Libreria
LedControl lc=LedControl(4,5,6,1); //(DIN_pin, CLK_pin, LOAD_pin, number_of_chips MAX7219);
volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
int PinSensor = 2;    //Sensor conectado en el pin 2
float factor_conversion=7.5; //para convertir de frecuencia a caudal

//---Función que se ejecuta en interrupción---------------
void ContarPulsos ()
{ 
  NumPulsos++;  //incrementamos la variable de pulsos
} 

//---Función para obtener frecuencia de los pulsos--------
int ObtenerFrecuencia() 
{
  int frecuencia;
  NumPulsos = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  delay(1000);   //muestra de 1 segundo
  noInterrupts(); //Desabilitamos las interrupciones
  frecuencia=NumPulsos; //Hz(pulsos por segundo)
  return frecuencia;
}

void setup()
{ 
  Serial.begin(9600); 
   // Inicializa o Max7219
  lc.shutdown(0,false); // Habilita el display 
  lc.setIntensity(0,15); // Intensidad luminica (MAX 15)
  lc.clearDisplay(0); 
  pinMode(PinSensor, INPUT); 
  attachInterrupt(0,ContarPulsos,RISING); //(Interrupcion 0(Pin2),funcion,Flanco de subida)
} 

void loop ()    
{
  float frecuencia=ObtenerFrecuencia(); //obtenemos la Frecuencia de los pulsos en Hz
  float caudal_L_m=frecuencia/factor_conversion; //calculamos el caudal en L/m
  float caudal_L_h=caudal_L_m*60; //calculamos el caudal en L/h
  int entera = caudal_L_h*10

  //###····CONVERSION DE DIGITOS DE CAUDAL·····###
       // EJEMPLO "2386" -- %: divide y toma decimales
      int DIG0 = (entera  / 1000)  ; // Divido por 1000 es "2"
      int DIG1 = (entera % 1000) / 100 ; // toma "386", divido 100, entero "3"
      int DIG2 = (entera % 100) / 10 ; // (toma el 86 , ese entero / 10 es "8"
      int DIG3 = (entera %10) ; // % tomo solo los decimales, es decir "6"
      //###········································###

    lc.setDigit(0,0,DIG0,0);  //(chip, digit_position, number, dot);
    lc.setDigit(0,1,DIG1,0);  
    lc.setDigit(0,2,DIG2,1);  // activo el punto decimal
    lc.setDigit(0,3,DIG3,0);  
    delay(250); 
  

  //-----Enviamos por el puerto serie---------------
  Serial.print ("FrecuenciaPulsos: "); 
  Serial.print (frecuencia,0); 
  Serial.print ("Hz\tCaudal: "); 
  Serial.print (caudal_L_m,3); 
  Serial.print (" L/m\t"); 
   Serial.print (caudal_L_h,3); 
  Serial.println ("L/h"); 
}
