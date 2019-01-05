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

#include <boost/test/included/unit_test.hpp>
using namespace boost::unit_test;

#include "../sort_and_replace.h"

void testInvalidInstantiation()
{
    auto input = std::make_shared<std::vector<double>>(std::vector<double>({}));
    // case: input is empty
    BOOST_CHECK_THROW(sliding_window_collection::SortAndReplace s(input, 1), std::invalid_argument);

    input = std::make_shared<std::vector<double>>(std::vector<double>({1.0, 2.0, 3.0, 4.0}));

    // case: window size is smaller than 1
    BOOST_CHECK_THROW(sliding_window_collection::SortAndReplace s(input, 0), std::invalid_argument);

    // case: window size exceeds input size
    BOOST_CHECK_THROW(sliding_window_collection::SortAndReplace s(input, 5), std::invalid_argument);
}

void testWindowEqualsInputSize()
{
    auto input = std::make_shared<std::vector<double>>(std::vector<double>({1.0, 2.0, 3.0, 4.0}));
    sliding_window_collection::SortAndReplace s(input, 4);
    {
        const auto result = s.get();
        BOOST_TEST(result == std::vector<double>({1.0, 2.0, 3.0, 4.0}));
    }
    {
        BOOST_TEST(s.last());
        s.next(); // should have no effect
        const auto result = s.get(); // result should not change
        BOOST_TEST(result == std::vector<double>({1.0, 2.0, 3.0, 4.0}));
   }
}

void testStep()
{
    auto input = std::make_shared<std::vector<double>>(std::vector<double>({1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0}));
    sliding_window_collection::SortAndReplace s(input, 5);
    {
        const auto result = s.get();
        BOOST_TEST(result == std::vector<double>({1.0, 2.0, 3.0, 4.0, 5.0}));
    }
    {
        s.next();
        const auto result = s.get();
        BOOST_TEST(result == std::vector<double>({2.0, 3.0, 4.0, 5.0, 6.0}));
    }
    {
        s.next();
        const auto result = s.get();
        BOOST_TEST(result == std::vector<double>({3.0, 4.0, 5.0, 6.0, 7.0}));
    }
}

void testIteration()
{
    {
        auto input = std::make_shared<std::vector<double>>(std::vector<double>({3.1, 7.2, 1.9, 4.5, 5.1, 18.1, 0.9}));
        sliding_window_collection::SortAndReplace s(input, 3);

        auto result = s.get();
        for (;!s.last();)
        {
            s.next();
            result = s.get();
        }
        BOOST_TEST(result == std::vector<double>({0.9, 5.1, 18.1}));
    }
    {
        auto input = std::make_shared<std::vector<double>>(std::vector<double>({3.1}));
        sliding_window_collection::SortAndReplace s(input, 1);
        auto result = s.get();
        for (;!s.last();)
        {
            s.next();
            result = s.get();
        }
        BOOST_TEST(result == std::vector<double>({3.1}));
    }
}

test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] )
{
    framework::master_test_suite().add(BOOST_TEST_CASE(&testInvalidInstantiation));
    framework::master_test_suite().add(BOOST_TEST_CASE(&testWindowEqualsInputSize));
    framework::master_test_suite().add(BOOST_TEST_CASE(&testStep));
    framework::master_test_suite().add(BOOST_TEST_CASE(&testIteration));
    return 0;
}
