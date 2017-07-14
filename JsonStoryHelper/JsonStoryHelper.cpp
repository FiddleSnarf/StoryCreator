#include <QFileDialog>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>

#include "JsonStoryHelper.h"
#include "Common/StoryJsonTags.hpp"

bool JsonStoryHelper::loadJsonStory(const QString& filePath, StoryCommon::StoryInfo& storyInfo)
{
    // Загружаем саму историю
    QFile storyFile(filePath);
    if (!storyFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(Q_NULLPTR, QObject::tr("Warning"), QObject::tr("Failed to open story file"));
        return false;
    }
    storyInfo.filePath = filePath;
    const QJsonDocument loadStoryDoc = QJsonDocument::fromJson(storyFile.readAll());
    storyFile.close();
    if (!loadJsonStory(loadStoryDoc.object(), storyInfo))
        return false;

    // Проверяем есть ли файл с графическими параметрами истории (Если его нет или он не открылся ну хрен с ним)
    const QString gFilePath = filePath + StoryGUI::GRAPHIC_FILE_EX;
    if (!QFile::exists(gFilePath))
        return true;

    QFile storyGraphicFile(gFilePath);
    if (!storyGraphicFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(Q_NULLPTR, QObject::tr("Warning"), QObject::tr("Failed to open graphic story file"));
        return true;
    }
    const QJsonDocument loadGStoryDoc = QJsonDocument::fromJson(storyGraphicFile.readAll());
    loadJsonStoryG(loadGStoryDoc.object(), storyInfo);

    return true;
}

bool JsonStoryHelper::saveJsonStory(const QString& filePath, const StoryCommon::StoryInfo& storyInfo)
{
    if (filePath.isEmpty())
        return false;

    QFile storyFile(filePath);
    QFile storyGraphicFile(filePath + StoryGUI::GRAPHIC_FILE_EX);

    // Сначала сохраняем саму историю
    if (!storyFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(Q_NULLPTR, QObject::tr("Warning"), QObject::tr("Failed to save story")); // TODO Избавиться от мессаджбоксов, сделать Result class
        return false;
    }

    QJsonObject storyObject;
    if (!saveJsonStory(storyObject, storyInfo))
    {
        storyFile.close();
        return false;
    }
    const QJsonDocument saveStoryDoc(storyObject);
    storyFile.write(saveStoryDoc.toJson());
    storyFile.close();
    // ===============================

    // Теперь сохраняем графические параметры истории
    if (!storyGraphicFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(Q_NULLPTR, QObject::tr("Warning"), QObject::tr("Failed to save graphic story")); // TODO Избавиться от мессаджбоксов, сделать Result class
        return false;
    }

    QJsonObject storyGObject;
    if (!saveJsonStoryG(storyGObject, storyInfo))
    {
        storyGraphicFile.close();
        return true;
    }
    const QJsonDocument saveStoryGDoc(storyGObject);
    storyGraphicFile.write(saveStoryGDoc.toJson());
    storyGraphicFile.close();
    // ===============================

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

bool JsonStoryHelper::loadJsonStoryG(const QJsonObject& jsonStory, StoryCommon::StoryInfo& storyInfo)
{
    for (QJsonObject::const_iterator it = jsonStory.constBegin(); it != jsonStory.constEnd(); ++it)
    {
        bool isNodeId = false;
        const int id = it.key().toInt(&isNodeId);
        if (!isNodeId)
            continue;

        bool isCoord = false;
        const QStringList strCoordsList = it.value().toString().split(StoryGUI::NODE_COORDS_SPLITTER);
        if (strCoordsList.size() != 2) // т. к. у нас Декартова система координат
            continue;

        const double x = strCoordsList.first().toDouble(&isCoord);
        if (!isCoord)
            continue;

        const double y = strCoordsList.last().toDouble(&isCoord);
        if (!isCoord)
            continue;

        storyInfo.additionalViewParams.nodesPosMap[id] = QPointF(x, y);
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

bool JsonStoryHelper::saveJsonStoryG(QJsonObject& jsonStory, const StoryCommon::StoryInfo& storyInfo)
{
    for(StoryGUI::SceneNodePositionMap::const_iterator it = storyInfo.additionalViewParams.nodesPosMap.cbegin(); it != storyInfo.additionalViewParams.nodesPosMap.cend(); ++it)
    {
        const QPointF& pos = *it;
        jsonStory[QString::number(it.key())] = QString::number(pos.x()) + StoryGUI::NODE_COORDS_SPLITTER + QString::number(pos.y());
    }
    return true;
}
