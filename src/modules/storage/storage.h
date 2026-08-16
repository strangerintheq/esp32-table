#pragma once

bool initStorage();

String readString(String fileName);

void writeString(String filaName, String content);

void listDirectory(
    String dirPath, 
    std::function<void(const String&)> fn
);

size_t readBinaryFile(
    String fileName, 
    size_t stride, 
    std::function<void(const uint8_t*)> fn
);

bool startWrite(String fileName);

void writeBytes(
    String fileName, 
    uint8_t *data, 
    size_t len
);

void endWrite(String fileName);

size_t fileSize(String fileName);

void startReadBinary(String fileName);

size_t readBinary(uint8_t* buffer, size_t byteCount);

void endReadBinary();
