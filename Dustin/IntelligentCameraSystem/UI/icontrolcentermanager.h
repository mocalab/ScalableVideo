#ifndef ICONTROLCENTERMANAGER_H
#define ICONTROLCENTERMANAGER_H

/**
 * @brief This class defines an interface to the control center window.
 *Any external modules that wish to obtain data from the control center window should access them through this interface.
 *Therefore, any data that should be made available to external modules should be accessible via this interface.
 */
class IControlCenterManager
{
public:

    /**
     * @brief Determine if the application is in learning mode or not.
     * @return True if the application is in learning mode, false if not.
     */
    virtual bool inLearningMode() = 0;

};

#endif // ICONTROLCENTERMANAGER_H
