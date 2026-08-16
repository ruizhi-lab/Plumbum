#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <functional>

namespace Plumbum::common::network
{
    class NetworkRequestHelper : QObject
    {
        Q_OBJECT
        explicit NetworkRequestHelper(QObject *parent) : QObject(parent){};
        ~NetworkRequestHelper(){};

      public:
        static void AsyncHttpGet(const QString &url, std::function<void(const QByteArray &)> funcPtr);
        static QByteArray HttpGet(const QUrl &url);

      private:
        static void setAccessManagerAttributes(QNetworkRequest &request, QNetworkAccessManager &accessManager);
        static void setHeader(QNetworkRequest &request, const QByteArray &key, const QByteArray &value);
    };
} // namespace Plumbum::common::network

using namespace Plumbum::common::network;
