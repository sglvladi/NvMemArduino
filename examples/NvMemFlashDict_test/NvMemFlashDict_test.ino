/**
 * @file NvMemFlashDict_test.ino
 * @brief Simple example damonstrating basic usage
 *        of NvMemFlashDict class
 * @author Lyudmil Vladimirov
 * @details https://github.com/sglvladi/NvMemArduino
 */

#include <NvMemFlashDict.h>

// Initialise a config manager
NvMemFlashDict config;

String key = "testKey";
String new_value = "test";

void setup()
{

  Serial.begin(9600);
  Serial.println("NvMemFlashDict SimpleReadWrite demo");

  // Check to see if specified key has non-empty value
  // Will return empty String ("") if key value is empty
  Serial.println("Checking to see if a dictionary exists with key: " + key);
  if (config.contains(key))
  {
    Serial.println("Retrieved stored dictionary -> {" + key + ": " + config.get(key) + "}");
  }
  else
  {
    Serial.println("No dictionary was found for key: " + key);
  }
  Serial.println("Dictionary has " + String(config.freeEntries()) + " free entries");

  // Set new value to key
  Serial.println("Storing dictionary -> {" + key + ": " + new_value + "}");
  config.set(key, new_value);

  // Read new value stored in dictionary
  Serial.println("Reading value of dictionary with key: " + key);
  if (config.contains(key))
  {
    Serial.println("Retrieved stored dictionary -> {" + key + ": " + config.get(key) + "}");
  }
  else
  {
    Serial.println("No dictionary was found for key: " + key);
  }
  Serial.println("Dictionary has " + String(config.freeEntries()) + " free entries");

  Serial.println("Reseting dictionary");
  config.reset();
  Serial.println("Dictionary has " + String(config.freeEntries()) + " free entries");
}

void loop()
{
  // put your main code here, to run repeatedly:
}
