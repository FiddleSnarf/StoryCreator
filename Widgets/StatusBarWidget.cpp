#include "StatusBarWidget.hpp"
#include "StoryManager.hpp"
#include "ui_StatusBarWidget.h"

StatusBarWidget::StatusBarWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::StatusBarWidget),
    m_storyState(false)
{
    m_ui->setupUi(this);
    initialization();
}

StatusBarWidget::~StatusBarWidget()
{

}

void StatusBarWidget::initialization()
{
    m_ui->storyNameLineEdit->setValidator(new QRegExpValidator(QRegExp(QString::fromLocal8Bit("[A-Za-zА-Яа-я1-9]{1,50}")), this));
    updateUI();
    resetData();
}

void StatusBarWidget::setStoryManager(StoryManager* storyManager)
{
    m_storyManager = storyManager;

    connect(m_storyManager, &StoryManager::signalStoryStateChanged, this, &StatusBarWidget::slotStoryStateChanged);
    connect(m_storyManager, &StoryManager::signalStoryNodeAdded, this, &StatusBarWidget::slotNodeCountChanged);
    connect(m_storyManager, &StoryManager::signalStoryNodeDeleted, this, &StatusBarWidget::slotNodeCountChanged);

    connect(m_ui->storyNameLineEdit, &QLineEdit::textChanged, this, &StatusBarWidget::slotStoryNameEdit);
}

void StatusBarWidget::slotStoryNameEdit(const QString& storyName)
{
    if (!m_storyManager)
        return;

    m_storyManager->slotUpdateStoryName(storyName);
}

void StatusBarWidget::slotNodeCountChanged()
{
    if (!m_storyManager)
        return;

    m_ui->lcdNodeCount->display(m_storyManager->getCountStoryNodes());
}

void StatusBarWidget::slotStoryStateChanged(bool state)
{
    if (!m_storyManager)
        return;

    m_storyState = state;
    if (state)
    {
        m_ui->storyNameLineEdit->setText(m_storyManager->getCurrentStoryName());
        m_ui->lcdNodeCount->display(m_storyManager->getCountStoryNodes());
    }else
        resetData();

    updateUI();
}

void StatusBarWidget::updateUI()
{
    m_ui->storyNameLineEdit->setEnabled(m_storyState);
    m_ui->lcdNodeCount->setEnabled(m_storyState);
}

void StatusBarWidget::resetData()
{
    m_ui->storyNameLineEdit->setText(QString());
    m_ui->lcdNodeCount->display("-----");
}
