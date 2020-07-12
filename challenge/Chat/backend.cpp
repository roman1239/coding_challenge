#include "backend.h"

BackEnd::BackEnd()
{

}

QString BackEnd::getMsgMy()
{
    return msgMyStr;
}

QString BackEnd::getMsgGuest()
{
    return msgGuestStr;
}

void BackEnd::setMsgMy( QString str )
{
    msgMyStr = str;
}

void BackEnd::setMsgGuest( QString str )
{
    msgGuestStr = str;
}

void BackEnd::msgMyChanged( QString str )
{
    setMsgMy( str );
}

void BackEnd::msgGuestChanged( QString str )
{
    setMsgGuest( str );
}
