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
        ConnectionCountRole
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

    // Subscriptions
    void updateSubscription(const QString &groupId);
    void updateAllSubscriptions();

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
    void onSubscriptionUpdated(const GroupId &id);

  private:
    void refreshAll();
    void updateConnectivityState();

    ConnectionListModel _connectionModel;
    GroupListModel _groupModel;
    GroupId _currentGroupId;
    bool _connected = false;
    ConnectionGroupPair _connectedPair;
    QString _connectedName;
    //
    QString _upSpeedText;
    QString _downSpeedText;
    QString _upTotalText;
    QString _downTotalText;
    QString _kernelStatusText;
};
