#ifndef IVIDEOWINDOWMANAGER_H
#define IVIDEOWINDOWMANAGER_H

class QStringList;

/**
 * @brief This class defines an interface for external modules to access specific features of video windows.
 */
class IVideoWindowManager
{

public:
    /**
     * @brief Get the video window's list of sizes.
     * @return The list of video sizes.
     */
    virtual QStringList getVideoSizes() = 0;
};

#endif // IVIDEOWINDOWMANAGER_H
