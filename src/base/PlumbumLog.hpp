#pragma once

#include "3rdparty/QJsonStruct/macroexpansion.hpp"
#include "base/PlumbumBaseApplication.hpp"
#include "base/models/QvStartupConfig.hpp"

#include <QPair>
#include <QString>
#include <QTextStream>
#include <iostream>


#define NEWLINE "\r\n"
#define QVLOG_A_DO_EXPAND(___x) , QPair<std::string, decltype(___x)>(std::string(#___x), [&] { return ___x; }())
#define QVLOG_A(...) FOREACH_CALL_FUNC(QVLOG_A_DO_EXPAND, __VA_ARGS__)

#ifdef QT_DEBUG
#define PLUMBUM_IS_DEBUG true
// __FILE__ ":" QT_STRINGIFY(__LINE__),
#define PLUMBUM_LOG_PREPEND_CONTENT Q_FUNC_INFO,
#else
#define PLUMBUM_IS_DEBUG false
#define PLUMBUM_LOG_PREPEND_CONTENT
#endif

#define _LOG_ARG_(...) PLUMBUM_LOG_PREPEND_CONTENT "[" QV_MODULE_NAME "]", __VA_ARGS__

#define LOG(...) Plumbum::base::log_internal<PLUMBUM_LOG_NORMAL>(_LOG_ARG_(__VA_ARGS__))
#define DEBUG(...) Plumbum::base::log_internal<PLUMBUM_LOG_DEBUG>(_LOG_ARG_(__VA_ARGS__))

enum QvLogType
{
    PLUMBUM_LOG_NORMAL = 0,
    PLUMBUM_LOG_DEBUG = 1
};

Q_DECLARE_METATYPE(const char *)

namespace Plumbum::base
{
    inline QString logBuffer;
    inline QString tempBuffer;
    inline QTextStream logStream{ &logBuffer };
    inline QTextStream tempStream{ &tempBuffer };

    inline QString ReadLog()
    {
        return logStream.readAll();
    }

    template<QvLogType t, typename... T>
    inline void log_internal(T... v)
    {
        ((logStream << v << " "), ...);
        ((tempStream << v << " "), ...);
        logStream << NEWLINE;
#ifndef QT_DEBUG
        // We only process DEBUG log in Release mode
        // Prevent QvCoreApplication nullptr
        // TODO: Move log function inside QvCoreApplication
        if (t == PLUMBUM_LOG_DEBUG && QvCoreApplication && !QvCoreApplication->StartupArguments.debugLog)
        {
            // Discard debug log in non-debug Plumbum version with
            // no-debugLog mode.
            return;
        }
#endif

        const auto logString = tempStream.readAll();
    std::cout << logString.toStdString() << std::endl;
    }
} // namespace Plumbum::base

template<typename TKey, typename TVal>
QTextStream &operator<<(QTextStream &stream, const QPair<TKey, TVal> &pair)
{
    return stream << pair.first << ": " << pair.second;
}

inline QTextStream &operator<<(QTextStream &stream, const std::string &ss)
{
    return stream << ss.data();
}

template<typename TKey, typename TVal>
QTextStream &operator<<(QTextStream &stream, const QMap<TKey, TVal> &map)
{
    stream << "{ ";
    for (const auto &[k, v] : map.toStdMap())
        stream << QPair<TKey, TVal>(k, v) << "; ";
    stream << "}";
    return stream;
}

template<typename TVal>
QTextStream &operator<<(QTextStream &stream, const std::initializer_list<TVal> &init_list)
{
    for (const auto &x : init_list)
        stream << x;
    return stream;
}
