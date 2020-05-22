# Weather Clock

This is a weather clock that tells you the next 24 hours of data; the current temperature; and each pixel clockwise is one hour into the future, warmer or colder than now, and whether or not it's raining during that hour.

Uses the [Darksky API](https://darksky.net/dev/docs) (which will remain in service through the end of 2021).

### Parts

- [Particle Photon](https://store.particle.io/products/photon)
- [Adafruit 24 Neopixel ring](https://www.adafruit.com/product/1586)
- [Dual alphanumeric display](https://www.adafruit.com/product/2153) (seven segment is fine)
- [MAX7219 LED display driver](https://www.adafruit.com/product/453)

### Backend

The Particle Photon calls a proxy server, which in turn calls the DarkSky API. The proxy server itself requires a password in the query string, it caches the response from the Weather API, cutting down on costs, and it shrinks the JSON response to only what the Arduino needs to know.

