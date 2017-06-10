#include "JsonStoryHelper.h"

#include <QFileDialog>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>

bool JsonStoryHelper::loadJsonStory(const QString& filePath, StoryCommon::StoryInfo& storyInfo)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Failed to open file"));
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
        QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Failed to save file"));
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
    // TODO мне кажется что лучше сделать расширение файла *.story
    return QFileDialog::getOpenFileName(nullptr, "Open story", QApplication::applicationDirPath(), "*.json");
}

QString JsonStoryHelper::selectSaveStoryFilePath()
{
    // TODO мне кажется что лучше сделать расширение файла *.story
    const QString defaultPathName = QApplication::applicationDirPath() + "/storyName"; // TODO подумать над именем файла по умолчанию
    return QFileDialog::getSaveFileName(nullptr, "Open story", defaultPathName, "*.json");
}

bool JsonStoryHelper::loadJsonStory(const QJsonObject &jsonStory, StoryCommon::StoryInfo& storyInfo)
{
    if (!jsonStory.contains("story_format_ver") || !jsonStory.contains("story_node_arr")) // TODO вынести имена тегов в Common
        return false;

    storyInfo.version = jsonStory["story_format_ver"].toString();
    const QJsonArray nodesArray = jsonStory["story_node_arr"].toArray();
    for(int nodeIdx = 0; nodeIdx < nodesArray.size(); nodeIdx++)
    {
        StoryNode node;
        node.read(nodesArray[nodeIdx].toObject());
        if (node.isValid())
            storyInfo.nodeList << node; // TODO надо еще подумать добавлять ли невалидные ноды или нет (наверное лучше добавлять, и отображать как недоделанные, чтобы иметь возможность их редактировать)
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
    jsonStory["story_format_ver"] = storyInfo.version;
    jsonStory["story_node_arr"] = nodesArray;
    return true;
}
