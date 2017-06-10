#include "JsonStoryHelper.h"

#include <QFileDialog>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>

QString JsonStoryHelper::storyFileExtension = "*.json"; // TODO мне кажется что лучше сделать расширение файла *.story

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
    return QFileDialog::getOpenFileName(nullptr, "Open story", QApplication::applicationDirPath(), storyFileExtension);
}

QString JsonStoryHelper::selectSaveStoryFilePath(const QString& defaultFileName)
{
    const QString defaultPathName = QApplication::applicationDirPath() + "/" + defaultFileName;
    return QFileDialog::getSaveFileName(nullptr, "Open story", defaultPathName, storyFileExtension);
}

bool JsonStoryHelper::loadJsonStory(const QJsonObject &jsonStory, StoryCommon::StoryInfo& storyInfo)
{
    if (!jsonStory.contains(StoryJsonTags::story_format_ver_tag) || !jsonStory.contains(StoryJsonTags::story_node_arr_tag))
        return false;

    storyInfo.version = jsonStory[StoryJsonTags::story_format_ver_tag].toString();
    const QJsonArray nodesArray = jsonStory[StoryJsonTags::story_node_arr_tag].toArray();
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
    jsonStory[StoryJsonTags::story_format_ver_tag] = storyInfo.version;
    jsonStory[StoryJsonTags::story_node_arr_tag] = nodesArray;
    return true;
}
