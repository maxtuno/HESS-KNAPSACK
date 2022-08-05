/*
MIT License

Copyright (c) 2012-2022 Oscar Riveros (https://twitter.com/maxtuno, Chile).

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

template<typename T>
T oracle(const std::vector<int> &sequence, const std::vector<T> &values, const std::vector<T> &weights, const T &capacity, const int &n) {
    T weight = 0;
    T value = 0;
    for (int i{0}; i < n; i++) {
        if (weight + weights[i] * sequence[i] > capacity) {
            return 0;
        }
        weight += weights[i] * sequence[i];
        value += values[i] * sequence[i];
    }
    return value;
}

void step(const int &i, const int &j, std::vector<int> &sequence) {
    if (i != j) {
        sequence[i] = 1 - sequence[i];
        sequence[j] = 1 - sequence[j];
    } else {
        sequence[i] = 1 - sequence[j];
    }
}

template<typename T>
std::vector<int> hess(const std::vector<T> &values, const std::vector<T> &weights, const T &capacity, const int &n) {
    std::vector<int> sequence(n, 0), optimal(sequence);
    T global = -1, local;
    for (;;) {
        bool done = true;
        for (int i{0}; i < n; i++) {
            for (int j{0}; j < n; j++) {
                step(i, j, sequence);
                local = oracle<T>(sequence, values, weights, capacity, n);
                if (local > global) {
                    global = local;
                    std::cout << local << std::endl;
                    optimal.assign(sequence.begin(), sequence.end());
                    done = false;
                } else if (local < global) {
                    step(i, j, sequence);
                }
            }
        }
        if (done) {
            break;
        }
    }
    return optimal;
}

int main(int argc, char *argv[]) {

    using T = long long;

    int n, idx;
    T capacity;
    std::vector<T> values, weights;

    std::ifstream instance(argv[1]);
    instance >> n;
    values.resize(n);
    weights.resize(n);
    for (int i{0}; i < n; i++) {
        instance >> idx;
        instance >> values[idx];
        instance >> weights[idx];
    }
    instance >> capacity;
    instance.close();

    auto sequence = hess(values, weights, capacity, n);

    std::cout << "\n" << std::string(20, '-') << "\n" << std::endl;

    std::cout << oracle(sequence, values, weights, capacity, n) << std::endl;
    for (int i{0}; i < n; i++) {
        if (sequence[i] == 1) {
            std::cout << values[i] << " " << weights[i] << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
