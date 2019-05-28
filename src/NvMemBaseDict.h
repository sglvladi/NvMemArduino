#ifndef NvMemBaseDict_h
#define NvMemBaseDict_h

#include <Arduino.h>

/**
 * @brief Base/Abstract dictionary object
 * 
 * Sets out the main interface requirements for derived classes.
 * 
 */
class NvMemBaseDict
{

public:
  /**
     * @brief Get the stored value of a given entry with key \p key
     *
     * @param key Key of entry whose value to get
     */
  virtual String get(const String &key) = 0;
  /**
     * @brief Set the value of a given entry with key \p key
     *
     * @param key Key of entry whose value to set
     */
  virtual bool set(const String &key, const String &value) = 0;
  /**
     * @brief Remove an entry that matches a given \p key
     *
     * @param key Key of entry to be removed
     */
  virtual bool remove(const String &key) = 0;
  /**
     * @brief Check if a given \p key is available in the dictionary
     *
     * @param key The key to be searched in the dictionary
     */
  virtual bool contains(const String &key) = 0;
  /**
     * @brief Clear all dictionary contents
     *
     */
  virtual void reset() = 0;
};

#endif