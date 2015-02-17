#define CATCH_CONFIG_MAIN
#include "Catch/catch.hpp"
#include "Client.hpp"

TEST_CASE("Data received bumps time last updated", "[client]") {
    Client c;
    std::string str_a("aaa");
    std::string str_b("bbb");
    auto time_before = c.getTimeLastUpdated();
    c.receive(str_a.c_str());
    auto time_after = c.getTimeLastUpdated();
    REQUIRE(time_before != time_after);
    time_before = time_after;
    c.receive(str_b.c_str());
    time_after = c.getTimeLastUpdated();
    REQUIRE(time_before < time_after);
}
