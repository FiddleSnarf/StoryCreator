#pragma once

#include <QGraphicsObject>
#include <QIcon>
#include <QSize>
#include <QPen>
#include <QTimeLine>
#include <QUuid>

#include "JsonStoryHelper/StoryNode.h"


/** \class StoryNodeItem
 *  \brief Класс описывает ноду с графической точки зрения, так же хранит информацию
 *         в том виде как она представлена в json.
 */
class StoryNodeItem : public QGraphicsObject
{
    Q_OBJECT

    // Параметры для мигания
    static const int RED_MIN    = 180;
    static const int RED_MAX    = 255;
    static const int GREEN_MIN  = 130;
    static const int GREEN_MAX  = 190;
    static const int GREEN_MIN_HEAD  = 25;
    static const int GREEN_MAX_HEAD  = 125;
    static const int FRAMES     = 30;
    static const int TIME_LINE  = 600;

    static const int SELECTED_NODE_FRAME_WIDTH = 7; /**< Толщина рамки выделенного нода. */

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

    /** \brief Устанавливает статус нода как "выбранный".
     */
    void setNodeSelection(bool state);

    /** \brief Получить GUID нода.
     */
    const QUuid& getNodeGUID();

private:
    /** \brief Инициализация начальных параметров.
     */
    void initialization();

    /** \brief Инициализация параметров пульсации при выделении.
     */
    void initSelectedPulse();

    /** \brief Инициализация параметров пульсации при ошибке.
     */
    void initErrorPulse();

    /** \brief Устанавливает цвет рамки нода по умолчанию.
     */
    void setDefaultPen();

    /** \brief Устанавливает заливку нода по умолчанию.
     */
    void setDefaultBrush();

    /** \brief Переопределенный метод возвращающий область для перерисовки (весь айтем должен быть нарисован внутри этой области).
     */
    QRectF boundingRect() const override;

    /** \brief Переопределенный метод отрисовки нода.
     */
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    QPen m_borderPen;       /**< Ручка для отрисовки нода. */
    QBrush m_brush;         /**< Заливка фона нода. */

    QRectF m_boundingRect;  /**< Перерисовываемая область. */
    QIcon m_icon;           /**< Иконка. */
    StoryNode m_nodeInfo;   /**< Информация о ноде в том виде в котором она хранится в json. */

    QTimeLine m_selectedTimeline;   /**< Для пульсации рамки нода во время выделения */
    QTimeLine m_errorTimeline;      /**< Для пульсации рамки нода в случае ошибки */

    QUuid m_guid;                   /**< Совсем уникальный идентификатор нода */
};

typedef class StoryNodeItem* StoryNodeItemPtr;
typedef QList<StoryNodeItemPtr> StoryNodeItemList;
