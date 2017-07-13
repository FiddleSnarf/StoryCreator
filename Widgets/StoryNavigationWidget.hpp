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

    struct NodeNavigationInfo
    {
        NodeNavigationInfo() : nodeItemPtr(nullptr)
        {}

        void updateInfo()
        {
            if (nodeItemPtr)
            {
                title = nodeItemPtr->getNodeInfo().getTitle();
                text = nodeItemPtr->getNodeInfo().getText();
            }
        }

        StoryNodeItem* nodeItemPtr;
        QString title;
        QString text;
    };
    typedef QMap<int, NodeNavigationInfo> NodeNavigationInfoMap;

public:
    explicit StoryNavigationWidget(QWidget* parent = Q_NULLPTR);
    virtual ~StoryNavigationWidget();

    void setCore(ICorePtr core);

signals:
    void signalDeleteKeyPressed();

protected:
    /** \brief Обработка событий нажатия клавиш клавиатуры
     */
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void slotStoryStateChanged(bool state);
    void slotItemSelectedChanged(bool state, StoryNodeItem* selectedNode);
    void slotNavigationItemClicked(class QTableWidgetItem* item);
    void slotUserAddedNode(StoryNodeItem* addedNode);
    void slotUserDeletedNode(int nodeID);
    void slotUserEditedNode(int nodeID);
    void slotSearchTextChanged(const QString& searchText);

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

    NodeNavigationInfoMap m_nodeNaviInfoMap;
    int m_currentSelectedNodeId;
};
