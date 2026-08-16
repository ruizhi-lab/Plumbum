#pragma once
#include "base/PlumbumBase.hpp"
namespace uvw
{
    class Loop;
}
struct sockaddr_storage;
namespace Plumbum::components::latency
{
    class LatencyTestThread;
    struct LatencyTestResult
    {
        QString errorMessage;
        int totalCount;
        int failedCount;
        long worst = LATENCY_TEST_VALUE_ERROR;
        long best = LATENCY_TEST_VALUE_ERROR;
        long avg = LATENCY_TEST_VALUE_ERROR;
        PlumbumLatencyTestingMethod method;
    };
    struct LatencyTestRequest
    {
        ConnectionId id;
        QString host;
        int port;
        int totalCount;
        PlumbumLatencyTestingMethod method;
    };

    class LatencyTestHost : public QObject
    {
        Q_OBJECT
      public:
        explicit LatencyTestHost(const int defaultCount = 3, QObject *parent = nullptr);
        void TestLatency(const ConnectionId &connectionId, PlumbumLatencyTestingMethod);
        void TestLatency(const QList<ConnectionId> &connectionIds, PlumbumLatencyTestingMethod);
        void StopAllLatencyTest();

        ~LatencyTestHost() override;

      signals:
        void OnLatencyTestCompleted(ConnectionId id, LatencyTestResult data);

      private:
        int totalTestCount;
        // we're not introduce multi latency test thread for now,
        // cause it's easy to use a scheduler like round-robin scheme
        // and libuv event loop is fast.
        LatencyTestThread *latencyThread;
    };
} // namespace Plumbum::components::latency

using namespace Plumbum::components::latency;
Q_DECLARE_METATYPE(LatencyTestResult)
