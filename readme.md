# arduino weather station

# reader

visualizes next 24 hrs of weather using [darksky api](https://developer.forecast.io/)

* neopixel ring
* dual segment display
* Arduino Yún, updated:

```
opkg update
opkg install python-openssl
opkg upgrade cpu-mcu-bridge
```

# sensor

broadcast to a [thingspeak](https://thingspeak.com/) channel 

* arduino yun or CC3000 wifi shield
* adafruit BMP180 pressure + temperature
* adafruit DHT humidity + temperature