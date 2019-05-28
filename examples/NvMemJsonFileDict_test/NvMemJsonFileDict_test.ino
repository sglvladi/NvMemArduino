/**
 * @file NvMemJsonFileDict_test.ino
 * @brief Simple example damonstrating basic usage
 *        of NvMemJsonFileDict class
 * @author Lyudmil Vladimirov
 * @details https://github.com/sglvladi/NvMemArduino
 */

#include <NvMemJsonFileDict.h>

// Initialise a Json dictionary file
NvMemJsonFileDict dict("/test1.json");

String keys[50];
String values[50];

void setupDictEntries()
{
  for (int i = 0; i < 50; i++)
  {
    keys[i] = "testKey" + String(i);
    values[i] = "testValue" + String(i);
    dict.set(keys[i], values[i]);
  }
}

void verifyDictEntries()
{
  for (int i = 0; i < 50; i++)
  {
    String val = dict.get(keys[i]);
    if (val != values[i])
    {
      Serial.println("ERROR! Stored values don't match -> " + String(i));
    }
  }
}

void setup()
{

  Serial.begin(9600);

  // Open dictionary file
  Serial.println("Opening dictionary file...");
  dict.open(true);

  // Populate the dictionary
  Serial.print("Populating dictionary.");
  setupDictEntries();
  dict.save(); // Save to SPIFFS

  // Verify entries have been written correctly
  Serial.print("Verifying dictionary entries.");
  verifyDictEntries();

  // Reset the dictionary
  dict.reset();
}

void loop()
{
  // put your main code here, to run repeatedly:
}