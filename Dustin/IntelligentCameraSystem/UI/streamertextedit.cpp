#include "UI/streamertextedit.h"
#include <QKeyEvent>
#include "Global/Log.h"

StreamerTextEdit::StreamerTextEdit(QWidget *parent) :
    QTextEdit(parent),
    m_commands()
{   
    //Initialize list of commands
    m_commands.clear();
    m_commands.push_front("<Enter command>");
    this->m_last_command = this->m_commands.begin();

    MainWindowUI::CommandListItem selected = *(m_last_command);
    this->setText(selected.c_str());


}

void StreamerTextEdit::addCommandToList(MainWindowUI::CommandListItem cmd)
{
    this->m_commands.push_front(cmd);
}

void StreamerTextEdit::keyPressEvent(QKeyEvent *e)
{
    //Determine which Key was pressed
    if(e->key() == Qt::Key_Up)
    {
        INFO() << "Up arrow pressed.";
        if(this->toPlainText().compare("") == 0)
        {
            this->m_last_command = this->m_commands.begin();
        }
        else
        {
            this->m_last_command++;
            //Iterate back to beginning of list
            if(this->m_last_command == m_commands.end())
                m_last_command = m_commands.begin();
        }
        MainWindowUI::CommandListItem& selected = *(m_last_command);
        this->setText(selected.c_str());
    }
    else if(e->key() == Qt::Key_Down)
    {
        INFO() << "Down arrow pressed.";

        //Iterate back to beginning of list
        if(this->m_last_command == m_commands.begin())
        {
            this->setText("");
        }
        else
        {
            this->m_last_command--;

            MainWindowUI::CommandListItem& selected = *(m_last_command);
            this->setText(selected.c_str());
        }
    }
    else if(e->key() == Qt::Key_Enter)
    {
        //Emit the clicked button signal
        emit enterPressed();
    }
    else
    {
        QTextEdit::keyPressEvent(e);
    }

}
