import urllib2, json, sys
sys.path.insert(0, '/usr/lib/python2.7/bridge/')
from bridgeclient import BridgeClient as bridgeclient
value = bridgeclient()
url = 'https://api.forecast.io/forecast/2c3297c78ee5f772a0005a607f7d6a53/40.694,-73.9186?exclude=[minutely,daily,alerts,flags]'
response = urllib2.urlopen(url)
data = json.loads(response.read())
a = data['hourly']['data']
for i in range(len(a)):
    stringName = 'D' + str(i)
    temp = '%.2f' % float( a[i]['temperature'] )
    value.put(stringName, temp)