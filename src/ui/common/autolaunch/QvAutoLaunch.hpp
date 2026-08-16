#pragma once

namespace Plumbum::components::autolaunch
{
    bool GetLaunchAtLoginStatus();
    void SetLaunchAtLoginStatus(bool enable);
} // namespace Plumbum::components::autolaunch

using namespace Plumbum::components;
using namespace Plumbum::components::autolaunch;
