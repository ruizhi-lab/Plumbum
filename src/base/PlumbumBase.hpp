#pragma once
//
#include <QMap>
#include <QtCore>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <optional>
#include <vector>
// Base support.
#include "base/JsonHelpers.hpp"
#include "base/PlumbumFeatures.hpp"
#include "base/PlumbumLog.hpp"
// Code Models
#include "base/PlumbumBaseApplication.hpp"
#include "base/models/CoreObjectModels.hpp"
#include "base/models/QvConfigIdentifier.hpp"
#include "base/models/QvRuntimeConfig.hpp"
#include "base/models/QvSafeType.hpp"
#include "base/models/QvSettingsObject.hpp"
#include "base/models/QvStartupConfig.hpp"

using namespace Plumbum;
using namespace Plumbum::base;
using namespace Plumbum::base::safetype;
using namespace Plumbum::base::config;
using namespace Plumbum::base::objects;
using namespace Plumbum::base::objects::protocol;
using namespace Plumbum::base::objects::transfer;

#define PLUMBUM_BUILD_INFO QString(_PLUMBUM_BUILD_INFO_STR_)
#define PLUMBUM_BUILD_EXTRA_INFO QString(_PLUMBUM_BUILD_EXTRA_INFO_STR_)

// Base folder suffix.
#ifdef QT_DEBUG
#define PLUMBUM_CONFIG_DIR_SUFFIX "_debug/"
#define _BOMB_ (static_cast<QObject *>(nullptr)->event(nullptr))
#else
#define _BOMB_
#define PLUMBUM_CONFIG_DIR_SUFFIX "/"
#endif

#ifdef Q_OS_WIN
#define PLUMBUM_EXECUTABLE_SUFFIX ".exe"
#else
#define PLUMBUM_EXECUTABLE_SUFFIX ""
#endif

#ifdef Q_OS_WIN
#define PLUMBUM_LIBRARY_SUFFIX ".dll"
#else
#define PLUMBUM_LIBRARY_SUFFIX ".so"
#endif

// Get Configured Config Dir Path
#define PLUMBUM_CONFIG_DIR (QvCoreApplication->ConfigPath)
#define PLUMBUM_CONFIG_FILE (PLUMBUM_CONFIG_DIR + "Plumbum.conf")
#define PLUMBUM_CONNECTIONS_DIR (PLUMBUM_CONFIG_DIR + "connections/")
#define PLUMBUM_PLUGIN_SETTINGS_DIR (PLUMBUM_CONFIG_DIR + "plugin_settings/")
#define PLUMBUM_CONFIG_FILE_EXTENSION ".plumbum.json"
#define PLUMBUM_GENERATED_DIR (PLUMBUM_CONFIG_DIR + "generated/")

#if !defined(PLUMBUM_DEFAULT_VCORE_PATH) && !defined(PLUMBUM_DEFAULT_VASSETS_PATH)
#define PLUMBUM_DEFAULT_VASSETS_PATH (PLUMBUM_CONFIG_DIR + "vcore/")
#define PLUMBUM_DEFAULT_VCORE_PATH (PLUMBUM_CONFIG_DIR + "vcore/v2ray" PLUMBUM_EXECUTABLE_SUFFIX)
#if !defined(PLUMBUM_USE_V5_CORE)
#define PLUMBUM_DEFAULT_VCTL_PATH (PLUMBUM_CONFIG_DIR + "vcore/v2ctl" PLUMBUM_EXECUTABLE_SUFFIX)
#endif
#elif defined(PLUMBUM_DEFAULT_VCORE_PATH) && defined(PLUMBUM_DEFAULT_VASSETS_PATH)
// ---- Using user-specified VCore and VAssets path
#else
#error Both PLUMBUM_DEFAULT_VCORE_PATH and PLUMBUM_DEFAULT_VASSETS_PATH need to be presented when using manually specify the paths.
#endif

#define QSTRN(num) QString::number(num)

#define OUTBOUND_TAG_BLACKHOLE "BLACKHOLE"
#define OUTBOUND_TAG_DIRECT "DIRECT"
#define OUTBOUND_TAG_PROXY "PROXY"
#define OUTBOUND_TAG_FORWARD_PROXY "PLUMBUM_FORWARD_PROXY"

#define API_TAG_DEFAULT "PLUMBUM_API"
#define API_TAG_INBOUND "PLUMBUM_API_INBOUND"

#define PLUMBUM_USE_FPROXY_KEY "_PLUMBUM_USE_GLOBAL_FORWARD_PROXY_"
