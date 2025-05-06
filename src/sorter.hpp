#ifndef SORTER_HPP
#define SORTER_HPP

#include "tape_interface.hpp"
#include "config.hpp"

#include <string>
#include <memory>
#include <vector>

namespace tape_sorting {
    class Sorter {
    private:
        std::shared_ptr<TapeInterface> _input_tape;
        std::shared_ptr<TapeInterface> _output_tape;
        std::vector<std::shared_ptr<TapeInterface>> _temporal_tapes;
        config _config;

        void initial_sort();
        void writeResult();
        void readTapes();
    public:
        Sorter() = default;

        explicit Sorter(std::shared_ptr<TapeInterface>, std::shared_ptr<TapeInterface>, const config&);

        Sorter(const Sorter& other) = default;

        ~Sorter() {
            for (int i = 0; i<_temporal_tapes.size(); ++i) {
                std::remove(("/tmp/temp_tape_" + std::to_string(i)).c_str());
            }
        }

        void sort();
    };
} // namepace tape_sorting

#endif