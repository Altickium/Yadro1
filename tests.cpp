#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <random>
#include <chrono>

#include "tape/tape.hpp"
#include "sorter/sorter.hpp"
#include "simple_test.h"


std::random_device dev;
std::mt19937 rng(dev());

void make_file(std::string output, std::vector<int32_t> vec) {

    std::filesystem::path p = output;
    std::filesystem::path mainFile = std::filesystem::current_path().remove_filename() / p;

    std::ofstream out(mainFile, std::ios::binary);

	for (auto& u : vec) {
        out.write(reinterpret_cast<char*>(&u), sizeof(int32_t));
	}
    out.close();
}

void sort_with_tape(std::string input, std::string output, size_t M) {
    Sorter sorter(input, output, M);
    sorter.sort();
}

void check_sorted(size_t n, std::vector<int32_t>& customVector, size_t M) {
	if (customVector.size() == 0 && n != 0) {
		for (size_t i = 0; i<n; ++i) {
			std::uniform_int_distribution<std::mt19937::result_type> dist6(1, n);
            int32_t val =  dist6(rng);
            customVector.push_back(val);
		}
	}

	make_file("input", customVector);
	std::sort(customVector.begin(), customVector.end());
	sort_with_tape("input", "output", M);

    std::filesystem::path p = "output";
    std::filesystem::path fullPathO = std::filesystem::current_path().remove_filename() / p;
    std::ifstream if2(fullPathO, std::ios::binary);
    for (size_t i = 0; i<n; ++i) {
        int32_t val;
        if2.read(reinterpret_cast<char*>(&val), sizeof(int32_t));
        EXPECT_EQ(val, customVector[i]);
    }

    customVector.clear();
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
    std::vector<int32_t> customVector(0);
    if (customVector.size() == 0 && n != 0) {
		for (size_t i = 0; i<n; ++i) {
			std::uniform_int_distribution<std::mt19937::result_type> dist6(1, n);
            int32_t val =  dist6(rng);
            customVector.push_back(val);
		}
	}

    try {
        make_file("input1", customVector);
        std::sort(customVector.begin(), customVector.end());
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