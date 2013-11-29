/**
 *  @file Definition of the Camera class and associated types.
 */
#ifndef CAMERA_H
#define CAMERA_H

#include <QListWidgetItem>
#include <Network/address.h>

class Camera;

/**
 * @brief List of cameras declaration.
 */
typedef std::list<Camera *>                  CameraList;
/**
 * @brief Iterator for a list of cameras.
 */
typedef std::list<Camera *>::iterator        CameraListIterator;

/**
 * @brief An enumeration of camera content types.
 */
typedef enum{
    CONTENT_TYPE_NONE = 0,          /**< No content type specified.*/
    CONTENT_TYPE_PERSONAL,          /**< A personal stream server.*/
    CONTENT_TYPE_P2P,               /**< For peer to peer applications. */
    CONTENT_TYPE_SURVEILLANCE,      /**< A one-way surveillance camera. */
    CONTENT_TYPE_MEDICAL,           /**< Camera used for medical purposes (i.e. consultation w/ doctor). */
    CONTENT_TYPE_BROADCAST          /**< A broadcast streaming server. */
} CameraContentType;

/**
 * @brief The Camera class.
 *This class describes a camera that can stream video to the client. It inherits from the QListWidgetItem
 *class in order to facilitate its use in list boxes.
 */
class Camera : public QListWidgetItem
{
public:


    /**
     * @brief Default value constructor.
     */
    Camera();
    /**
     * @brief Explicit value constructor.
     * @param name The name of this camera.
     * @param address The address of the server to which this camera refers to.
     * @param content_type The type of content of this camera.
     */
    Camera(QString &name, Address& address, CameraContentType content_type);

    /**
     * @brief Accessor for the camera name field.
     * @return The name of the camera.
     */

    QString name() const;
    /**
     * @brief Mutator for the camera name field.
     * @param name The new camera name.
     */
    void setName(const QString &name);

    /**
     * @brief Accessor for the server address field.
     * @return The address of this camera's server.
     */
    Address server_address() const;
    /**
     * @brief Mutator for the server address field.
     * @param server_address The new address of the server.
     */
    void setServer_address(const Address &server_address);

    /**
     * @brief Accessor for the content type field.
     * @return The content type of this camera.
     */
    CameraContentType content_type() const;
    /**
     * @brief Mutator for the content type field
     * @param content_type The new content type for this camera.
     */
    void setContent_type(const CameraContentType &content_type);

    /**
     * @brief Overloaded equivalence operator.
     * @param rhs The Camera object to compare to.
     * @return True if the rhs Camera refers to the same server as this Camera.
     */
    bool operator==(const Camera& rhs);


private:
    QString                 m_name;
    Address                 m_server_address;

    CameraContentType       m_content_type;
};

#endif // CAMERA_H
