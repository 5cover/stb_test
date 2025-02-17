# stb_test: A Lightweight Unit Testing Library for C

## Overview

**stb_test** is a simple and lightweight unit testing library for C, designed to facilitate quick and easy test case management. Inspired by the **stb single-file header approach**, it enables developers to write unit tests with minimal boilerplate while maintaining clarity and flexibility.

The library is designed with the following principles in mind:

- **Simplicity**: No complex setup is required; just include the header file.
- **Minimalism**: A small, single-header implementation without dependencies (except `stb_ds.h` for dynamic arrays).
- **Readability**: Test results are displayed in an intuitive table format.
- **Flexibility**: Offers configurable macros for ANSI terminal colors and definition modifiers.

## Getting Started

### 1. Integration

To use stb_test, you need to include its header file in your test source file. You also need to define `STB_TEST_IMPLEMENTATION` in **one** of your source files to include the function definitions.

#### Example

```c
#define STB_TEST_IMPLEMENTATION  // Define in exactly one source file
#include "stb_test.h"
```

### 2. Writing Tests

stb_test uses a simple API to define test suites and test cases. Here’s how you can write tests:

```c
#include "stb_test.h"
#include <stdio.h>

int main(void) {
    struct test my_tests = test_start("Example Test Suite");

    test_case(&my_tests, 1 + 1 == 2, "Addition works");
    test_case(&my_tests, 2 * 2 == 5, "Multiplication fails");
    test_fail(&my_tests, "Intentional failure");

    bool all_passed = test_end(&my_tests, stdout);
    return all_passed ? 0 : 1;
}
```

### 3. Running Tests

Compile and run your test program:

```sh
gcc test_program.c -o test_program && ./test_program
```

The output will show a summary and details of failed test cases in a table format.

## API Reference

### 1. Creating a Test Suite

```c
struct test test_start(char const *name);
```

- **name**: Name of the test suite.
- **Returns**: A `struct test` instance to store test cases.

### 2. Adding Test Cases

```c
#define test_case(test, expr, name, ...)
```

- **test**: Pointer to the test suite.
- **expr**: Expression to evaluate (should return `true` or `false`).
- **name**: Description of the test case.
- **...**: Optional additional information formatted as `printf`.

```c
#define test_fail(test, name, ...)
```

- Adds a failing test case for verification purposes.

### 3. Completing a Test Suite

```c
bool test_end(struct test *test, FILE *output);
```

- **test**: Pointer to the test suite.
- **output**: Output stream (e.g., `stdout`).
- **Returns**: `true` if all tests passed, `false` otherwise.

## Configuration Macros

stb_test provides several macros to modify its behavior:

| Macro Name            | Default Value  | Description                                        |
| --------------------- | -------------- | -------------------------------------------------- |
| `STB_TEST_DEFINITION` | *(empty)*      | Define function modifiers (e.g., `static inline`). |
| `STB_TEST_ANSI_OK`    | `"\033[32;49"` | ANSI escape code for success (green).              |
| `STB_TEST_ANSI_KO`    | `"\033[31;49"` | ANSI escape code for failure (red).                |
| `STB_TEST_ANSI_REST`  | `"\033[39;49"` | ANSI escape code reset.                            |

Override these macros before including `stb_test.h` to customize the output.

### Example: Disable ANSI Colors

```c
#define STB_TEST_ANSI_OK ""
#define STB_TEST_ANSI_KO ""
#define STB_TEST_ANSI_REST ""
#include "stb_test.h"
```

## License

This library is released into the **Public Domain** under **The Unlicense**.

## Contribution

Contributions and feedback are welcome! Feel free to open issues or submit pull requests.

---

Happy testing! 🚀
