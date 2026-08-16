#pragma once

#define SET_RUNTIME_CONFIG(conf, val) RuntimeConfig.conf = val();
#define RESTORE_RUNTIME_CONFIG(conf, func) func(RuntimeConfig.conf);

namespace Plumbum::base
{
    struct PlumbumRuntimeConfig
    {
        bool screenShotHidePlumbum = false;
    };
    inline base::PlumbumRuntimeConfig RuntimeConfig = base::PlumbumRuntimeConfig();
} // namespace Plumbum::base
