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
        Serial.printf("listDirectory: dir %s not found!\n", dirPath.c_str());
        return;
    }
    if (!root.isDirectory()) {
        Serial.printf("listDirectory: dir %s not found!\n", dirPath.c_str());
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
        Serial.printf("Can't open file for reading: %s\n" ,fileName.c_str());
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
        Serial.printf("Error writing to: %s\n" , fileName.c_str());
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

File uploadFile;

bool startWrite(String fileName){
    fileName = ensure(fileName);
    uploadFile = LittleFS.open(fileName, "w");
    if (!uploadFile) {
        Serial.printf("Cant open file %s for writing\n", fileName.c_str());
        return false;
    }
    return true;
}

void writeBytes(String fileName, uint8_t *data, size_t len) {
    if (uploadFile && len > 0) {
        uploadFile.write(data, len);
    }
}

void endWrite(String fileName){
    if (uploadFile) {
        uploadFile.close(); 
        Serial.println("File written: " + fileName);
    }
}

size_t fileSize(String fileName) {
    fileName = ensure(fileName);
    File file = LittleFS.open(fileName, "r");
    if (!file) {
        Serial.printf("Failed to open file for size check: %s\n", fileName.c_str());
        return 0;
    }
    size_t size = file.size();
    file.close();
    return size;
}


File currentReadBinaryFile;

void endReadBinary() {
    if (currentReadBinaryFile) {
        currentReadBinaryFile.close();
    }
}

void startReadBinary(String fileName) {
    endReadBinary();
    fileName = ensure(fileName);
    currentReadBinaryFile = LittleFS.open(fileName, "r");
    if (!currentReadBinaryFile) {
        Serial.printf("Failed to open file for read: %s\n", fileName.c_str());
    }
}

size_t readBinary(uint8_t* buffer, size_t byteCount) {
    if (!currentReadBinaryFile || !currentReadBinaryFile.available()) {
        return 0;
    }
    // Read directly into the provided destination buffer
    return currentReadBinaryFile.read(buffer, byteCount);
}

