#include <iostream>
#include "Car.h"

int main() {
	Car c;
	Car c1(c);

	std::cout << "car c1's dashboard " << std::endl;
	c1.Dashboard();

	return 0;
}