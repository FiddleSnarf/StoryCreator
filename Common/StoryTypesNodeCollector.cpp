#include "StoryTypesNodeCollector.hpp"

StoryTypesNodeCollector::StoryTypesNodeCollector()
{
    // Добавляем все возможные стори ноды
    m_typesMap["common"] = TypeInfo(QObject::tr("The most typical node"), QIcon(":/story_node_icons/Resources/new_icon.png"));
}

StoryTypesNodeCollector::~StoryTypesNodeCollector()
{

}

QStringList StoryTypesNodeCollector::getNodesNameList() const
{
    return m_typesMap.keys();
}

StoryTypesNodeCollector::TypeInfo StoryTypesNodeCollector::getNodeTypeInfo(const QString nodeType) const
{
    TypeInfoMap::const_iterator it = m_typesMap.find(nodeType);
    if (it != m_typesMap.cend())
        return *it;

    return StoryTypesNodeCollector::TypeInfo();
}
