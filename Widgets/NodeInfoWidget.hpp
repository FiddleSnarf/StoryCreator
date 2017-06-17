#pragma once

#include <QWidget>

namespace Ui
{
    class NodeInfoWidget;
}

class NodeInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NodeInfoWidget(QWidget* parent = Q_NULLPTR);
    virtual ~NodeInfoWidget();

private:
    QScopedPointer<Ui::NodeInfoWidget> m_ui;
};
