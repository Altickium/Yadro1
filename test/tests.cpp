#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <random>
#include <chrono>
#include <thread>

#include "../src/tape.hpp"
#include "../src/sorter.hpp"
#include "../src/config.hpp"
#include "simple_test.h"

std::random_device dev;
std::mt19937 rng(dev());

void make_file(std::string output, std::vector<int32_t> vec) {
    std::ofstream out("/tmp/input", std::ios::binary | std::ios::out);

	for (auto& u : vec) {
        out << u << ' ';
	}
    out.close();
}

void sort_with_tape(std::string input, std::string output, size_t M) {
    tape_sorting::Sorter sorter(std::make_shared<tape_sorting::Tape>(input), 
                                std::make_shared<tape_sorting::Tape>(output, tape_sorting::config(0, 0, 1000 * 1000 * 1000 + 7)), 
                                tape_sorting::config(0, 0, M));
    sorter.sort();
}

void check_sorted(size_t n, std::vector<int32_t>& custom_vector, size_t M) {
	if (custom_vector.size() == 0 && n != 0) {
		for (size_t i = 0; i<n; ++i) {
			std::uniform_int_distribution<std::mt19937::result_type> dist6(1, n);
            int32_t val =  dist6(rng);
            custom_vector.push_back(val);
		}
	}

	make_file("input", custom_vector);
	std::sort(custom_vector.begin(), custom_vector.end());
	sort_with_tape("/tmp/input", "/tmp/output", M);

    std::ifstream if2("/tmp/output", std::ios::binary);
    for (size_t i = 0; i<n; ++i) {
        int32_t val;
        if2 >> val;
        EXPECT_EQ(val, custom_vector[i]);
    }

    std::remove("/tmp/input");
    std::remove("/tmp/output");
    custom_vector.clear();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TEST(tape_sorter, general) {
    std::vector<int32_t> emp(0);

    check_sorted(1, emp, 1);

    check_sorted(10, emp, 2);
    check_sorted(10, emp, 5);
    check_sorted(10, emp, 10);
    check_sorted(10, emp, 31);

    check_sorted(100, emp, 2);
    check_sorted(100, emp, 20);
    check_sorted(100, emp, 31);
    check_sorted(100, emp, 100);

    check_sorted(1000, emp, 100);
}	

TEST(tape_sorter, missing_file) {
    size_t n = 10ul, M = 5ul;
    std::vector<int32_t> custom_vector(0);
    if (custom_vector.size() == 0 && n != 0) {
		for (size_t i = 0; i<n; ++i) {
			std::uniform_int_distribution<std::mt19937::result_type> dist6(1, n);
            int32_t val =  dist6(rng);
            custom_vector.push_back(val);
		}
	}

    try {
        make_file("input1", custom_vector);
        std::sort(custom_vector.begin(), custom_vector.end());
        sort_with_tape("input2", "output", M);
    } catch (const std::exception& e) {
        std::string msge = e.what(), msgo = "File not found or empty";
        ASSERT_EQ(msge, msgo);
    }
}

TEST(tape_sorter, empty) {
    std::vector<int32_t> emp(0);
    try {
        check_sorted(0, emp, 0);
    } catch (const std::exception& e) {
        std::string msge = e.what(), msgo = "File not found or empty";
        ASSERT_EQ(msge, msgo);
    }
    
    try {
        check_sorted(0, emp, 5);
    } catch (const std::exception& e) {
        std::string msge = e.what(), msgo = "File not found or empty";
        ASSERT_EQ(msge, msgo);
    }
}

TEST(tape_sorter, delay) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int32_t> emp(0);
    check_sorted(100, emp, 20);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    ASSERT_LE(duration.count(), 100 * 50); // n * (n - 1) / 2
}

TESTING_MAIN()