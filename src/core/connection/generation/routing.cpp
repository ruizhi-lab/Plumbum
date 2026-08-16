#include "core/connection/Generation.hpp"
namespace Plumbum::core::connection::generation::routing
{
    QJsonObject GenerateDNS(const QvConfig_DNS &dnsServer)
    {
        QJsonObject root = dnsServer.toJson();
        QJsonArray servers;
        for (const auto &serv : dnsServer.servers)
            servers << (serv.PLUMBUM_DNS_IS_COMPLEX_DNS ? serv.toJson() : QJsonValue(serv.address));
        root["servers"] = servers;
        return root;
    }

    ROUTERULE GenerateSingleRouteRule(RuleType t, const QString &str, const QString &outboundTag, const QString &type)
    {
        return GenerateSingleRouteRule(t, QStringList{ str }, outboundTag, type);
    }

    ROUTERULE GenerateSingleRouteRule(RuleType t, const QStringList &rules, const QString &outboundTag, const QString &type)
    {
        ROUTERULE root;
        auto list = rules;
        list.removeAll("");
        switch (t)
        {
            case RULE_DOMAIN: root.insert("domain", QJsonArray::fromStringList(rules)); break;
            case RULE_IP: root.insert("ip", QJsonArray::fromStringList(rules)); break;
            default: Q_UNREACHABLE();
        }
        JADD(outboundTag, type)
        return root;
    }

    // -------------------------- BEGIN CONFIG GENERATIONS
    //
    // PAC routing modes (v2rayN style):
    //   WHITELIST: mainland (geoip:cn / geosite:cn) -> direct, everything else -> proxy (default).
    //   BLACKLIST: geosite:gfw -> proxy, everything else -> direct.
    //   GLOBAL:    everything -> proxy (default outbound).
    ROUTING GenerateRoutes(bool enableProxy, bool bypassCN, int pacMode, bool bypassLAN, const QString &outTag, const QvConfig_Route &routeConfig)
    {
        ROUTING root;
        root.insert("domainStrategy", routeConfig.domainStrategy);
        root.insert("domainMatcher", routeConfig.domainMatcher);
        //
        // For Rules list
        QJsonArray rulesList;
        if (bypassLAN)
            rulesList << GenerateSingleRouteRule(RULE_IP, "geoip:private", OUTBOUND_TAG_DIRECT);
        //
        if (!enableProxy)
        {
            // This is added to disable all proxies, as a alternative influence of #64
            rulesList << GenerateSingleRouteRule(RULE_DOMAIN, "regexp:.*", OUTBOUND_TAG_DIRECT);
            rulesList << GenerateSingleRouteRule(RULE_IP, "0.0.0.0/0", OUTBOUND_TAG_DIRECT);
            rulesList << GenerateSingleRouteRule(RULE_IP, "::/0", OUTBOUND_TAG_DIRECT);
        }
        else
        {
            //
            // Blocked.
            if (!routeConfig.ips.block.isEmpty())
                rulesList << GenerateSingleRouteRule(RULE_IP, routeConfig.ips.block, OUTBOUND_TAG_BLACKHOLE);
            if (!routeConfig.domains.block.isEmpty())
                rulesList << GenerateSingleRouteRule(RULE_DOMAIN, routeConfig.domains.block, OUTBOUND_TAG_BLACKHOLE);
            //
            // Explicit proxy/direct overrides take priority over PAC mode.
            if (!routeConfig.ips.proxy.isEmpty())
                rulesList << GenerateSingleRouteRule(RULE_IP, routeConfig.ips.proxy, outTag);
            if (!routeConfig.domains.proxy.isEmpty())
                rulesList << GenerateSingleRouteRule(RULE_DOMAIN, routeConfig.domains.proxy, outTag);
            //
            if (!routeConfig.ips.direct.isEmpty())
                rulesList << GenerateSingleRouteRule(RULE_IP, routeConfig.ips.direct, OUTBOUND_TAG_DIRECT);
            if (!routeConfig.domains.direct.isEmpty())
                rulesList << GenerateSingleRouteRule(RULE_DOMAIN, routeConfig.domains.direct, OUTBOUND_TAG_DIRECT);
            //
            // PAC mode routing rules.
            switch (pacMode)
            {
                case QvConfig_Connection::PAC_MODE_WHITELIST:
                {
                    // Bypass mainland China: cn direct, the rest falls through to proxy.
                    // bypassCN is the legacy toggle; old configs default pacMode to WHITELIST
                    // and rely on bypassCN, so respect it here.
                    if (bypassCN)
                    {
                        rulesList << GenerateSingleRouteRule(RULE_IP, "geoip:cn", OUTBOUND_TAG_DIRECT);
                        rulesList << GenerateSingleRouteRule(RULE_DOMAIN, "geosite:cn", OUTBOUND_TAG_DIRECT);
                    }
                    break;
                }
                case QvConfig_Connection::PAC_MODE_BLACKLIST:
                {
                    // GFW list proxied, everything else direct.
                    rulesList << GenerateSingleRouteRule(RULE_DOMAIN, "geosite:gfw", outTag);
                    rulesList << GenerateSingleRouteRule(RULE_DOMAIN, "regexp:.*", OUTBOUND_TAG_DIRECT);
                    rulesList << GenerateSingleRouteRule(RULE_IP, "0.0.0.0/0", OUTBOUND_TAG_DIRECT);
                    rulesList << GenerateSingleRouteRule(RULE_IP, "::/0", OUTBOUND_TAG_DIRECT);
                    break;
                }
                case QvConfig_Connection::PAC_MODE_GLOBAL:
                default:
                {
                    // Everything proxied (default outbound), nothing to add.
                    break;
                }
            }
        }

        root.insert("rules", rulesList);
        return root;
    }

} // namespace Plumbum::core::connection::generation::routing
