#pragma once

#include <QGraphicsScene>
#include <QList>
#include <set>

#include "Items/StoryNodeItemImpl.hpp"
#include "Common/StoryCommon.hpp"
#include "Common/StoryTypesNodeCollector.hpp"

class StoryNodeItem;

/** \class StoryScene
 *  \brief Класс главной сцены.
 */
class StoryScene : public QGraphicsScene
{
    Q_OBJECT

private:
    static const int NODE_Z_DEPTH = 2;  /**< Глубина слоя нодов. */
    static const int LINK_Z_DEPTH = 1;  /**< Глубина слоя связей между нодами. */

public:
    StoryScene(StoryTypesNodeCollector& collector, QObject* parent = Q_NULLPTR);
	~StoryScene();

    /** \brief Количество нодов на сцене.
     */
    int nodeCount() const;

    /** \brief Получить список нодов.
     */
    const StoryNodeItemList& getStoryNodeList() const;

    /** \brief Получить указатель на итем нода по ID.
     */
    StoryNodeItemPtr getNodeItemForID(int nodeId) const;

    /** \brief Инициализация сцены новой историей.
     */
    void initStoryInfo(const StoryCommon::StoryInfo& storyInfo);

    /** \brief Получение свободного ID для нода.
     */
    int getFreeID() const;

    /** \brief Выделить нод с таким ID.
     *  \param [in] centerOn - Отцентрировать вьюху относительно нода.
     */
    void selectNodeForID(int nodeId, bool centerOn = false);

    /** \brief Получить список выделенных нодов.
     */
    StoryNodeItemList getSelectedNodeItems() const;

public slots:

    /** \brief Очистка сцены.
     */
    void slotClearScene();

    /** \brief Удаление выделенных нодов.
     */
    void slotDeleteSelectedNode();

signals:

    /** \brief Сигнал испускается при выделении ноды.
     */
    void signalItemSelectedChanged(bool state, StoryNodeItem* selectedNode);

    /** \brief Сигнал испускается при добавлении нода.
     */
    void signalStoryNodeAdded(StoryNodeItem* addedNode);

    /** \brief Сигнал испускается при удалении нода.
     */
    void signalStoryNodeDeleted(int deletedNodeId);

    /** \brief Сигнал оповещает о том что данные нода были изменены.
     */
    void signalDataNodeChanged(int nodeId);

    /** \brief Сигнал оповещает о том что геометрия нода была изменена.
     */
    void signalGeometryNodeChanged(int nodeId);

protected:
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    virtual void dropEvent(QGraphicsSceneDragDropEvent* event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:

    /** \brief Добавить на сцену пустой шаблон нода.
     */
    bool addEmptyStoryNode(const QString& nodeType, const QIcon& icon, const QPointF& pos);

    /** \brief Добавить на сцену ноду.
     */
    bool addStoryNode(const StoryNode& nodeInfo, const QPointF& pos);

    /** \brief Выделить нод.
     */
    void selectNode(StoryNodeItem* node, bool centerOn = false);

private:
    StoryTypesNodeCollector& m_storyTypesNodeCollector;  /**< Класс хранящий информацию о возможных типах нодов. */
    std::set<int> m_idSet;                               /**< Множество выданных id-шников. */
    StoryNodeItemList m_nodesList;                       /**< Список нодов. */
};

typedef QSharedPointer<StoryScene> StoryScenePtr;
