# arduino weather station

# reader

visualizes next 24 hrs of temperature and precipitation

* [darksky api](https://developer.forecast.io/)

__parts__
* neopixel ring
* dual segment display

__update Yún__
* run `opkg update` then:
* `opkg install python-openssl`
* `opkg upgrade cpu-mcu-bridge`

# sensor

broadcast to a [thingspeak](https://thingspeak.com/) channel 

* arduino yun or CC3000 wifi shield
* adafruit BMP180 pressure + temperature
* adafruit DHT humidity + temperature