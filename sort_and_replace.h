// The MIT License (MIT)

// Copyright (c) 2018-2019 Daniel Feist

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

#include <list>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>

namespace sliding_window_collection
{

class SortAndReplace
{
public:
    SortAndReplace(const std::shared_ptr<std::vector<double>> input, const typename std::vector<double>::size_type window)
    : data(std::move(input))
    , window(window)
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
        is_sorted = true;

        std::vector<double> result;
        result.reserve(window);
        std::transform(sorter.begin(), sorter.end(), std::back_inserter(result), [](const auto& e){return e.second;});
        return result;
    }
private:
    void step()
    {
        auto add = [this]()
        {
            for (auto it = data->begin(); it != std::next(data->begin(), window); ++it)
            {
                sorter.emplace_back(std::make_pair(std::distance(data->begin(), it), *it));
            }
            sorter.sort([](const auto& lhs, const auto& rhs){return lhs.second <= rhs.second;});
        };
        auto replace = [this]()
        {
            const auto position {std::distance(data->begin(), std::prev(index))};
            auto to_remove = std::find_if(sorter.begin(), sorter.end(), [position](auto e){return e.first == position;});
            if (to_remove == sorter.end())
            {
                throw std::runtime_error("Unexpected error.");
            }
            sorter.erase(to_remove);

            auto new_element {std::next(std::prev(index), window)};
            auto to_insert {std::make_pair(std::distance(data->begin(), new_element), *new_element)};
            auto first_greater = std::find_if(sorter.begin(), sorter.end(), [to_insert](auto e){return e.second >= to_insert.second;});
            sorter.emplace(first_greater, to_insert);
        };

        if (index == data->begin())
        {
            add();
            return;
        }

        replace();
    }

    std::shared_ptr<std::vector<double>> data = nullptr;
    const std::vector<double>::size_type window;
    std::list<std::pair<int, double>> sorter;
    std::vector<double>::iterator index;
    bool is_sorted;
};

}
