#pragma once

#include "ui/PlumbumPlatformApplication.hpp"

#include <QSystemTrayIcon>

class MainWindow;

namespace Plumbum
{
    class PlumbumWidgetApplication : public PlumbumPlatformApplication
    {
        Q_OBJECT
      public:
        explicit PlumbumWidgetApplication(int &argc, char *argv[]);
        QJsonObject UIStates;

      public:
        void MessageBoxWarn(QWidget *parent, const QString &title, const QString &text) override;
        void MessageBoxInfo(QWidget *parent, const QString &title, const QString &text) override;
        MessageOpt MessageBoxAsk(QWidget *parent, const QString &title, const QString &text, const QList<MessageOpt> &buttons) override;
        void ShowTrayMessage(const QString &m, int msecs = 10000);
        void OpenURL(const QString &url) override;

        inline QSystemTrayIcon **GetTrayIcon()
        {
            return &hTray;
        }

      private:
        QStringList checkPrerequisitesInternal() override;
        PlumbumExitReason runPlumbumInternal() override;
        bool isInitialized;
        void terminateUIInternal() override;
#ifndef PLUMBUM_NO_SINGLEAPPLICATON
        void onMessageReceived(quint32 clientID, QByteArray msg) override;
#endif
        QSystemTrayIcon *hTray;
        MainWindow *mainWindow;
    };
} // namespace Plumbum

#ifdef PlumbumApplication
#undef PlumbumApplication
#endif
#define PlumbumApplication PlumbumWidgetApplication

#define QvWidgetApplication static_cast<PlumbumWidgetApplication *>(qApp)
#define qvAppTrayIcon (*(QvWidgetApplication->GetTrayIcon()))

using namespace Plumbum;
