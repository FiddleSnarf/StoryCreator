#include "StoryManager.hpp"

StoryManager::StoryManager(QObject* parent) : QObject(parent)
{

}

StoryScene& StoryManager::getStoryScene()
{
    return m_storyScene;
}

SelectNodeModel& StoryManager::getStoryNodeSelectModel()
{
    return m_storyNodeSelectModel;
}

void StoryManager::setSelectListStoryNodes(const StoryCommon::SelectNodeList& listTNodes)
{
    m_storyNodeSelectModel.clear();
    m_storyNodeSelectModel.addTemplateNodesList(listTNodes);
}
