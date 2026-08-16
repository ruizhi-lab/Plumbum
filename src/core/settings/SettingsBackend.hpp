#pragma once
#include "base/PlumbumBase.hpp"

namespace Plumbum::core::config
{
    void SaveGlobalSettings();
    bool LocateConfiguration();
    void SetConfigDirPath(const QString &path);
    bool CheckSettingsPathAvailability(const QString &_path, bool checkExistingConfig);
} // namespace Plumbum::core::config

namespace Plumbum
{
    // Extra header for QvConfigUpgrade.cpp
    QJsonObject UpgradeSettingsVersion(int fromVersion, int toVersion, const QJsonObject &root);
} // namespace Plumbum

using namespace Plumbum::core;
using namespace Plumbum::core::config;
