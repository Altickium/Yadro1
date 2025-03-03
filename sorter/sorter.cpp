#include "sorter.hpp"
#include "../tape/tape.hpp"

#include <vector>
#include <algorithm>
#include <filesystem>
#include <iostream>

void Sorter::readTapes() {
    std::filesystem::path p = inputPath;
    std::filesystem::path fullPathI = std::filesystem::current_path().remove_filename() / p;

    std::ifstream in(fullPathI, std::ios::binary);

    if (!in.is_open() || std::filesystem::file_size(fullPathI) == 0) {
      throw std::runtime_error("File not found or empty");
    }

    size_t lenNum = (std::filesystem::file_size(fullPathI) / 4ul + tapeSize - 1) / tapeSize;\
    numberOfTapes = lenNum;

    for (size_t i = 0; i < numberOfTapes; ++i) {
        Tape newTape("temp_tape_" + std::to_string(i), tapeSize);
        for (size_t j = 0; j < tapeSize; ++j) {
            int32_t val;
            in.read(reinterpret_cast<char*>(&val), sizeof(int32_t));
            if (!in.good()) {
                break;
            }
            newTape.write(val);
            newTape.moveNext();
        }
        newTape.writeFile();
    }
}

void Sorter::sort() {
    readTapes();

    if (numberOfTapes == 0) {
        throw std::invalid_argument("Zero tapes cannot be sorted");
    }

    bool sortedCheck = true;
    while(sortedCheck) {
        sortedCheck = false;
        if (numberOfTapes > 1) {
            for (size_t i = 0; i<numberOfTapes - 1; ++i) {
                sortedCheck |= sortTwoTapes(i);
            }
        } else if (numberOfTapes == 1) {
            std::string pathI = _TAPES_PATH + std::to_string(0);
            Tape tapeI(pathI, tapeSize);
            tapeI.readFile();
            sortedCheck = sortTape(tapeI);
            tapeI.writeFile();
        } else {
            throw std::invalid_argument("Zero tapes cannot be sorted");
        }
    };
    
    writeResult();
}

bool Sorter::sortTwoTapes(size_t i) {
    bool checker = false;
    std::string pathI = _TAPES_PATH + std::to_string(i), pathJ = _TAPES_PATH + std::to_string(i + 1);
    Tape tapeI(pathI, tapeSize), tapeJ(pathJ, tapeSize);
    tapeI.readFile();
    checker = sortTape(tapeI);
    tapeI.movePrev();
    tapeJ.readFile();
    if (tapeI.read() > tapeJ.read()) {
        tapeI.swap(tapeJ);
        checker |= true;
    }
    tapeI.writeFile();
    checker |= sortTape(tapeJ);
    tapeJ.writeFile();
    return checker;
}

bool Sorter::sortTape(TapeInterface& tape) {
    int32_t aux;
    bool checker = false;
    while (!tape.isEnd()) {
        if (tape.getPosition() == 0) {
            aux = tape.read();
        } else if (aux > tape.read()) {
            tape.swap(aux);
            tape.movePrev();
            tape.swap(aux);
            tape.moveNext();
            checker = true;
        } else {
            aux = tape.read();
        }
        tape.moveNext();
    }
    return checker;
}

void Sorter::writeResult() {
    std::filesystem::path p = outputPath;
    std::filesystem::path fullPathO = std::filesystem::current_path().remove_filename() / p;

    std::ofstream out(fullPathO, std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Cannot open file: " + outputPath);
    }


    for (size_t i = 0; i<numberOfTapes; ++i) {
        std::string path = _TAPES_PATH + std::to_string(i);
        Tape tape(path, tapeSize);
        tape.readFile();
        while (!tape.isEnd()) {
            int32_t o = tape.read();
            out.write(reinterpret_cast<char*>(&o), sizeof(int32_t));
            tape.moveNext();
        }
    }
}