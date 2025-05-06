#include "tape.hpp"
#include "tape.hpp"

#include "sorter.hpp"
#include "config.hpp"

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <memory>

void process_file(std::string output, std::vector<int32_t>& vec, bool mode = false) {
    std::fstream val_stream(std::filesystem::current_path().remove_filename() / std::filesystem::path(output), 
                            std::ios::binary | std::ios::out | std::ios::in);

    if (!val_stream) {
        val_stream.open(std::filesystem::current_path().remove_filename() / std::filesystem::path(output), 
                        std::ios::binary | std::ios::out | std::ios::in | std::ios::trunc);
    }

    int32_t val;
    if (!mode) {
        for (auto& u : vec) {
            if (!mode) {
                val_stream << u << ' ';
            }
        }
    } else while(val_stream >> val) {
        vec.push_back(val);
    }
    val_stream.close();
}

void generate_default() {
    std::cout << "USING DEFAULT PARAMETERS:\n"
          << "N: 10\n"
          << "ARRAY NUMBERS ARE: 10 9 8 7 6 5 4 3 2 1 \n";
    std::vector<int> data_vec{10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    process_file("input", data_vec);
    std::vector<int> config_vec{5, 1, 1};
    process_file("config", config_vec);
    std::cout << "M: 5, readDelay: 1, writeDelay: 1\n";
}

int main(int argc, char* argv[]) {
    std::string inputFile = "input";
    std::string outputFile = "output";
    std::string configFile = "config";

    if (argc == 4) {
        inputFile = argv[1];
        outputFile = argv[2];
        configFile = argv[3];
    } else if (argc == 1) {
        generate_default();
    } else {
        return 1;
    }

    std::vector<int> config_vec;
    process_file(configFile, config_vec, true);
    if (config_vec.size() == 3) {
        for (auto val : config_vec) {
            if (val < 0) {
                throw std::invalid_argument("Wrong config arguments");
            }
        }
    } else {
        throw std::invalid_argument("Invalid number of arguments");
    }
    
    std::string o_path = std::filesystem::current_path().remove_filename() / std::filesystem::path(outputFile);
    std::string i_path = std::filesystem::current_path().remove_filename() / std::filesystem::path(inputFile);
    tape_sorting::Sorter sorter(std::make_shared<tape_sorting::Tape>(i_path), 
                                std::make_shared<tape_sorting::Tape>(o_path, tape_sorting::config(0, 0, 1000 * 1000 * 1000 + 7)), 
                                tape_sorting::config(config_vec[1], config_vec[2], config_vec[0]));
    sorter.sort();

    std::cout << "AFTER SORTING: \n";
    std::vector<int> sorted_vec;
    process_file(outputFile, sorted_vec, true);
    for (auto val : sorted_vec) {
        std::cout << val << ' ';
    }

    //заккоментите, если не нужно удалять файл output
    std::remove(o_path.c_str());
    return 0;
}