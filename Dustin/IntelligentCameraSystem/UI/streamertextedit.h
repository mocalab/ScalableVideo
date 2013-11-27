#ifndef STREAMERTEXTEDIT_H
#define STREAMERTEXTEDIT_H

#include <QTextEdit>
#include <Global/StdTypes.h>
class StreamerTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit StreamerTextEdit(QWidget *parent = 0);
    void addCommandToList(MainWindowUI::CommandListItem cmd);

signals:
    void enterPressed();
public slots:

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    //List of commands entered
    MainWindowUI::CommandList                m_commands;
    MainWindowUI::CommandListIterator        m_last_command;
    
};

#endif // STREAMERTEXTEDIT_H
