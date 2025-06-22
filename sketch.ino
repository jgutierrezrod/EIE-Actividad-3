// UNIR - MUIT
// Electrónica e Instrumentación Básicas - Actividad 2
// Autores:
// - Javier Gutiérrez Rodríguez
// - David Viñuela Reyes
// - Javier Sánchez Redondo


#include <LiquidCrystal_I2C.h> // Librería para el manejo de la pantalla LCD
#include <DHT.h> // Libreria para sensor DHT de temperatura y humedad
#include <math.h>
#include <Servo.h> // Incluye la biblioteca servo


#define DHTPIN 2 // En este pin es donde conectamos el sensor
#define DHTTYPE DHT22 // Usaremos el detector DHT22
#define BUTTON A2 // En este pin es donde conectamos el botón
#define SERVOPIN 9 // En este pin es donde conectamos el servomotor

Servo myservo; // Crea un objeto servo para controlar el servomotor
LiquidCrystal_I2C lcd(0x27, 16, 2); // Creación del objeto de la pantalla LCD
DHT dht(DHTPIN, DHTTYPE); // Creacion del objeto del sensor DHT


// Inicializamos valores
int ValorLDR = 0; 
int brilloLED = 0;
int angle = 0;      // Ángulo del servo (0 a 180)
byte cantidad_LEDs = 0;
byte pinesLED[4] = {3, 5, 6, 11}; // En este array contiene los pines conectados a los LEDs
byte ledControl = 13;
byte ledDiagnostico = 10;
bool status = true;
int menu = 0;
int TempMedida = 0;
float TempDeseada = 25.00;
//--------------------------------------------
#include <IRremote.h>			// importa libreria IRremote
#define Boton_1 0xCF30FF00		// reemplaza cada instancia de Boton_1 por su valor HEX
#define Boton_2 0xE718FF00		// reemplaza cada instancia de Boton_2 por su valor HEX
#define Boton_3 0x857AFF00		// reemplaza cada instancia de Boton_3 por su valor HEX
#define Boton_mas 0xfd02ff00
#define Boton_menos 0x6798ff00 
#define Boton_on 0x5da2ff00
#define Boton_menu 0x1de2ff00
#define Boton_atras 0x1fe0ff00

int SENSOR = 12; 			// sensor IR reciver

int LEDROJO = 8;
int LEDVERDE = 7;
int LEDAZUL = 4;

//---------------------------------------------
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("MONITORIZACION");
  lcd.setCursor(5, 1);
  lcd.print("CLIMA");
  delay(4000);
  lcd.clear();
  dht.begin();
  for (byte i = 0; i < 4; i++) {
    pinMode(pinesLED[i], OUTPUT);
  }
  pinMode(BUTTON, INPUT_PULLUP);
  
  // Autodiagnóstico y Control
  pinMode(ledControl, OUTPUT);
  pinMode(ledDiagnostico, OUTPUT);
  myservo.attach(SERVOPIN);

  //---------------------------------------------
  IrReceiver.begin(SENSOR, DISABLE_LED_FEEDBACK);	// inicializa recepcion de datos
  pinMode(LEDROJO, OUTPUT);		// pin 2 como salida
  pinMode(LEDVERDE, OUTPUT);		// pin 3 como salida
  pinMode(LEDAZUL, OUTPUT);		// pin 4 como salida
  //---------------------------------------------
}

void loop() {
  float humidity = dht.readHumidity(); // Leemos el valor de la humedad del sensor DHT
  float temperature = dht.readTemperature(); // Leemos la temperatura en grados celsius

  // Autodiagnóstico

  if (isnan(temperature) || isnan(humidity) || temperature < -38 || temperature > 78 || humidity < 2 || humidity > 98) {
    digitalWrite(ledDiagnostico, HIGH);
    digitalWrite(ledControl, LOW);
    return;
  } else {
    digitalWrite(ledDiagnostico, LOW);
  }

  if (temperature >= 0 && temperature <= 32) {
    digitalWrite(ledControl, HIGH);
  } else {
    digitalWrite(ledControl, LOW);
  }


  int ValorLDR = analogRead(A0); // Leemos el valor del sensor de luz
  float voltage = ValorLDR / 1024. * 5; // Convertimos el valor a voltaje
  float resistance = 2000 * voltage / (1 - voltage / 5); // Calculamos la resistencia
  float lux = pow(50 * 1e3 * pow(10, 0.7) / resistance, (1 / 0.7)); // Calculamos el nivel de luz en lux

  // Determinamos los LEDs que encender en función del valor recogido por el LDR

  cantidad_LEDs = calcular_cantidad_leds(ValorLDR);

  brilloLED = map(ValorLDR, 1023, 0, 0, 255); // Ponemos el valor LDR a brillo PWM

  // Encendemos los LEDs según los valores recogidos  
  for (byte i = 0; i < 4; i++) {
    if (i < cantidad_LEDs) {
      analogWrite(pinesLED[i], brilloLED); // Encendemos LEDs ajustando brillo
    } else {
      analogWrite(pinesLED[i], 0); // Apagamos LEDs
    }
  }

  if (status) {
    // Comprobamos si el botón ha sido pulsado
    if (digitalRead(BUTTON) == LOW) {
      lcd.clear();
      while (digitalRead(BUTTON) == LOW); // Esperamos a soltar el botón, para evitar más de un cambio por pulsación
      cambiar_informacion_lcd();
    }
  }

  // Mostramos la información correspondiente al menú actual
  if(status) {
    if (menu == 0) {
      mostrar_temperatura(temperature);
    } else if (menu == 1) {
      mostrar_temperatura_ext(TempDeseada);
    } else if (menu == 2) {
      mostrar_humedad(humidity);
    } else if (menu == 3) {
      mostrar_lux(lux);
    }
  }

  // Modificamos la posición del servomotor en función de la temperatura leída para simular la apertura de una compuerta
  IntSupVis(temperature);

  //---------------------------------------------
  if (IrReceiver.decode()) {				// si existen datos ya decodificados
    if (IrReceiver.decodedIRData.decodedRawData == Boton_1)		// si codigo recibido es igual a Boton_1
    {digitalWrite(LEDROJO, !digitalRead(LEDROJO));
    }	// enciende o apaga componente rojo

    if (IrReceiver.decodedIRData.decodedRawData == Boton_2)		// si codigo recibido es igual a Boton_2
    digitalWrite(LEDVERDE, !digitalRead(LEDVERDE));	// enciende o apaga componente verde

    if (IrReceiver.decodedIRData.decodedRawData == Boton_3)		// si codigo recibido es igual a Boton_3
    digitalWrite(LEDAZUL, !digitalRead(LEDAZUL));	// enciende o apaga componente azul
   

    IrReceiver.resume();
    delay (100);						// breve demora de 100 ms.
  }
  //---------------------------------------------
}

// Creamos una funcion para calcular la cantidad de LEDs que hay que encender
int calcular_cantidad_leds(int ValorLDR) {
  if (ValorLDR > 845) {
    cantidad_LEDs = 4;
  } else if (ValorLDR > 675) {
    cantidad_LEDs = 3;
  } else if (ValorLDR > 505) {
    cantidad_LEDs = 2;
  } else if (ValorLDR > 335) {
    cantidad_LEDs = 1;
  } else if (ValorLDR > 165) {
    cantidad_LEDs = 0;
  }

  return cantidad_LEDs;
}

// Creamos una funcion que nos muestre la temperatura medida por el sensor por el LCD
void mostrar_temperatura(float temperature) {
  lcd.setCursor(0,0);
  lcd.print("Temperatura: ");
  lcd.setCursor(0,1);
  lcd.print(temperature);
  lcd.print(" *C");
}

void mostrar_temperatura_ext(float TempDeseada) {
  lcd.setCursor(0,0);
  lcd.print("Temperatura Ext: ");
  lcd.setCursor(0,1);
  lcd.print(TempDeseada);
  lcd.print(" *C");
}

// Creamos una funcion que nos muestre la humedad medida por el sensor por el LCD
void mostrar_humedad(float humidity) {
  lcd.setCursor(0,0); // Posicionamiento de cursor
  lcd.print("Humedad: ");
  lcd.setCursor(0,1);
  lcd.print(humidity);
  lcd.print(" %");
}

// Creamos una funcion que nos muestre el nivel de lux medido por el fotoresistor por el LCD
void mostrar_lux(float lux) {
  lcd.setCursor(0,0);
  lcd.print("Nivel de luz: ");
  lcd.setCursor(0,1);
  lcd.print(lux);
  lcd.print(" lux ");
}

// Creamos una funcion que vaya cambiando de menú (usada al pulsar el botón)
void cambiar_informacion_lcd() {
  menu = menu >= 3 ? 0 : menu + 1;
  delay(50);
}

// Algoritmo de supervisión inteligente variando de la posición del servo según la temperatura
void IntSupVis(int temperature) {
  // Calculo de zonas de control
  int ZonaBaja = TempDeseada - 41;
  int ZonaMediaBaja = TempDeseada - 17;
  int ZonaMedia= TempDeseada + 7;
  int ZonaMediaAlta = TempDeseada + 31;

  TempMedida = (int) temperature;

  // Variar movimiento del servomotor para que te varíe en función de la temperatura deseada 
  if (TempMedida<=ZonaBaja) { 
    // Zona "baja"
    angle = 30;      
  }  
  else if (TempMedida>=ZonaBaja && TempMedida<=ZonaMediaBaja) {
    // Zona "media-baja"
    angle = 60;      
  }
  else if (TempMedida>=ZonaMediaBaja && TempMedida<=ZonaMedia) {
    // Zona "media"
    angle = 90;      
  }
  else if (TempMedida>=ZonaMedia && TempMedida<=ZonaMediaAlta) {
    // Zona "media-alta"
    angle = 120;      
  }
  else if (TempMedida > ZonaMediaAlta) {
    // Zona "alta"
     angle = 150;
  }

  myservo.write(angle);  // Mover el servo
}