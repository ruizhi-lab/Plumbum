#include "QvPortDetector.hpp"

#include <QHostAddress>
#include <QTcpServer>

namespace Plumbum::components::port
{
    bool CheckTCPPortStatus(const QString &addr, int port)
    {
        QTcpServer server;
        QHostAddress address(addr);
        if (address == QHostAddress::AnyIPv4 || address == QHostAddress::AnyIPv6)
        {
            address = QHostAddress::Any;
        }
        return server.listen(address, port);
    }

} // namespace Plumbum::components::port
