#ifndef TAPE_HPP
#define TAPE_HPP

#include "tape_interface.hpp"
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

class Tape : public TapeInterface {
private:
    std::string _filePath;
    size_t capacity;
    std::vector<int32_t> buffer;
    int readDelay;
    int writeDelay;
    size_t currentPosition;

public:
    size_t size;
    Tape() = default;

    explicit Tape(const std::string& _filePath, size_t capacity,  int readDelay = 0, int writeDelay = 0);

    Tape(const Tape& other) = default;

    ~Tape() override {
        buffer.clear();
    }

    // insert
    int& read() override;

    void write(int value) override;

    // file work

    void readFile();

    void writeFile();

    // move carette
    void moveNext() override;

    void movePrev() override;

    // other
    bool isEnd() const override;

    void reset() override;

    void reset(size_t i) override;

    size_t getPosition();

    void swap(int32_t&) override;

    void swap(Tape&);
};

#endif // TAPE_HPP