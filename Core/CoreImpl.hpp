#pragma once

#include "ICore.hpp"
#include "Common/StoryTypesNodeCollector.hpp"

/** \class Core
 *  \brief Реализация интерфейса ICore.
 */
class CoreImpl : public ICore
{
public:
    /** \brief Конструктор класса. Создает все основные компоненты.
     */
    CoreImpl(StoryTypesNodeCollector& collector);

    /** \brief Виртуальный деструктор класса.
     */
    virtual ~CoreImpl();

    //==================== наследие ICore ======================
    StoryManagerPtr getStoryManager() const override;
    //==========================================================

private:
    StoryManagerPtr m_storyManager;
};
