#pragma once
#include "base/PlumbumBase.hpp"
#include "core/kernel/QvKernelABIChecker.hpp"

class QProcess;

namespace Plumbum::core::kernel
{
    class APIWorker;
    class V2RayKernelInstance : public QObject
    {
        Q_OBJECT
      public:
        explicit V2RayKernelInstance(QObject *parent = nullptr);
        ~V2RayKernelInstance() override;
        //
        std::optional<QString> StartConnection(const CONFIGROOT &root);
        void StopConnection();
        bool IsKernelRunning() const
        {
            return kernelStarted;
        }
        //
        static std::optional<QString> ValidateConfig(const QString &path);
        static std::pair<bool, std::optional<QString>> ValidateKernel(const QString &vCorePath, const QString &vAssetsPath);
#if PLUMBUM_FEATURE(kernel_check_permission)
        static std::pair<bool, std::optional<QString>> CheckAndSetCoreExecutableState(const QString &vCorePath);
#endif

      signals:
        void OnProcessErrored(const QString &errMessage);
        void OnProcessOutputReadyRead(const QString &output);
        void OnNewStatsDataArrived(const QMap<StatisticsType, QvStatsSpeed> &data);

      private:
        APIWorker *apiWorker;
        QProcess *vProcess;
        bool apiEnabled;
        bool kernelStarted = false;
    };
} // namespace Plumbum::core::kernel

using namespace Plumbum::core::kernel;
