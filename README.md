# Solar-collector-pump-control
# Upravljanje pumpom za solarni kolektor 

Two arduino microcontrollers are controlling the pump that pushes water from a solar collector to a boiler located away from the solar collector. Such a solution was not possible to achieve with a classic thermostat because information on the temperature difference between the boiler and the collector is required to control the pump.  The pump works when the desired temperature difference is achieved between the collector and the boiler, wich user can modify in the system application interface. The user has an insight into the historical representation of the data in the diagram, therefore he can see exactly what was the temperature movement in the boiler and collector and when the pump was running.  Also user on the application interface can switch off the automatic operation itself and start the pump "manually".

![](Images/IMG_2178.jpg)

It consists of two subsystems, or two microcontrollers esp8266 and two temperature sensors (one in the boiler, the other in the solar panel), two voltage suply and relay for pump driving.  Subsystems exchange temperature data with client-server method and comparing them, after it is desired temperature difference is achieved (the user can unijent through blynk applications ) the relay to which the pump is connected is switched on and the heated water from the solar collector goes to the boiler.

1. The subsystem ESP8266 installed in the boiler consists of the power supply, the relay that controls the pump, and the temperature sensor that is installed in the boiler.

![](Images/IMG_4781.jpg)
