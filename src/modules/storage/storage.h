#pragma once

bool initStorage();

String readString(String);

void writeString(String, String);

void listDirectory(
    String dirPath, 
    std::function<void(const String&)> fn
);

size_t readBinaryFile(
    String fileName, 
    size_t stride, 
    std::function<void(const uint8_t*)> fn
);