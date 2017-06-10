#pragma once

#include <QSharedPointer>

typedef QSharedPointer<class StoryManager> StoryManagerPtr;

class ICore
{
public:

    /** \brief Виртуальный деструктор класса.
     */
    virtual ~ICore() {}

    virtual StoryManagerPtr getStoryManager() const = 0;
};
