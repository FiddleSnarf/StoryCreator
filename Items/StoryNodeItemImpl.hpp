#pragma once

#include <QGraphicsObject>
#include <QIcon>
#include <QSize>

#include "JsonStoryHelper/StoryNode.h"

class StoryNodeItem : public QGraphicsObject
{
public:
    enum { Type = UserType + 1 };
    int type() const override{return Type;}

public:
    explicit StoryNodeItem(const StoryNode& nodeInfo, QGraphicsItem* parent = Q_NULLPTR);
    explicit StoryNodeItem(int nodeID, const QString& typeNode, QGraphicsItem* parent = Q_NULLPTR);
    ~StoryNodeItem();

    void setIcon(const QIcon& icon);
    StoryNode& getNodeInfo();
    const StoryNode& getNodeInfo() const;

private:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    bool isHeadNode() const;

private:
    QRectF m_boundingRect;
    QIcon m_icon;

    StoryNode m_nodeInfo;   /**< Информация о ноде в том виде в котором она хранится в json. */
};

typedef class StoryNodeItem* StoryNodeItemPtr;
typedef QList<StoryNodeItemPtr> StoryNodeItemList;
