#pragma once

#include "base/PlumbumBaseApplication.hpp"
#include "components/translations/QvTranslator.hpp"
#include "core/handler/ConfigHandler.hpp"
#include "core/handler/RouteHandler.hpp"
#include "core/settings/SettingsBackend.hpp"
#include "utils/QvHelpers.hpp"

#ifndef PLUMBUM_NO_SINGLEAPPLICATON
#ifdef Q_OS_ANDROID
// No SingleApplication on Android platform
#define PLUMBUM_NO_SINGLEAPPLICATON
#elif PLUMBUM_WORKAROUND_MACOS_MEMLOCK
// No SingleApplication on macOS locking error
#define PLUMBUM_NO_SINGLEAPPLICATON
#endif
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#ifdef PLUMBUM_GUI
#include <QApplication>
#include <QMessageBox>
const static inline QMap<MessageOpt, QMessageBox::StandardButton> MessageBoxButtonMap //
    = { { No, QMessageBox::No },
        { OK, QMessageBox::Ok },
        { Yes, QMessageBox::Yes },
        { Cancel, QMessageBox::Cancel },
        { Ignore, QMessageBox::Ignore } };
#else
#include <QCoreApplication>
#endif

#ifndef PLUMBUM_NO_SINGLEAPPLICATON
#include <SingleApplication>
#define QVBASEAPPLICATION SingleApplication
#define QVBASEAPPLICATION_CTORARGS argc, argv, true, User | ExcludeAppPath | ExcludeAppVersion
#else
#define QVBASEAPPLICATION QAPPLICATION_CLASS
#define QVBASEAPPLICATION_CTORARGS argc, argv
#endif

class PlumbumPlatformApplication
    : public QVBASEAPPLICATION
    , public PlumbumApplicationInterface
{
    Q_OBJECT
  public:
    PlumbumPlatformApplication(int &argc, char *argv[]) : QVBASEAPPLICATION(QVBASEAPPLICATION_CTORARGS), PlumbumApplicationInterface(){};
    virtual ~PlumbumPlatformApplication(){};
    virtual PlumbumExitReason GetExitReason() const final
    {
        return _exitReason;
    }

    virtual QStringList CheckPrerequisites() final;
    virtual bool Initialize() final;
    virtual PlumbumExitReason RunPlumbum() final;

  protected:
    virtual QStringList checkPrerequisitesInternal() = 0;
    virtual PlumbumExitReason runPlumbumInternal() = 0;
    virtual void terminateUIInternal() = 0;
    virtual void SetExitReason(PlumbumExitReason r) final
    {
        _exitReason = r;
    }

#ifndef PLUMBUM_NO_SINGLEAPPLICATON
    virtual void onMessageReceived(quint32 clientId, QByteArray msg) = 0;
#endif

  private:
    void quitInternal();
    PlumbumExitReason _exitReason;
    bool parseCommandLine(QString *errorMessage, bool *canContinue);
};
