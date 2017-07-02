#pragma once

#include <QWidget>

#include "Common/StoryTypesNodeCollector.hpp"

class StoryNodeItem;

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

    void setCurrentNodeItem(StoryNodeItem* node);

protected:
    virtual void hideEvent(QHideEvent* event) override;

signals:
    void signalNodeInfoUpdated(int nodeId);

private slots:
    void slotUpdateNodeData();

private:
    void initialization();
    void updateUI();
    void blockUISignals();
    void unblockUISignals();


private:
    QScopedPointer<Ui::NodeInfoWidget> m_ui;

    StoryTypesNodeCollector m_typesNodeCollector;
    StoryNodeItem* m_currentNodeItem;
};
