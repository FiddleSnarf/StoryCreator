#include "CoreImpl.hpp"
#include "StoryManager.hpp"

CoreImpl::CoreImpl()
{
    m_storyManager.reset(new StoryManager());
}

CoreImpl::~CoreImpl()
{

}

StoryManagerPtr CoreImpl::getStoryManager() const
{
    return m_storyManager;
}
