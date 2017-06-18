#pragma once

#include <QString>
#include <QList>
#include <QIcon>
#include <QPen>
#include "JsonStoryHelper/StoryNode.h"

namespace StoryGUI
{
    const QString NODE_MIME_TYPE = "story/node-template"; /**< Для передачи параметров через drag&drop. */

    typedef QMap<int, QPointF> SceneNodePositionMap; /**< key - nodeID, value - координата на сцене. */

    /** \struct StoryAdditionalInfo
     *  \brief Структура описывает параметры отображения story на графической сцене.
     *  \note Эти дополнительные параметры будут храниться в отдельном файле.
     *        если при загрузке story файл с параметрами отображения не будет найден
     *        то размещение нодов на сцене произойдет по автоматическому алгоритму.
     */
    struct StoryAdditionalInfo
    {
        SceneNodePositionMap nodesPosMap;   /**< Позиция нодов на сцене. */

        /** \brief Проверка на валидность
         */
        bool isValid() const
        {
            return !nodesPosMap.isEmpty();
        }

        void clear()
        {
            nodesPosMap.clear();
        }
    };

    const int DEFAULT_NODE_WIDTH = 150;
    const int DEFAULT_NODE_HEIGHT = 150;
    const QRectF DEFAULT_NODE_RECT = QRect(0, 0, DEFAULT_NODE_WIDTH, DEFAULT_NODE_HEIGHT);
    const QPen NODE_PEN = QPen(QColor("#009966"), 2);           /**< Pen (в данном случае - цвет рамки) для нода. */
    const QPen HEAD_NODE_PEN = QPen(QColor("#7308D1"), 3);      /**< Pen (в данном случае - цвет рамки) для головного нода. */
    const QBrush NODE_BRUSH = QBrush(QColor("#D4FFEB"));        /**< Brush (в данном случае - заливка) для нода. */
    const QBrush HEAD_NODE_BRUSH = QBrush(QColor("#EFDAF5"));   /**< Brush (в данном случае - заливка) для головного нода. */

    const int DEFAULT_SCENE_WIDTH = 1200;
    const int DEFAULT_SCENE_HEIGHT = 1200;
}

namespace StoryCommon
{
    const QString CURR_JSON_VERSION = "1.0";
    const QString STORY_FILE_EXTENSION = "*.json";

    const int HEAD_NODE_ID = 0;     /**< ID головного нода. */
    const int ERROR_NODE_ID = -1;    /**< некорректный ID нода. */

    /** \struct StoryInfo
     *  \brief Структура описывает параметры ноды (в том виде в котором они хранятся в json).
     *  \note В конечном счете все сводится к заполнению и редактированию этих данных.
     */
    struct StoryInfo
    {
        QString version;        /**< Версия json story. */
        QString storyName;      /**< Название story. */
        QString filePath;       /**< Путь к файлу json. */
        StoryNodeList nodeList; /**< Список нодов. */
        StoryGUI::StoryAdditionalInfo additionalViewParams;  /**< Дополнительные параметры отображения. */

        StoryInfo()
        {

        }

        StoryInfo(const QString& _version)
        {
            version = _version;
        }

        void clear()
        {
            version.clear();
            storyName.clear();
            filePath.clear();
            nodeList.clear();
            additionalViewParams.clear();
        }

        /** \brief Проверка на валидность
         */
        bool isValid() const
        {
            return (!version.isEmpty());
        }
    };


    /** \struct NodeSelectTemplate
     *  \brief Структура описывает параметры шаблона пустого story node.
     *  \note Для добавления на графическую сцену методом drag&drop.
     */
    struct NodeSelectTemplate
    {
        QString nodeType;   /**< Тип ноды (совпадает с указываемым в json). */
        QIcon icon;         /**< Иконка. */
        QString toolTip;    /**< Описание. */

        bool operator==(const NodeSelectTemplate& other) const
        {
            return nodeType == other.nodeType &&
                   toolTip == other.toolTip;
        }

        bool operator!=(const NodeSelectTemplate& other) const
        {
            return !(*this == other);
        }
    };
    typedef QList<NodeSelectTemplate> SelectTNodeList;
}
