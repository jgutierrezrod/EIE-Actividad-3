# Actividad 3 - Sistema de medición, control, actuación y presentación del clima, utilizando técnicas de comunicaciones, avanzadas y/o inteligentes
- **Asignatura**: Electrónica e Instrumentación Básicas
- ***Alumnos***:
  - Javier Gutiérrez Rodríguez
  - David Viñuela Reyes
  - Javier Sánchez Redondo
 
  *Se añade el video de simulación del proyecto en un archivo .zip debido a su tamaño*
    
 *Máster Universitario en Ingeniería de Telecomunicación*

 ## Diseño del sistema

 ### Hardware
 #### Equipamiento de pruebas
![image](https://github.com/user-attachments/assets/b6e0f493-0a45-41ba-b8c1-8ceb5ffbd2d6)

 ### Software

El software realizado durante esta actividad tiene varias partes. Se ha partido de inicializar todas las librerías, pines y crear objetos utilizando la placa base Arduino Uno. 

![image](https://github.com/user-attachments/assets/5951f1cc-c17e-435b-a7ee-c005dfce4604)

A continuación, en la función setup() hemos inicializado el sistema, dando por el LCD un mensaje introductorio "MONITORIZACIÓN CLIMA". Además, se han inicializado los pines de los LEDs, el sensor DHT, el fotorresistor, el botón, el IR Receiver, el IR Remote y el servomotor.


![image](https://github.com/user-attachments/assets/2a8b781c-f9f8-4416-8682-05ef09d776a9)

Una vez realizados estos pasos previos al funcionamiento final del sistema, se pasa a crear funciones que se utilizarán dentro de la función loop(). Las funciones son:

- Una función de autodiagnóstico para evaluar las condiciones del ambiente.
  
  ![image](https://github.com/user-attachments/assets/49bcc467-33a4-47e9-835d-75c1445283f2)

-  Una función que calcule la cantidad de LEDs a encender en función del valor registrado por el LDR.

  ![image](https://github.com/user-attachments/assets/d00dc5a4-4599-471c-b65f-7e3742b59692)

- Varias funciones que imprimen por pantalla la información recogida por los distintos sensores:

  ![image](https://github.com/user-attachments/assets/c6ab0124-e39e-4844-9735-56c6b3c8f37b)

- Una función para cambiar la información que se muestra por el LCD cuando se pulse el botón rojo:

  ![image](https://github.com/user-attachments/assets/715d565b-02f8-4a1b-894c-47527dfff736)

- Una función que cambia la posición de un servomotor según la temperatura registrada por el sensor DHT22 mediante algoritmo de supervisión inteligente:

  ![image](https://github.com/user-attachments/assets/ba43ddf1-c6cb-4de2-8596-e928bc1003b7)
  
- Una función que controla remotamente la estación meteorológica:
  
  ![image](https://github.com/user-attachments/assets/8fa5a288-283e-496d-92e6-01eeb2119c86)

A continuación, se ha configurado en la función loop() toda la lógica del sistema apoyándonos en todo lo mencionado anteriormente:

![image](https://github.com/user-attachments/assets/a4a2ae61-7ce7-4bd0-97a0-d65f6030a7b3)

En esta primera imagen, se recogen, tal y como se hizo en la actividad 1, los valores de temperatura, humedad y lux. Como añadido en esta nueva actividad, se usa la función que calcula la cantidad de LEDs que se encenderán en función de el valor LDR recogido por el fotorresistor y además se hace un cambio de este valor LDR a brillo PWM. A continuación, según el valor recogido por el fotorresistor, encenderemos más o menos LEDs. 

También se puede apreciar en la imagen el sistema utilizado para ver por el LCD los datos recogidos por los distintos sensores según se va apretando un botón. Para esto, utilizamos las funciones explicadas en el apartado superior que imprimen por pantalla la información recogida por los distintos sensores y que ambiar la información que se muestra por el LCD cuando se pulse el botón rojo.

![image](https://github.com/user-attachments/assets/cd932d1a-e31e-4ae3-a042-c79e2cd53f7c)

Finalmente, en esta última línea de la función loop() se puede ver la llamada a la función que activa el servomotor en función de la temperatura recogida por el sensor DHT22.

## Conclusiones

Se ha desarrollado un sistema de monitorización de clima utilizando distintos sensores para poder posteriormente representar la información recogida de forma visual, a partir de una pantalla LCD. La integración de este sistema ha constado de numerosos elementos con distintas funcionalidades, como pueden ser sensores de humedad, temperatura y luz, LEDs, botones o servomotores, consiguiendo así una solución funcional para este sistema. 
