#pragma once

#include <QGraphicsObject>
#include <QIcon>
#include <QSize>
#include <QPen>

#include "JsonStoryHelper/StoryNode.h"


/** \class StoryNodeItem
 *  \brief Класс описывает ноду с графической точки зрения, так же хранит информацию
 *         в том виде как она представлена в json.
 */
class StoryNodeItem : public QGraphicsObject
{
    Q_OBJECT

public:
    enum { Type = UserType + 1 };
    int type() const override{return Type;}

public:
    explicit StoryNodeItem(const StoryNode& nodeInfo, QGraphicsItem* parent = Q_NULLPTR);
    explicit StoryNodeItem(int nodeID, const QString& typeNode, QGraphicsItem* parent = Q_NULLPTR);
    virtual ~StoryNodeItem();

    /** \brief Устанавливает иконку.
     */
    void setIcon(const QIcon& icon);

    /** \brief Получить информацию о ноде.
     */
    StoryNode& getNodeInfo();
    const StoryNode& getNodeInfo() const;

    /** \brief Говорит о том является ли нод головным.
     */
    bool isHeadNode() const;

signals:
    /** \brief Сигнал испускается при выделении нода.
     */
    void signalSelected();

protected:
    /** \brief Переопределенное событие нажатия мыши.
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    /** \brief Инициализация начальных параметров.
     */
    void initialization();

    /** \brief Переопределенный метод возвращающий область для перерисовки (весь айтем должен быть нарисован внутри этой области).
     */
    QRectF boundingRect() const override;

    /** \brief Переопределенный метод отрисовки нода.
     */
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    QPen m_borderPen;       /**< Ручка для отрисовки нода. */ // TODO
    QRectF m_boundingRect;  /**< Перерисовываемая область. */
    QIcon m_icon;           /**< Иконка. */
    StoryNode m_nodeInfo;   /**< Информация о ноде в том виде в котором она хранится в json. */
};

typedef class StoryNodeItem* StoryNodeItemPtr;
typedef QList<StoryNodeItemPtr> StoryNodeItemList;
