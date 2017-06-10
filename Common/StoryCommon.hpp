#pragma once

#include <QString>
#include <QList>
#include <QIcon>
#include "JsonStoryHelper/StoryNode.h"

namespace SoryGUI
{
    const QString NODE_MIME_TYPE = "story/node-template"; /**< Для работы drag&drop. */


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
        QRectF sceneRect;                   /**< Параметры сцены. */

        /** \brief Проверка на валидность
         */
        bool isValid() const
        {
            return !sceneRect.isNull();
        }
    };
}

namespace StoryCommon
{
    struct StoryInfo
    {
        QString version;        /**< Версия json story. */
        QString storyName;      /**< Название story. */
        QString filePath;       /**< Путь к файлу json. */
        StoryNodeList nodeList; /**< Список нодов. */
        SoryGUI::StoryAdditionalInfo additionalViewParams;  /**< Дополнительные параметры отображения. */

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
