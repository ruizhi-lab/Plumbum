#pragma once
#include "base/PlumbumBase.hpp"
namespace Plumbum::core::connection::connectionIO
{
    CONFIGROOT ConvertConfigFromFile(const QString &sourceFilePath, bool importComplex);
} // namespace Plumbum::core::connection::connectionIO

using namespace Plumbum::core::connection;
using namespace Plumbum::core::connection::connectionIO;
