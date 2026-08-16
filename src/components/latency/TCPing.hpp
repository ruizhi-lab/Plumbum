#pragma once
#include "DNSBase.hpp"
#include "LatencyTest.hpp"
#include "base/PlumbumBase.hpp"

#include <type_traits>
namespace Plumbum::components::latency::tcping
{
    class TCPing : public DNSBase<TCPing>
    {
      public:
        using DNSBase<TCPing>::DNSBase;
        void start();
        ~TCPing() override;

      protected:
      private:
        void ping() override;
        void notifyTestHost();
    };
} // namespace Plumbum::components::latency::tcping
