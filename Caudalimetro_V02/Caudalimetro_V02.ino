//--------Display

#include "LedControl.h" //Libreria
LedControl lc=LedControl(4,5,6,1); //(DIN_pin, CLK_pin, LOAD_pin, number_of_chips MAX7219);

//--------Caudalimetro

volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
#define PinSensor = 2;    //Sensor conectado en el pin 2
float factor_conversion=7.11; //para convertir de frecuencia a caudal
float volumen=0;
long dt=0; //variación de tiempo por cada bucle
long t0=0; //millis() del bucle anterior

//--------Tiempos no bloqueantes--
//------------------------------T_0  Display () ;
unsigned long previousMillis = 0;
const long interval = 1000 ;
//------------------------------T_1 Caudalimetro ();
unsigned long previousMillis1 = 0;
const long interval1 = 1000 ;
//------------------------------T_2 Caudalimetro_Puerto Serie ();
unsigned long previousMillis2 = 0;
const long interval2 = 1000 ;
//------------------------------T_3 ContarPulsos ();
unsigned long previousMillis3 = 0;
const long interval3 = 1000 ;

// PONER LAS VARIABLES GLOBALES DEL CAUDALIMETRO!!!

void setup() { 
  
  // Inicializa o Max7219
  lc.shutdown(0,false); // Habilita el display 
  lc.setIntensity(0,15); // Intensidad luminica (MAX 15)
  lc.clearDisplay(0); 
  
  //Config Puerto Serie
  Serial.begin(9600); 
  pinMode(PinSensor, INPUT); 
  
  //Config Entradas Sensor Caudal
  attachInterrupt(0,ContarPulsos,RISING);//(Interrupción 0(Pin2),función,Flanco de subida)
  Serial.println ("Envie 'r' para restablecer el volumen a 0 Litros"); 
  t0=millis();

}

void loop() { 

    unsigned long currentMillis = millis();

    // Gestionamos los Tiempos
    
    if ((unsigned long)(currentMillis - previousMillis) >= interval)    //----T_0
    {
       Display () ;
      previousMillis = millis();
    }

    if ((unsigned long)(currentMillis - previousMillis1) >= interval1)    //----T_1
      {
        Caudalimetro ();
        previousMillis1 = millis();
      }

    if ((unsigned long)(currentMillis - previousMillis2) >= interval2)    //----T_2
      {
        Caudalimetro_Puerto Serie ();
        previousMillis2 = millis();
      }

    if ((unsigned long)(currentMillis - previousMillis3) >= interval3)    //----T_3
      {
        ContarPulsos ();
        previousMillis3 = millis();
      }
/*
 Display () ;
 Caudalimetro ();
 Caudalimetro_Puerto Serie ();
 ContarPulsos ();
 */ 
}

void Display () {
  
       //###····CONVERSION DE DIGITOS DE CAUDAL·····###
      int Num = caudal_L_m*10; // Valor Tomado del Caudalímetro, se multiplica por 10 para generar la décima
      int DIG0 = (Num  / 1000)  ; // Divido por 1000 es "2"
      int DIG1 = (Num % 1000) / 100 ; // toma "386", divido 100, entero "3"
      int DIG2 = (Num % 100) / 10 ; // (toma el 86 , ese entero / 10 es "8"
      int DIG3 = (Num %10) ; // % tomo solo los decimales, es decir "6"
      //###········································###

    lc.setDigit(0,0,DIG0,0);  //(chip, digit_position, number, dot);
    lc.setDigit(0,1,DIG1,0);  
    lc.setDigit(0,2,DIG2,1);  // activo el punto decimal
    lc.setDigit(0,3,DIG3,0);  
    //delay(250);  
 
}

//------Funciones Para el caudalímetro

void Caudalimetro () {
  
    if (Serial.available()) {
    if(Serial.read()=='r')volumen=0;//restablecemos el volumen si recibimos 'r'
  }
  float frecuencia=ObtenerFrecuecia(); //obtenemos la frecuencia de los pulsos en Hz
  float caudal_L_m=frecuencia/factor_conversion; //calculamos el caudal en L/m
  dt=millis()-t0; //calculamos la variación de tiempo
  t0=millis();
  volumen=volumen+(caudal_L_m/60)*(dt/1000); // volumen(L)=caudal(L/s)*tiempo(s)
}

void Caudalimetro_Puerto Serie () {
  
  //-----Enviamos por el puerto serie---------------
  Serial.print ("Caudal: "); 
  Serial.print (caudal_L_m,3); 
  Serial.print ("L/min\tVolumen: "); 
  Serial.print (volumen,3); 
  Serial.println (" L");
  
}
//---Función que se ejecuta en interrupción

void ContarPulsos ()  
{ 
  NumPulsos++;  //incrementamos la variable de pulsos
} 

//---Función para obtener frecuencia de los pulsos--------

int ObtenerFrecuecia() 
{
  int frecuencia;
  NumPulsos = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  // delay(1000);   //muestra de 1 segundo - VER SI FUNCIONA SIN LOS TIEMPOS NO BLOQUEANTES!!!!!!
  noInterrupts(); //Deshabilitamos  las interrupciones
  frecuencia=NumPulsos; //Hz(pulsos por segundo)
  return frecuencia;
}


