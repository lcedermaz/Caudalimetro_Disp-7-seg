//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(7,6,5,1); // DIN, CLK, LOAD, cantidad de modulos "MAX72XX"

/* we always wait a bit between updates of the display */
unsigned long delaytime=250;

void setup() {

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}


/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void writeArduinoOn7Segment() {
  lc.setChar(0,0,'a',false);
  delay(delaytime);
  lc.setRow(0,0,0x05);
  delay(delaytime);
  lc.setChar(0,0,'d',false);
  delay(delaytime);
  lc.setRow(0,0,0x1c);
  delay(delaytime);
  lc.setRow(0,0,B00010000);
  delay(delaytime);
  lc.setRow(0,0,0x15);
  delay(delaytime);
  lc.setRow(0,0,0x1D);
  delay(delaytime);
  lc.clearDisplay(0);
  delay(delaytime);
} 

//##########################################################################
//###################### MUESTRA EN CADA SEGMENTO ##########################
//##########################################################################
void mostrarCAUDAL() {
      //###····CONVERSION DE DIGITOS DE CAUDAL·····###
      int Num=2386; // EJEMPLO "2386" pasa que esto es con un entero, tengo que ver de acuerdo a los valores que me arroja el caudalimetro
      int Digit0 = (Num  / 1000)  ; // Divido por 1000 es "2"
      int Digit1 = (Num % 1000) / 100 ; // toma "386", divido 100, entero "3"
      int Digit2 = (Num % 100) / 10 ; // (Num % 100)= toma el 86 , ese entero / 10 es "8"
      int Digit3 = (Num %10) ; // % tomo solo los decimales, es decir "6"
      //###········································###
      
    lc.setDigit(0,0,Digit0,false); //adress of the display, posicion DIG, the value to be displayed, sets the decimal point)
    lc.setDigit(0,1,Digit1,false);
    lc.setDigit(0,2,Digit2,true); // para poner el .decimal
    lc.setDigit(0,3,Digit3,false);
    delay(delaytime);
 
  lc.clearDisplay(0);
  delay(delaytime);
}

void loop() { 
  //writeArduinoOn7Segment();
  mostrarCAUDAL();
}
