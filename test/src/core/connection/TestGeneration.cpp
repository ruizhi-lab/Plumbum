#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "core/connection/Generation.hpp"

TEST_CASE("routing generation preserves rule precedence", "[routing]")
{
    QvConfig_Route route;
    route.domainStrategy = "AsIs";
    route.domains.block = { "blocked.example" };
    route.domains.proxy = { "proxy.example" };
    route.domains.direct = { "direct.example" };
    route.ips.direct = { "192.0.2.0/24" };

    const auto generated = GenerateRoutes(true, true, QvConfig_Connection::PAC_MODE_WHITELIST, true, OUTBOUND_TAG_PROXY, route);
    const auto rules = generated.value("rules").toArray();

    REQUIRE(generated.value("domainStrategy").toString() == "AsIs");
    REQUIRE(rules.size() == 7);
    REQUIRE(rules.at(0).toObject().value("outboundTag").toString() == OUTBOUND_TAG_DIRECT);
    REQUIRE(rules.at(1).toObject().value("outboundTag").toString() == OUTBOUND_TAG_BLACKHOLE);
    REQUIRE(rules.at(2).toObject().value("outboundTag").toString() == OUTBOUND_TAG_PROXY);
    REQUIRE(rules.at(3).toObject().value("outboundTag").toString() == OUTBOUND_TAG_DIRECT);
    REQUIRE(rules.at(4).toObject().value("outboundTag").toString() == OUTBOUND_TAG_DIRECT);
    REQUIRE(rules.at(5).toObject().value("outboundTag").toString() == OUTBOUND_TAG_DIRECT);
    REQUIRE(rules.at(6).toObject().value("outboundTag").toString() == OUTBOUND_TAG_DIRECT);
}

TEST_CASE("routing modes generate deterministic fallback rules", "[routing]")
{
    QvConfig_Route route;

    const auto disabled = GenerateRoutes(false, false, QvConfig_Connection::PAC_MODE_GLOBAL, false, OUTBOUND_TAG_PROXY, route);
    REQUIRE(disabled.value("rules").toArray().size() == 3);

    const auto blacklist = GenerateRoutes(true, false, QvConfig_Connection::PAC_MODE_BLACKLIST, false, OUTBOUND_TAG_PROXY, route);
    const auto rules = blacklist.value("rules").toArray();
    REQUIRE(rules.size() == 4);
    REQUIRE(rules.at(0).toObject().value("outboundTag").toString() == OUTBOUND_TAG_PROXY);
    REQUIRE(rules.at(1).toObject().value("outboundTag").toString() == OUTBOUND_TAG_DIRECT);
}
