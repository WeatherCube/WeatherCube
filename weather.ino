

// Particle Specific Info
// Enter your latitude, longitude, and API key (as a string) below
float LOCATION_LAT = YOUR_INFO_HERE;
float LOCATION_LON = YOUR_INFO_HERE;
String WEATHER_API_KEY = YOUR_INFO_HERE;


// Misc Variables
#define WEATHER_API_REQUEST_INTERVAL_MS 60000    // interval to wait between API calls in milliseconds
String weather_parameter_string = 0;

// called once on startup
void setup() {
    // For simplicity, we'll format our weather data as text, and pipe it to serial.
    // but you could just as easily display it in a webpage or pass the data to another system.

    // Learn more about the serial commands here http://docs.particle.io/firmware/#communication-serial
    // You can also watch what's sent over serial with the particle cli with
    //  particle serial monitor
    Serial.begin(115200);

    // Lets listen for the hook response
    Spark.subscribe("hook-response/get_weather", gotWeatherData, MY_DEVICES);


    // Lets give ourselves 10 seconds before we actually start the program.
    // That will just give us a chance to open the serial monitor before the program sends the request
    for(int i=0;i<10;i++) {
        Serial.println("waiting " + String(10-i) + " seconds before we publish");
        delay(1000);
    }

    build_weather_string(LOCATION_LAT, LOCATION_LON, WEATHER_API_KEY);

}

void build_weather_string(float lat, float lon, String api_key) {

    // Parameters to be sent to the webhook for get_weather
    // String needs to be escaped and encoded correctly so we build it here and store it globally
    weather_parameter_string = "{ \"lat\": \"" + String(lat) + "\", \"lon\": \"" + String(lon) + "\", \"weather_key\":\"" + api_key + "\"}";
    Serial.print("Weather string is: "); Serial.println(weather_parameter_string);
}


// called forever really fast
void loop() {

    if ( ! weather_parameter_string ) {

        Serial.println("Cannot request weather. No parameters!");

    } else {

        // Let's request the weather, but no more than once every 60 seconds.
        Serial.println("Requesting Weather!");

        // publish the event that will trigger our Webhook

        Spark.publish("get_weather", weather_parameter_string);
    }

    // and wait at least 60 seconds before doing it again
    delay(WEATHER_API_REQUEST_INTERVAL_MS);
}

// This function will get called when weather data comes in
void gotWeatherData(const char *name, const char *data) {
    // Important note!  -- Right now the response comes in 512 byte chunks.  
    //  This code assumes we're getting the response in a single chunks (ie < 512)

    String str = String(data);

    Serial.print("Received Data:");
    Serial.println(str);
}

// This might be useful later. From Particle example code
// // Returns any text found between a start and end string inside 'str'
// // example: startfooend  -> returns foo
// String tryExtractString(String str, const char* start, const char* end) {
//     if (str == NULL) {
//         return NULL;
//     }

//     int idx = str.indexOf(start);
//     if (idx < 0) {
//         return NULL;
//     }

//     int endIdx = str.indexOf(end);
//     if (endIdx < 0) {
//         return NULL;
//     }

//     return str.substring(idx + strlen(start), endIdx);
// }