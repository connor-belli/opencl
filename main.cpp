#include <iostream>

#include "blur.h"

int main() {
	std::vector<int> a = {1, 2, 3, 4};
	std::vector<int> b = {1, 2, 3, 4};
	std::vector<int> c = {1, 2, 3, 4};

	blur(a, b, c);

	std::cout << c[0] << std::endl;
}
