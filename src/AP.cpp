#include "AP.h"

/// Temperature variable
float temperature;
/// Humidity Variable
float humidity;

/**
 * @brief Set the Temperature
 * 
 * @param temperatureReal 
 */
void setTemp(float temperatureReal){
    temperature = temperatureReal;
}

/**
 * @brief Get the Temperature
 * 
 * @return float 
 */
float getTemp(){
    return temperature;
}

/**
 * @brief Set the Humidity
 * 
 * @param humidityReal 
 */
void setHumi(float humidityReal){
    humidity = humidityReal;
}

/**
 * @brief Get the Humidity
 * 
 * @return float 
 */
float getHumi(){
    return humidity;
}