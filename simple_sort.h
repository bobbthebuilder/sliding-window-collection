// The MIT License (MIT)

// Copyright (c) 2018 Daniel Feist

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

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>

namespace sliding_window_collection
{

class SimpleSort
{
public:
    SimpleSort(const std::shared_ptr<std::vector<double>> input, const typename std::vector<double>::size_type window)
    : data(std::move(input))
    , window(window)
    , sorter(window)
    , index(data->begin())
    , is_sorted(false)
    {
        if (input->empty())
        {
            throw std::invalid_argument("Input must be non-empty.");
        }
        if (window < 1)
        {
            throw std::invalid_argument("Window size must be greater than zero.");
        }
        if (data->size() < window)
        {
            throw std::invalid_argument("Window size exceeds input size.");
        }
    }
    bool last() const
    {
        return index >= std::prev(data->end(), window);
    }
    void next()
    {
        if (last())
        {
            return;
        }
        ++index;
        is_sorted = false;
    }
    std::vector<double> get()
    {
        if (!is_sorted)
        {
            step();
        }
        return sorter;
    }
private:
    void step()
    {
        std::copy_n(index, window, sorter.begin());
        std::sort(sorter.begin(), sorter.end(), std::less<double>());
        is_sorted = true;
    }
    std::shared_ptr<std::vector<double>> data;
    const std::vector<double>::size_type window;
    std::vector<double> sorter;
    std::vector<double>::iterator index;
    bool is_sorted;
};

}
