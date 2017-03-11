#ifndef STORYCREATER_HPP
#define STORYCREATER_HPP

#include <QMainWindow>

#include "StoryManager.hpp"

namespace Ui
{
    class StoryCreator;
}

class StoryCreator : public QMainWindow
{
    Q_OBJECT

public:
    explicit StoryCreator(QWidget* parent = 0);
    ~StoryCreator();

private:
    void initialize();
    void initSelectNodes(const StoryCommon::SelectNodeList& nodeList);

private:
    Ui::StoryCreator* ui;

    StoryManager m_storyManager;
};

#endif // STORYCREATER_HPP
