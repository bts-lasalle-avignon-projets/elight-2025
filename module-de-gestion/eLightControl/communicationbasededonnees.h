#ifndef COMMUNICATIONBASEDEDONNEES_H
#define COMMUNICATIONBASEDEDONNEES_H

#include <QObject>

class CommunicationBaseDeDonnees : public QObject
{
    Q_OBJECT
public:
    explicit CommunicationBaseDeDonnees(QObject *parent = nullptr);

signals:

};

#endif // COMMUNICATIONBASEDEDONNEES_H
