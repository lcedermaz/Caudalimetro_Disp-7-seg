#include "LedControl.h" //Libreria
LedControl lc=LedControl(4,5,6,1); //(DIN_pin, CLK_pin, LOAD_pin, number_of_chips MAX7219);

void setup() { 
  // Inicializa o Max7219
  lc.shutdown(0,false); // Habilita el display 
  lc.setIntensity(0,15); // Intensidad luminica (MAX 15)
  lc.clearDisplay(0); 

} 

void loop() { 

      //###····CONVERSION DE DIGITOS DE CAUDAL·····###
      int Num=2386; // EJEMPLO "2386" -- %: divide y toma decimales
      int DIG0 = (Num  / 1000)  ; // Divido por 1000 es "2"
      int DIG1 = (Num % 1000) / 100 ; // toma "386", divido 100, entero "3"
      int DIG2 = (Num % 100) / 10 ; // (toma el 86 , ese entero / 10 es "8"
      int DIG3 = (Num %10) ; // % tomo solo los decimales, es decir "6"
      //###········································###

    lc.setDigit(0,0,DIG0,0);  //(chip, digit_position, number, dot);
    lc.setDigit(0,1,DIG1,0);  
    lc.setDigit(0,2,DIG2,1);  // activo el punto decimal
    lc.setDigit(0,3,DIG3,0);  
    delay(250);  

}
