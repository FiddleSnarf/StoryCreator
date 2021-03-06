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
        NodeNavigationInfo() :
            nodeItemPtr(nullptr),
            isVisible(false)
        {}

        StoryNodeItem* nodeItemPtr;
        bool isVisible;
    };
    typedef QMap<int, NodeNavigationInfo> NodeNavigationInfoMap;

public:
    explicit StoryNavigationWidget(QWidget* parent = Q_NULLPTR);
    virtual ~StoryNavigationWidget();

    void setCore(ICorePtr core);

signals:
    void signalDeleteKeyPressed();
    void signalSearchTextChanged(const QString& requiredText);

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
    void appendNodeRow(int nodeId, const QString& nodeTitle, bool nodeIsValid, bool isVisible);
    void refillNodeTable();
    QColor getColorForNodeRow(bool isValid) const;
    void checkVisibleNaviItem(NodeNavigationInfo& item, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    void checkVisibleNaviItem(NodeNavigationInfo& item, const QString& searchText, bool textIsInteger, Qt::CaseSensitivity cs = Qt::CaseSensitive);

    QScopedPointer<Ui::StoryNavigationWidget> m_ui;
    ICorePtr m_core;

    NodeNavigationInfoMap m_nodeNaviInfoMap;
    int m_currentSelectedNodeId;
};
