#include "PlumbumQMLProperty.hpp"
#include "base/PlumbumBaseApplication.hpp"
#include "ui/PlumbumPlatformApplication.hpp"

#include <QQuickView>

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
#ifndef PLUMBUM_NO_SINGLEAPPLICATON
        void onMessageReceived(quint32, QByteArray) override;
#endif
      private:
        QQuickView qmlViewer;
        PlumbumQMLProperty uiProperty;
    };

#ifdef PlumbumApplication
#undef PlumbumApplication
#endif

#define PlumbumApplication PlumbumQMLApplication
#define QvQmlApplication static_cast<PlumbumQMLApplication *>(qApp)

} // namespace Plumbum
