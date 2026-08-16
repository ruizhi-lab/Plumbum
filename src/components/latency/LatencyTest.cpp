#include "LatencyTest.hpp"

#include "LatencyTestThread.hpp"
#include "core/handler/ConfigHandler.hpp"

namespace Plumbum::components::latency
{
    LatencyTestHost::LatencyTestHost(const int defaultCount, QObject *parent) : QObject(parent)
    {
        qRegisterMetaType<ConnectionId>();
        qRegisterMetaType<LatencyTestResult>();
        totalTestCount = defaultCount;
        latencyThread = new LatencyTestThread(this);
        latencyThread->start();
    }

    LatencyTestHost::~LatencyTestHost()
    {
        latencyThread->stopLatencyTest();
        latencyThread->wait();
    }

    void LatencyTestHost::StopAllLatencyTest()
    {
        latencyThread->stopLatencyTest();
        latencyThread->wait();
        latencyThread->start();
    }

    void LatencyTestHost::TestLatency(const ConnectionId &id, PlumbumLatencyTestingMethod method)
    {
        latencyThread->pushRequest(id, totalTestCount, method);
    }
    void LatencyTestHost::TestLatency(const QList<ConnectionId> &ids, PlumbumLatencyTestingMethod method)
    {
        latencyThread->pushRequest(ids, totalTestCount, method);
    }

} // namespace Plumbum::components::latency
