#include "NvMemJsonFileDict.h"

NvMemJsonFileDict::NvMemJsonFileDict(String path, bool auto_save)
    : _path(path), _auto_save(auto_save) {}

int NvMemJsonFileDict::open(bool force)
{

    _verifySpiffsStarted();

    int status;

    // Open the file
    bool path_exists = SPIFFS.exists(_path);
    if (!path_exists)
    {
        if (force)
        {
            _file = SPIFFS.open(_path, "w");
            String data = "{}";
            dataBuffer(data, data.length());
            status = 2;
        }
        else
        {
            Serial.println("Could not find file at path: " + _path);
            Serial.println("To force creation of new file,\
                            call dict.open(true)");
            status = -1;
            return status;
        }
    }
    else
    {
        _file = SPIFFS.open(_path, "r");
        if (!_file)
        {
            Serial.println("Could not open file at path: " + _path);
            status = -1;
            return status;
        }
        else
        {
            String data = _file.readString();
            if (data == "")
            {
                data = "{}";
            }
            Serial.println("Opening file: " + data +
                           " - Size: " + String(data.length()));
            dataBuffer(data, data.length());
            status = 1;
        }
    }
    // Deserialize
    DynamicJsonDocument doc(_calcDocSize(_size));
    DeserializationError derror = deserializeJson(doc, dataBuffer());
    JsonObject obj = doc.as<JsonObject>();

    _file.close();

    if (derror)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(derror.c_str());
        status = 0;
    }
    _status = status;

    return _status;
}

template <typename T>
String NvMemJsonFileDict::get(const String &key, T defaultValue)
{
    // Deserialize
    DynamicJsonDocument doc(_calcDocSize(_size));
    DeserializationError derror = deserializeJson(doc, dataBuffer());
    JsonObject obj = doc.as<JsonObject>();

    String result = obj[key] | String(defaultValue);

    return result;
}

String NvMemJsonFileDict::get(const String &key) { return get(key, ""); }

String NvMemJsonFileDict::init(const String &key, String defaultValue)
{
    if (!contains(key))
    {
        set(key, defaultValue);
    }
    return get(key);
}

bool NvMemJsonFileDict::set(const String &key, const String &value)
{
    if (value == "")
    {
        return remove(key);
    }

    // Deserialize
    size_t doc_size = _calcDocSize(_size);
    DynamicJsonDocument doc(doc_size);
    DeserializationError derror = deserializeJson(doc, dataBuffer());
    if (derror)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(derror.c_str());
        return false;
    }
    JsonObject obj = doc.as<JsonObject>();
    // Serial.println("Key: "+key + " - Value: "+ value);
    size_t size_1 = measureJson(doc) + 1;

    // Set value
    doc[key.c_str()] = value;

    // Serialize
    size_t size_2 = measureJson(doc) + 1;
    // Serial.println(String(doc_size) + " - " + String(size_1) + " - " +
    // String(size_2));
    char buffer[size_2];
    if (!serializeJson(doc, buffer, size_2))
    {
        Serial.print(F("serializeJson() failed: "));
        return false;
    }
    dataBuffer(String(buffer), size_2);

    // Auto-save check
    _autoSaveHelper();

    return true;
}

bool NvMemJsonFileDict::remove(const String &key)
{
    // Deserialize
    DynamicJsonDocument doc(_calcDocSize(_size));
    DeserializationError derror = deserializeJson(doc, dataBuffer());
    if (derror)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(derror.c_str());
        return false;
    }
    JsonObject obj = doc.as<JsonObject>();

    // Remove key
    obj.remove(key);

    // Serialize
    size_t size = measureJson(doc) + 1;
    char buffer[size];
    if (!serializeJson(doc, buffer, size))
    {
        Serial.print(F("serializeJson() failed: "));
        return false;
    }
    dataBuffer(String(buffer), size);

    // Auto-save check
    _autoSaveHelper();

    return true;
}

bool NvMemJsonFileDict::contains(const String &key)
{
    // Deserialize
    DynamicJsonDocument doc(_calcDocSize(_size));
    // Serial.println(dataBuffer());
    DeserializationError derror = deserializeJson(doc, dataBuffer());
    if (derror)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(derror.c_str());
        return false;
    }
    JsonObject obj = doc.as<JsonObject>();
    return obj.containsKey(key);
}

void NvMemJsonFileDict::reset()
{
    String data = "{}";
    dataBuffer(data, data.length());
    save();
}

bool NvMemJsonFileDict::save()
{
    SPIFFS.remove(_path);
    _file = SPIFFS.open(_path, "w");
    if (!_file)
    {
        Serial.println(F("Failed to create file"));
        return false;
    }
    // Write to file
    if (_file.print(dataBuffer()) == 0)
    {
        Serial.println(F("Failed to write to file"));
        return false;
    }
    // Close the file
    _file.close();
    return true;
}

size_t NvMemJsonFileDict::size() { return _size; }

String NvMemJsonFileDict::path() { return _path; }

void NvMemJsonFileDict::setPath(String path) { _path = path; }

bool NvMemJsonFileDict::autoSave() { return _auto_save; }

void NvMemJsonFileDict::setAutoSave(bool autoSave) { _auto_save = autoSave; }

void NvMemJsonFileDict::_autoSaveHelper()
{
    if (_auto_save)
        save();
}

String NvMemJsonFileDict::dataBuffer()
{
    std::string str(_dataBuffer.begin(), _dataBuffer.end());
    return String(str.c_str());
}

void NvMemJsonFileDict::dataBuffer(String data, size_t size)
{
    _dataBuffer.clear();
    for (int i = 0; i < size; i++)
        _dataBuffer.push_back(data[i]);
    _size = size;
}

void NvMemJsonFileDict::_verifySpiffsStarted()
{
    // Mount SPIFFS if not mounted
    while (!SPIFFS.begin())
    {
        Serial.println(F("Failed to initialize SPIFFS library"));
        delay(1000);
    }
}

size_t NvMemJsonFileDict::_calcDocSize(size_t data_len)
{
    return data_len + 300;
}