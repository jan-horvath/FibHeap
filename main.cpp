#if 0

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
#include <limits>
#include "FibHeap.hpp"

const unsigned MY_MAX = 1000000000;

/**
 * Structure for coparison of strings
 * (Makes copies of string)
 */
struct compareString_copy {
  bool operator()(const std::string& a, const std::string& b) {
    if (a.size() != b.size())
      return (a.size() < b.size());
    std::string cpyA = a;
    std::string cpyB = b;
    std::transform(cpyA.begin(), cpyA.end(), cpyA.begin(), ::tolower);
    std::transform(cpyB.begin(), cpyB.end(), cpyB.begin(), ::tolower);
    return cpyA < cpyB;
  }
};

/**
 * Structure for coparison of strings
 * (Does not make any copies)
 */
struct compareString_nocopy {
  bool operator()(std::string& a, std::string& b) {
    if (a.size() != b.size())
      return (a.size() < b.size());
    std::transform(a.begin(), a.end(), a.begin(), ::tolower);
    std::transform(b.begin(), b.end(), b.begin(), ::tolower);
    return a < b;
  }
};

bool isNotAlpha(int ch) {
  return !isalpha(ch);
}

/**
 * Removes all non-alphabetic characters
 * @param word a string to be modified
 * @return modified string
 */
bool modify(std::string& word) {
  word.erase(std::remove_if(word.begin(), word.end(), isNotAlpha), word.end());
  return !word.empty();
}

/**
 * Reads words from file and stores them in vectors
 * @param fileName
 * @param C_W vector to be filled
 * @param N_W vector to be filled
 * @return number of words read
 */
int readFile(const std::string& fileName,
             std::vector<std::string>& C_W,
             std::vector<std::string>& N_W) {
  using namespace std;
  ifstream file;
  file.open(fileName);
  if (!file.is_open())
    return 0;

  int word_count = 0;
  string word;
  while (file >> word) {
    if (modify(word)) {
      C_W.push_back(word);
      N_W.push_back(word);
      ++word_count;
    }
  }
  return word_count;
}

/**
 * Fills and empties priority queue and Fibonacci heap with strings from file
 * @param fileName
 * @param count How many times should the fill & empty process be repeated
 */
void FillNEmptyTest_str(const std::string& fileName, unsigned count) {
  using namespace std;
  vector<string> const_words, non_const_words;
  priority_queue<string, std::vector<string>, compareString_copy> c_pQueue;
  priority_queue<string, std::vector<string>, compareString_nocopy> n_pQueue;
  FibHeap<string, compareString_copy> c_fibHeap;
  FibHeap<string, compareString_nocopy> n_fibHeap;

  chrono::time_point<chrono::steady_clock> start, end;
  std::chrono::duration<double> average(0);

  int word_count = readFile(fileName, const_words, non_const_words);

  for (unsigned i = 0; i < count; ++i) {
    start = chrono::steady_clock::now();
    for (const string& word : const_words) {
      n_pQueue.push(word);
    }
    while (!n_pQueue.empty()) {
      n_pQueue.pop();
    }
    end = chrono::steady_clock::now();
    average += end - start;
  }

  cout << "Filling and emptying priority queue " << count << " times." << endl;
  cout << "Function: Sort by word length, then alphabetically (no copying)."
       << endl;
  cout << "Total number of words: " << word_count << endl;
  cout << "Total time: " << average.count() << " seconds." << endl;
  cout << "Average time: " << average.count() / 10 << " seconds." << endl
       << endl;

  average = std::chrono::duration<double>(0);

  for (unsigned i = 0; i < count; ++i) {
    start = chrono::steady_clock::now();
    for (string& word : non_const_words) {
      c_pQueue.push(word);
    }
    while (!c_pQueue.empty()) {
      c_pQueue.pop();
    }
    end = chrono::steady_clock::now();
    average += end - start;
  }

  cout << "Filling and emptying priority queue " << count << " times." << endl;
  cout << "Function: Sort by word length, then alphabetically (compare "
          "function copies strings)."
       << endl;
  cout << "Total number of words: " << word_count << endl;
  cout << "Total time: " << average.count() << " seconds." << endl;
  cout << "Average time: " << average.count() / 10 << " seconds." << endl
       << endl;

  average = std::chrono::duration<double>(0);

  for (unsigned i = 0; i < count; ++i) {
    start = chrono::steady_clock::now();
    for (const string& word : const_words) {
      n_fibHeap.insert(word);
    }
    while (!n_fibHeap.empty()) {
      n_fibHeap.extract_top();
    }
    end = chrono::steady_clock::now();
    average += end - start;
  }

  cout << "Filling and emptying Fibonacci heap " << count << " times." << endl;
  cout << "Function: Sort by word length, then alphabetically (no copying)."
       << endl;
  cout << "Total number of words: " << word_count << endl;
  cout << "Total time: " << average.count() << " seconds." << endl;
  cout << "Average time: " << average.count() / 10 << " seconds." << endl
       << endl;

  average = std::chrono::duration<double>(0);

  for (unsigned i = 0; i < count; ++i) {
    start = chrono::steady_clock::now();
    for (string& word : non_const_words) {
      c_fibHeap.insert(word);
    }
    while (!c_fibHeap.empty()) {
      c_fibHeap.extract_top();
    }
    end = chrono::steady_clock::now();
    average += end - start;
  }

  cout << "Filling and emptying Fibonacci heap " << count << " times." << endl;
  cout << "Function: Sort by word length, then alphabetically (compare "
          "function copies strings)."
       << endl;
  cout << "Total number of words: " << word_count << endl;
  cout << "Total time: " << average.count() << " seconds." << endl;
  cout << "Average time: " << average.count() / 10 << " seconds." << endl
       << endl;
}

/**
 * Fills and empties priority queue and Fibonacci heap with random integers
 * @param pushCount How many integers to generate
 * @param repeatCount How many times should the whole process be repeated
 */
void FillNEmptyTest_int(unsigned pushCount, unsigned repeatCount) {
  using namespace std;
  priority_queue<int> pQueue;
  FibHeap<int> fibHeap;
  vector<int> vector;
  chrono::time_point<chrono::steady_clock> start, end;
  std::chrono::duration<double> total(0);
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
    total += end - start;
  }
  std::cout << "Priority queue" << endl;
  std::cout << "Pushed and popped " << pushCount << " random values "
            << repeatCount << " times" << endl;
  std::cout << "Total time: " << total.count()
            << "s   Average time: " << total.count() / repeatCount << "s\n\n";
  total = std::chrono::duration<double>(0);

  for (unsigned j = 0; j < repeatCount; ++j) {
    start = chrono::steady_clock::now();
    for (unsigned i = 0; i < pushCount; ++i) {
      fibHeap.insert(vector.at(i));
    }

    for (unsigned i = 0; i < pushCount; ++i) {
      fibHeap.extract_top();
    }
    end = chrono::steady_clock::now();
    total += end - start;
  }
  std::cout << "Fibonacci heap" << endl;
  std::cout << "Pushed and popped " << pushCount << " random values." << endl;
  std::cout << "Total time: " << total.count()
            << "s   Average time: " << total.count() / repeatCount << "s\n";
}

/**
 * Interactive test for pushing and poping random numbers into priority queue
 * and Fibonacci heap
 */
void UserTest() {
  using namespace std;
  unsigned count;
  string cmd;

  priority_queue<int> pQueue;
  FibHeap<int> fibHeap;
  vector<int> vector;
  chrono::time_point<chrono::steady_clock> start, end;
  std::chrono::duration<double> total_fib(0), total_queue(0);
  random_device rd;
  mt19937 generator;
  generator.seed(rd());

  cout << "Commands:\n"
       << "push <number> - inserts <number> of random integers to priority "
          "queue and Fibonacci heap\n"
       << "pop <number> - extracts <number> of integers from priority queue "
          "and Fibonacci heap\n"
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
      total_queue += end - start;

      start = chrono::steady_clock::now();
      for (unsigned i = 0; i < count; ++i) {
        fibHeap.insert(vector.at(i));
      }
      end = chrono::steady_clock::now();
      total_fib += end - start;
    }
    if (cmd == "pop") {
      cin >> count;

      start = chrono::steady_clock::now();
      for (unsigned i = 0; i < count; ++i) {
        pQueue.pop();
      }
      end = chrono::steady_clock::now();
      total_queue += end - start;

      start = chrono::steady_clock::now();
      for (unsigned i = 0; i < count; ++i) {
        fibHeap.extract_top();
      }
      end = chrono::steady_clock::now();
      total_fib += end - start;
    }
    if (cmd == "exit") {
      cout << "Fibonacci heap\n"
           << "Total time: " << total_fib.count() << endl
           << endl
           << "Priority queue\n"
           << "Total time: " << total_queue.count() << endl
           << endl;
      break;
    }
    cout << "DONE\n";
  }
}

/**
 * Structure representing a single vertex in a graph
 */
struct Vertex {
  Vertex(unsigned id) : dist(MY_MAX), ID(id) {}
  Vertex(unsigned id, unsigned distance) : dist(distance), ID(id) {}

  unsigned dist;
  unsigned ID;
};

/**
 * Structure for Vertex comparison
 */
struct cmpVertex {
  bool operator()(const Vertex& first, const Vertex& second) {
    return second.dist == first.dist ? first.ID > second.ID
                                     : first.dist > second.dist;
  }
};

/**
 * Graph represented using adjacency matrix
 */
struct Graph {
  Graph(size_t s) : matrix(s * s, 0), size(s) {}

  int at(size_t row, size_t col) { return matrix[row * size + col]; }

  void generateSparseGraph(int max) { generateGraph(0.33, max); }
  void generateDenseGraph(int max) { generateGraph(0.66, max); }

  /**
   * Generates adjacency matrix
   * @param fill - a number from 0 to 1
   *             - specifies how many edges should the graph have
   *             (0 meaning none and 1 meaning all pairs of vertices should have
   * an edge)
   * @param max - maximum weight of edges
   */
  void generateGraph(float fill, int max) {
    std::random_device rd;
    std::mt19937 generator;
    unsigned x = rd();
    generator.seed(x);
    std::cout << x << std::endl;

    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < i; ++j) {
        if (generator() % 1000 < fill * 1000) {
          matrix.at(i * size + j) = (generator() % max) + 1;
          matrix.at(j * size + i) = matrix.at(i * size + j);
        } else {
          matrix.at(i * size + j) = MY_MAX;
          matrix.at(j * size + i) = MY_MAX;
        }
      }
    }
  }

  void shortestPathPriorityQueue(unsigned fromID,
                                 bool showResult,
                                 bool showTime) {
    using namespace std;
    chrono::time_point<chrono::steady_clock> start, end;
    chrono::duration<double> duration(0);

    std::priority_queue<Vertex, std::vector<Vertex>, cmpVertex> pQueue;
    std::vector<unsigned> distances(size, MY_MAX);
    for (unsigned i = 0; i < size; ++i) {
      pQueue.push(Vertex(i));
    }
    pQueue.push(Vertex(fromID, 0));
    distances[fromID] = 0;
    assert(pQueue.top().dist == 0);

    start = chrono::steady_clock::now();
    unsigned u;

    while (!pQueue.empty()) {
      u = pQueue.top().ID;
      pQueue.pop();
      for (unsigned v = 0; v < size; ++v) {
        if (distances[v] > distances[u] + at(u, v)) {
          distances[v] = distances[u] + at(u, v);
          pQueue.push(Vertex(v, distances[v]));
        }
      }
    }

    end = chrono::steady_clock::now();
    duration = end - start;

    if (showResult) {
      std::cout << "Shortest distances from vertex " << fromID
                << "(priority queue)" << std::endl;
      for (unsigned i = 0; i < size; ++i) {
        std::cout << "ID: " << i << "    d = " << distances[i] << std::endl;
      }
    }

    if (showTime) {
      cout << "Shortest path (Priority queue)" << endl;
      cout << "Graph size: " << size << endl;
      cout << "Time: " << duration.count() << "s" << endl;
    }
  };

  void shortestPathFibHeap(unsigned fromID, bool showResult, bool showTime) {
    using namespace std;
    chrono::time_point<chrono::steady_clock> start, end;
    chrono::duration<double> duration(0);

    FibHeap<Vertex, cmpVertex> fibHeap;
    std::vector<FibHeap<Vertex, cmpVertex>::Handler> handlers;
    std::vector<unsigned> distances(size, MY_MAX);
    for (unsigned i = 0; i < size; ++i) {
      handlers.push_back(fibHeap.insert(Vertex(i, MY_MAX)));
    }
    fibHeap.increase_key(handlers[fromID], Vertex(fromID, 0));
    // std::cout << "after init" << std::endl;

    start = chrono::steady_clock::now();

    unsigned ID, distance;
    while (!fibHeap.empty()) {
      ID = fibHeap.top().ID;
      distance = fibHeap.top().dist;
      distances[ID] = distance;
      for (unsigned v = 0; v < size; ++v) {
        if (handlers[v].isValid() &&
            handlers[v].value().dist > distance + at(ID, v)) {
          fibHeap.increase_key(handlers[v], Vertex(v, distance + at(ID, v)));
        }
      }
      fibHeap.extract_top();
    }

    end = chrono::steady_clock::now();
    duration = end - start;

    if (showResult) {
      std::cout << "Shortest distances from vertex " << fromID
                << "(Fibonacci heap)" << std::endl;
      for (unsigned i = 0; i < size; ++i) {
        std::cout << "ID: " << i << "    d = " << distances[i] << std::endl;
      }
      std::cout << "End of results" << std::endl;
    }
    if (showTime) {
      cout << "Shortest path (Fibonacci heap)" << endl;
      cout << "Graph size: " << size << endl;
      cout << "Time: " << duration.count() << "s" << endl;
    }
  }

  std::vector<int> matrix;
  size_t size;
};

int main() {
  // FillNEmptyTest_str("input.txt", 10);
  // FillNEmptyTest_int(100000, 1);
  // UserTest();

  /*Graph graph(8);
  graph.matrix = {
  0,      MY_MAX, MY_MAX, 3,      MY_MAX, 5,      7,      MY_MAX,
  MY_MAX, 0,      MY_MAX, 3,      8,      4,      MY_MAX, MY_MAX,
  MY_MAX, MY_MAX, 0,      2,      5,      MY_MAX, MY_MAX, MY_MAX,
  3,      3,      2,      0,      MY_MAX, MY_MAX, 1,      MY_MAX,
  MY_MAX, 8,      5,      MY_MAX, 0,      MY_MAX, 6,      MY_MAX,
  5,      4,      MY_MAX, MY_MAX, MY_MAX, 0,      MY_MAX, 2,
  7,      MY_MAX, MY_MAX, 1,      6,      MY_MAX, 0,      4,
  MY_MAX, MY_MAX, MY_MAX, MY_MAX, MY_MAX, 2,      4,      0};

  graph.shortestPathPriorityQueue(5, true, true);
  graph.shortestPathFibHeap(5, true, true);*/

  /*Graph graph1(20000);
  graph1.generateSparseGraph(50);
  std::cout << "begin" << std::endl;
  graph1.shortestPathFibHeap(2, false, true);
  std::cout << "end" << std::endl;
  graph1.shortestPathPriorityQueue(15, false, true);*/
}

#endif