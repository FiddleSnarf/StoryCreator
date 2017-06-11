#pragma once

#include <QString>
#include <QIcon>
#include <QMap>

class StoryTypesNodeCollector
{
public:
    struct TypeInfo
    {
        TypeInfo()
        {
            // TODO добавить иконку по умолчанию unknown
        }

        TypeInfo(const QString& description, const QIcon& icon)
        {
            descriptionType = description;
            iconType = icon;
        }

        QString descriptionType;    /**< Описание типа. */
        QIcon iconType;             /**< Иконка. */
    };

    StoryTypesNodeCollector()
    {
        // Добавляем все возможные стори ноды
        m_typesMap["common"] = TypeInfo(QObject::tr("The most typical node"), QIcon(":/story_node_icons/Resources/new_icon.png"));
    }

    QStringList getNodesNameList() const
    {
        return m_typesMap.keys();
    }

    StoryTypesNodeCollector::TypeInfo getNodeTypeInfo(const QString nodeType) const
    {
        TypeInfoMap::const_iterator it = m_typesMap.find(nodeType);
        if (it != m_typesMap.cend())
            return *it;

        return StoryTypesNodeCollector::TypeInfo();
    }

private:
    typedef QMap<QString, TypeInfo> TypeInfoMap;
    TypeInfoMap m_typesMap;     /**< Мапа всех допустимых типов нодов. */
};
