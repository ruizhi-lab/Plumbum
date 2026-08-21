#include "QvAutoLaunch.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

namespace Plumbum::components::autolaunch
{
    namespace
    {
        QString userAutostartDir()
        {
            return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart/";
        }

        QString desktopFilePath()
        {
            return userAutostartDir() + QCoreApplication::applicationName() + ".desktop";
        }
    }

    bool GetLaunchAtLoginStatus()
    {
        return QFile::exists(desktopFilePath());
    }

    void SetLaunchAtLoginStatus(bool enable)
    {
        const auto desktopPath = desktopFilePath();
        if (!enable)
        {
            QFile::remove(desktopPath);
            QFile::remove(userAutostartDir() + "Plumbum.desktop");
            return;
        }

        const auto autostartDir = userAutostartDir();
        if (!QDir().mkpath(autostartDir))
            return;

        QFile desktopFile(desktopPath);
        if (!desktopFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
            return;

        const auto appName = QCoreApplication::applicationName();
        const auto binPath = qEnvironmentVariableIsSet("APPIMAGE") ? qEnvironmentVariable("APPIMAGE")
                                                                     : QCoreApplication::applicationFilePath();
        QTextStream stream(&desktopFile);
        stream << "[Desktop Entry]\n"
               << "Name=" << appName << "\n"
               << "GenericName=V2Ray Frontend\n"
               << "Exec=" << binPath << "\n"
               << "Terminal=false\n"
               << "Icon=plumbum\n"
               << "Categories=Network;\n"
               << "Type=Application\n"
               << "StartupNotify=false\n"
               << "X-GNOME-Autostart-enabled=true\n";
    }
} // namespace Plumbum::components::autolaunch
