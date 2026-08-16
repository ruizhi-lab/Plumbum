set(PLUMBUM_QWIDGETS_UI_BASEDIR ${CMAKE_SOURCE_DIR}/src/ui/widgets)
add_definitions(-DQAPPLICATION_CLASS=QApplication)

set(_PLUMBUM_UI_FORMS
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_OutboundEditor.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_InboundEditor.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_JsonEditor.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_RoutesEditor.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_ChainSha256Editor.ui
    #
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/StreamSettingsWidget.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/ConnectionInfoWidget.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/ConnectionItemWidget.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/RouteSettingsMatrix.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/InboundSettingsWidget.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/ConnectionSettingsWidget.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/DnsSettingsWidget.ui
    #
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_GroupManager.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_ImportConfig.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_MainWindow.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_PreferencesWindow.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_PluginManager.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_ScreenShot_Core.ui
    #
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/complex/ChainEditorWidget.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/complex/RoutingEditorWidget.ui
    )

set(_PLUMBUM_UI_NODEEDITOR_SOURCES
    # NodeEditor Models
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/NodeBase.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/NodeBase.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/NodeDispatcher.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/NodeDispatcher.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/models/InboundNodeModel.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/models/InboundNodeModel.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/models/OutboundNodeModel.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/models/OutboundNodeModel.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/models/RuleNodeModel.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/models/RuleNodeModel.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/models/ChainOutboundNodeModel.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/models/ChainOutboundNodeModel.hpp
    # NodeEditor Widgets
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/RuleWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/RuleWidget.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/RuleWidget.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/InboundOutboundWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/InboundOutboundWidget.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/InboundOutboundWidget.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/ChainWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/ChainWidget.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/ChainWidget.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/BalancerWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/BalancerWidget.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/BalancerWidget.ui
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/ChainOutboundWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/ChainOutboundWidget.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/node/widgets/ChainOutboundWidget.ui
    )

set(_PLUMBUM_UI_SOURCES
    # Style Manager
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/PlumbumWidgetApplication.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/PlumbumWidgetApplication.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/common/WidgetUIBase.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/common/QJsonModel.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/common/QJsonModel.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/styles/StyleManager.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/styles/StyleManager.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/styles/StyleManager.hpp
    # Models
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/models/ConnectionModelHelper.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/models/ConnectionModelHelper.hpp
    # UI Widgets
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/ConnectionInfoWidget.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/ConnectionInfoWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/QvAutoCompleteTextEdit.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/QvAutoCompleteTextEdit.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/RouteSettingsMatrix.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/RouteSettingsMatrix.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/ConnectionSettingsWidget.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/ConnectionSettingsWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/ConnectionItemWidget.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/ConnectionItemWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/StreamSettingsWidget.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/StreamSettingsWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/InboundSettingsWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/InboundSettingsWidget.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/DnsSettingsWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/DnsSettingsWidget.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/CertificateItemWidget.ui
    # Complex Widgets
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/complex/ChainEditorWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/complex/ChainEditorWidget.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/complex/RoutingEditorWidget.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/widgets/complex/RoutingEditorWidget.hpp
    # Editors
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_InboundEditor.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_InboundEditor.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_JsonEditor.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_JsonEditor.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_OutboundEditor.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_OutboundEditor.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_RoutesEditor.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_RoutesEditor.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_ChainSha256Editor.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/editors/w_ChainSha256Editor.cpp
    # Windows
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_ImportConfig.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_ImportConfig.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_MainWindow.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_MainWindow.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_MainWindow_extra.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_PreferencesWindow.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_PreferencesWindow.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_PluginManager.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_PluginManager.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_ScreenShot_Core.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_ScreenShot_Core.cpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_GroupManager.hpp
    ${PLUMBUM_QWIDGETS_UI_BASEDIR}/windows/w_GroupManager.cpp
    )

set(PLUMBUM_UI_WIDGET_SOURCES ${_PLUMBUM_UI_FORMS} ${_PLUMBUM_UI_SOURCES} ${_PLUMBUM_UI_NODEEDITOR_SOURCES})
