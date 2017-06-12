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
    typedef QMap<QString, TypeInfo> TypeInfoMap;

    StoryTypesNodeCollector();
    virtual ~StoryTypesNodeCollector();

    QStringList getNodesNameList() const;
    StoryTypesNodeCollector::TypeInfo getNodeTypeInfo(const QString nodeType) const;

private:
    TypeInfoMap m_typesMap;     /**< Мапа всех допустимых типов нодов. */
};
