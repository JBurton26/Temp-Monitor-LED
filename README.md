# Temp-Monitor-LED
Edinburgh Napier University Coursework for Sensing Systems for Mobile Applications (SET09118)

Made to demonstrate 2-way communication between a mobile device and a microcontroller, thus project consists of two parts: 
 - a WeMos D1 mini with a DHT11 temp. and hum. sensor and an addressable LED strip, and
 - a simple Android Mobile Application that displays a temp. and hum. info and allows a user to change the RGB value of the LED strip.
The two components were connected using the MQTT protocol and an external MWTT Broker (now defunct).

 Mark for this coursework: 92/100.
 
 Temperature logging via MQTT component adapted and developed into [Honours Project](https://github.com/JBurton26/honours19-20)
 
 Future work:
 - [ ] Port mobile application to cross platform environment such as Xamarin (C#) or Ionic (Typescript)
 - [ ] Replace MQTT broker with updated system, possibly a localised broker hosted on a raspberry pi connected to the home network.
