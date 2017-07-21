#pragma once

#include <QWidget>

namespace Ui
{
    class StatusBarWidget;
}

class StoryManager;

class StatusBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusBarWidget(QWidget* parent = Q_NULLPTR);
    virtual ~StatusBarWidget();

    void setStoryManager(StoryManager* storyManager);

private slots:
    void slotStoryNameEdit(const QString& storyName);
    void slotNodeCountChanged();
    void slotStoryStateChanged(bool state);

private:
    void initialization();
    void updateUI();
    void resetData();

    QScopedPointer<Ui::StatusBarWidget> m_ui;
    StoryManager* m_storyManager;
    bool m_storyState;
};
