#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <fstream>
#include <string>

namespace tape_sorting {
    class config {
    public:
        config() = default;

        config(int read_delay, int write_delay, int buf_size) : _read_delay(read_delay), _write_delay(write_delay), _buf_size(buf_size) { }

        int get_read_delay() const {
            return _read_delay;
        }

        int get_write_delay() const {
            return _write_delay;
        }

        int get_buf_size() const {
            return _buf_size;
        }

        void set_buf_size(int nbuf) {
            _buf_size = nbuf;
        }

    private:
        int _read_delay;
        int _write_delay;
        int _buf_size;
    };
}  // namespace tape_sorting

#endif