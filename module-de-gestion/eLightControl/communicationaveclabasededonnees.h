#ifndef COMMUNICATIONAVECLABASEDEDONNEES_H
#define COMMUNICATIONAVECLABASEDEDONNEES_H

#include <QObject>

class CommunicationAvecLaBaseDeDonnees : public QObject
{
    Q_OBJECT
public:
    explicit CommunicationAvecLaBaseDeDonnees(QObject *parent = nullptr);

signals:

};

#endif // COMMUNICATIONAVECLABASEDEDONNEES_H
