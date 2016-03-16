"""
Darksky API JSON Bridge for Arduino Yun
"""
import urllib2, json, sys

DARKSKY_API_KEY = ''
LATITUDE = '40.694'
LONGITUDE = '-73.9186'


def main():
    url = 'https://api.forecast.io/forecast/' + DARKSKY_API_KEY + '/' + LATITUDE + ',' + LONGITUDE + '?exclude=[minutely,daily,alerts,flags]'
    response = urllib2.urlopen(url)
    data = json.loads(response.read())
    a = data['hourly']['data']
    for i in range(len(a)):
        temp_string = 'D' + str(i)
        precip_string = 'P' + str(i)
        temp = '%.2f' % float( a[i]['temperature'] )
        precip = '%.2f' % float( a[i]['precipProbability'] )
        sys.stdout.write(temp_string, temp)
        sys.stdout.write(precip_string, precip)


if __name__ == '__main__':
    main()
