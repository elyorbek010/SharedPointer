#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "shared_pointer.hpp"

TEST_CASE("test 1", "smth") {
	std::cout << smart_pointer::print() << std::endl;
	REQUIRE(1);
}