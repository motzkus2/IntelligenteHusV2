/* Simpel demo af OLED LCD Display med I2C */
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS3231.h>
#include <MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <string.h>


#define DHTPIN 2

#define DHTTYPE DHT11

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int positionX = 0;
int positionY = 0;

DS3231 clock;
RTCDateTime dt;

MPU6050 mpu;

DHT dht(DHTPIN, DHTTYPE);

void checkSettings()
{
	Serial.println();
	
	Serial.print(" * Sleep Mode:        ");
	Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
	
	Serial.print(" * Clock Source:      ");
	switch(mpu.getClockSource())
	{
		case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
		case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
		case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
		case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
		case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
		case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
		case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
	}
	
	Serial.print(" * Gyroscope:         ");
	switch(mpu.getScale())
	{
		case MPU6050_SCALE_2000DPS:        Serial.println("2000 dps"); break;
		case MPU6050_SCALE_1000DPS:        Serial.println("1000 dps"); break;
		case MPU6050_SCALE_500DPS:         Serial.println("500 dps"); break;
		case MPU6050_SCALE_250DPS:         Serial.println("250 dps"); break;
	}
	
	
	Serial.print(" * Gyroscope offsets: ");
	Serial.print(mpu.getGyroOffsetX());
	Serial.print(" / ");
	Serial.print(mpu.getGyroOffsetY());
	Serial.print(" / ");
	Serial.println(mpu.getGyroOffsetZ());
	
	Serial.println();
}


void setup()
{
	Serial.begin(9600);

	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
	{ // Address for 128x64
		Serial.println(F("SSD1306 allocation failed"));
		for(;;); // Don't proceed, loop forever
	}
		display.clearDisplay();

		while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
		{
			Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
			delay(500);
		}
		
		  Serial.begin(9600);
  Wire.begin();
  
  clock.begin();
  
  clock.setDateTime(__DATE__, __TIME__);
  
  mpu.calibrateGyro();
  
  //DS3231_init(DS3231_CONTROL_INTCN);
  /*----------------------------------------------------------------------------
  In order to synchronise your clock module, insert timetable values below !
  ----------------------------------------------------------------------------*/
  checkSettings();
  
  dht.begin();
		
}

void loop()
{

display.cp437(true);
display.setCursor(0,0);
display.setTextSize(1); // Normal 1:1 pixel scale
display.setTextColor(WHITE); // Draw white text
display.clearDisplay();


float humidity = dht.readHumidity();
float temperatur = dht.readTemperature();
float heatindex = dht.computeHeatIndex(temperatur, temperatur, false);

display.println(F("Humidity: "));
display.setTextSize(2);
display.print(humidity);
display.println(F(" %"));

display.setCursor(0, 32);

display.setTextSize(1);
display.println(F("Temperature: "));
display.setTextSize(2);
display.print(temperatur);
//display.write(167);

display.print(cout << temperatur << " " << 167 << "C")

display.print(F(" " cout << 167 << "C "));

//display.write(167);
//display.print(heatindex);
//display.write(167);
//display.print(F("°C "));

display.display();

delay(1000);

dht.computeHeatIndex();
}

void Gyro(){
	
	display.setCursor(0,0);
	display.setTextSize(1); // Normal 1:1 pixel scale
	display.setTextColor(WHITE); // Draw white text
	display.clearDisplay();
	
	Vector rawGyro = mpu.readRawGyro();
	Vector normGyro = mpu.readNormalizeGyro();

	display.print(" Xraw = ");
	display.print(rawGyro.XAxis);
	display.print(" Yraw = ");
	display.print(rawGyro.YAxis);
	display.print(" Zraw = ");
	display.println(rawGyro.ZAxis);

	display.print(" Xnorm = ");
	display.print(normGyro.XAxis);
	display.print(" Ynorm = ");
	display.print(normGyro.YAxis);
	display.print(" Znorm = ");
	display.println(normGyro.ZAxis);

	if (normGyro.XAxis)
	{
	}

	display.display();
	delay(10);

	
}

void BouncingClock(){
	display.setTextSize(1); // Normal 1:1 pixel scale
	display.setTextColor(WHITE); // Draw white text
	
	
	display.clearDisplay();
	//display.println("TIDEN: ");
	display.setTextSize(1);

	
	for (positionY = 0; positionY <= 56; positionY ++) {
		display.setCursor(0,positionY);
		display.clearDisplay();
		dt = clock.getDateTime();
		display.print(dt.year);   display.print("-");
		display.print(dt.month);  display.print("-");
		display.print(dt.day);    display.print(" ");
		display.print(dt.hour);   display.print(":");
		display.print(dt.minute); display.print(":");
		display.print(dt.second); display.println("");
		display.display();
		delay(15);
	}
	
	for (positionY = 56; positionY >= 0; positionY --) {
		display.setCursor(0,positionY);
		display.clearDisplay();
		dt = clock.getDateTime();
		display.print(dt.year);   display.print("-");
		display.print(dt.month);  display.print("-");
		display.print(dt.day);    display.print(" ");
		display.print(dt.hour);   display.print(":");
		display.print(dt.minute); display.print(":");
		display.print(dt.second); display.println("");
		display.display();
		delay(15);
	}
	
	
	display.display();
}

