#include "core/connection/Generation.hpp"
#include "core/connection/Serialization.hpp"

#include <QUrlQuery>

namespace Plumbum::core::connection::serialization::trojan
{
    CONFIGROOT Deserialize(const QString &link, QString *alias, QString *errMessage)
    {
        if (!link.startsWith("trojan://"))
        {
            *errMessage = QObject::tr("Trojan link should start with trojan://");
            return CONFIGROOT(QJsonObject{});
        }
        const QUrl url(link);
        if (!url.isValid() || url.host().isEmpty() || url.port() < 0 || url.userInfo().isEmpty())
        {
            *errMessage = QObject::tr("Invalid Trojan link: password, host and port are required.");
            return CONFIGROOT(QJsonObject{});
        }
        if (!url.fragment().isEmpty())
            *alias = QUrl::fromPercentEncoding(url.fragment().toUtf8());

        const QUrlQuery query(url);
        const auto network = query.queryItemValue("type").isEmpty() ? QStringLiteral("tcp") : query.queryItemValue("type");
        const auto security = query.queryItemValue("security").isEmpty() ? QStringLiteral("tls") : query.queryItemValue("security");
        const QJsonObject server{ { "address", url.host() },
                                  { "port", url.port() },
                                  { "password", QUrl::fromPercentEncoding(url.userInfo().toUtf8()) },
                                  { "level", 0 } };
        const QJsonObject settings{ { "servers", QJsonArray{ server } } };
        QJsonObject stream;
        if (network != "tcp")
            stream["network"] = network;
        if (security != "none")
        {
            stream["security"] = security;
            QJsonObject tls;
            const auto sni = query.queryItemValue("sni");
            if (!sni.isEmpty())
                tls["serverName"] = sni;
            if (query.queryItemValue("allowInsecure") == "1")
                tls["allowInsecure"] = true;
            stream[security == "xtls" ? "xtlsSettings" : "tlsSettings"] = tls;
        }
        if (network == "ws")
        {
            QJsonObject ws{ { "path", QUrl::fromPercentEncoding(query.queryItemValue("path").toUtf8()) } };
            const auto host = query.queryItemValue("host");
            if (!host.isEmpty())
                ws["headers"] = QJsonObject{ { "Host", host } };
            stream["wsSettings"] = ws;
        }
        else if (network == "grpc")
            stream["grpcSettings"] = QJsonObject{ { "serviceName", query.queryItemValue("serviceName") } };

        QJsonObject outbound{ { "protocol", "trojan" }, { "settings", settings }, { "tag", OUTBOUND_TAG_PROXY } };
        if (!stream.isEmpty())
            outbound["streamSettings"] = stream;
        return CONFIGROOT(QJsonObject{ { "outbounds", QJsonArray{ outbound } } });
    }

    QString Serialize(const QJsonObject &settings, const QJsonObject &stream, const QString &alias)
    {
        const auto server = settings["servers"].toArray().at(0).toObject();
        QUrl url;
        url.setScheme("trojan");
        url.setUserInfo(QUrl::toPercentEncoding(server["password"].toString()));
        url.setHost(server["address"].toString());
        url.setPort(server["port"].toInt());
        url.setFragment(QUrl::toPercentEncoding(alias));
        QUrlQuery query;
        const auto network = stream["network"].toString("tcp");
        const auto security = stream["security"].toString("tls");
        if (network != "tcp")
            query.addQueryItem("type", network);
        if (security != "tls")
            query.addQueryItem("security", security);
        const auto tlsKey = security == "xtls" ? "xtlsSettings" : "tlsSettings";
        const auto sni = stream[tlsKey].toObject()["serverName"].toString();
        if (!sni.isEmpty())
            query.addQueryItem("sni", sni);
        if (network == "ws")
        {
            const auto ws = stream["wsSettings"].toObject();
            query.addQueryItem("path", ws["path"].toString());
            query.addQueryItem("host", ws["headers"].toObject()["Host"].toString());
        }
        else if (network == "grpc")
            query.addQueryItem("serviceName", stream["grpcSettings"].toObject()["serviceName"].toString());
        url.setQuery(query);
        return url.toString(QUrl::FullyEncoded);
    }
} // namespace Plumbum::core::connection::serialization::trojan
