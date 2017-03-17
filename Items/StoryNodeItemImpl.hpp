#pragma once

#include <QGraphicsObject>
#include <QIcon>

class StoryNodeItem : public QGraphicsObject
{
public:
    enum { Type = UserType + 1 };
    int type() const override{return Type;}

public:
    explicit StoryNodeItem(QGraphicsItem* parent = Q_NULLPTR);
    ~StoryNodeItem();

    void setID(const int& id);
    void setIcon(const QIcon& icon);
    void setType(const QString& type);
    void setTitle(const QString& title);
    void setText(const QString& text);
    void setEntryPointFlag(bool state);

private:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    QRectF m_boundingRect;
    QPen m_pen;
    bool m_isEntryPoint;

    int m_nodeID;
    QIcon m_icon;
    QString m_typeNode;
    QString m_title;
    QString m_text;
};
