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

namespace StoryJsonTags
{
    const QString story_format_ver_tag = "story_format_ver";
    const QString story_node_arr_tag   = "story_node_arr";
    const QString type_tag             = "type";
    const QString action_logic_tag     = "action_logic";
    const QString text_tag             = "text";
    const QString nodeID_tag           = "nodeID";
    const QString title_tag            = "title";
    const QString actions_tag          = "actions";
}

namespace SoryGUI
{
    const QString NODE_MIME_TYPE = "story/node-template";
}
