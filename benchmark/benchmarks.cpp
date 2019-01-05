// The MIT License (MIT)

// Copyright (c) 2019 Daniel Feist

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <chrono>
#include <random>
#include <vector>
#include <memory>
#include <iostream>

#include "../simple_sort.h"
#include "../sort_and_replace.h"

void algorithms(const std::shared_ptr<std::vector<double>> input, const int window)
{
    ///
    auto start = std::chrono::high_resolution_clock::now();
    {
        sliding_window_collection::SimpleSort s(input, window) ;
        auto result = s.get();
        for (;!s.last();)
        {
            s.next();
            result = s.get();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapesedTime = end - start;
    std::cout << "Algorithm: SimpleSort     | Input size: " << input->size() << " | Window size: " << window << " | Duration (in sec): " << elapesedTime.count() << std::endl;

    ///
    start = std::chrono::high_resolution_clock::now();
    {
        sliding_window_collection::SortAndReplace s(input, window) ;
        auto result = s.get();
        for (;!s.last();)
        {
            s.next();
            result = s.get();
        }
    }
    end = std::chrono::high_resolution_clock::now();
    elapesedTime = end - start;
    std::cout << "Algorithm: SortAndReplace | Input size: " << input->size() << " | Window size: " << window << " | Duration (in sec): " << elapesedTime.count() << std::endl;
}

template <typename... Ts>
void benchmark(const Ts... inputs)
{
    int window1[sizeof...(Ts)] = { (algorithms(inputs, 3), 0)... };
    static_cast<void>(window1);

    int window2[sizeof...(Ts)] = { (algorithms(inputs, 8), 0)... };
    static_cast<void>(window2);

    int window3[sizeof...(Ts)] = { (algorithms(inputs, 100), 0)... };
    static_cast<void>(window3);

    int window4[sizeof...(Ts)] = { (algorithms(inputs, 800), 0)... };
    static_cast<void>(window4);
}

void run()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 100.0);
    auto input = std::make_shared<std::vector<double>>();
    input->reserve(1000000);
    for (std::size_t i = 0; i < 10000; ++i)
    {
        input->emplace_back(dis(gen));
    }
    benchmark(input);
    for (std::size_t i = 0; i < 90000; ++i)
    {
        input->emplace_back(dis(gen));
    }
    benchmark(input);
    for (std::size_t i = 0; i < 400000; ++i)
    {
        input->emplace_back(dis(gen));
    }
    benchmark(input);
    for (std::size_t i = 0; i < 500000; ++i)
    {
        input->emplace_back(dis(gen));
    }
    benchmark(input);
}

int main()
{
    run();
    return 0;
}
