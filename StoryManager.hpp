#pragma once

#include <QObject>
#include "Models/SelectNodeModel.hpp"
#include "Scene/StoryScene.hpp"


/** \class StoryManager
 *  \brief Менеджер, предоставляет весь необходимый интерфейс для работы с историей.
 */
class StoryManager : public QObject
{
    Q_OBJECT

    static const QString DEF_STORY_FILE_NAME;   /**< Дефолтное имя истории. */

public:
    StoryManager(StoryTypesNodeCollector& collector, QObject* parent = Q_NULLPTR);
    virtual ~StoryManager();

    /** \brief Возвращает указатель на сцену
     */
    StoryScenePtr getStoryScene() const;

    /** \brief Возвращает указатель на модель добавления довых нодов
     */
    SelectNodeModelPtr getStoryNodeSelectModel() const;

    /** \brief Возвращает текущее количество нодов
     */
    int getCountStoryNodes() const;

    /** \brief Проверяет открыта ли сейчас история
     */
    bool isStoryOpen() const;

    /** \brief Проверяет наличие у истории пути к файлу на диске (У новых историй, созданных через редактор, его нет)
     */
    bool isStoryBeLoaded() const;

    /** \brief Получить имя текущей истории
     */
    const QString& getCurrentStoryName() const;

    /** \brief Получить список нодов.
     */
    const StoryNodeItemList& getStoryNodeList() const;

    /** \brief Получить указатель на итем нода по ID.
     */
    StoryNodeItemPtr getNodeItemForID(int nodeId) const;

    /** \brief Получить указатель на выделенный итем нода.
     *  \return В случае если нет выделенного нода вернет nullptr.
     */
    StoryNodeItemPtr getSelectedNodeItem() const;

    /** \brief Выделить нод с таким ID.
     *  \param [in] centerOn - Отцентрировать вьюху относительно нода.
     */
    void selectNodeForID(int nodeId, bool centerOn = false);

signals:
    /** \brief Сигнал оповещает об изменении состояния истории (true - открыта, false - закрыта)
     */
    void signalStoryStateChanged(bool state);

    /** \brief Сигнал испускается при добавлении/удалении нодов.
     */
    void signalCountStoryNodesChanged();

    /** \brief Сигнал испускается при выделении ноды.
     */
    void signalItemSelectedChanged(bool state, StoryNodeItem* selectedNode);

public slots:
    /** \brief Слот вызывается при нажатии кнопки "Создать новую историю"
     */
    void slotCreateNewStory();

    /** \brief Слот вызывается при нажатии кнопки "Загрузить историю"
     */
    void slotLoadStory();

    /** \brief Слот вызывается при нажатии кнопки "Закрыть текущую историю"
     */
    void slotCloseStory();

    /** \brief Слот вызывается при нажатии кнопки "Сохранить историю"
     */
    void slotSaveStory();

    /** \brief Слот вызывается при нажатии кнопки "Сохранить историю как ..."
     */
    void slotSaveAsStory();

    /** \brief Слот вызывается при изменении имени(названия) истории
     */
    void slotUpdateStoryName(const QString& storyName);

private:
    /** \brief Инициализация начальными параметрами
     */
    void initialization();

    /** \brief Сохранить либо указанную историю, либо текущую
     */
    void saveStory(StoryCommon::StoryInfo* updatedStoryPtr = Q_NULLPTR);

    /** \brief Получить данные истории полученные в процессе редактирования
     */
    void fillUpdatedStoryInfo(StoryCommon::StoryInfo& updatedStory) const;

    /** \brief Коннектимся к сцене
     */
    void connectToScene();

private:
    StoryTypesNodeCollector& m_typesCollector;  /**< Ссылка на класс хранящий информацию о типах нодов(и экшенов). */
    StoryScenePtr m_storyScene;                 /**< Указатель на сцену. */
    SelectNodeModelPtr m_storyNodeSelectModel;  /**< Указатель на модель выбора новых нодов. */

    StoryCommon::StoryInfo m_currentStoryInfo;  /**< Указатель на сцену. */
    QString m_storyName;                        /**< Имя(название) текущей истории. */
    bool m_isStoryOpen;                         /**< Флаг, открыта ли история. */
    bool m_isLoadedStory;                       /**< Имеет ли текущая история путь к файлу для сохранения. */
};
