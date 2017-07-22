#include "StoryCreator.hpp"
#include "StoryManager.hpp"
#include "Widgets/StatusBarWidget.hpp"
#include "ui_StoryCreator.h"
#include <QGraphicsItem>
#include <QMessageBox>
#include <QFileDialog>


const QString StoryCreator::NODE_COUNT_CAPT = QObject::tr("Node count: %1");


StoryCreator::StoryCreator(ICorePtr core, QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::StoryCreator),
    m_core(core),
    m_storyManager(core->getStoryManager())
{
    m_ui->setupUi(this);
    initialize();
}

StoryCreator::~StoryCreator()
{

}

void StoryCreator::closeEvent(QCloseEvent* event)
{
    if (m_storyManager)
        m_storyManager->closeStory();

    QWidget::closeEvent(event);
}

//===================================== private =========================================

void StoryCreator::initialize()
{
    setWindowTitle(tr("Story Creator v0.9.0 beta"));
    showMaximized();

    initToolBar();
    initConnects();
    initStoryView();
    initSelectTemplateNodesView();

    m_ui->nodeEditorTab->setEnabled(false);
    m_ui->itemsEditorTab->setEnabled(false);
    m_ui->nodeInfoWidget->setVisible(false);
    m_ui->storyNavigationWidget->setCore(m_core);
}

void StoryCreator::initStoryView()
{
    m_ui->storyView->setCore(m_core);
    m_ui->storyView->setScene(m_storyManager->getStoryScene().data());          
}

void StoryCreator::initSelectTemplateNodesView()
{
    m_ui->viewNodes->setModel(m_storyManager->getStoryNodeSelectModel().data());

    // Устанавливаем размер иконок
    QSize sizeNode = m_ui->viewNodes->size();
    sizeNode.setWidth(sizeNode.width() - 10);
    sizeNode.setHeight(sizeNode.width());
    m_ui->viewNodes->setIconSize(sizeNode);
}

void StoryCreator::initToolBar()
{ 
    m_ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    QList<QAction*> actions;
    m_saveMenu = new QMenu(this);

    m_actSaveStory = new QAction(QIcon(":/tool_bar_icons/Resources/save_story.png"), tr("Save"));
    m_actSaveStory->setShortcut(QKeySequence::Save);

    m_actSaveAsStory = new QAction(QIcon(":/tool_bar_icons/Resources/save_story.png"), tr("Save as..."));
    m_actSaveAsStory->setShortcut(QKeySequence::SaveAs);

    m_saveMenu->addAction(m_actSaveStory);
    m_saveMenu->addAction(m_actSaveAsStory);
    m_saveMenu->setIcon(QIcon(":/tool_bar_icons/Resources/save_story.png"));
    m_saveMenu->setToolTip(tr("Save story"));

    m_actCreateNewStory = new QAction(QIcon(":/tool_bar_icons/Resources/new_story.png"), tr("Create new story"));
    m_actCreateNewStory->setShortcut(QKeySequence::New);

    m_actLoadStory = new QAction(QIcon(":/tool_bar_icons/Resources/load_story.png"), tr("Open story"));
    m_actLoadStory->setShortcut(QKeySequence::Open);

    m_actCloseStory = new QAction(QIcon(":/tool_bar_icons/Resources/close_story.png"), tr("Close story"));
    m_actCloseStory->setShortcut(QKeySequence::Close);

    m_actExportPng = new QAction(QIcon(":/tool_bar_icons/Resources/export_to_png.png"), tr("Export story to PNG"));

    actions << m_actCreateNewStory << m_actLoadStory << m_saveMenu->menuAction() << m_actCloseStory << m_actExportPng;
    m_ui->mainToolBar->addActions(actions);

    m_actCloseStory->setEnabled(false);
    m_actExportPng->setEnabled(false);
    m_saveMenu->setEnabled(false);

    m_ui->mainToolBar->addSeparator();
    m_ui->mainToolBar->addSeparator();
    m_statusBar = new StatusBarWidget();
    m_statusBar->setStoryManager(m_storyManager.data());
    m_ui->mainToolBar->addWidget(m_statusBar);
}

void StoryCreator::initConnects()
{
    connect(m_ui->tabStory, &QTabWidget::currentChanged, this, &StoryCreator::slotTabStoryCreatorChanged);

    // Подписываемся на изменения истории
    connect(m_storyManager.data(), &StoryManager::signalStoryStateChanged, this, &StoryCreator::slotStoryStateChanged);
    connect(m_storyManager.data(), &StoryManager::signalItemSelectedChanged, this, &StoryCreator::slotItemSelectedChanged);
    connect(m_storyManager.data(), &StoryManager::signalStorySaved, this, &StoryCreator::slotStorySaved);

    // Подписываемся на изменения данных нодов
    connect(m_storyManager.data(), &StoryManager::signalDataNodeChanged, this, &StoryCreator::slotStoryDataChanged);
    connect(m_storyManager.data(), &StoryManager::signalGeometryNodeChanged, this, &StoryCreator::slotStoryDataChanged);
    connect(m_storyManager.data(), &StoryManager::signalStoryNameChanged, this, &StoryCreator::slotStoryDataChanged);

    // Подписываемся на кнопки тулбара
    connect(m_actCreateNewStory, &QAction::triggered, m_storyManager.data(), &StoryManager::slotCreateNewStory);
    connect(m_actLoadStory, &QAction::triggered, m_storyManager.data(), &StoryManager::slotLoadStory);
    connect(m_actCloseStory, &QAction::triggered, m_storyManager.data(), &StoryManager::slotCloseStory);
    connect(m_actSaveStory, &QAction::triggered, m_storyManager.data(), &StoryManager::slotSaveStory);
    connect(m_actSaveAsStory, &QAction::triggered, m_storyManager.data(), &StoryManager::slotSaveAsStory);
    connect(m_saveMenu->menuAction(), &QAction::triggered, m_storyManager.data(), &StoryManager::slotSaveStory);
    connect(m_actExportPng, &QAction::triggered, this, &StoryCreator::slotExportToPng);

    // Подписываем окно редактирования нодов на необходимые события
    connect(m_storyManager.data(), &StoryManager::signalStoryNodeDeleted, m_ui->nodeInfoWidget, &NodeInfoWidget::slotNodeDeleted);
    connect(m_ui->storyNavigationWidget, &StoryNavigationWidget::signalSearchTextChanged, m_ui->nodeInfoWidget, &NodeInfoWidget::slotSearchTextChanged);
}

void StoryCreator::storyClosed()
{
    // Т.к. сцена была пересоздана, переназначаем ее для вьюхи
    const StoryScenePtr scenePtr = m_storyManager->getStoryScene();
    m_ui->storyView->setScene(scenePtr.data());
    m_ui->nodeInfoWidget->setVisible(false);
    m_ui->storyView->resetZoom();
    slotStorySaved();
}

//=======================================================================================

//============================================ private slots ============================

void StoryCreator::slotStoryStateChanged(bool state)
{
    m_ui->nodeEditorTab->setEnabled(state);
    m_ui->itemsEditorTab->setEnabled(state);
    m_actCloseStory->setEnabled(state);
    m_actExportPng->setEnabled(state);
    m_saveMenu->setEnabled(state);

    if (!state)
        storyClosed();
}

void StoryCreator::slotItemSelectedChanged(bool state, StoryNodeItem* selectedNode)
{
    if (state)
    {
        if (!selectedNode)
            return;

        m_ui->nodeInfoWidget->setCurrentNodeItem(selectedNode);
    }
    m_ui->nodeInfoWidget->setVisible(state);
}

void StoryCreator::slotStoryDataChanged()
{
    QString nodeEditorTabText = m_ui->tabStory->tabText(enNodeEditorTabIdx);
    if (!nodeEditorTabText.contains("*"))
        m_ui->tabStory->setTabText(enNodeEditorTabIdx, nodeEditorTabText + "*");
}

void StoryCreator::slotStorySaved()
{
    m_ui->tabStory->setTabText(enNodeEditorTabIdx, m_ui->tabStory->tabText(enNodeEditorTabIdx).remove(QChar('*')));
    m_ui->tabStory->setTabText(enItemsEditorTabIdx, m_ui->tabStory->tabText(enItemsEditorTabIdx).remove(QChar('*')));
}

void StoryCreator::slotTabStoryCreatorChanged(int index)
{
    if (!m_storyManager)
        return;

    if (index == enNodeEditorTabIdx)
    {
        StoryNodeItemPtr selectedNode = m_storyManager->getSelectedNodeItem();
        if(selectedNode)
            m_ui->nodeInfoWidget->setCurrentNodeItem(selectedNode);
    }
}

void StoryCreator::slotExportToPng()
{
    if (!m_storyManager)
        return;

    if (m_storyManager->getStoryNodeList().isEmpty())
    {
        QMessageBox::information(this, tr("Abort"), tr("Story does not contain nodes"));
        return;
    }

    const QString storyName = m_storyManager->getCurrentStoryName();
    const QString defaultPath = QApplication::applicationDirPath() + "//" + (storyName.isEmpty() ? tr("story") : storyName);
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Export to png"), defaultPath, "*.png");
    if (fileName.isEmpty())
        return;

    StoryScenePtr scene = m_storyManager->getStoryScene();
    QImage image(scene->width(), scene->height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(0);
    QPainter painter(&image);
    scene->render(&painter);
    if (!image.save(fileName))
        QMessageBox::warning(this, tr("Abort"), tr("Failed to save image"));
}

//=======================================================================================
