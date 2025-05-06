#include "tape.hpp"

#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <optional>

namespace tape_sorting
{

Tape::Tape(const std::string& file_path)
    : _file(file_path), _current_pos(0) {
    if (!_file) {
        _file.open(file_path, std::fstream::in | std::fstream::out | std::fstream::trunc);    
    }
    _config = config(0, 0, std::filesystem::file_size(file_path) / 2);
}

Tape::Tape(const std::string& file_path, const config& cfg)
    : _file(file_path), _current_pos(0), _config(cfg) {
    if (!_file) {
        _file.open(file_path, std::fstream::in | std::fstream::out | std::fstream::trunc);
    }
}

std::optional<int> Tape::read() {
    int32_t val;
    std::this_thread::sleep_for(std::chrono::milliseconds(_config.get_read_delay()));
    if (!(_file >> val)) {
        return std::nullopt;
    }      
    return std::optional{val};
}

void Tape::write(int val) {
    if (!(_file << val << " ")) {
        // error
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(_config.get_write_delay()));
    _file.flush();
}

void Tape::move_next() {
    if (_current_pos < _config.get_buf_size()) {
        ++_current_pos;
    }
}

void Tape::move_prev() {
    if (_current_pos > 0) {
        --_current_pos;
    }
}

bool Tape::is_end() const {
    return _current_pos == _config.get_buf_size() || _file.eof();
}

void Tape::reset() {
    _file.seekg(0);
    _current_pos = 0;
}  

tape_sorting::Tape::~Tape() = default;

} // namespace tape_sorting


