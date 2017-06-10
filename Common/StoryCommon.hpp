#pragma once

#include <QString>
#include <QList>
#include <QIcon>
#include "JsonStoryHelper/StoryNode.h"

namespace StoryCommon
{
    struct StoryInfo
    {
        QString version;
        QString storyName;
        QString filePath;
        StoryNodeList nodeList;

        void clear()
        {
            version.clear();
            storyName.clear();
            nodeList.clear();
        }

        bool isValid() const
        {
            return (!version.isEmpty() && !nodeList.isEmpty());
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

namespace SoryGUI
{
    const QString NODE_MIME_TYPE = "story/node-template";
}
