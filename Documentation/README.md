## A small histrory of what happended when


### 19.04.2021
Installing of required software:
 - node-red
 - EMOTIV BCI
 - node-red-emotiv-toolbox


### 21.04.2021
Trying to create a connection between node-red and arduino
 - Connunication will happen via USB serial connection
 - Add node-red-nodes-serialport addon for communication to serial ports
 - Add node-red-dashboard for quick UI testing
 - node-red dashboard [here](http://localhost:1880/ui) when node-red is running
 - Serial connection test
    - See `../Arduino/serial_connetion_test` and `../Node-RED/serial_connection_test.json`
    - Serial connection baud rate 9600 on COM3
        - > COM Port is hardware dependend
    - > Only one software can connect to a serial connection at a time. Do not use Arduinos (or any) serial monitor and node-red at the same time
    - The Arduino repeats the message sent from the node-red dashboard


### 26.04.2021
Rented the headset
 - Frame, two parts
 - Sensors
 - Liquid
 - Wire
 - USB-Stick
 - Serial Number

Headset intro
 - Not more than 8 refreshments per second
 - Realistically 1 thought/second
 - Threshold at about 50
 - Smoothing function -> Failsafe against wrong commands
 - 20-30 training sessions
 - Keep sensors slightly moist
 - Store sensors in packaging
 - Virtual headset in EMOTIV app
 - Live training feedback on
 - Learn commands one by one
 - Small trainings sessions

 > Does EMOTIV work with Linux?
