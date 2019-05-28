#include "NvMemFlashDict.h"

NvMemFlashDict::NvMemFlashDict()
{
#if not defined(ARDUINO_ARCH_ESP32)
    Embedis embedis(Serial);
#endif
}

NvMemFlashDict::NvMemFlashDict(String name)
: NvMemFlashDict()
{
    if (name != ""){
        _name = name;
    }
}

void NvMemFlashDict::open(String name)
{
    if(!_started)
    {
        if (name != ""){
            _name = name;
        }
    #if defined(ARDUINO_ARCH_ESP32)
        _preferences.begin(_name.c_str(), false);
    #else
        EEPROM.begin(EEPROM_SIZE);

        Embedis::dictionary(_name,
                            EEPROM_SIZE,
                            [](size_t pos) -> char { return EEPROM.read(pos); },
                            [](size_t pos, char value) { EEPROM.write(pos, value); },
        #if AUTO_SAVE
                            []() { EEPROM.commit(); }
        #else
                            []() {}
        #endif
        );
    #endif
        _started = true;
    }
}

void NvMemFlashDict::reset()
{
    #if defined(ARDUINO_ARCH_ESP32)
        _preferences.clear();
    #else
        for (unsigned int i = 0; i < EEPROM_SIZE; i++) {
            EEPROM.write(i, 0xFF);
            // yield();
        }
        EEPROM.commit();
    #endif
}

String NvMemFlashDict::name()
{
    return _name;
}

String NvMemFlashDict::init(const String &key, String defaultValue)
{
    if (!contains(key))
    {
        set(key, defaultValue);
    }
    return get(key);
}

template <typename T>
String NvMemFlashDict::get(const String &key, T defaultValue)
{
    _ensureStarted();

    String value;
#if defined(ARDUINO_ARCH_ESP32)
    value = _preferences.getString(key.c_str(), String(defaultValue));
#else
    if (!Embedis::get(key, value))
        value = String(defaultValue);
#endif
    return value;
}

String NvMemFlashDict::get(const String &key)
{
    return get(key, "");
}

bool NvMemFlashDict::set(const String &key, const String &value)
{
    _ensureStarted();
    
    if (value == "")
    {
        return remove(key);
    }
    else
    {
    #if defined(ARDUINO_ARCH_ESP32)
        return _preferences.putString(key.c_str(), String(value));
    #else
        return Embedis::set(key, String(value));
    #endif
    }
}

bool NvMemFlashDict::remove(const String &key)
{
    _ensureStarted();

#if defined(ARDUINO_ARCH_ESP32)
    return _preferences.remove(key.c_str());
#else
    return Embedis::del(key);
#endif
}

unsigned long NvMemFlashDict::freeEntries()
{
    _ensureStarted();
#if defined(ARDUINO_ARCH_ESP32)
    return _preferences.freeEntries();
#else
    unsigned pos = EEPROM_SIZE - 1;
    while (size_t len = EEPROM.read(pos)) {
        if (0xFF == len) break;
        pos = pos - len - 2;
    }
    return  pos;
#endif
}

bool NvMemFlashDict::contains(const String &key)
{
    return (get(key) != "");
}


void NvMemFlashDict::_ensureStarted()
{
    if(!_started)
    {
        open();
    }
}
