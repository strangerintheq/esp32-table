#include <Arduino.h>
#include <LittleFS.h>
#include <functional>

bool initStorage() {
    bool initialized = LittleFS.begin(true);
    if (initialized) {
        Serial.println("LittleFS mounted");
    } else {
        Serial.println("LittleFS error");
    }
    return initialized;
}

String ensure(String x) {
    return x.startsWith("/") ? x : "/" + x;
}

void listDirectory(
    String dirPath, 
    std::function<void(const String& fileName)> fn
) {
    dirPath = ensure(dirPath);
    File root = LittleFS.open(dirPath, "r");
    if (!root) {
        Serial.println("listDirectory: dir not found!");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println("listDirectory: dir not found!");
        root.close();
        return;
    }
    File file = root.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            fn(file.name());
        }
        file = root.openNextFile();
    }
    root.close(); 
}


String readString(String fileName) {
    fileName = ensure(fileName);
    File file = LittleFS.open(fileName, "r");
    if (!file) {
        Serial.println("Не удалось открыть файл: " + fileName);
        return "";
    }

    size_t size = file.size();
    if (size == 0) {
        file.close();
        return ""; // Файл физически пустой
    }

    // Создаем динамический буфер под размер файла + 1 байт для терминатора строки \0
    char* buffer = new char[size + 1];

    // Читаем ровно столько байт, сколько весит файл
    file.readBytes(buffer, size);
    buffer[size] = '\0'; // Закрываем строку null-терминатором

    file.close();

    // Переводим буфер в объект String
    String result = String(buffer);

    // Освобождаем память буфера
    delete[] buffer;

    // Очищаем невидимые пробелы и переносы строк с краев
    result.trim(); 
  
    return result;
}

void writeString(String fileName, String newValue) {
    newValue.trim();
    File file = LittleFS.open(fileName, "w");
    if (!file) {
        Serial.println("Wrror writing parameter: " + fileName);
        return;
    }

    file.print(newValue);
    file.close();
}

size_t readBinaryFile(
    String fileName, 
    size_t stride, 
    std::function<void(const uint8_t*)> fn
) {
  fileName = ensure(fileName);  
  File file = LittleFS.open(fileName, "r");
  size_t size = 0;
  if (file) {
      size = file.size() ;
      Serial.println(fileName + " size: " + size);
      uint8_t* buffer = new uint8_t[stride];
      for (size_t i = 0; i < size / stride; i++) {
          file.readBytes(reinterpret_cast<char*>(buffer), stride);
          fn(buffer);
      }
      delete[] buffer; 
      file.close();
  }
  return size;
}