#pragma once
#include "QvAutoCompleteTextEdit.hpp"
#include "base/PlumbumBase.hpp"
#include "ui_RouteSettingsMatrix.h"

#include <QMenu>
#include <QWidget>
#include <optional>

class RouteSettingsMatrixWidget
    : public QWidget
    , private Ui::RouteSettingsMatrix
{
    Q_OBJECT

  public:
    RouteSettingsMatrixWidget(const QString &assetsDirPath, QWidget *parent = nullptr);
    void SetRouteConfig(const QvConfig_Route &conf);
    QvConfig_Route GetRouteConfig() const;
    ~RouteSettingsMatrixWidget();

  private:
    std::optional<QString> openFileDialog();
    std::optional<QString> saveFileDialog();
    QList<QAction *> getBuiltInSchemes();
    QAction *schemeToAction(const QString &name, const QvConfig_Route &scheme);

  private:
    QMenu *builtInSchemesMenu;

  private slots:
    void on_importSchemeBtn_clicked();
    void on_exportSchemeBtn_clicked();

  private:
    const QString &assetsDirPath;

  private:
    Plumbum::ui::widgets::AutoCompleteTextEdit *directDomainTxt;
    Plumbum::ui::widgets::AutoCompleteTextEdit *proxyDomainTxt;
    Plumbum::ui::widgets::AutoCompleteTextEdit *blockDomainTxt;
    //
    Plumbum::ui::widgets::AutoCompleteTextEdit *directIPTxt;
    Plumbum::ui::widgets::AutoCompleteTextEdit *blockIPTxt;
    Plumbum::ui::widgets::AutoCompleteTextEdit *proxyIPTxt;
};
