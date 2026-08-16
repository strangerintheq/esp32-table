#ifndef BUFFER_JSON_BUILDER_H
#define BUFFER_JSON_BUILDER_H

#include <Arduino.h>

template <size_t BufferSize>
class JsonBuilder {
private:
    char buffer[BufferSize];
    size_t offset;
    bool hasElements;

    void appendComma() {
        if (hasElements && offset < BufferSize - 1) {
            buffer[offset++] = ',';
        }
    }

public:
    JsonBuilder() {
        buffer[0] = '{';
        offset = 1;
        hasElements = false;
    }

    void inc(int written){
        if (written > 0) {
            offset += written;
            hasElements = true;
        }
    }

    void append(const char* key, const char* value) {
        appendComma();
        // Format as "key":"value" safely inside the remaining space
        inc(snprintf(buffer + offset, BufferSize - offset, "\"%s\":\"%s\"", key, value));
    }

    void append(const char* key, String value) {
        append(key, value.c_str());
    }

    void append(const char* key, long value) {
        appendComma();
        // FIXED: Used "%ld" format specifier instead of "%d" for long integers
         inc(snprintf(buffer + offset, BufferSize - offset, "\"%s\":%ld", key, value));
    }

    void append(const char* key, unsigned long value) {
        appendComma();
        // Use "%lu" specifier for unsigned long values like millis() or heap sizes
        inc(snprintf(buffer + offset, BufferSize - offset, "\"%s\":%lu", key, value));
    
    }

    void append(const char* key, int value) {
        appendComma();
        // FIXED: Used dedicated "%d" for standard int to prevent recursive call loops
        inc(snprintf(buffer + offset, BufferSize - offset, "\"%s\":%d", key, value));
    }

    void append(const char* key, unsigned int value) {
        appendComma();
        inc(snprintf(buffer + offset, BufferSize - offset, "\"%s\":%u", key, value));

    }

    void append(const char* key, float value, int decimals = 2) {
        appendComma();
        // Dynamically create float format specifier like "%.2f"
        char floatFormat[10];
        snprintf(floatFormat, sizeof(floatFormat), "\"%%s\":%%.%df", decimals);
        inc(snprintf(buffer + offset, BufferSize - offset, floatFormat, key, value));
    }

    const char* c_str() {
        if (offset < BufferSize - 1) {
            buffer[offset] = '}';
            buffer[offset + 1] = '\0';
        }
        return buffer;
    }
};

#endif // BUFFER_JSON_BUILDER_H
