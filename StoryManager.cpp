#include "StoryManager.hpp"

//===================================== public ==========================================

StoryManager::StoryManager(QObject* parent) :
    QObject(parent),
    m_storyScene(new StoryScene),
    m_storyNodeSelectModel(new SelectNodeModel)
{

}

StoryScenePtr StoryManager::getStoryScene()
{
    return m_storyScene;
}

SelectNodeModelPtr StoryManager::getStoryNodeSelectModel()
{
    return m_storyNodeSelectModel;
}

/*void StoryManager::setSelectListStoryNodes(const StoryCommon::SelectNodeList& listTNodes)
{
    m_storyNodeSelectModel->clear();
    m_storyNodeSelectModel->addTemplateNodesList(listTNodes);
}*/

int StoryManager::getCountStoryNodes() const
{
    return m_storyScene->nodeCount();
}

//=======================================================================================
