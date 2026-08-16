#pragma once
#include <QHostAddress>
#include <QObject>
#include <QString>
//
namespace Plumbum::components::proxy
{
    void ClearSystemProxy();
    void SetSystemProxy(const QString &address, int http_port, int socks_port);
} // namespace Plumbum::components::proxy

using namespace Plumbum::components;
using namespace Plumbum::components::proxy;
