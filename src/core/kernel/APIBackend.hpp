#pragma once
#include "base/PlumbumBase.hpp"
#include "v2ray_api.grpc.pb.h"

#include <grpc++/grpc++.h>

#include <atomic>
#include <mutex>

// Check 10 times before telling user that API has failed.
constexpr auto PLUMBUM_API_CALL_FAILEDCHECK_THRESHOLD = 30;

namespace Plumbum::core::kernel
{
    struct APIConfigObject
    {
        QString protocol;
        StatisticsType type;
    };

    typedef std::map<QString, APIConfigObject> QvAPITagProtocolConfig;
    typedef std::map<StatisticsType, QStringList> QvAPIDataTypeConfig;

    class APIWorker : public QObject
    {
        Q_OBJECT

      public:
        APIWorker();
        ~APIWorker();
        void StartAPI(const QMap<bool, QMap<QString, QString>> &tagProtocolPair);
        void StopAPI();

      signals:
        void onAPIDataReady(const QMap<StatisticsType, QvStatsSpeed> &data);
        void OnAPIErrored(const QString &err);

      private slots:
        void process();

      private:
        qint64 CallStatsAPIByName(const QString &name);
        QvAPITagProtocolConfig tagProtocolConfig;
        mutable std::mutex stateMutex;
        QThread *workThread;
        //
        std::atomic_bool started = false;
        std::atomic_bool running = false;
        std::shared_ptr<::grpc::Channel> grpc_channel;
        std::unique_ptr<::v2ray::core::app::stats::command::StatsService::Stub> stats_service_stub;
    };
} // namespace Plumbum::core::kernel

using namespace Plumbum::core::kernel;
