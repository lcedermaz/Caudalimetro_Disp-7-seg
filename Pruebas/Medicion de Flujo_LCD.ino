
//------------------------------Config LCD---------------
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //Direccion de LCD, Pines A4 (SDA) y A5 (SCL)
#include <Wire.h>
#include <math.h>

//--------Tiempos no bloqueantes--
//------------------------------T_0  Display () ;
unsigned long previousMillis = 0;
const long interval = 1000 ;
//------------------------------T_0  Monitor () ;
/*unsigned long previousMillis_1 = 0;
const long interval_1 = 1000 ;*/

//----------Config Caudalimetro---------------------------
volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
int PinSensor = 2;    //Sensor conectado en el pin 2
float factor_conversion=7.11; //para convertir de frecuencia a caudal
float volumen=0;
long dt=0; //variación de tiempo por cada bucle
long t0=0; //millis() del bucle anterior
float frecuencia ; // Globalizamos las variables
float caudal_L_m ;


//---Función que se ejecuta en interrupción---------------
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
  delay(1000);   //muestra de 1 segundo
  noInterrupts(); //Deshabilitamos  las interrupciones
  frecuencia=NumPulsos; //Hz(pulsos por segundo)
  return frecuencia;
}

void setup() 
{ 
  //----Config LCD
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();// Indicamos medidas de LCD
  Wire.begin(); //configura el bus I2C estableciendo arduino como MASTER
  /*
  //----Mensaje de Bienvenida----//
  lcd.setCursor(4, 0);
  lcd.print("LAMyEN");

  lcd.setCursor(2, 1);
  lcd.print(" Cargando.");
  delay(1000);
  lcd.setCursor(2, 1);
  lcd.print(" Cargando..");
  delay(1000);
  lcd.setCursor(2, 1);
  lcd.print(" Cargando...");
  delay(1000);
  lcd.clear();

  lcd.setCursor(4, 0);
  lcd.print("LAMyEN");

  lcd.setCursor(2, 1);
  lcd.print(" Cargando.");
  delay(1000);
  lcd.setCursor(2, 1);
  lcd.print(" Cargando..");
  delay(1000);
  lcd.setCursor(2, 1);
  lcd.print(" Cargando...");
  delay(100);
  lcd.clear();
*/
  //----Config Serial
  Serial.begin(9600);
  
  //----Config E/S
  pinMode(PinSensor, INPUT); 
  attachInterrupt(0,ContarPulsos,RISING);//(Interrupción 0(Pin2),función,Flanco de subida)
  Serial.println ("Envie 'r' para restablecer el volumen a 0 Litros"); 
  t0=millis();
} 

void loop ()    
{
  if (Serial.available()) {
  if(Serial.read()=='r')volumen=0;//restablecemos el volumen si recibimos 'r'
  }
  float frecuencia=ObtenerFrecuecia(); //obtenemos la frecuencia de los pulsos en Hz
  float caudal_L_m=frecuencia/factor_conversion; //calculamos el caudal en L/m
  dt=millis()-t0; //calculamos la variación de tiempo
  t0=millis();
  volumen=volumen+(caudal_L_m/60)*(dt/1000); // volumen(L)=caudal(L/s)*tiempo(s)
  
   //-----Enviamos por el puerto serie---------------
  Serial.print ("Caudal: "); 
  Serial.print (caudal_L_m,3); 
  Serial.print ("L/min\tVolumen: "); 
  Serial.print (volumen,3); 
  Serial.println (" L");
  
  //*****Funcion Para el lcd (tiempos no bloqueantes)
  
    unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    lcd_display ();
  }
  
}

void lcd_display (){
   
    lcd.setCursor(0, 0);
    lcd.print(caudal_L_m, 3); // lo dejamos en milesima
    lcd.print("L/min ");
    
    lcd.setCursor(0, 1);
    lcd.print(volumen, 3); // lo dejamos en milesima
    lcd.print("L ");
}

