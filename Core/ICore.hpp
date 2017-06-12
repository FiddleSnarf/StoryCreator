#pragma once

#include <QSharedPointer>

typedef QSharedPointer<class StoryManager> StoryManagerPtr;

class ICore
{
public:

    /** \brief Виртуальный деструктор класса.
     */
    virtual ~ICore() {}

    /** \brief Метод возвращает указатель на StoryManager.
     */
    virtual StoryManagerPtr getStoryManager() const = 0;
};

typedef QSharedPointer<ICore> ICorePtr;
