#pragma once

#include "StoryNode.h"
#include "Common/StoryCommon.hpp"


class JsonStoryHelper
{
public:
    static QString selectLoadStoryFilePath();
    static QString selectSaveStoryFilePath(const QString& defaultFileName);
    static bool loadJsonStory(const QString& filePath, StoryCommon::StoryInfo& storyInfo);
    static bool saveJsonStory(const QString& filePath, const StoryCommon::StoryInfo& storyInfo);

private:
    static QString storyFileExtension;
    static bool loadJsonStory(const QJsonObject& jsonStory, StoryCommon::StoryInfo& storyInfo);
    static bool saveJsonStory(QJsonObject& jsonStory, const StoryCommon::StoryInfo& storyInfo);
};
