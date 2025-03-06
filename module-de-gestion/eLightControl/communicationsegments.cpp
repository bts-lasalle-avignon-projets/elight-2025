#include "communicationsegments.h"
#include <QDebug>

CommunicationSegments::CommunicationSegments(QObject* parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO << this << "parent" << parent;
}
