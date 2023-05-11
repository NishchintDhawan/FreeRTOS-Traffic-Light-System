# Traffic-Light-System
A Traffic Light system using FreeRTOS on STM32F4 discovery board (ECE 455 at UVic)

## Project Design and Specification

![image](stop.png)
Red Traffic Light

<br>

![image](go.png)
Green Traffic Light when potentiometer is at low/medium resistance

<br>

![image](go2.png)
Green traffic light when potentiometer is at highest resistance

<br>

There are 3 core tasks performed by the system -

### Traffic Flow task
This task reads the resistance from the potentiometer and creates a flow value. It pushes the flow value to the main queue and sets the flow mutex.

### Traffic Manage task
It performs the traffic generation and display in one task making it easier to manage the data flow between them and making the system faster by eliminating the need for seperate tasks. It can read the flow data using the queue and the light data using the light mutex.

### Traffic Light task
It is responsiblle for managing the LED lights using software timers. It can read the flow data using the flow mutex. The global light variables containing the information about the traffic light color value are handled using mutexes to ensure that no task gets the older value. 
