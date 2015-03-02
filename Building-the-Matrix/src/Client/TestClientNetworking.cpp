#define CATCH_CONFIG_MAIN
#include "Catch/catch.hpp"
#include "Client.hpp"

TEST_CASE("Client timeouts", "[timeouts]") {
    Client c;
    SECTION("Timeout set") {
        REQUIRE(c.timeout == std::chrono::seconds(6));
        REQUIRE(c.timeout != std::chrono::seconds(5));
    }
    SECTION("Data received bumps time last updated, timedOut") {
        std::string str_a("aaa");
        char received[1024];
        auto time_before = c.getTimeLastUpdated();
        c.receive(received, 1024);
        auto time_after = c.getTimeLastUpdated();
        REQUIRE(time_before != time_after);
        time_before = time_after;
        c.receive(received, 1024);
        time_after = c.getTimeLastUpdated();
        REQUIRE(time_before < time_after);

        REQUIRE(!c.timedOut());
        sleep(7);
        REQUIRE(c.timedOut());
    }
}
