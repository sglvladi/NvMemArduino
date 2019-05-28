#ifndef NvMemFlashDict_h
#define NvMemFlashDict_h

#include <NvMemBaseDict.h>

#include <Arduino.h>
#include <memory>

#if defined(ARDUINO_ARCH_ESP32)
// ESP32
#include <Preferences.h>
#else
// Other
#include <EEPROM.h>
#include <Embedis.h>

#if defined(ARDUINO_ARCH_ESP8266)
// ESP8266
#include "spi_flash.h"
const size_t EEPROM_SIZE = SPI_FLASH_SEC_SIZE;
#define AUTO_SAVE 1
#else
/* If E2END isn't defined you can manually set this.
 * Set to 1024 bytes by default if undefined
 */
#ifndef E2END
#define E2END 1023
#warning "EEPROM size set to 1024 by default!"
#endif
const size_t EEPROM_SIZE = E2END + 1;
#endif

#endif

/**
 * @brief A simple, dictionary-like, class for storing and retrieving
 *        key-value pairs in Flash memory.

  Includes support for:

  - ESP8266 (tested):
    - Makes use of the [Embedis](https://github.com/thingSoC/embedis) library.
  - ESP32 (tested):
    - Makes use of the
 [Preferences](https://github.com/espressif/arduino-esp32/tree/master/libraries/Preferences).
  - Generic Arduino (untested):
    - Should work with all platforms supported by
 [Embedis](https://github.com/thingSoC/embedis).
 *
 */
class NvMemFlashDict : public NvMemBaseDict
{

public:
  /**
     * @brief Construct a new NvMemFlashDict object.
     *
     * \note
     * Although the constructor allows for an empty name (i.e.
     * <tt>name=""</tt>) to be passed as an argument, as explained above,
     * this will result in the name being set as <tt>name="root"</tt>,
     * rather than <tt>name=""</tt>.
     */
  NvMemFlashDict();

  /**
     * @brief Construct a new NvMemFlashDict object.
     *
     * @param name Name of the dictionary. This allows multiple dictionaries
     * to be defined in memory. The dictionary name cannot be changed after
     * \ref begin "begin()" is called. The default is <tt>name=""</tt> in
     * which case it will be set as <tt>name="root"</tt>.
     *
     * \note
     * Although the constructor allows for an empty name (i.e.
     * <tt>name=""</tt>) to be passed as an argument, as explained above,
     * this will result in the name being set as <tt>name="root"</tt>,
     * rather than <tt>name=""</tt>.
     */
  NvMemFlashDict(String name);

  /**
     * @brief Start the NvMemFlashDict object
     *
     * @param name Name of the dictionary. This allows multiple dictionaries
     * to be defined in memory. The dictionary name cannot be changed after
     * this function is called. The default is <tt>name=""</tt> in which
     * case the value set during the \ref NvMemFlashDict "constructor" will
     * be used.
     *
     */
  void open(String name = "");

  /**
     * @brief Clear all dictionary contents
     *
     */
  void reset();

  /**
     * @brief Get the name of the dictionary
     *
     * @return String Name of the dictionary
     */
  String name();

  /**
     * @brief Initialise an entry and return its value.
     *
     * The function will check to see if an entry exists with key \p key. If
     * such an entry exists, its value will be returned. Otherwise, a new
     * entry will be initialised with key \p key and value \p defaultValue,
     * and its value will be returned.
     *
     * @param key Key of entry to be initialised
     * @param defaultValue A default value to be used to initialise the
     * entry if one doesn't exist already.
     * @return String The value of the existing or newly-initialised entry
     */
  String init(const String &key, String defaultValue);

  /**
     * @brief Get the stored value of a given entry with key \p key. If the
     * \p key does not exist, a provided \p defaultValue will be returned.
     *
     * @param key The key whose value to get
     * @param defaultValue A default value to be returned if \p key does not
     *                     exist
     * @return String The returned value
     */
  template <typename T>
  String get(const String &key, T defaultValue);

  /**
     * @brief Get the stored value of a given entry with key \p key. If the
     * \p key does not exist, an empty \c String (i.e. \c "").
     *
     * @param key The key whose value to get
     * @return String The returned value
     */
  String get(const String &key);

  /**
     * @brief Set the value of a given entry
     *
     * Set the value of a given entry with key \p key to \p value.
     *
     * If \key matches an existing entry, its value will be overwritted by
     * \p value.
     * If <tt>value=""</tt>, any matched entry will be removed from memory,
     * i.e. the function becomes equivalent to \ref removeEntry
     * "removeEntry(key)". If \p key is not matched against any entry, a new
     * entry will be created.
     *
     * Note: A <tt>value=""</tt> will cause any existing entry to be
     * removed.
     *
     * @param key Key of entry whose value to set
     * @param value Value to be set to the entry. If <tt>value=""</tt>, then
     *              no new entry will be created and any existing
     * @return true if the function succeded in setting the value
     * @return false otherwise
     */
  bool set(const String &key, const String &value);

  /**
     * @brief Remove an entry that matches a given \p key
     *
     * @param key Key of entry to be removed
     * @return true if an existing entry was deleted
     * @return false otherwise
     */
  bool remove(const String &key);

  /**
     * @brief Get number of remaining free entries
     *
     * Calculates and returns the maximum number of dictionaries that can
     * still be allocated by the manager.
     *
     * @return unsigned long The number of free entries
     */
  unsigned long freeEntries();

  /**
     * @brief Check if a given \p key is available in the dictionary
     *
     * @param key The key to be searched in the dictionary
     * @return true if given key is available in the dictionary
     * @return false otherwise
     */
  bool contains(const String &key);

protected:
  String _name = "root";
  bool _started = false;
#if defined(ARDUINO_ARCH_ESP32)
  Preferences _preferences;
#endif

private:
  void _ensureStarted();
};
#endif