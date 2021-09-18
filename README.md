# MyProject

MyProject is a C++ library for for adding and subtracting numbers.

## Installation

Use the classic cmake method.

```bash
mkdir build
cd build
cmake ..
make install
```

## Usage

```c++
// src/Adder.cc

#include <cstdlib>
#include <iostream>
#include <string>

#include "MyProject/Add.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " [number_a] [number_b]" << '\n';
    return EXIT_FAILURE;
  }
  std::cout << MyProject::Add(std::stod(argv[1]), std::stod(argv[2])) << '\n';
  return EXIT_SUCCESS;
}
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[MIT](LICENSE)
