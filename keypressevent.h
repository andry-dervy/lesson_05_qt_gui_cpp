#ifndef KEYPRESSEVENT_H
#define KEYPRESSEVENT_H

#include <QObject>
#include <QEvent>
#include <widgetsettings.h>

class KeyPressEvent : public QObject
{
    Q_OBJECT
public:
    explicit KeyPressEvent(QObject *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
signals:
    void newFile();
    void openFile();
    void saveFile();
    void quit();

public slots:
    void setHotKey(eTYPE_HOTKEY typeHotKey,
                   std::pair<int,Qt::KeyboardModifiers>& p);

private:
    std::map<eTYPE_HOTKEY,std::pair<int,Qt::KeyboardModifiers>> hotkeys;
};

#endif // KEYPRESSEVENT_H
