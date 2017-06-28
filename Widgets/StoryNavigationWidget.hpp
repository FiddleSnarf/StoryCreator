#pragma once

#include <QWidget>
#include "Core/ICore.hpp"
#include "Items/StoryNodeItemImpl.hpp"

namespace Ui
{
    class StoryNavigationWidget;
}

class StoryNodeItem;

class StoryNavigationWidget : public QWidget
{
    Q_OBJECT

    enum EnNodeColumns
    {
        enIdColumn = 0,
        enTitleColumn,

        enCountColumn
    };

    typedef QPair<QString, bool> NodeRowInfo;
    typedef QMap<int, NodeRowInfo> NodeRowMap;

public:
    explicit StoryNavigationWidget(QWidget* parent = Q_NULLPTR);
    virtual ~StoryNavigationWidget();

    void setCore(ICorePtr core);

private slots:
    void slotStoryStateChanged(bool state);
    void slotItemSelectedChanged(bool state, StoryNodeItem* selectedNode);
    void slotNavigationItemClicked(class QTableWidgetItem* item);
    void slotUserAddedNode(int nodeID);
    void slotUserDeletedNode(int nodeID);

private:
    void initialization();
    void initConnects();
    void clearSelection();
    void clearNodeTable();
    void appendNodeRow(int nodeId, const QString& nodeTitle, bool nodeIsValid);
    void refillNodeTable();
    QColor getColorForNodeRow(bool isValid) const;

    QScopedPointer<Ui::StoryNavigationWidget> m_ui;
    ICorePtr m_core;

    NodeRowMap m_nodeRowMap;
    int m_currentSelectedNodeId;
};
