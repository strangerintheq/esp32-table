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
        if (hasElements && offset < BufferSize - 2) {
            buffer[offset++] = ',';
        }
    }

    void inc(int written) {
        if (written > 0) {
            size_t nextOffset = offset + written;
            if (nextOffset < BufferSize - 1) { 
                offset = nextOffset;
                hasElements = true;
            } else {
                buffer[offset] = '\0'; 
            }
        }
    }

public:
    JsonBuilder() {
        clear();
    }

    // Полная очистка буфера
    void clear() {
        buffer[0] = '{';
        buffer[1] = '\0';
        offset = 1;
        hasElements = false;
    }

    void append(const char* key, const char* value) {
        appendComma();
        inc(snprintf(buffer + offset, BufferSize - offset, "\"%s\":\"%s\"", key, value));
    }

    void append(const char* key, const String& value) {
        append(key, value.c_str());
    }

    void append(const char* key, long value) {
        appendComma();
        inc(snprintf(buffer + offset, BufferSize - offset, "\"%s\":%ld", key, value));
    }

    void append(const char* key, unsigned long value) {
        appendComma();
        inc(snprintf(buffer + offset, BufferSize - offset, "\"%s\":%lu", key, value));
    }

    void append(const char* key, int value) {
        appendComma();
        inc(snprintf(buffer + offset, BufferSize - offset, "\"%s\":%d", key, value));
    }

    void append(const char* key, unsigned int value) {
        appendComma();
        inc(snprintf(buffer + offset, BufferSize - offset, "\"%s\":%u", key, value));
    }

    void append(const char* key, float value, int decimals = 2) {
        appendComma();
        char floatFormat[16];
        snprintf(floatFormat, sizeof(floatFormat), "\"%%s\":%%.%df", decimals);
        inc(snprintf(buffer + offset, BufferSize - offset, floatFormat, key, value));
    }

    const char* c_str() {
        if (offset < BufferSize - 1) {
            buffer[offset] = '}';
            buffer[offset + 1] = '\0';
        } else if (BufferSize > 0) {
            buffer[BufferSize - 1] = '\0';
        }
        return buffer;
    }
};

#endif // BUFFER_JSON_BUILDER_H
