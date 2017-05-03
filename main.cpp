#if 1

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#else

#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <cctype>
#include <chrono>
#include <queue>
#include <algorithm>
#include <cassert>
#include <random>
#include "FibHeap.hpp"


struct compareString_copy {
    bool operator() (const std::string &a, const std::string &b) {
        if (a.size() != b.size())
        return  (a.size() < b.size());
        std::string cpyA = a;
        std::string cpyB = b;
        std::transform(cpyA.begin(), cpyA.end(), cpyA.begin(), ::tolower);
        std::transform(cpyB.begin(), cpyB.end(), cpyB.begin(), ::tolower);
        return cpyA < cpyB;
    }
};

struct compareString_nocopy {
    bool operator() (std::string &a, std::string &b) {
        if (a.size() != b.size())
            return  (a.size() < b.size());
        std::transform(a.begin(), a.end(), a.begin(), ::tolower);
        std::transform(b.begin(), b.end(), b.begin(), ::tolower);
        return a < b;
    }
};

bool isNotAlpha(int ch) {
	bool b = !isalpha(ch);
	return !isalpha(ch);
}

bool modify(std::string &word) {
	word.erase(std::remove_if(word.begin(), word.end(), isNotAlpha), word.end());
	return !word.empty();
}


int readFile(const std::string fileName, std::vector<std::string> &C_W, std::vector<std::string> &N_W) {
	using namespace std;
	ifstream file;
	file.open(fileName);
	if (!file.is_open()) return 0;

    int word_count = 0;
	string word;
	while (file >> word)
	{
		if (modify(word)) {
            C_W.push_back(word);
            N_W.push_back(word);
            ++word_count;
        }
	}
    return word_count;
}

void FillNEmptyTest_str(const std::string fileName, int count) {
    using namespace std;
    vector<string> const_words, non_const_words;
    priority_queue<string, std::vector<string>, compareString_copy> c_pQueue;
    priority_queue<string, std::vector<string>, compareString_nocopy> n_pQueue;

    chrono::time_point<chrono::steady_clock> start, end;
    std::chrono::duration< double > average(0);

    int word_count = readFile(fileName, const_words, non_const_words);

    for (unsigned i = 0; i < count ; ++i) {
        start = chrono::steady_clock::now();
        for (const string &word : const_words) {
            n_pQueue.push(word);
        }
        for (const string &word : const_words) {
            n_pQueue.pop();
        }
        end = chrono::steady_clock::now();
        average += end - start;
    }

    cout << "Filling and emptying priority queue " << count << " times." << endl;
    cout << "Function: Sort by word length, then alphabetically (no copying)." << endl;
    cout << "Total number of words: " << word_count << endl;
    cout << "Total time: " << average.count() << " seconds." << endl;
    cout << "Average time: " << average.count()/10 << " seconds." << endl << endl;

    for (unsigned i = 0; i < count ; ++i) {
        start = chrono::steady_clock::now();
        for (string &word : non_const_words) {
            c_pQueue.push(word);
        }
        for (string &word : non_const_words) {
            c_pQueue.pop();
        }
        end = chrono::steady_clock::now();
        average += end - start;
    }

    cout << "Filling and emptying priority queue " << count << " times." << endl;
    cout << "Function: Sort by word length, then alphabetically (compare function copies strings)." << endl;
    cout << "Total number of words: " << word_count << endl;
    cout << "Total time: " << average.count() << " seconds." << endl;
    cout << "Average time: " << average.count()/10 << " seconds." << endl << endl;


}

void FillNEmptyTest_int(unsigned pushCount, unsigned repeatCount) {
    using namespace std;
    priority_queue<int> pQueue;
    //FibHeap<int> fibHeap;
    vector<int> vector;
    chrono::time_point<chrono::steady_clock> start, end;
    std::chrono::duration< double > total;
    random_device rd;
    mt19937 generator;
    generator.seed(rd());

    for (unsigned i = 0; i < pushCount; ++i) {
        vector.push_back(generator());
    }

    for (unsigned j = 0; j < repeatCount; ++j) {
        start = chrono::steady_clock::now();
        for (unsigned i = 0; i < pushCount; ++i) {
            pQueue.push(vector.at(i));
        }

        for (unsigned i = 0; i < pushCount; ++i) {
            pQueue.pop();
        }
        end = chrono::steady_clock::now();
        total = end - start;
    }
    std::cout << "Priority queue" << endl;
    std::cout << "Pushed and popped " << pushCount << " random values " << repeatCount << " times" << endl;
    std::cout << "Total time: " << total.count() << "s   Average time: " << total.count()/repeatCount << "s\n\n";
    total = std::chrono::duration<double>(0);
/*
    for (unsigned j = 0; j < repeatCount; ++j) {
        start = chrono::steady_clock::now();
        for (unsigned i = 0; i < pushCount; ++i) {
            FibHeap.insert(vector.at(i));
        }

        for (unsigned i = 0; i < pushCount; ++i) {
            FibHeap.extract_top();
        }
        end = chrono::steady_clock::now();
        total = end - start;
    }
    std::cout << "Fibonacci heap" << endl;
    std::cout << "Pushed and popped " << pushCount << " random values." << endl;
    std::cout << "Total time: " << total.count() << "s   Average time: " << total.count()/repeatCount << "s\n";
    */
}

void UserTest() {
    using namespace std;
    unsigned count;
    bool pop;
    bool exit = false;
    string cmd;

    priority_queue<int> pQueue;
    FibHeap<int> fibHeap;
    vector<int> vector;
    chrono::time_point<chrono::steady_clock> start, end;
    std::chrono::duration< double > total_fib, total_queue;
    random_device rd;
    mt19937 generator;
    generator.seed(rd());

    cout << "Commands:\n"
         << "push <number> - inserts <number> of random integers to priority queue and Fibonacci heap\n"
         << "pop <number> - extracts <number> of integers from priority queue and Fibonacci heap\n"
         << "exit - ends test and prints the results\n";

    while (true) {
        cin >> cmd;
        if (cmd == "push") {
            cin >> count;

            for (unsigned i = 0; i < count; ++i) {
                vector.push_back(generator());
            }

            start = chrono::steady_clock::now();
            for (unsigned i = 0; i < count; ++i) {
                pQueue.push(vector.at(i));
            }
            end = chrono::steady_clock::now();
            total_queue = end - start;

            /*
            start = chrono::steady_clock::now();
            for (unsigned i = 0; i < count; ++i) {
                fibHeap.insert(vector.at(i));
            }
            end = chrono::steady_clock::now();
            total_fib = end - start;
            */
        }
        if (cmd == "pop") {
            cin >> count;

            start = chrono::steady_clock::now();
            for (unsigned i = 0; i < count; ++i) {
                pQueue.pop();
            }
            end = chrono::steady_clock::now();
            total_queue = end - start;
            /*
            start = chrono::steady_clock::now();
            for (unsigned i = 0; i < count; ++i) {
                fibHeap.extract_top();
            }
            end = chrono::steady_clock::now();
            total_fib = end - start;
            */
        }
        if (cmd == "exit") {
            cout << "Fibonacci heap\n"
                 << "Total time: " << total_fib.count() << endl << endl
                 << "Priority queue\n"
                 << "Total time: " << total_queue.count() << endl;
            break;
        }
        cout << "DONE\n";
    }
}

int main() {
    //FillNEmptyTest_str("input.txt", 10);
    //FillNEmptyTest_int(200000, 10);
    UserTest();
}

//prepare for comparisons
//numeric types and strings from file

#endif