#pragma once

#include "StoryNode.h"

struct StoryInfo
{
    QString version;
    QString filePath;
    StoryNodeList nodeList;

    void clear()
    {
        version.clear();
        nodeList.clear();
    }

    bool isValid() const
    {
        return (!version.isEmpty() && !nodeList.isEmpty());
    }
};


class JsonStoryProvider : public QObject
{
    Q_OBJECT

public:
    enum class EnProcessType
    {
        enLoadStoryJson,
        enSaveStoryJson
    };

public:
    JsonStoryProvider(QObject* parent = Q_NULLPTR);
    virtual ~JsonStoryProvider();

    bool startOpenStoryFileDialog();
    bool startSaveStoryFileDialog();

    void setStoryInfo(const StoryInfo& storyInfo);
    const StoryInfo& getStoryInfo() const;

signals:
    void signalStartProcess(const EnProcessType& typeProcess, int countIteration);
    void signalEndIteration(int iteration);
    void signalFinishProcess();

private:
    bool loadJsonStory(const QJsonObject& jsonStory); // TODO везде добавить комментарии в стиле doxygen
    bool saveJsonStory(QJsonObject& jsonStory);

private:
    StoryInfo m_storyInfo;
};
