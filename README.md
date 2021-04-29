# Det Intelligente hus

## Projektet:
I dette projekt skulle vi lave en nyere version af det intelligente hus, ved hjælp af WiFi og IoT.
Vi har arbejdet med LED Display, DHT sensor(temperatur og luftfugtighed), Accelerometer, og et ur.

Vores projekt består af et Arduino Mega 2560, der måler luftfughtighed og temperatur, og som så videresender det vha. I2C til et Mkr WiFi 1010, som så uploader det til vores ThingSpeak cloud server vha. MQTT.
Derefter har vi et android projekt lavet med Xamarin, som henter dataerne ned og viser det med en graf.

## Mega 2560
### Includes:
Her bliver listet de nødvendige includes for at få projektet til at fungere.
\- Arduino.h
\- Wire.h
\- DHT.h
\- string.h

### Funktioner:
Vi har kun brugt én funktion udover Setup() og Loop(), og det er en funktion kaldet **I2CScanner** 
#### I2CScanner():
Funktionen bliver brugt til at finde tilknyttede devices til ens I2C setup. Den sender et signal ud til alle adresser, og tjekker return valued om der er et svar eller ej.

#### Setup():
Det eneste vi har i vores setup er funktionerne:
**Serial.begin(9600)** - der starter vores serial så vi udskrive og læse vores værdier i en terminal
**Wire.begin()** - som starter I2C og sætter sig selv som master.
**dht.being()** - som starter vores dht11 så den er klar til at læse værdier.

#### Loop():
I vores loop putter vi vores læste data i en string i denne linje:
https://imgur.com/a/AbUN3Vm

efterfølgende starter vi vores transmission med Wire.beginTransmission(4).
4 tallet er adressen på vores slave.
Så bruger vi Wire.print(data) til at sende vores data, efter fulgt af Wire.endTransmission(); til at slutte afsendelsen.

Derefter har vi tilføjet et delay på 1 sekund, så der bliver sendt data hvert sekund.

## MKR Wifi 1010
### Includes:
Her bliver listet de nødvendige includes for at få projektet til at fungere.
\- Arduino.h
\- Wire.h
\- WiFiNINA.h
\- MQTT.h

### Funktioner:
Udover vores setup() og loop() bruger vi disse Funktioner:
**void Connect()** - en funktion til at få forbindelse til WiFi
**void receiveEvent()** - funktionen der modtager vores data fra Master igennem I2C

#### Setup():
I setup starter vi vores Serial som på det andet board så vi kan læse data.
Derefter bruger WiFi.begin(ssid, pass) så vi får forbindelse til WiFi.
så bruger vi MQTT library til at starte en forbindelse til vores ThingSpeak server vha. client.begin().
Vi bruger funktionen Wire.begin(4), til at give vores slave adressen 4 i I2C bussen, samt starter Wire.onReceive(receiveEvent) som modtager vores data fra Master.

#### Loop():
I vores loop tjekker vi hele tiden om vi har forbindelse til wifi med connect(). hvis vi er connected og har modtaget data, som vi har gemt i en string vha. vores receiveEvent, så bruger vi client.publish(topic, data)
til at sende vores data op til ThingSpeak.

## Opsætning
Her er et billede af vores opsætning. da Mega 2560:
https://imgur.com/gallery/Hlh5TJE


