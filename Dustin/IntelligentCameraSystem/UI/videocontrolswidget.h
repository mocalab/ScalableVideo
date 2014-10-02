/**
 *  @file Definition of a widget that conatins the controls for a playing video.
 */
#ifndef VIDEOCONTROLSWIDGET_H
#define VIDEOCONTROLSWIDGET_H
//Macro to define whether or not a combo box is used for resolution
#define USE_COMBO_BOX       01


#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

#include "Global/Constants.h"

/**
 * @brief This class defines the controls widget for a video window.
 */
class VideoControlsWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Explicit value constructor.
     * @param parent The parent of this widget.
     */
    explicit VideoControlsWidget(QWidget *parent = 0);
    ~VideoControlsWidget();
    /**
     * @brief Widget fade out.
     * @deprecated
     */
    void fadeOut();

    /**
     * @brief Widget fade in.
     * @deprecated
     */
    void fadeIn();

    /**
     * @brief Get the video sizes from the combo box.
     * @return The video sizes available.
     */
    QStringList &getSizes();

    bool use_standard_video() const;
    void setUse_standard_video(bool use_standard_video);

protected:
    //    virtual void enterEvent(QEvent *e);
    //    virtual void leaveEvent(QEvent *e);
    virtual void paintEvent(QPaintEvent *e);

signals:
    /**
     * @brief Send button clicked
     */
    void sendClicked();
    /**
     * @brief Send button clicked.
     *This signal will pass to the subscriber the values of the width/height combo box, frame rate text box, and bitrate textbox.
     * @param width The value of the width in the width/height combo box.
     * @param height The value of the height in the width/height combo box.
     * @param fps The value of the frame rate text box.
     * @param bps The value of the bitrate text box.
     */
    void sendClicked(QString width, QString height, QString fps, QString bps);
public slots:
    /**
     * @brief Slot invoked when the send button is clicked.
     */
    void sendButtonClicked();
private slots:
    //To delay the hide on a timer
    void hideDelay();
private:
    void setUpUI();

    //UI Elements

    //Labels
    QLabel              *lblWidth;
    QLabel              *lblHeight;
    QLabel              *lblWidthHeight;
    QLabel              *lblFPS;
    QLabel              *lblbitRate;

    //Line Edits
    QLineEdit           *leWidth;
    QLineEdit           *leHeight;
    QLineEdit           *leFPS;
    QComboBox           *cbFps;
    QLineEdit           *lebitRate;

    //Combo box for Width x Height
    QComboBox           *cbResolution;
    QStringList          all_sizes;

    //Button
    QPushButton         *bSend;

    //Whether or not standard file being used
    bool                m_use_standard_video;
};

#endif // VIDEOCONTROLSWIDGET_H
