#include "PlumbumQMLProperty.hpp"
#include "base/PlumbumBaseApplication.hpp"
#include "ui/PlumbumPlatformApplication.hpp"

#include <QMenu>
#include <QPointer>
#include <QPixmap>
#include <QQuickWindow>
#include <QTimer>
#include <QSystemTrayIcon>

namespace Plumbum
{
    class PlumbumQMLApplication : public PlumbumPlatformApplication
    {
        Q_OBJECT
      public:
        explicit PlumbumQMLApplication(int &argc, char *argv[]);
        void MessageBoxWarn(QWidget *parent, const QString &title, const QString &text) override;
        void MessageBoxInfo(QWidget *parent, const QString &title, const QString &text) override;
        MessageOpt MessageBoxAsk(QWidget *parent, const QString &title, const QString &text, const QList<MessageOpt> &buttons) override;
        QStringList checkPrerequisitesInternal() override;
        PlumbumExitReason runPlumbumInternal() override;
        void terminateUIInternal() override;
        void OpenURL(const QString &url) override;

      private slots:
        void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
        void onTrayConnect();
        void onTrayDisconnect();
        void updateTrayMenu();
#ifndef PLUMBUM_NO_SINGLEAPPLICATON
        void onMessageReceived(quint32, QByteArray) override;
#endif
      private:
        void setupTrayIcon();
        void toggleMainWindowVisibility();
        void animateTrayIcon();
        void setTrayAnimationEnabled(bool enabled);

      private:
        QPointer<QQuickWindow> mainWindow;
        PlumbumQMLProperty uiProperty;
        QSystemTrayIcon *trayIcon = nullptr;
        QMenu *trayMenu = nullptr;
        QAction *trayShowAction = nullptr;
        QAction *trayConnInfoAction = nullptr;
        QAction *trayConnectAction = nullptr;
        QAction *trayDisconnectAction = nullptr;
        QAction *trayQuitAction = nullptr;
        QTimer trayAnimationTimer;
        QPixmap trayBasePixmap;
        qreal trayRotation = 0.0;
    };

#ifdef PlumbumApplication
#undef PlumbumApplication
#endif

#define PlumbumApplication PlumbumQMLApplication
#define QvQmlApplication static_cast<PlumbumQMLApplication *>(qApp)

} // namespace Plumbum
