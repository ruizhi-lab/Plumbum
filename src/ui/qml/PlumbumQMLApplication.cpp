#include "PlumbumQMLApplication.hpp"

#include "components/translations/QvTranslator.hpp"
#include "core/settings/SettingsBackend.hpp"
#include "core/handler/ConfigHandler.hpp"

#define QV_MODULE_NAME "QMLApplication"

#include <QApplication>
#include <QDesktopServices>
#include <QMessageBox>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QSystemTrayIcon>

#ifdef PLUMBUM_QMLLIVE_DEBUG
#include <qmllive/livenodeengine.h>
#include <qmllive/qmllive.h>
#include <qmllive/remotereceiver.h>
#endif

PlumbumQMLApplication::PlumbumQMLApplication(int &argc, char *argv[]) : PlumbumPlatformApplication(argc, argv)
{
}

void PlumbumQMLApplication::MessageBoxWarn(QWidget *parent, const QString &title, const QString &text)
{
    QMessageBox::warning(parent, title, text);
}
void PlumbumQMLApplication::MessageBoxInfo(QWidget *parent, const QString &title, const QString &text)
{
    QMessageBox::information(parent, title, text);
}

MessageOpt PlumbumQMLApplication::MessageBoxAsk(QWidget *parent, const QString &title, const QString &text, const QList<MessageOpt> &list)
{
    QFlags<QMessageBox::StandardButton> btns;
    for (const auto &b : list)
    {
        btns.setFlag(MessageBoxButtonMap[b]);
    }
    return MessageBoxButtonMap.key(QMessageBox::question(parent, title, text, btns));
}

QStringList PlumbumQMLApplication::checkPrerequisitesInternal()
{
    return {};
}

PlumbumExitReason PlumbumQMLApplication::runPlumbumInternal()
{
    uiProperty.initialize();
    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;
    const QUrl url("qrc:/forms/MainWindow.qml");
    const auto connectLambda = [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    };
    connect(&engine, &QQmlApplicationEngine::objectCreated, this, connectLambda, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("plumbum", &uiProperty);
    engine.addImportPath(QStringLiteral("qrc:/forms/"));
    engine.load(url);

    // System tray icon (app icon + connection shortcuts).
    setupTrayIcon();
    if (trayIcon)
        trayIcon->show();

    // Set window icon from bundled resources (ApplicationWindow has no 'icon' property in this Qt).
    const auto rootObjects = engine.rootObjects();
    if (!rootObjects.isEmpty())
    {
        if (auto *win = qobject_cast<QQuickWindow *>(rootObjects.first()))
            win->setIcon(QIcon(QStringLiteral(":/assets/icons/plumbum.png")));
    }
    // Application-wide icon fallback (used by window managers / dialogs).
    QApplication::setWindowIcon(QIcon(QStringLiteral(":/assets/icons/plumbum.png")));

#ifdef PLUMBUM_QMLLIVE_DEBUG
    LiveNodeEngine node;

    // Let QmlLive know your runtime
    node.setQmlEngine(&engine);

    // Allow it to display QML components with non-QQuickWindow root object
    QQuickView fallbackView(&engine, 0);
    node.setFallbackView(&fallbackView);

    // Tell it where file updates should be stored relative to
    node.setWorkspace(applicationDirPath() + "/forms", LiveNodeEngine::AllowUpdates | LiveNodeEngine::UpdatesAsOverlay);

    // Listen to IPC call from remote QmlLive Bench
    RemoteReceiver receiver;
    receiver.registerNode(&node);
    receiver.listen(10234);

    QQuickWindow *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());

    // Advanced use: let it know the initially loaded QML component (do this
    // only after registering to receiver!)
    QList<QQmlError> warnings;
    node.usePreloadedDocument(applicationDirPath() + "/forms/MainWindow.qml", window, warnings);
#endif
    return (PlumbumExitReason) exec();
}

void PlumbumQMLApplication::terminateUIInternal()
{
    if (trayIcon)
        trayIcon->hide();
}

void PlumbumQMLApplication::setupTrayIcon()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        LOG("System tray is not available on this system.");
        return;
    }

    trayIcon = new QSystemTrayIcon(QIcon(QStringLiteral(":/assets/icons/plumbum.png")), this);
    trayIcon->setToolTip(QStringLiteral("Plumbum - Xray/V2Ray Client"));
    LOG("Tray icon loaded: " + QString::number(!trayIcon->icon().isNull()) + ", tray available: " + QString::number(QSystemTrayIcon::isSystemTrayAvailable()));

    trayMenu = new QMenu();

    trayShowAction = trayMenu->addAction(QObject::tr("Show / Hide"), this, [this]() {
        toggleMainWindowVisibility();
    });
    trayMenu->addSeparator();
    trayConnInfoAction = trayMenu->addAction(QObject::tr("Not Connected"));
    trayConnInfoAction->setEnabled(false);
    trayConnectAction = trayMenu->addAction(QObject::tr("Connect"), this, &PlumbumQMLApplication::onTrayConnect);
    trayDisconnectAction = trayMenu->addAction(QObject::tr("Disconnect"), this, &PlumbumQMLApplication::onTrayDisconnect);
    trayMenu->addSeparator();
    trayQuitAction = trayMenu->addAction(QObject::tr("Quit"), this, [this]() {
        if (ConnectionManager)
            ConnectionManager->StopConnection();
        QCoreApplication::quit();
    });

    trayIcon->setContextMenu(trayMenu);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &PlumbumQMLApplication::onTrayActivated);

    // Keep tray state in sync with connection events.
    if (ConnectionManager)
    {
        connect(ConnectionManager, &QvConfigHandler::OnConnected, this, [this](const ConnectionGroupPair &id) {
            if (ConnectionManager && ConnectionManager->IsValidId(id))
            {
                const auto name = ConnectionManager->GetConnectionMetaObject(id.connectionId).displayName;
                trayConnInfoAction->setText(QObject::tr("Connected: %1").arg(name));
                trayIcon->setToolTip(QObject::tr("Plumbum - %1").arg(name));
            }
            trayConnectAction->setEnabled(false);
            trayDisconnectAction->setEnabled(true);
        });
        connect(ConnectionManager, &QvConfigHandler::OnDisconnected, this, [this](const ConnectionGroupPair &) {
            trayConnInfoAction->setText(QObject::tr("Not Connected"));
            trayIcon->setToolTip(QStringLiteral("Plumbum - Xray/V2Ray Client"));
            trayConnectAction->setEnabled(true);
            trayDisconnectAction->setEnabled(false);
        });
        connect(ConnectionManager, &QvConfigHandler::OnKernelCrashed, this, [this](const ConnectionGroupPair &, const QString &) {
            trayConnInfoAction->setText(QObject::tr("Not Connected"));
            trayConnectAction->setEnabled(true);
            trayDisconnectAction->setEnabled(false);
        });
    }
}

void PlumbumQMLApplication::toggleMainWindowVisibility()
{
    QQuickWindow *win = nullptr;
    for (auto *w : QApplication::topLevelWindows())
    {
        if (auto *qw = qobject_cast<QQuickWindow *>(w))
        {
            win = qw;
            break;
        }
    }
    if (!win)
        return;
    if (win->isVisible())
        win->hide();
    else
    {
        win->show();
        win->raise();
        win->requestActivate();
    }
}

void PlumbumQMLApplication::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick)
        toggleMainWindowVisibility();
}

void PlumbumQMLApplication::onTrayConnect()
{
    const auto groups = ConnectionManager->AllGroups();
    for (const auto &gid : groups)
    {
        for (const auto &cid : ConnectionManager->GetConnections(gid))
        {
            ConnectionManager->StartConnection({ cid, gid });
            return;
        }
    }
}

void PlumbumQMLApplication::onTrayDisconnect()
{
    if (ConnectionManager)
        ConnectionManager->StopConnection();
}

void PlumbumQMLApplication::updateTrayMenu()
{
    if (!trayIcon)
        return;
    const bool connected = KernelInstance && !KernelInstance->CurrentConnection().isEmpty();
    trayConnectAction->setEnabled(!connected);
    trayDisconnectAction->setEnabled(connected);
    if (connected && ConnectionManager)
    {
        const auto pair = KernelInstance->CurrentConnection();
        if (ConnectionManager->IsValidId(pair))
            trayConnInfoAction->setText(QObject::tr("Connected: %1").arg(ConnectionManager->GetConnectionMetaObject(pair.connectionId).displayName));
    }
    else
    {
        trayConnInfoAction->setText(QObject::tr("Not Connected"));
    }
}

void PlumbumQMLApplication::OpenURL(const QString &url)
{
    QDesktopServices::openUrl(url);
}

#ifndef PLUMBUM_NO_SINGLEAPPLICATON
void PlumbumQMLApplication::onMessageReceived(quint32, QByteArray)
{
}
#endif
