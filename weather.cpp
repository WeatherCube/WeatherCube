#include "application.h"
#include "settings.h"
#include "beta-cube-library.h"
#define CUBE_DEBUG 1

#define WEATHER_API_REQUEST_INTERVAL_MS 60000    // interval to wait between API calls in milliseconds
#define CLEAR_DAY 0
#define CLEAR_NIGHT 1
#define RAIN 2
#define SNOW 3
#define SLEET 4
#define WIND 5
#define FOG 6
#define PARTLY_CLOUDY_DAY 7
#define PARTLY_CLOUDY_NIGHT 8

unsigned long lastRequestedUpdate = 0;

String weather_parameter_string = 0;
int weather = -1;
int weatherOverride = -1;
Cube cube = Cube();

void debug_log(String string) {
    if (CUBE_DEBUG) {
        Serial.print(string);
        Serial.println();
        Spark.publish("debug_log", string);
    }
}

void setWeather(String weatherString) {
    debug_log("Setting weather to '" + weatherString + "'");

    // Don't change the weather while we're testing particular weather animations
    if (CUBE_DEBUG && weatherOverride != -1) {
        return;
    }

    if (weatherString == "clear-day") {
        weather = CLEAR_DAY;
    } else if (weatherString == "clear-night") {
        weather = CLEAR_NIGHT;
    } else if (weatherString == "rain") {
        weather = CLEAR_NIGHT;
    } else if (weatherString == "snow") {
        weather = CLEAR_NIGHT;
    } else if (weatherString == "sleet") {
        weather = CLEAR_NIGHT;
    } else if (weatherString == "wind") {
        weather = CLEAR_NIGHT;
    } else if (weatherString == "fog") {
        weather = CLEAR_NIGHT;
    } else if (weatherString == "partly-cloudy-day") {
        weather = PARTLY_CLOUDY_DAY;
    } else if (weatherString == "partly-cloudy-night") {
        weather = PARTLY_CLOUDY_NIGHT;
    }

    debug_log("Set weather int to " + String(weather));
}

int setWeatherOverride(String override);
int setWeatherOverride(String override) {
    int intOverride = override.toInt();
    if (intOverride >= 0 && intOverride < 10) {
        weatherOverride = intOverride;
    }
    return weatherOverride;
}

// This function will get called when weather data comes in
void gotWeatherData(const char *name, const char *data) {
    // Important note!  -- Right now the response comes in 512 byte chunks.  
    //  This code assumes we're getting the response in a single chunks (ie < 512)
    String str = String(data);
    // Strip extra quotes
    String cleaned = str.substring(1, str.length() -1);
    int delimiter = cleaned.indexOf("~");

    String weatherString = cleaned.substring(0, delimiter);
    setWeather(weatherString);
}

// called once on startup
void setup() {
    cube.begin();
    cube.background(black);
    cube.show();

    if (CUBE_DEBUG) {
        Spark.function("setWeather", setWeatherOverride);
        // Grace period so that during dev it's easy to reflash
        delay(10000);
    }

    // Parameters to be sent to the webhook for get_weather
    // String needs to be escaped and encoded correctly so we build it here and store it globally
    weather_parameter_string = "{ \"lat\": \"" + String(LOCATION_LAT) + "\", \"lon\": \"" + String(LOCATION_LON) + "\", \"weather_key\":\"" + WEATHER_API_KEY + "\"}";
    debug_log("Weather string is:\n" + weather_parameter_string);

    // This crashes my particle core for some reason
    // Serial.begin(115200)

    // Lets listen for the hook response
    Spark.subscribe("hook-response/get_weather", gotWeatherData, MY_DEVICES);

    cube.background(blue);
    cube.show();
}

void clear_day() {
    cube.sphere(3, 3, 3, 3, yellow);
}

void clear_night() {
    cube.sphere(3, 3, 3, 3, purple);
}

void rain() {
    cube.sphere(3, 3, 3, 3, blue);
}

void snow() {
    cube.sphere(3, 3, 3, 3, white);
}

void sleet() {
    cube.sphere(3, 3, 3, 3, teal);
}

void wind() {
    cube.sphere(3, 3, 3, 3, pink);
}

void fog() {
    cube.sphere(3, 3, 3, 3, grey);
}

void partly_cloudy_day() {
    cube.sphere(3, 3, 3, 3, magenta);
}

void partly_cloudy_night() {
    cube.sphere(3, 3, 3, 3, brown);
}

// called forever really fast
void loop() {
    
    if (CUBE_DEBUG && weatherOverride != -1) {
        weather = weatherOverride;
    } else {
        unsigned long now = millis();
        if (!lastRequestedUpdate || (now > (lastRequestedUpdate + WEATHER_API_REQUEST_INTERVAL_MS))) {
            // publish the event that will trigger our Webhook
            Spark.publish("get_weather", weather_parameter_string);
            lastRequestedUpdate = now;
        }
    }

    cube.background(black);

    switch (weather) {
        case CLEAR_DAY:
            clear_day();
            break;
        case CLEAR_NIGHT:
            clear_night();
            break;
        case RAIN:
            rain();
            break;
        case SNOW:
            snow();
            break;
        case SLEET:
            sleet();
            break;
        case WIND:
            wind();
            break;
        case FOG:
            fog();
            break;
        case PARTLY_CLOUDY_DAY:
            partly_cloudy_day();
            break;
        case PARTLY_CLOUDY_NIGHT:
            partly_cloudy_night();
            break;
    }

    cube.show();
}

