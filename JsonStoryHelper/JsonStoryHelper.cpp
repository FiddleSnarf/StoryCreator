#include <QFileDialog>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>

#include "JsonStoryHelper.h"
#include "Common/StoryJsonTags.hpp"

bool JsonStoryHelper::loadJsonStory(const QString& filePath, StoryCommon::StoryInfo& storyInfo)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(Q_NULLPTR, QObject::tr("Warning"), QObject::tr("Failed to open file"));
        return false;
    }
    storyInfo.filePath = filePath;
    const QJsonDocument loadStoryDoc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!loadJsonStory(loadStoryDoc.object(), storyInfo))
        return false;

    return true;
}

bool JsonStoryHelper::saveJsonStory(const QString& filePath, const StoryCommon::StoryInfo& storyInfo)
{
    if (filePath.isEmpty())
        return false;

      QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(Q_NULLPTR, QObject::tr("Warning"), QObject::tr("Failed to save file")); // TODO Избавиться от мессаджбоксов, сделать Result class
        return false;
    }

    QJsonObject storyObject;
    if (!saveJsonStory(storyObject, storyInfo))
    {
        file.close();
        return false;
    }

    const QJsonDocument saveStoryDoc(storyObject);
    file.write(saveStoryDoc.toJson());
    file.close();
    return true;
}

QString JsonStoryHelper::selectLoadStoryFilePath()
{
    return QFileDialog::getOpenFileName(Q_NULLPTR, "Open story", QApplication::applicationDirPath(), StoryCommon::STORY_FILE_EXTENSION);
}

QString JsonStoryHelper::selectSaveStoryFilePath(const QString& defaultFileName)
{
    const QString defaultPathName = QApplication::applicationDirPath() + "/" + defaultFileName;
    return QFileDialog::getSaveFileName(Q_NULLPTR, "Save story", defaultPathName, StoryCommon::STORY_FILE_EXTENSION);
}

bool JsonStoryHelper::loadJsonStory(const QJsonObject &jsonStory, StoryCommon::StoryInfo& storyInfo)
{
    if (!jsonStory.contains(StoryJsonTags::story_format_ver_tag) || !jsonStory.contains(StoryJsonTags::story_node_arr_tag))
        return false;

    storyInfo.version = jsonStory[StoryJsonTags::story_format_ver_tag].toString();
    storyInfo.storyName = jsonStory[StoryJsonTags::story_name_tag].toString();
    const QJsonArray nodesArray = jsonStory[StoryJsonTags::story_node_arr_tag].toArray();
    for(int nodeIdx = 0; nodeIdx < nodesArray.size(); nodeIdx++)
    {
        StoryNode node;
        node.read(nodesArray[nodeIdx].toObject());
        storyInfo.nodeList << node;
    }
    return true;
}

bool JsonStoryHelper::saveJsonStory(QJsonObject& jsonStory, const StoryCommon::StoryInfo& storyInfo)
{
    if (!storyInfo.isValid())
        return false;

    QJsonArray nodesArray;
    foreach (const StoryNode& storyNode, storyInfo.nodeList)
    {
        QJsonObject nodeObject;
        storyNode.write(nodeObject);
        nodesArray.append(nodeObject);
    }
    jsonStory[StoryJsonTags::story_format_ver_tag] = storyInfo.version;
    jsonStory[StoryJsonTags::story_name_tag] = storyInfo.storyName;
    jsonStory[StoryJsonTags::story_node_arr_tag] = nodesArray;
    return true;
}
