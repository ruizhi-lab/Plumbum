#include "PlumbumQMLProperty.hpp"

#include "components/translations/QvTranslator.hpp"
#include "core/connection/Serialization.hpp"
#include "core/CoreUtils.hpp"
#include "core/handler/KernelInstanceHandler.hpp"
#include "core/settings/SettingsBackend.hpp"

#include <QClipboard>
#include <QFileInfo>
#include <QGuiApplication>
#include <QStyleHints>

#ifdef Q_OS_LINUX
#include <unistd.h>
#endif

using namespace Plumbum;
using namespace Plumbum::core::connection;
using namespace Plumbum::core::handler;

// =================================================================================
// ConnectionListModel
// =================================================================================
ConnectionListModel::ConnectionListModel(QObject *parent) : QAbstractListModel(parent)
{
    _groupId = DefaultGroupId;
}

int ConnectionListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return _connections.count();
}

QVariant ConnectionListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _connections.count())
        return {};
    const auto id = _connections[index.row()];
    const auto meta = ConnectionManager->GetConnectionMetaObject(id);
    const auto info = GetConnectionInfo(ConnectionManager->GetConnectionRoot(id));


    switch (role)
    {
        case ConnectionIdRole: return id.toString();
        case DisplayNameRole: return meta.displayName;
        case ProtocolRole: return info.protocol.toUpper();
        case AddressRole: return info.address;
        case PortRole: return info.port;
        case LatencyTextRole:
        {
            if (meta.latency == LATENCY_TEST_VALUE_ERROR)
                return tr("Timeout");
            if (meta.latency == LATENCY_TEST_VALUE_NODATA)
                return tr("N/A");
            return tr("%1 ms").arg(meta.latency);
        }
        case IsConnectedRole:
            return ConnectionManager->IsConnected({ id, _groupId });
        case IsRunningRole:
            return ConnectionManager->IsConnected({ id, _groupId });
        case UpTotalRole:
        {
            auto stats = meta.stats;
            return FormatBytes(stats[API_OUTBOUND_PROXY].upLinkData);
        }
        case DownTotalRole:
        {
            auto stats = meta.stats;
            return FormatBytes(stats[API_OUTBOUND_PROXY].downLinkData);
        }
        case LastConnectedRole:
        {
            if (meta.lastConnected <= 0)
                return tr("Never");
            return QDateTime::fromSecsSinceEpoch(meta.lastConnected).toString("yyyy-MM-dd hh:mm");
        }
        default: return {};
    }
}

QHash<int, QByteArray> ConnectionListModel::roleNames() const
{
    return { { ConnectionIdRole, "connectionId" },       //
             { DisplayNameRole, "displayName" },         //
             { ProtocolRole, "protocol" },               //
             { AddressRole, "address" },                 //
             { PortRole, "port" },                       //
             { LatencyTextRole, "latencyText" },         //
             { IsConnectedRole, "isConnected" },         //
             { IsRunningRole, "isRunning" },             //
             { UpTotalRole, "upTotal" },                 //
             { DownTotalRole, "downTotal" },             //
             { LastConnectedRole, "lastConnected" } };
}

void ConnectionListModel::setGroup(const GroupId &groupId)
{
    if (_groupId == groupId)
        return;
    _groupId = groupId;
    refresh();
}

void ConnectionListModel::refresh()
{
    beginResetModel();
    _connections.clear();
    if (ConnectionManager && ConnectionManager->IsValidId(_groupId))
        _connections = ConnectionManager->GetConnections(_groupId);
    endResetModel();
}

// =================================================================================
// GroupListModel
// =================================================================================
GroupListModel::GroupListModel(QObject *parent) : QAbstractListModel(parent)
{
}

int GroupListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return _groups.count();
}

QVariant GroupListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _groups.count())
        return {};
    const auto id = _groups[index.row()];
    const auto meta = ConnectionManager->GetGroupMetaObject(id);
    switch (role)
    {
        case GroupIdRole: return id.toString();
        case DisplayNameRole: return meta.displayName;
        case IsSubscriptionRole: return meta.isSubscription;
        case ConnectionCountRole: return meta.connections.count();
        case SubscriptionAddressRole:
            return meta.isSubscription ? meta.subscriptionOption.address : QString{};
        case SubscriptionIntervalRole:
            return meta.isSubscription ? meta.subscriptionOption.updateInterval : 0.0;
        case SubscriptionLastUpdatedRole:
            return meta.isSubscription ? QDateTime::fromSecsSinceEpoch(meta.lastUpdatedDate).toString("yyyy-MM-dd hh:mm") : QString{};
        default: return {};
    }
}

QHash<int, QByteArray> GroupListModel::roleNames() const
{
    return { { GroupIdRole, "groupId" },               //
             { DisplayNameRole, "displayName" },       //
             { IsSubscriptionRole, "isSubscription" }, //
             { ConnectionCountRole, "connectionCount" }, //
             { SubscriptionAddressRole, "subscriptionAddress" },
             { SubscriptionIntervalRole, "subscriptionInterval" },
             { SubscriptionLastUpdatedRole, "subscriptionLastUpdated" } };
}

void GroupListModel::refresh()
{
    beginResetModel();
    _groups.clear();
    if (ConnectionManager)
        _groups = ConnectionManager->AllGroups();
    endResetModel();
}

// =================================================================================
// PlumbumQMLProperty
// =================================================================================
PlumbumQMLProperty::PlumbumQMLProperty(QObject *parent) : QObject(parent)
{
    _connectionModel.setGroup(DefaultGroupId);
    _currentGroupId = DefaultGroupId;
}

PlumbumQMLProperty::~PlumbumQMLProperty()
{
}

void PlumbumQMLProperty::setCurrentGroupId(const QString &id)
{
    const GroupId gid(id);
    if (!ConnectionManager || !ConnectionManager->IsValidId(gid))
        return;
    if (_currentGroupId == gid)
        return;
    _currentGroupId = gid;
    _connectionModel.setGroup(gid);
    emit currentGroupIdChanged();
}

void PlumbumQMLProperty::refreshAll()
{
    _groupModel.refresh();
    _connectionModel.refresh();
}

void PlumbumQMLProperty::updateConnectivityState()
{
    const bool wasConnected = _connected;
    _connected = KernelInstance && !KernelInstance->CurrentConnection().isEmpty();
    if (_connected)
    {
        _connectedPair = KernelInstance->CurrentConnection();
        if (ConnectionManager && ConnectionManager->IsValidId(_connectedPair))
            _connectedName = ConnectionManager->GetConnectionMetaObject(_connectedPair.connectionId).displayName;
        else
            _connectedName = tr("(Unknown)");
        _connectedConnectionId = _connectedPair.connectionId;
    }
    else
    {
        _connectedPair = {};
        _connectedConnectionId = NullConnectionId;
        _connectedName = {};
    }
    emit connectivityChanged();
}

// ---- Connectivity ----
void PlumbumQMLProperty::connectConnection(const QString &connectionId)
{
    if (!ConnectionManager)
        return;
    const ConnectionGroupPair pair(ConnectionId(connectionId), _currentGroupId);
    if (!ConnectionManager->StartConnection(pair))
        emit toastMessage(tr("Failed to start connection."));
}

void PlumbumQMLProperty::disconnectConnection()
{
    if (ConnectionManager)
        ConnectionManager->StopConnection();
}

void PlumbumQMLProperty::restartConnection()
{
    if (ConnectionManager)
        ConnectionManager->RestartConnection();
}

void PlumbumQMLProperty::startLatencyTest()
{
    if (!ConnectionManager)
        return;
    if (_currentGroupId == DefaultGroupId)
        ConnectionManager->StartLatencyTest();
    else
        ConnectionManager->StartLatencyTest(_currentGroupId);
}

void PlumbumQMLProperty::startLatencyTestFor(const QString &connectionId)
{
    if (ConnectionManager)
        ConnectionManager->StartLatencyTest(ConnectionId(connectionId));
}

// ---- Import & Create ----
bool PlumbumQMLProperty::importFromClipboard()
{
    const auto clipboard = QGuiApplication::clipboard();
    if (!clipboard)
        return false;
    return importFromLink(clipboard->text());
}

bool PlumbumQMLProperty::importFromLink(const QString &link)
{
    if (!ConnectionManager || link.trimmed().isEmpty())
        return false;
    QStringList lines = SplitLines(link);
    int count = 0;
    for (const auto &line : lines)
    {
        QString errMessage, aliasPrefix, newGroupName;
        const auto configs = ConvertConfigFromString(line.trimmed(), &aliasPrefix, &errMessage, &newGroupName);
        if (configs.isEmpty())
        {
            if (!errMessage.isEmpty())
                emit logMessage(tr("Import failed: %1").arg(errMessage));
            continue;
        }
        for (const auto &[displayName, root] : configs)
        {
            QString finalName = aliasPrefix.isEmpty() ? displayName : aliasPrefix + " - " + displayName;
            ConnectionManager->CreateConnection(root, finalName, _currentGroupId);
            count++;
        }
    }
    if (count > 0)
        emit toastMessage(tr("Imported %1 connection(s)").arg(count));
    return count > 0;
}

QString PlumbumQMLProperty::createGroup(const QString &name)
{
    if (!ConnectionManager || name.trimmed().isEmpty())
        return {};
    const auto gid = ConnectionManager->CreateGroup(name.trimmed(), false);
    emit toastMessage(tr("Group \"%1\" created").arg(name.trimmed()));
    return gid.toString();
}

void PlumbumQMLProperty::deleteGroup(const QString &groupId)
{
    if (!ConnectionManager)
        return;
    const auto result = ConnectionManager->DeleteGroup(GroupId(groupId));
    if (result)
        emit toastMessage(tr("Failed to delete group: %1").arg(*result));
}

void PlumbumQMLProperty::renameGroup(const QString &groupId, const QString &newName)
{
    if (!ConnectionManager || newName.trimmed().isEmpty())
        return;
    const auto result = ConnectionManager->RenameGroup(GroupId(groupId), newName.trimmed());
    if (result)
        emit toastMessage(tr("Failed to rename group: %1").arg(*result));
}

void PlumbumQMLProperty::deleteConnection(const QString &connectionId)
{
    if (!ConnectionManager)
        return;
    const ConnectionId cid(connectionId);
    const ConnectionGroupPair pair(cid, _currentGroupId);
    if (KernelInstance && KernelInstance->CurrentConnection() == pair)
        ConnectionManager->StopConnection();
    if (!ConnectionManager->RemoveConnectionFromGroup(cid, _currentGroupId))
        emit toastMessage(tr("Connection is not in the current group."));
}

void PlumbumQMLProperty::moveConnectionToGroup(const QString &connectionId, const QString &groupId)
{
    if (!ConnectionManager)
        return;
    ConnectionManager->MoveConnectionFromToGroup(ConnectionId(connectionId), _currentGroupId, GroupId(groupId));
}

QString PlumbumQMLProperty::connectionJson(const QString &connectionId) const
{
    if (!ConnectionManager)
        return {};
    const auto id = ConnectionId(connectionId);
    if (!ConnectionManager->IsValidId(id))
        return {};
    return JsonToString(ConnectionManager->GetConnectionRoot(id), QJsonDocument::Indented);
}

bool PlumbumQMLProperty::updateConnectionJson(const QString &connectionId, const QString &json)
{
    if (!ConnectionManager)
        return false;

    QJsonParseError parseError;
    const auto document = QJsonDocument::fromJson(json.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject())
    {
        emit toastMessage(tr("Invalid JSON: %1").arg(parseError.errorString()));
        return false;
    }

    const auto id = ConnectionId(connectionId);
    if (!ConnectionManager->IsValidId(id))
    {
        emit toastMessage(tr("Connection no longer exists."));
        return false;
    }

    if (!ConnectionManager->UpdateConnection(id, CONFIGROOT(document.object())))
    {
        emit toastMessage(tr("Failed to save connection configuration."));
        return false;
    }
    emit toastMessage(tr("Connection configuration saved."));
    return true;
}

// ---- Subscriptions ----
void PlumbumQMLProperty::updateSubscription(const QString &groupId)
{
    if (ConnectionManager)
        ConnectionManager->UpdateSubscriptionAsync(GroupId(groupId));
}

void PlumbumQMLProperty::updateAllSubscriptions()
{
    if (!ConnectionManager)
        return;
    const auto groups = ConnectionManager->AllGroups();
    for (const auto &gid : groups)
    {
        if (ConnectionManager->GetGroupMetaObject(gid).isSubscription)
            ConnectionManager->UpdateSubscriptionAsync(gid);
    }
}

void PlumbumQMLProperty::setSubscriptionInterval(const QString &groupId, double days)
{
    if (!ConnectionManager)
        return;
    days = qBound(0.0, days, 365.0);
    if (ConnectionManager->SetSubscriptionData(GroupId(groupId), std::nullopt, std::nullopt, static_cast<float>(days)))
    {
        ConnectionManager->SaveConnectionConfig();
        emit toastMessage(days <= 0.0 ? tr("Automatic subscription updates disabled.")
                                      : tr("Subscription update interval set to %1 day(s).").arg(days));
        _groupModel.refresh();
    }
}

QString PlumbumQMLProperty::createSubscription(const QString &name, const QString &url)
{
    if (!ConnectionManager || name.trimmed().isEmpty() || url.trimmed().isEmpty())
        return {};
    const auto gid = ConnectionManager->CreateGroup(name.trimmed(), true);
    ConnectionManager->SetSubscriptionData(gid, true, url.trimmed());
    emit toastMessage(tr("Subscription \"%1\" created").arg(name.trimmed()));
    return gid.toString();
}

// ---- Misc ----
QString PlumbumQMLProperty::groupDisplayName(const QString &groupId) const
{
    if (!ConnectionManager)
        return {};
    return ConnectionManager->GetGroupMetaObject(GroupId(groupId)).displayName;
}

QString PlumbumQMLProperty::connectionDisplayName(const QString &connectionId) const
{
    if (!ConnectionManager)
        return {};
    return ConnectionManager->GetConnectionMetaObject(ConnectionId(connectionId)).displayName;
}

QString PlumbumQMLProperty::connectionProtocol(const QString &connectionId) const
{
    if (!ConnectionManager)
        return {};
    const auto info = GetConnectionInfo(ConnectionManager->GetConnectionRoot(ConnectionId(connectionId)));
    return info.protocol.toUpper();
}

QString PlumbumQMLProperty::connectionAddress(const QString &connectionId) const
{
    if (!ConnectionManager)
        return {};
    const auto info = GetConnectionInfo(ConnectionManager->GetConnectionRoot(ConnectionId(connectionId)));
    return info.address;
}

int PlumbumQMLProperty::connectionPort(const QString &connectionId) const
{
    if (!ConnectionManager)
        return 0;
    const auto info = GetConnectionInfo(ConnectionManager->GetConnectionRoot(ConnectionId(connectionId)));
    return info.port;
}

void PlumbumQMLProperty::clearConnectionUsage(const QString &connectionId)
{
    if (ConnectionManager)
        ConnectionManager->ClearConnectionUsage({ ConnectionId(connectionId), _currentGroupId });
}

void PlumbumQMLProperty::clearGroupUsage(const QString &groupId)
{
    if (ConnectionManager)
        ConnectionManager->ClearGroupUsage(GroupId(groupId));
}

void PlumbumQMLProperty::copyConnectionLink(const QString &connectionId)
{
    if (!ConnectionManager)
        return;
    const auto pair = ConnectionGroupPair(ConnectionId(connectionId), _currentGroupId);
    const auto link = ConvertConfigToString(pair, true);
    if (link.isEmpty() || link == PLUMBUM_SERIALIZATION_COMPLEX_CONFIG_PLACEHOLDER)
    {
        emit toastMessage(tr("This connection cannot be serialized to a share link."));
        return;
    }
    QGuiApplication::clipboard()->setText(link);
    emit toastMessage(tr("Link copied to clipboard."));
}

// ---- Settings ----
void PlumbumQMLProperty::setV2rayCorePath(const QString &path)
{
    GlobalConfig.kernelConfig.KernelPath(path.trimmed());
    SaveGlobalSettings();
    emit settingsChanged();
}

void PlumbumQMLProperty::setV2rayAssetsPath(const QString &path)
{
    GlobalConfig.kernelConfig.AssetsPath(path.trimmed());
    SaveGlobalSettings();
    emit settingsChanged();
}

void PlumbumQMLProperty::setKernelApiEnabled(bool enabled)
{
    GlobalConfig.kernelConfig.enableAPI = enabled;
    SaveGlobalSettings();
    emit settingsChanged();
}

void PlumbumQMLProperty::setStatsPort(int port)
{
    if (port > 0 && port < 65536)
    {
        GlobalConfig.kernelConfig.statsPort = port;
        SaveGlobalSettings();
        emit settingsChanged();
    }
}

void PlumbumQMLProperty::setPacMode(int mode)
{
    if (mode < 0 || mode > 2)
        return;
    if (GlobalConfig.defaultRouteConfig.connectionConfig.pacMode == mode)
        return;
    GlobalConfig.defaultRouteConfig.connectionConfig.pacMode = mode;
    SaveGlobalSettings();
    emit pacModeChanged();
    // Apply the new routing immediately if a connection is active.
    if (KernelInstance && !KernelInstance->CurrentConnection().isEmpty())
    {
        ConnectionManager->RestartConnection();
        emit toastMessage(tr("PAC mode changed, restarting connection..."));
    }
    else
    {
        emit toastMessage(tr("PAC mode changed."));
    }
}

void PlumbumQMLProperty::setTunEnabled(bool enabled)
{
    if (GlobalConfig.inboundConfig.tunSettings.enabled == enabled)
        return;
    GlobalConfig.inboundConfig.tunSettings.enabled = enabled;
    SaveGlobalSettings();
    emit settingsChanged();
    if (KernelInstance && !KernelInstance->CurrentConnection().isEmpty())
    {
        ConnectionManager->RestartConnection();
        emit toastMessage(enabled ? tr("TUN mode enabled, restarting connection...") : tr("TUN mode disabled, restarting connection..."));
    }
    else
    {
        emit toastMessage(enabled ? tr("TUN mode enabled.") : tr("TUN mode disabled."));
    }
}

void PlumbumQMLProperty::setTunIpv4(const QString &ip)
{
    if (ip.trimmed().isEmpty())
        return;
    GlobalConfig.inboundConfig.tunSettings.ipv4 = ip.trimmed();
    SaveGlobalSettings();
    emit settingsChanged();
}

void PlumbumQMLProperty::setTunMtu(int mtu)
{
    if (mtu < 576 || mtu > 65535)
        return;
    GlobalConfig.inboundConfig.tunSettings.mtu = mtu;
    SaveGlobalSettings();
    emit settingsChanged();
}

bool PlumbumQMLProperty::tunAvailable() const
{
    const auto coreName = QFileInfo(GlobalConfig.kernelConfig.KernelPath()).completeBaseName().toLower();
    const bool isXrayCore = coreName == "xray" || coreName.startsWith("xray-");
    if (!isXrayCore)
        return false;

    // On Linux, TUN interface creation requires root or CAP_NET_ADMIN.
#ifdef Q_OS_LINUX
    return geteuid() == 0 || access("/dev/net/tun", W_OK) == 0;
#else
    return true;
#endif
}

void PlumbumQMLProperty::setThemeMode(int mode)
{
    if (mode < 0 || mode > 2)
        return;
    if (GlobalConfig.uiConfig.themeMode == mode)
        return;
    GlobalConfig.uiConfig.themeMode = mode;
    SaveGlobalSettings();
    emit themeModeChanged();
}

bool PlumbumQMLProperty::systemDark() const
{
#ifdef Q_OS_WIN
    QSettings reg(R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)", QSettings::NativeFormat);
    return reg.value("AppsUseLightTheme", 1).toInt() == 0;
#else
    return QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
#endif
}

void PlumbumQMLProperty::setLanguage(const QString &code)
{
    if (code.isEmpty() || GlobalConfig.uiConfig.language == code)
        return;
    GlobalConfig.uiConfig.language = code;
    SaveGlobalSettings();
    // Apply the new translator immediately.
    if (PlumbumTranslator)
        PlumbumTranslator->InstallTranslation(code);
    emit settingsChanged();
}

QStringList PlumbumQMLProperty::availableLanguages() const
{
    if (PlumbumTranslator)
        return PlumbumTranslator->GetAvailableLanguages();
    return {};
}

// =================================================================================
// Event slots (bound to ConfigHandler signals)
// =================================================================================
void PlumbumQMLProperty::onConnectionCreated(const ConnectionGroupPair &id, const QString &displayName)
{
    Q_UNUSED(id)
    Q_UNUSED(displayName)
    _connectionModel.refresh();
    _groupModel.refresh();
}

void PlumbumQMLProperty::onConnectionModified(const ConnectionId &id)
{
    Q_UNUSED(id)
    _connectionModel.refresh();
}

void PlumbumQMLProperty::onConnectionRenamed(const ConnectionId &id, const QString &originalName, const QString &newName)
{
    Q_UNUSED(id)
    Q_UNUSED(originalName)
    Q_UNUSED(newName)
    _connectionModel.refresh();
}

void PlumbumQMLProperty::onConnectionRemovedFromGroup(const ConnectionGroupPair &pairId)
{
    Q_UNUSED(pairId)
    _connectionModel.refresh();
    _groupModel.refresh();
}

void PlumbumQMLProperty::onConnectionLinkedWithGroup(const ConnectionGroupPair &newPair)
{
    Q_UNUSED(newPair)
    _connectionModel.refresh();
    _groupModel.refresh();
}

void PlumbumQMLProperty::onGroupCreated(const GroupId &id, const QString &displayName)
{
    Q_UNUSED(id)
    Q_UNUSED(displayName)
    _groupModel.refresh();
}

void PlumbumQMLProperty::onGroupRenamed(const GroupId &id, const QString &oldName, const QString &newName)
{
    Q_UNUSED(id)
    Q_UNUSED(oldName)
    Q_UNUSED(newName)
    _groupModel.refresh();
}

void PlumbumQMLProperty::onGroupDeleted(const GroupId &id, const QList<ConnectionId> &connections)
{
    Q_UNUSED(id)
    Q_UNUSED(connections)
    _groupModel.refresh();
    _connectionModel.refresh();
    if (_currentGroupId == id)
    {
        _currentGroupId = DefaultGroupId;
        _connectionModel.setGroup(DefaultGroupId);
        emit currentGroupIdChanged();
    }
}

void PlumbumQMLProperty::onLatencyTestFinished(const ConnectionId &id, const int average)
{
    Q_UNUSED(id)
    Q_UNUSED(average)
    _connectionModel.refresh();
}

void PlumbumQMLProperty::onConnected(const ConnectionGroupPair &id)
{
    _connectedPair = id;
    _connectedConnectionId = id.connectionId;
    _connectedName = ConnectionManager->GetConnectionMetaObject(id.connectionId).displayName;
    _connected = true;
    emit connectivityChanged();
    _connectionModel.refresh();
}

void PlumbumQMLProperty::onDisconnected(const ConnectionGroupPair &id)
{
    Q_UNUSED(id)
    _connected = false;
    _connectedPair = {};
    _connectedConnectionId = NullConnectionId;
    _connectedName = {};
    emit connectivityChanged();
    _connectionModel.refresh();
}

void PlumbumQMLProperty::onKernelCrashed(const ConnectionGroupPair &id, const QString &errMessage)
{
    Q_UNUSED(id)
    _connected = false;
    emit connectivityChanged();
    _connectionModel.refresh();
    emit toastMessage(tr("Kernel crashed: %1").arg(errMessage));
}

void PlumbumQMLProperty::onStatsAvailable(const ConnectionGroupPair &id, const QMap<StatisticsType, QvStatsSpeedData> &data)
{
    if (!_connected || !(id == _connectedPair))
        return;
    qvspeed upSpeed = 0, downSpeed = 0;
    qvdata upTotal = 0, downTotal = 0;
    for (const auto &[type, d] : data.toStdMap())
    {
        upSpeed += d.first.first;
        downSpeed += d.first.second;
        upTotal += d.second.first;
        downTotal += d.second.second;
    }
    _upSpeedText = FormatBytes(upSpeed) + "/s";
    _downSpeedText = FormatBytes(downSpeed) + "/s";
    _upTotalText = FormatBytes(upTotal);
    _downTotalText = FormatBytes(downTotal);
    emit statsChanged();
}

void PlumbumQMLProperty::onKernelLog(const ConnectionGroupPair &id, const QString &log)
{
    Q_UNUSED(id)
    _kernelStatusText = log.trimmed();
    emit kernelStatusChanged();
    emit logMessage(log);
}

void PlumbumQMLProperty::onSubscriptionUpdated(const GroupId &id, bool success)
{
    Q_UNUSED(id)
    _groupModel.refresh();
    _connectionModel.refresh();
    emit toastMessage(success ? tr("Subscription updated.") : tr("Subscription update failed."));
}

// =================================================================================
// Initialization
// =================================================================================
void PlumbumQMLProperty::initialize()
{
    if (!ConnectionManager)
        return;
    _connectionModel.setGroup(_currentGroupId);
    refreshAll();

    connect(ConnectionManager, &QvConfigHandler::OnConnectionCreated, this, &PlumbumQMLProperty::onConnectionCreated);
    connect(ConnectionManager, &QvConfigHandler::OnConnectionModified, this, &PlumbumQMLProperty::onConnectionModified);
    connect(ConnectionManager, &QvConfigHandler::OnConnectionRenamed, this, &PlumbumQMLProperty::onConnectionRenamed);
    connect(ConnectionManager, &QvConfigHandler::OnConnectionRemovedFromGroup, this, &PlumbumQMLProperty::onConnectionRemovedFromGroup);
    connect(ConnectionManager, &QvConfigHandler::OnConnectionLinkedWithGroup, this, &PlumbumQMLProperty::onConnectionLinkedWithGroup);
    connect(ConnectionManager, &QvConfigHandler::OnGroupCreated, this, &PlumbumQMLProperty::onGroupCreated);
    connect(ConnectionManager, &QvConfigHandler::OnGroupRenamed, this, &PlumbumQMLProperty::onGroupRenamed);
    connect(ConnectionManager, &QvConfigHandler::OnGroupDeleted, this, &PlumbumQMLProperty::onGroupDeleted);
    connect(ConnectionManager, &QvConfigHandler::OnLatencyTestFinished, this, &PlumbumQMLProperty::onLatencyTestFinished);
    connect(ConnectionManager, &QvConfigHandler::OnConnected, this, &PlumbumQMLProperty::onConnected);
    connect(ConnectionManager, &QvConfigHandler::OnDisconnected, this, &PlumbumQMLProperty::onDisconnected);
    connect(ConnectionManager, &QvConfigHandler::OnKernelCrashed, this, &PlumbumQMLProperty::onKernelCrashed);
    connect(ConnectionManager, &QvConfigHandler::OnStatsAvailable, this, &PlumbumQMLProperty::onStatsAvailable);
    connect(ConnectionManager, &QvConfigHandler::OnKernelLogAvailable, this, &PlumbumQMLProperty::onKernelLog);
    connect(ConnectionManager, &QvConfigHandler::OnSubscriptionAsyncUpdateFinished, this, &PlumbumQMLProperty::onSubscriptionUpdated);

    // Follow system theme changes.
    connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged, this, [this]() {
        emit systemThemeChanged();
    });
}
