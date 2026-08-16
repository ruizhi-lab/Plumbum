#include "base/PlumbumBaseApplication.hpp"
using namespace Plumbum;
int fakeArgc = 0;
char *fakeArgv[]{};

class QvTestApplication
    : public QCoreApplication
    , public PlumbumApplicationInterface
{
  public:
    explicit QvTestApplication() : QCoreApplication(fakeArgc, fakeArgv), PlumbumApplicationInterface(){};
    virtual void MessageBoxWarn(QWidget *, const QString &, const QString &) override{};
    virtual void MessageBoxInfo(QWidget *, const QString &, const QString &) override{};
    virtual MessageOpt MessageBoxAsk(QWidget *, const QString &, const QString &, const QList<MessageOpt> &) override
    {
        return {};
    };
    virtual void OpenURL(const QString &) override{};
};
