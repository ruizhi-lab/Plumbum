#pragma once
#include <QString>
namespace Plumbum::components::port
{
    bool CheckTCPPortStatus(const QString &addr, int port);
    inline bool CheckTCPPortStatus(std::pair<QString, int> config)
    {
        return CheckTCPPortStatus(config.first, config.second);
    }
} // namespace Plumbum::components::port
