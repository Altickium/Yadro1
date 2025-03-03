#include "tape.hpp"

#include <iostream>
#include <filesystem>

 Tape::Tape(const std::string& _filePath, size_t capacity,  int readDelay, int writeDelay)
    : _filePath("/tmp/" + _filePath), capacity(capacity), size(0), buffer(capacity, 0), readDelay(readDelay), writeDelay(writeDelay), currentPosition(0) {
    /*
    this->capacity = capacity;
    buffer.resize(capacity, 0);
    */
}

int& Tape::read() {
    if (currentPosition < size) {
        std::this_thread::sleep_for(std::chrono::milliseconds(readDelay));
        return buffer[currentPosition];
    }
    return buffer[currentPosition];
}

void Tape::write(int value) {
    if (currentPosition < capacity) {
        if (currentPosition == size) {
            size++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(writeDelay));
        buffer[currentPosition] = value;
    }
}

void Tape::readFile() {
    std::ifstream infile (_filePath, std::ios::binary);
    if (!infile.is_open()) {
        throw std::runtime_error("Cannot open file: " + _filePath);
    }

    buffer.resize(capacity);
    int32_t val;
    while(currentPosition < capacity) {
        infile.read(reinterpret_cast<char*>(&val), sizeof(int32_t));
        if (!infile.good()) {
            break;
        }
        write(val);
        moveNext();
    }
    while(currentPosition != 0) {
        movePrev();
    }
    infile.close();
}

void Tape::writeFile() {
    std::filesystem::remove(_filePath);
    std::ofstream outfile (_filePath, std::ios::binary);
    if (!outfile.is_open()) {
        throw std::runtime_error("Cannot open file: " + _filePath);
    }
    
    while(currentPosition != 0) {
        movePrev();
    }
    while(!isEnd()) {
        int32_t val = buffer[currentPosition];
        outfile.write(reinterpret_cast<char*>(&val), sizeof(int32_t));
        moveNext();
    }
    outfile.close();
}

void Tape::moveNext() {
    if (currentPosition < buffer.size()) {
        ++currentPosition;
    }
}

void Tape::movePrev() {
    if (currentPosition > 0) {
        --currentPosition;
    }
}

bool Tape::isEnd() const {
    return currentPosition >= size;
}

void Tape::reset() {
    currentPosition = 0;
}

void Tape::reset(size_t i) {
    while(currentPosition - i != 0) {
        if (currentPosition - i > 0) {
            movePrev();
        } else {
            moveNext();
        }
    }
}

size_t Tape::getPosition() {
    return currentPosition;
}

void Tape::swap(int32_t& elem) {
    std::swap(buffer[currentPosition], elem);
}

void Tape::swap(Tape& tape) {
    std::swap(read(), tape.read());
}

