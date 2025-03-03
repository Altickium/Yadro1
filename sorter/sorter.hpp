#ifndef SORTER_HPP
#define SORTER_HPP

#include "../tape/tape_interface.hpp"

#include <string>

class Sorter {
private:
    const std::string configFilePath;
    const std::string inputPath;
    const std::string outputPath;
    const size_t tapeSize;  
    const size_t _tapeReadDelay; 
    const size_t _tapeWriteDelay; 
    size_t numberOfTapes;

    inline static std::string _TAPES_PATH = "temp_tape_";

    bool sortTwoTapes(size_t i);
    bool sortTape(TapeInterface& tape);
    void writeResult();
    void readTapes();
public:
    Sorter() = default;

    explicit Sorter(const std::string& input, const std::string& output, const size_t M = 2, const size_t readDelay = 0, const size_t writeDelay = 0) 
        : inputPath(input), outputPath(output), tapeSize(((M + 1ul) / 2)), _tapeReadDelay(readDelay), _tapeWriteDelay(writeDelay), numberOfTapes(0) { }

    Sorter(const Sorter& other) = default;

    ~Sorter() = default;

    void sort();
};

#endif