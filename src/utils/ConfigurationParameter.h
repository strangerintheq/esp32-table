#ifndef CONFIGURATION_PARAMETER_H
#define CONFIGURATION_PARAMETER_H

#include <Arduino.h>
#include <LittleFS.h>
#include <modules/storage/storage.h>

class ConfigurationParameter {
    
private:
    String _fileName;
    String _value;

public:
    ConfigurationParameter(String fileName) {
        _fileName = fileName;
        read(); 
    }

    String read() {
        _value = readString(_fileName);
        Serial.println("ConfigurationParameter: " + _fileName + ": \"" + _value + "\"");
        return _value;
    }

    bool write(String newValue) {
        writeString(_fileName, newValue);    
        _value = newValue; 
        return true;
    }

    String get() const {
        return _value;
    }
};

#endif // CONFIGURATION_PARAMETER_H
