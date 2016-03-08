# Darksky API JSON Bridge for Arduino Yun
DARKSKY_API_KEY = ''
LATITUDE = '40.694'
LONGITUDE = '-73.9186'

import urllib2, json, sys
sys.path.insert(0, '/usr/lib/python2.7/bridge/')
from bridgeclient import BridgeClient as bridgeclient
value = bridgeclient()
url = 'https://api.forecast.io/forecast/' + DARKSKY_API_KEY + '/' + LATITUDE + ',' + LONGITUDE + '?exclude=[minutely,daily,alerts,flags]'
response = urllib2.urlopen(url)
data = json.loads(response.read())
a = data['hourly']['data']
for i in range(len(a)):
    stringName = 'D' + str(i)
    temp = '%.2f' % float( a[i]['temperature'] )
    value.put(stringName, temp)