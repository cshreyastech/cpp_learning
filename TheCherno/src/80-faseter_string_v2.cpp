#include <iostream>
#include <string>

// Custom memory tracking class
class MemoryTracker {
public:
  static int allocations;  // Number of memory allocations

  static void* operator new(std::size_t size) {
    ++allocations;
    return malloc(size);
  }

  static void operator delete(void* ptr) noexcept {
    --allocations;
    free(ptr);
  }
};

int MemoryTracker::allocations = 0;

int main() {
  // Enable memory tracking for strings
  std::allocator<char> alloc;
  std::basic_string<char, std::char_traits<char>, std::allocator<char>> myString(alloc);
  myString = "Hello, World!";
  std::string first_word = myString.substr(0, 5);

  // Print the number of memory allocations
  std::cout << "Number of string memory allocations: " << MemoryTracker::allocations << std::endl;

  // Other operations involving strings...

  // Deallocate memory
  myString.~basic_string();

  // Print the number of memory allocations after deallocation
  std::cout << "Number of string memory allocations after deallocation: " << MemoryTracker::allocations << std::endl;

  return 0;
}
