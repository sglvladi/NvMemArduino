#ifndef NvMemJsonFileDict_h
#define NvMemJsonFileDict_h

#include <NvMemBaseDict.h>

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>

#if defined(ARDUINO_ARCH_ESP32)
// ESP32
#include "SPIFFS.h"
#endif

/**
 * @brief A Json file wrapper class for storing and retrieving key-value pairs
 *        in non-volatile memory.

  Includes support for:

  - ESP8266 && ESP32 (tested):
    - Makes use of the
 [SPIFFS(ESP8266)](https://github.com/esp8266/Arduino/tree/master/cores/esp8266/spiffs)
      and
 [SPIFFS(ESP32)](https://github.com/espressif/arduino-esp32/tree/master/libraries/SPIFFS)
      libraries to create, access (read/write) and manage a given Json file
      in the filesystem.
  - Generic Arduino (untested):
    - Should work with all Arduino platforms, but has not been tested!
 *
 */
class NvMemJsonFileDict : public NvMemBaseDict
{

public:
  /**
	 * @brief Construct a new NvMemJsonFileDict object.
	 *
	 * @param path Path to the file. (e.g. "/settings.json")
	 *
	 */
  NvMemJsonFileDict(String path, bool auto_save = false);

  /**
	 * @brief Override bool operator
	 *
	 * @return true If file is open
	 * @return false Otherwise
	 */
  explicit operator bool() { return _status > 0; }

  /**
	 * @brief Check if a given \p key is available in the dictionary
	 *
	 * @param key The key to be searched in the dictionary
	 * @return true if given key is available in the dictionary
	 * @return false otherwise
	 */
  bool contains(const String &key);

  /**
	 * @brief Get size of file
	 *
	 * @return size_t Size of the file in bytes
	 */
  size_t size();

  /**
	 * @brief Empty all file contents
	 *
	 */
  void reset();

  /**
	 * @brief Save contents of buffer to file
	 *
	 * @return true If the buffer was successfully writen to file
	 * @return false Otherwise
	 */
  bool save();

  /**
	 * @brief Get the file path
	 *
	 * @return String Path to the file
	 */
  String path();

  /**
	 * @brief Set the file path
	 *
	 * @param path Path to the file
	 */
  void setPath(String path);

  /**
	 * @brief Check if file autoSave has been turned on
	 *
	 * @return true if autoSave is enabled
	 * @return false otherwise
	 */
  bool autoSave();

  /**
	 * @brief Enable/Disable autoSave
	 *
	 * @param autoSave `true` to enable, `false` to disable
	 */
  void setAutoSave(bool autoSave);

  /**
	 * @brief Open the Json file.
	 *
	 * The class implements internal checks to make sure that the file is
	 * open, before attempting to access it, but ideally the \ref open
	 * "open()" function should be called before any other function.
	 *
	 * @param force If \p true and file cannot be opened, a new file will be
	 *              created. Default is `true`.
	 * @return int Error code taking the following values:
	 *             - `-2`: Failed to read and write to file. (`force=true`)
	 *             - `-1`: Failed to read file. (`force=false`)
	 *             -  `0`: Failed to convert to Json (`deserializeJson`
	 * error)
	 *             -  `1`: Successfully opened requested file
	 *             -  `2`: Created a new file (`force=true`)
	 */
  int open(bool force = true);

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

  String dataBuffer();
  void dataBuffer(String data, size_t size);

protected:
  File _file;
  bool _auto_save;
  size_t _size;
  int _status = -3;
  String _path = "/";
  std::vector<char> _dataBuffer;

private:
  void _autoSaveHelper();
  void _verifySpiffsStarted();
  size_t _calcDocSize(size_t data_len);
};
#endif