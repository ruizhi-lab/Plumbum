#pragma once

#include "core/handler/ConfigHandler.hpp"
#include "utils/QvHelpers.hpp"

#include <QAbstractListModel>
#include <QClipboard>
#include <QGuiApplication>
#include <QObject>
#include <QQmlEngine>
#include <QVariant>

// =================================================================================
// Connection List Model
// Exposes connections of the currently selected group to QML.
// =================================================================================
class ConnectionListModel : public QAbstractListModel
{
    Q_OBJECT
  public:
    enum Roles
    {
        ConnectionIdRole = Qt::UserRole + 1,
        DisplayNameRole,
        ProtocolRole,
        AddressRole,
        PortRole,
        LatencyTextRole,
        IsConnectedRole,
        IsRunningRole,
        UpTotalRole,
        DownTotalRole,
        LastConnectedRole,
        InCurrentGroupRole
    };

    explicit ConnectionListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setGroup(const GroupId &groupId);
    GroupId currentGroup() const { return _groupId; }
    void refresh();

  private:
    GroupId _groupId;
    QList<ConnectionId> _connections;
};

// =================================================================================
// Group List Model
// Exposes all connection groups to QML.
// =================================================================================
class GroupListModel : public QAbstractListModel
{
    Q_OBJECT
  public:
    enum Roles
    {
        GroupIdRole = Qt::UserRole + 1,
        DisplayNameRole,
        IsSubscriptionRole,
        ConnectionCountRole,
        SubscriptionAddressRole,
        SubscriptionIntervalRole,
        SubscriptionLastUpdatedRole
    };

    explicit GroupListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void refresh();

  private:
    QList<GroupId> _groups;
};

// =================================================================================
// QML Property Bridge
// Exposed to QML as the `plumbum` context property.
// =================================================================================
class PlumbumQMLProperty : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ConnectionListModel *connectionModel READ connectionModel CONSTANT)
    Q_PROPERTY(GroupListModel *groupModel READ groupModel CONSTANT)
    Q_PROPERTY(QString currentGroupId READ currentGroupId WRITE setCurrentGroupId NOTIFY currentGroupIdChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectivityChanged)
    Q_PROPERTY(QString connectedConnectionId READ connectedConnectionId NOTIFY connectivityChanged)
    Q_PROPERTY(QString connectedName READ connectedName NOTIFY connectivityChanged)
    Q_PROPERTY(QString upSpeedText READ upSpeedText NOTIFY statsChanged)
    Q_PROPERTY(QString downSpeedText READ downSpeedText NOTIFY statsChanged)
    Q_PROPERTY(QString upTotalText READ upTotalText NOTIFY statsChanged)
    Q_PROPERTY(QString downTotalText READ downTotalText NOTIFY statsChanged)
    Q_PROPERTY(QString kernelStatusText READ kernelStatusText NOTIFY kernelStatusChanged)
    Q_PROPERTY(QString v2rayCorePath READ v2rayCorePath WRITE setV2rayCorePath NOTIFY settingsChanged)
    Q_PROPERTY(QString v2rayAssetsPath READ v2rayAssetsPath WRITE setV2rayAssetsPath NOTIFY settingsChanged)
    Q_PROPERTY(bool kernelApiEnabled READ kernelApiEnabled WRITE setKernelApiEnabled NOTIFY settingsChanged)
    Q_PROPERTY(int statsPort READ statsPort WRITE setStatsPort NOTIFY settingsChanged)
    Q_PROPERTY(int pacMode READ pacMode WRITE setPacMode NOTIFY pacModeChanged)
    Q_PROPERTY(bool tunEnabled READ tunEnabled WRITE setTunEnabled NOTIFY settingsChanged)
    Q_PROPERTY(QString tunIpv4 READ tunIpv4 WRITE setTunIpv4 NOTIFY settingsChanged)
    Q_PROPERTY(int tunMtu READ tunMtu WRITE setTunMtu NOTIFY settingsChanged)
    Q_PROPERTY(bool tunAvailable READ tunAvailable NOTIFY settingsChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY settingsChanged)
    Q_PROPERTY(int themeMode READ themeMode WRITE setThemeMode NOTIFY themeModeChanged)
    Q_PROPERTY(bool systemDark READ systemDark NOTIFY systemThemeChanged)
    Q_PROPERTY(QString versionString READ versionString CONSTANT)

  public:
    explicit PlumbumQMLProperty(QObject *parent = nullptr);
    ~PlumbumQMLProperty();

    ConnectionListModel *connectionModel() { return &_connectionModel; }
    GroupListModel *groupModel() { return &_groupModel; }

    QString currentGroupId() const { return _currentGroupId.toString(); }
    void setCurrentGroupId(const QString &id);

    bool connected() const { return _connected; }
    QString connectedConnectionId() const { return _connectedConnectionId.toString(); }
    QString connectedName() const { return _connectedName; }
    QString upSpeedText() const { return _upSpeedText; }
    QString downSpeedText() const { return _downSpeedText; }
    QString upTotalText() const { return _upTotalText; }
    QString downTotalText() const { return _downTotalText; }
    QString kernelStatusText() const { return _kernelStatusText; }

    // Settings (GlobalConfig)
    QString v2rayCorePath() const { return GlobalConfig.kernelConfig.KernelPath(); }
    Q_INVOKABLE void setV2rayCorePath(const QString &path);
    QString v2rayAssetsPath() const { return GlobalConfig.kernelConfig.AssetsPath(); }
    Q_INVOKABLE void setV2rayAssetsPath(const QString &path);
    bool kernelApiEnabled() const { return GlobalConfig.kernelConfig.enableAPI; }
    Q_INVOKABLE void setKernelApiEnabled(bool enabled);
    int statsPort() const { return GlobalConfig.kernelConfig.statsPort; }
    Q_INVOKABLE void setStatsPort(int port);
    // PAC routing mode (0=whitelist, 1=blacklist, 2=global)
    int pacMode() const { return GlobalConfig.defaultRouteConfig.connectionConfig.pacMode; }
    Q_INVOKABLE void setPacMode(int mode);
    // TUN system-level proxy
    bool tunEnabled() const { return GlobalConfig.inboundConfig.tunSettings.enabled; }
    Q_INVOKABLE void setTunEnabled(bool enabled);
    QString tunIpv4() const { return GlobalConfig.inboundConfig.tunSettings.ipv4; }
    Q_INVOKABLE void setTunIpv4(const QString &ip);
    int tunMtu() const { return GlobalConfig.inboundConfig.tunSettings.mtu; }
    Q_INVOKABLE void setTunMtu(int mtu);
    // Whether the current process can create TUN interfaces (root / CAP_NET_ADMIN).
    bool tunAvailable() const;
    // Theme: 0=follow system, 1=light, 2=dark
    int themeMode() const { return GlobalConfig.uiConfig.themeMode; }
    Q_INVOKABLE void setThemeMode(int mode);
    // Whether the OS is currently in dark mode.
    bool systemDark() const;
    // Real version from build (makespec/VERSION), e.g. 1.0.0
    QString versionString() const { return QString(PLUMBUM_VERSION_STRING); }
    // Language (locale code, e.g. en_US, zh_CN, zh_TW, ru_RU)
    QString language() const { return GlobalConfig.uiConfig.language; }
    Q_INVOKABLE void setLanguage(const QString &code);
    QStringList availableLanguages() const;

  public:
    // Call this once, after ConnectionManager has been created.
    void initialize();

  public slots:
    // Connectivity
    void connectConnection(const QString &connectionId);
    void disconnectConnection();
    void restartConnection();
    void startLatencyTest();
    void startLatencyTestFor(const QString &connectionId);

    // Import & Create
    bool importFromClipboard();
    bool importFromLink(const QString &link);
    QString createGroup(const QString &name);
    void deleteGroup(const QString &groupId);
    void renameGroup(const QString &groupId, const QString &newName);
    void deleteConnection(const QString &connectionId);
    void moveConnectionToGroup(const QString &connectionId, const QString &groupId);
    QString connectionJson(const QString &connectionId) const;
    bool updateConnectionJson(const QString &connectionId, const QString &json);

    // Subscriptions
    void updateSubscription(const QString &groupId);
    void updateAllSubscriptions();
    void setSubscriptionInterval(const QString &groupId, double days);
    QString createSubscription(const QString &name, const QString &url);

    // Misc
    QString groupDisplayName(const QString &groupId) const;
    QString connectionDisplayName(const QString &connectionId) const;
    QString connectionProtocol(const QString &connectionId) const;
    QString connectionAddress(const QString &connectionId) const;
    int connectionPort(const QString &connectionId) const;
    void clearConnectionUsage(const QString &connectionId);
    void clearGroupUsage(const QString &groupId);
    void copyConnectionLink(const QString &connectionId);

  signals:
    void currentGroupIdChanged();
    void connectivityChanged();
    void statsChanged();
    void kernelStatusChanged();
    void settingsChanged();
    void pacModeChanged();
    void themeModeChanged();
    void systemThemeChanged();
    void toastMessage(const QString &message);
    void logMessage(const QString &message);

  private slots:
    void onConnectionCreated(const ConnectionGroupPair &id, const QString &displayName);
    void onConnectionModified(const ConnectionId &id);
    void onConnectionRenamed(const ConnectionId &id, const QString &originalName, const QString &newName);
    void onConnectionRemovedFromGroup(const ConnectionGroupPair &pairId);
    void onConnectionLinkedWithGroup(const ConnectionGroupPair &newPair);
    void onGroupCreated(const GroupId &id, const QString &displayName);
    void onGroupRenamed(const GroupId &id, const QString &oldName, const QString &newName);
    void onGroupDeleted(const GroupId &id, const QList<ConnectionId> &connections);
    void onLatencyTestFinished(const ConnectionId &id, const int average);
    void onConnected(const ConnectionGroupPair &id);
    void onDisconnected(const ConnectionGroupPair &id);
    void onKernelCrashed(const ConnectionGroupPair &id, const QString &errMessage);
    void onStatsAvailable(const ConnectionGroupPair &id, const QMap<StatisticsType, QvStatsSpeedData> &data);
    void onKernelLog(const ConnectionGroupPair &id, const QString &log);
    void onSubscriptionUpdated(const GroupId &id, bool success);

  private:
    void refreshAll();
    void updateConnectivityState();

    ConnectionListModel _connectionModel;
    GroupListModel _groupModel;
    GroupId _currentGroupId;
    bool _connected = false;
    ConnectionGroupPair _connectedPair;
    ConnectionId _connectedConnectionId;
    QString _connectedName;
    //
    QString _upSpeedText;
    QString _downSpeedText;
    QString _upTotalText;
    QString _downTotalText;
    QString _kernelStatusText;
};
