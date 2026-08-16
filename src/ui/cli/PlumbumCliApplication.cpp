#include "PlumbumCliApplication.hpp"

PlumbumCliApplication::PlumbumCliApplication(int &argc, char *argv[]) : PlumbumPlatformApplication(argc, argv)
{
}

QStringList PlumbumCliApplication::checkPrerequisitesInternal()
{
    return {};
}
PlumbumExitReason PlumbumCliApplication::runPlumbumInternal()
{
    return (PlumbumExitReason) exec();
}

void PlumbumCliApplication::terminateUIInternal()
{
}

#ifndef PLUMBUM_NO_SINGLEAPPLICATON
void PlumbumCliApplication::onMessageReceived(quint32 clientID, QByteArray msg)
{
}
#endif
