#include "CoreImpl.hpp"
#include "StoryManager.hpp"

CoreImpl::CoreImpl(StoryTypesNodeCollector &collector)
{
    m_storyManager.reset(new StoryManager(collector));
}

CoreImpl::~CoreImpl()
{

}

StoryManagerPtr CoreImpl::getStoryManager() const
{
    return m_storyManager;
}
