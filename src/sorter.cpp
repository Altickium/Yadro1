#include "sorter.hpp"
#include "tape.hpp"
#include "config.hpp"

#include <vector>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <set>
#include <optional>

tape_sorting::Sorter::Sorter(std::shared_ptr<TapeInterface> input_tape, std::shared_ptr<TapeInterface> output_tape, const config& cfg) 
: _input_tape(std::move(input_tape)), 
  _output_tape(std::move(output_tape)), 
  _config(cfg) {
    // something
}

void tape_sorting::Sorter::readTapes() {
    int tape_num = 0;
    std::shared_ptr<tape_sorting::TapeInterface> current_tape = std::make_shared<tape_sorting::Tape>(
        "/tmp/temp_tape_" + std::to_string(tape_num), _config
    );

    while(!_input_tape->is_end()) {
        if (current_tape->is_end()) {
            _temporal_tapes.push_back(current_tape);
            current_tape.reset(new tape_sorting::Tape("/tmp/temp_tape_" + std::to_string(++tape_num), _config));
        }
        std::optional<int> read_number = _input_tape->read();
        if (!read_number.has_value()) {
            break;
        }
        current_tape->write(read_number.value());
        current_tape->move_next();
        _input_tape->move_next();
    }
    _temporal_tapes.push_back(current_tape);
}

void tape_sorting::Sorter::sort() {
    readTapes();

    if (_temporal_tapes.size() == 0) {
        return;
    }

    initial_sort();

    std::set<std::pair<int32_t, size_t>> top_heap;
    for(size_t i = 0; i<_temporal_tapes.size(); ++i) {
        std::optional<int> read_number = _temporal_tapes[i]->read();
        if (!read_number.has_value()) {
            break;
        }
        top_heap.insert(std::make_pair(read_number.value(), i));
    }

    // std::cout << _temporal_tapes.size() << '\n';
    while(!top_heap.empty()) {
        auto [elem, id] = *top_heap.begin();
        // std::cout << "ELEM : " << elem << '\n';
        // std::cout << "ID : " << id << '\n';
        top_heap.erase(top_heap.begin());
        _output_tape->write(elem);
        _output_tape->move_next();

        std::optional<int> read_number = _temporal_tapes[id]->read();
        if (!read_number.has_value()) {
            continue;
        }
        _temporal_tapes[id]->move_next();
        if (!_temporal_tapes[id]->is_end()) {
            top_heap.insert(std::make_pair(read_number.value(), id));
        }
    }
}

void tape_sorting::Sorter::initial_sort() {
    for(int i = 0; i<_temporal_tapes.size(); ++i) {
        auto& current_tape = _temporal_tapes[i];
        current_tape->reset();
        std::vector<int32_t> sorting_tape;
        while(!current_tape->is_end()) {
            std::optional<int> read_number = current_tape->read();
            if (!read_number.has_value()) {
                break;
            }
            sorting_tape.push_back(read_number.value());
            current_tape->move_next();
        }
        std::sort(sorting_tape.begin(), sorting_tape.end());
        current_tape->reset();

        std::remove(("/tmp/temp_tape_" + std::to_string(i)).c_str());
        current_tape.reset(new tape_sorting::Tape("/tmp/temp_tape_" + std::to_string(i), _config));

        for (auto u : sorting_tape) {
            current_tape->write(u);
            if (!current_tape->is_end()) {
                current_tape->move_next();
            }
        }
        current_tape->reset();
    }
}
