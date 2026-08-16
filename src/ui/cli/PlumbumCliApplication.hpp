#include "base/PlumbumBaseApplication.hpp"
#include "src/ui/PlumbumPlatformApplication.hpp"

namespace Plumbum
{
    class PlumbumCliApplication : public PlumbumPlatformApplication
    {
        Q_OBJECT
      public:
        explicit PlumbumCliApplication(int &argc, char *argv[]);
        QStringList checkPrerequisitesInternal() override;
        PlumbumExitReason runPlumbumInternal() override;

      public:
        void MessageBoxWarn(QWidget *, const QString &, const QString &) override
        {
        }
        void MessageBoxInfo(QWidget *, const QString &, const QString &) override
        {
        }
        MessageOpt MessageBoxAsk(QWidget *, const QString &, const QString &, const QList<MessageOpt> &) override
        {
            return OK;
        }
        void OpenURL(const QString &) override
        {
        }

      private:
        void terminateUIInternal() override;
#ifndef PLUMBUM_NO_SINGLEAPPLICATON
        void onMessageReceived(quint32 clientID, QByteArray msg) override;
#endif
    };

#ifdef PlumbumApplication
#undef PlumbumApplication
#endif

#define PlumbumApplication PlumbumCliApplication
#define QvCliApplication static_cast<PlumbumCliApplication *>(qApp)

} // namespace Plumbum
