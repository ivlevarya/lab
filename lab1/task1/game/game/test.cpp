#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "Header.h"

TEST_CASE("Moves a tile to the empty position if the move is valid.") {
	Game a;
	REQUIRE(a.Change(3, 2) == true);
	REQUIRE(a.Change(0, 0) == false);
}

TEST_CASE("Get number of tile.") {
	Game a;
	REQUIRE(a.GetNum(3, 3) == 0);
	REQUIRE(a.GetNum(0, 0) == 1);
}
TEST_CASE("Compare two tiles.") {
	Game a;
	Game b;
	a.Change(3, 2);
	REQUIRE((a == b) == false);
	REQUIRE((a != b) == true);
}
TEST_CASE("Shaker move empty tile.") {
	Game a;
	Game b;
	a.Shaker(200);
	REQUIRE((a == b) == false);
}
