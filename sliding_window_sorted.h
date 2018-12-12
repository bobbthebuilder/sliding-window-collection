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

#include <list>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>

namespace sliding_window_collection
{

std::ostream& operator<<(std::ostream& os, const std::list<std::pair<std::ptrdiff_t, double>>& sorter)
{
    for (const auto& i : sorter)
    {
        os << "(" << i.first << "," << i.second << ") ";
    }
    os << "\n";
    return os;
}

class Sorter
{
    using SortedWindow = std::list<std::pair<typename std::vector<double>::difference_type, double>>;
public:
    Sorter (std::shared_ptr<std::vector<double>> input, const typename std::vector<double>::difference_type window)
    : input(std::move(input))
    , window(window)
    {
    }
    bool hasNext() const
    {
        return std::next(inputIt, window) != input->end();
    }
    void next()
    {
        if (hasNext())
        {
            ++inputIt;
        }
    }
    const SortedWindow& get()
    {
        step();
        return sorter;
    }
private:

    void step()
    {
        if (sorter.empty())
        {
            for (auto it = input->begin(); it != std::next(input->begin(), window); ++it)
            {
                sorter.emplace_back(std::make_pair(std::distance(input->begin(), it), *it));
            }
            sorter.sort([](const auto& lhs, const auto& rhs){return lhs.second <= rhs.second;});
            std::cout << sorter;
            inputIt = input->begin();
        }

        const auto position {std::distance(input->begin(), inputIt)};
        auto to_remove = std::find_if(sorter.begin(), sorter.end(), [position](auto e){return e.first == position;});
        if (to_remove == sorter.end())
        {
            return;
        }
        std::cout << "erase element=" << (*to_remove).first << ", " << (*to_remove).second << " | ";
        sorter.erase(to_remove);

        auto new_element {std::next(inputIt, window)};
        std::cout << "new element=" << *new_element << " | ";
        auto to_insert {std::make_pair(std::distance(input->begin(), new_element), *new_element)};
        //std::cout << "insert element=" << to_insert.second << " | ";
        auto first_greater = std::find_if(sorter.begin(), sorter.end(), [to_insert](auto e){return e.second >= to_insert.second;});
        sorter.emplace(first_greater, to_insert);
        std::cout << sorter;
    }

    std::shared_ptr<std::vector<double>> input;
    std::vector<double>::iterator inputIt;
    SortedWindow sorter;
    const std::vector<double>::difference_type window;
};

}
