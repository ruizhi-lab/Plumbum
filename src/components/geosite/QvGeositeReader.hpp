#pragma once
#include "base/PlumbumBase.hpp"

namespace Plumbum::components::geosite
{
    QStringList ReadGeoSiteFromFile(const QString &filepath);
} // namespace Plumbum::components::geosite

using namespace Plumbum::components;
using namespace Plumbum::components::geosite;
