#ifndef TESTOVACIACLASS_H
#define TESTOVACIACLASS_H

#include <QObject>
#include <QString>
#include <qqml.h>

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString getMsgMy READ getMsgMy WRITE setMsgMy NOTIFY msgMySet )
    Q_PROPERTY( QString getMsgGuest READ getMsgGuest WRITE setMsgGuest NOTIFY msgGuestSet )
    QML_ELEMENT

public:
    explicit BackEnd();
    QString getUserName();

    QString getMsgMy();
    QString getMsgGuest();
    void setMsgMy( QString str );
    void setMsgGuest( QString str );

signals:
    void msgMySet();
    void msgGuestSet();

public slots:
    void msgMyChanged( QString str );
    void msgGuestChanged( QString str );

private:
    QString msgMyStr;
    QString msgGuestStr;
};

#endif // TESTOVACIACLASS_H
