#include <QString>
#include "JsonStoryHelper/StoryNode.h"

namespace StoryCommon
{
    struct StoryInfo
    {
        QString version;
        QString storyName;
        QString filePath;
        StoryNodeList nodeList;

        void clear()
        {
            version.clear();
            storyName.clear();
            nodeList.clear();
        }

        bool isValid() const
        {
            return (!version.isEmpty() && !nodeList.isEmpty());
        }
    };


}

namespace SoryGUI
{
    const QString NODE_MIME_TYPE = "story/node-template";
}
