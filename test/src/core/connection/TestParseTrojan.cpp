#include "Common.hpp"
#include "src/core/connection/Serialization.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Test Trojan URL Parsing")
{
    QvTestApplication app;
    QString alias;
    QString error;
    const auto root = trojan::Deserialize(
        "trojan://secret%40pass@example.com:443?security=tls&sni=cdn.example.com&type=ws&path=%2Fproxy&host=cdn.example.com#demo",
        &alias, &error);

    REQUIRE(error.isEmpty());
    REQUIRE(alias == "demo");
    const auto outbound = root["outbounds"].toArray().first().toObject();
    REQUIRE(outbound["protocol"] == "trojan");
    REQUIRE(outbound["settings"].toObject()["servers"].toArray().first().toObject()["address"] == "example.com");
    REQUIRE(outbound["streamSettings"].toObject()["network"] == "ws");
    REQUIRE(outbound["streamSettings"].toObject()["tlsSettings"].toObject()["serverName"] == "cdn.example.com");
}

TEST_CASE("Test Trojan URL preserves insecure TLS setting")
{
    const QJsonObject settings{ { "servers", QJsonArray{ QJsonObject{ { "address", "example.com" }, { "port", 443 }, { "password", "secret" } } } } };
    const QJsonObject stream{ { "security", "tls" }, { "tlsSettings", QJsonObject{ { "serverName", "example.com" }, { "allowInsecure", true } } } };
    const auto link = trojan::Serialize(settings, stream, "demo");
    REQUIRE(QUrlQuery(QUrl(link)).queryItemValue("allowInsecure") == "1");
}
