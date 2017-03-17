#pragma once

#include <QGraphicsObject>
#include <QIcon>

#include "Common/NodeInfo.hpp"

class StoryNodeItem : public QGraphicsObject
{
public:
    enum { Type = UserType + 1 };
    int type() const override{return Type;}

public:
    explicit StoryNodeItem(int nodeID, const QString& typeNode, QGraphicsItem* parent = Q_NULLPTR);
    ~StoryNodeItem();

    void setIcon(const QIcon& icon);
    StoryCommon::NodeInfo& getNodeInfo();
    StoryCommon::NodeInfo getNodeInfo() const;

private:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    QRectF m_boundingRect;
    QPen m_pen;
    QIcon m_icon;

    StoryCommon::NodeInfo m_nodeInfo;
};
