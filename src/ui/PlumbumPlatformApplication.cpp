#include "PlumbumPlatformApplication.hpp"

#include "core/settings/SettingsBackend.hpp"

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QSessionManager>
#endif

#include <QSslSocket>
#define QV_MODULE_NAME "PlatformApplication"

#ifdef QT_DEBUG
const static inline QString PLUMBUM_URL_SCHEME = "plumbum-debug";
#else
const static inline QString PLUMBUM_URL_SCHEME = "plumbum";
#endif

QStringList PlumbumPlatformApplication::CheckPrerequisites()
{
    QStringList errors;
    if (!QSslSocket::supportsSsl())
    {
        // Check OpenSSL version for auto-update and subscriptions
        const auto osslReqVersion = QSslSocket::sslLibraryBuildVersionString();
        const auto osslCurVersion = QSslSocket::sslLibraryVersionString();
        LOG("Current OpenSSL version: " + osslCurVersion);
        LOG("Required OpenSSL version: " + osslReqVersion);
        errors << "Plumbum cannot run without OpenSSL.";
        errors << "This is usually caused by using the wrong version of OpenSSL";
        errors << "Required=" + osslReqVersion + "Current=" + osslCurVersion;
    }
    return errors + checkPrerequisitesInternal();
}

bool PlumbumPlatformApplication::Initialize()
{
    QString errorMessage;
    bool canContinue;
    const auto hasError = parseCommandLine(&errorMessage, &canContinue);
    if (hasError)
    {
        LOG("Command line:" QVLOG_A(errorMessage));
        if (!canContinue)
        {
            LOG("Fatal, Plumbum cannot continue.");
            return false;
        }
        else
        {
            LOG("Non-fatal error, continue starting up.");
        }
    }

#ifdef Q_OS_WIN
    const auto appPath = QDir::toNativeSeparators(applicationFilePath());
    const auto regPath = "HKEY_CURRENT_USER\\Software\\Classes\\" + PLUMBUM_URL_SCHEME;
    QSettings reg(regPath, QSettings::NativeFormat);
    reg.setValue("Default", "Plumbum");
    reg.setValue("URL Protocol", "");
    reg.beginGroup("DefaultIcon");
    reg.setValue("Default", QString("%1,1").arg(appPath));
    reg.endGroup();
    reg.beginGroup("shell");
    reg.beginGroup("open");
    reg.beginGroup("command");
    reg.setValue("Default", appPath + " %1");
#endif

    connect(this, &PlumbumPlatformApplication::aboutToQuit, this, &PlumbumPlatformApplication::quitInternal);
#ifndef PLUMBUM_NO_SINGLEAPPLICATON
    connect(this, &SingleApplication::receivedMessage, this, &PlumbumPlatformApplication::onMessageReceived, Qt::QueuedConnection);
    if (isSecondary())
    {
        StartupArguments.version = PLUMBUM_VERSION_STRING;
        StartupArguments.buildVersion = PLUMBUM_VERSION_BUILD;
        StartupArguments.fullArgs = arguments();
        if (StartupArguments.arguments.isEmpty())
            StartupArguments.arguments << PlumbumStartupArguments::NORMAL;
        bool status = sendMessage(JsonToString(StartupArguments.toJson(), QJsonDocument::Compact).toUtf8());
        if (!status)
            LOG("Cannot send message.");
        SetExitReason(EXIT_SECONDARY_INSTANCE);
        return false;
    }
#endif

#ifdef PLUMBUM_GUI
#ifdef Q_OS_LINUX
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    setFallbackSessionManagementEnabled(false);
#endif
    connect(this, &QGuiApplication::commitDataRequest, [] {
        RouteManager->SaveRoutes();
        ConnectionManager->SaveConnectionConfig();
        PluginHost->SavePluginSettings();
        SaveGlobalSettings();
    });
#endif

#ifdef Q_OS_WIN
    SetCurrentDirectory(applicationDirPath().toStdWString().c_str());
    // Set special font in Windows
    QFont font;
    font.setPointSize(9);
    font.setFamily("Microsoft YaHei");
    setFont(font);
#endif
#endif

    // Install a default translater. From the OS/DE
    PlumbumTranslator = std::make_unique<QvTranslator>();
    PlumbumTranslator->InstallTranslation(QLocale::system().name());
    const auto allTranslations = PlumbumTranslator->GetAvailableLanguages();
    const auto osLanguage = QLocale::system().name();
    //
    LocateConfiguration();
    if (!allTranslations.contains(GlobalConfig.uiConfig.language))
    {
        // If we need to reset the language.
        if (allTranslations.contains(osLanguage))
        {
            GlobalConfig.uiConfig.language = osLanguage;
        }
        else if (!allTranslations.isEmpty())
        {
            GlobalConfig.uiConfig.language = allTranslations.first();
        }
    }

    if (!PlumbumTranslator->InstallTranslation(GlobalConfig.uiConfig.language))
    {
        QvMessageBoxWarn(nullptr, "Translation Failed",
                         "Cannot load translation for " + GlobalConfig.uiConfig.language + NEWLINE + //
                             "English is now used." + NEWLINE + NEWLINE +                            //
                             "Please go to Preferences Window to change language or open an Issue");
        GlobalConfig.uiConfig.language = "en_US";
    }

    return true;
}

PlumbumExitReason PlumbumPlatformApplication::RunPlumbum()
{
    PluginHost = new QvPluginHost();
    RouteManager = new RouteHandler();
    ConnectionManager = new QvConfigHandler();
    return runPlumbumInternal();
}

void PlumbumPlatformApplication::quitInternal()
{
    // Do not change the order.
    ConnectionManager->StopConnection();
    RouteManager->SaveRoutes();
    ConnectionManager->SaveConnectionConfig();
    PluginHost->SavePluginSettings();
    SaveGlobalSettings();
    terminateUIInternal();
    delete ConnectionManager;
    delete RouteManager;
    delete PluginHost;
    ConnectionManager = nullptr;
    RouteManager = nullptr;
    PluginHost = nullptr;
}

bool PlumbumPlatformApplication::parseCommandLine(QString *errorMessage, bool *canContinue)
{
    *canContinue = true;
    QStringList filteredArgs;
    for (const auto &arg : arguments())
    {
#ifdef Q_OS_MACOS
        if (arg.contains("-psn"))
            continue;
#endif
        filteredArgs << arg;
    }
    QCommandLineParser parser;
    //
    QCommandLineOption noAPIOption("noAPI", QObject::tr("Disable gRPC API subsystem"));
    QCommandLineOption noPluginsOption("noPlugin", QObject::tr("Disable plugins feature"));
    QCommandLineOption debugLogOption("debug", QObject::tr("Enable debug output"));
    QCommandLineOption noAutoConnectionOption("noAutoConnection", QObject::tr("Do not automatically connect"));
    QCommandLineOption disconnectOption("disconnect", QObject::tr("Stop current connection"));
    QCommandLineOption reconnectOption("reconnect", QObject::tr("Reconnect last connection"));
    QCommandLineOption exitOption("exit", QObject::tr("Exit Plumbum"));
    //
    parser.setApplicationDescription(QObject::tr("Plumbum - A cross-platform Qt frontend for V2Ray."));
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    //
    parser.addOption(noAPIOption);
    parser.addOption(noPluginsOption);
    parser.addOption(debugLogOption);
    parser.addOption(noAutoConnectionOption);
    parser.addOption(disconnectOption);
    parser.addOption(reconnectOption);
    parser.addOption(exitOption);
    //
    const auto helpOption = parser.addHelpOption();
    const auto versionOption = parser.addVersionOption();

    if (!parser.parse(filteredArgs))
    {
        *canContinue = true;
        *errorMessage = parser.errorText();
        return false;
    }

    if (parser.isSet(versionOption))
    {
        parser.showVersion();
        return true;
    }

    if (parser.isSet(helpOption))
    {
        parser.showHelp();
        return true;
    }

    for (const auto &arg : parser.positionalArguments())
    {
        if (arg.startsWith(PLUMBUM_URL_SCHEME + "://"))
        {
            StartupArguments.arguments << PlumbumStartupArguments::PLUMBUM_LINK;
            StartupArguments.links << arg;
        }
    }

    if (parser.isSet(exitOption))
    {
        DEBUG("disconnectOption is set.");
        StartupArguments.arguments << PlumbumStartupArguments::EXIT;
    }

    if (parser.isSet(disconnectOption))
    {
        DEBUG("disconnectOption is set.");
        StartupArguments.arguments << PlumbumStartupArguments::DISCONNECT;
    }

    if (parser.isSet(reconnectOption))
    {
        DEBUG("reconnectOption is set.");
        StartupArguments.arguments << PlumbumStartupArguments::RECONNECT;
    }

#define ProcessExtraStartupOptions(option)                                                                                                           \
    DEBUG("Startup Options:" QVLOG_A(parser.isSet(option##Option)));                                                                                 \
    StartupArguments.option = parser.isSet(option##Option);

    ProcessExtraStartupOptions(noAPI);
    ProcessExtraStartupOptions(debugLog);
    ProcessExtraStartupOptions(noAutoConnection);
    ProcessExtraStartupOptions(noPlugins);
    return true;
}
