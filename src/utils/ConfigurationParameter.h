#ifndef CONFIGURATION_PARAMETER_H
#define CONFIGURATION_PARAMETER_H

#include <Arduino.h>
#include <LittleFS.h>

class ConfigurationParameter {
    
private:
    String _fileName;
    String _value;
    String _prefix = "/configuration/";

public:
    ConfigurationParameter(String name) {
        _fileName = _prefix + name;
        read(); 
    }

    String read() {
        if (!LittleFS.exists(_fileName)) {
            _value = ""; // Если файла нет, параметр пустой
            return _value;
        }

        File file = LittleFS.open(_fileName, "r");
        if (!file) {
            _value = "";
            return _value;
        }

        size_t size = file.size();
        if (size == 0) {
            file.close();
            _value = "";
            Serial.println("ConfigurationParameter: " + _fileName + " IS MISSING");
            return _value;
        }

        // Безопасное побайтовое чтение без таймаутов readString()
        char* buffer = new char[size + 1];
        file.readBytes(buffer, size);
        buffer[size] = '\0';
        file.close();

        _value = String(buffer);
        delete[] buffer;

        _value.trim(); // Удаляем невидимые пробелы и переносы строк
        Serial.println("ConfigurationParameter: " + _fileName + ": \"" + _value + "\"");
        return _value;
    }

    // Метод записи нового значения в файл LittleFS
    bool write(String newValue) {
        newValue.trim();
        File file = LittleFS.open(_fileName, "w");
        if (!file) {
            Serial.println("Wrror writing parameter: " + _fileName);
            return false;
        }

        file.print(newValue);
        file.close();
        
        _value = newValue; // Обновляем значение в оперативной памяти
        return true;
    }

    // Метод для получения значения в виде стандартной строки C++
    String get() const {
        return _value;
    }
};

#endif // CONFIGURATION_PARAMETER_H
