# WeatherCube
Source code for WeatherCube project

# Goals
- Cube animations corresponding to different weather types (clear-day, clear-night, rain, snow, wind, etc)
- Cube animations that convey when rain will be starting within the next hour. Perhaps a countdown.
- Cube can get forecast data from a webservice such as Forecast.io or Yahoo Weather. This can be accomplished by either a) making request directly to the API and parsing JSON response or b) use an intermediate server to poll the forecast and relay a simplified version of the forecast to the Cube via a Particle function.
- Determine location of the Cube in a way that allows the code to be shared (ie. avoid hard-coding coordinates if possible)
- Instructions on how to run this code on cube via USB (much faster for debugging)

Community participation is welcomed! If you are interested then introduce yourself on the CubeTube forum (http://www.cubetube.org/forum/?place=topic%2Fcubetalk%2FHTJx6q50WAo%2Fdiscussion), and state which parts of the project you would be excited to work on. 

## Useful Info

Commands

	# listen to your events 
	particle subscribe mine
	
	# build & flash from command line
	particle flash <core id> <sketch name>.ino
	
	# serial monitor
	particle serial monitor
	
	# list webhooks
	particle webhook list
	
	# create webhook
	particle webhook create webhook.json

	# call webhook. You will need to provide you own `weather_key`
	particle publish get_weather "{\"lat\":\"38.8977\",\"lon\":\"-77.0366\", \"weather_key\":\"AABBCCDDEEFFGGHHIIJJKKLLMM\"}"

Links

- Parsing JSON in the cloud: https://community.particle.io/t/cloud-json-parsing/11223/2
- Calling webhook from CLI: https://community.particle.io/t/syntax-for-sending-webhook-variables-from-cli/12341
- Working with WebHooks: https://docs.particle.io/guide/tools-and-features/webhooks/

# Weather API

## Forecast.io

Project currently uses [forecast.io API](https://developer.forecast.io/). Other weather APIs could be used, but you would need to redefine the webhook `requestTemplate`. The advantage of forecast.io is they provide hyper-accurate weather for your GPS location, and they can give you a very accurate predicition about minutes until rain will start. 

![Screenshots from Dark Sky App which uses forecast.io data](http://www.tech-shutter.com/wp-content/uploads/2015/05/screen-shot-2014-01-27-at-5-22-25-pm.png)
