#include "JsonStoryProvider.h"

#include <QFileDialog>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>

JsonStoryProvider::JsonStoryProvider(QObject* parent) :
    QObject(parent)
{

}

JsonStoryProvider::~JsonStoryProvider()
{

}

void JsonStoryProvider::setStoryInfo(const StoryInfo& storyInfo)
{
    m_storyInfo = storyInfo;
}

const StoryInfo& JsonStoryProvider::getStoryInfo() const
{
    return m_storyInfo;
}

bool JsonStoryProvider::startOpenStoryFileDialog()
{
    // TODO мне кажется что лучше сделать расширение файла *.story
    const QString filePath = QFileDialog::getOpenFileName(nullptr, "Open story", QApplication::applicationDirPath(), "*.json");
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(nullptr, tr("Warning"), tr("Failed to open file"));
        return false;
    }
    m_storyInfo.filePath = filePath;
    const QJsonDocument loadStoryDoc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!loadJsonStory(loadStoryDoc.object()))
        return false;

    return true;
}

bool JsonStoryProvider::startSaveStoryFileDialog()
{
    // TODO мне кажется что лучше сделать расширение файла *.story
    const QString defaultPathName = QApplication::applicationDirPath() + "/storyName"; // TODO подумать над именем файла по умолчанию
    const QString filePath = QFileDialog::getSaveFileName(nullptr, "Open story", defaultPathName, "*.json");
    if (filePath.isEmpty())
        return false;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        // TODO Сделать какой-нить класс Result
        QMessageBox::warning(nullptr, tr("Warning"), tr("Failed to save file"));
        return false;
    }

    QJsonObject storyObject;
    if (!saveJsonStory(storyObject))
    {
        file.close();
        return false;
    }

    const QJsonDocument saveStoryDoc(storyObject);
    file.write(saveStoryDoc.toJson());
    file.close();
    m_storyInfo.filePath = filePath;
    return true;
}

bool JsonStoryProvider::loadJsonStory(const QJsonObject &jsonStory)
{
    if (!jsonStory.contains("story_format_ver") || !jsonStory.contains("story_node_arr"))
        return false;

    m_storyInfo.version = jsonStory["story_format_ver"].toString();
    const QJsonArray nodesArray = jsonStory["story_node_arr"].toArray();

    emit signalStartProcess(EnProcessType::enLoadStoryJson, nodesArray.size());
    for(int nodeIdx = 0; nodeIdx < nodesArray.size(); nodeIdx++)
    {
        StoryNode node;
        node.read(nodesArray[nodeIdx].toObject());
        if (node.isValid())
            m_storyInfo.nodeList << node; // TODO надо еще подумать добавлять ли невалидные ноды или нет (наверное лучше добавлять, и отображать как недоделанные, чтобы иметь возможность их редактировать)

        emit signalEndIteration(nodeIdx);
    }
    emit signalFinishProcess();
    return true;
}

bool JsonStoryProvider::saveJsonStory(QJsonObject& jsonStory)
{
    if (!m_storyInfo.isValid())
        return false;

    QJsonArray nodesArray;
    foreach (const StoryNode& storyNode, m_storyInfo.nodeList)
    {
        QJsonObject nodeObject;
        storyNode.write(nodeObject);
        nodesArray.append(nodeObject);
    }
    jsonStory["story_format_ver"] = m_storyInfo.version;
    jsonStory["story_node_arr"] = nodesArray;
    return true;
}
