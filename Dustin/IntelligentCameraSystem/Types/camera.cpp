
#include "camera.h"

Camera::Camera()
{
}

Camera::Camera(QString &name, Address &address, CameraContentType content_type) :
    QListWidgetItem(name),
    m_name(name),
    m_server_address(address),
    m_content_type(content_type)
{
}

Address Camera::server_address() const
{
    return m_server_address;
}

void Camera::setServer_address(const Address &server_address)
{
    m_server_address = server_address;
}

//Equivalence operator
bool Camera::operator ==(const Camera &rhs)
{
    return (this->name() == rhs.name() && this->server_address() == rhs.server_address());
}

QString Camera::name() const
{
    return m_name;
}

void Camera::setName(const QString &name)
{
    m_name = name;
    this->setText(name);
}

CameraContentType Camera::content_type() const
{
    return m_content_type;
}

void Camera::setContent_type(const CameraContentType &content_type)
{
    m_content_type = content_type;
}
