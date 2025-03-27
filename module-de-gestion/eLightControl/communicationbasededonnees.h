#ifndef COMMUNICATIONBASEDEDONNEES_H
#define COMMUNICATIONBASEDEDONNEES_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class CommunicationBaseDeDonnees : public QObject
{
    Q_OBJECT
  public:
    explicit CommunicationBaseDeDonnees(QObject* parent = nullptr);
    ~CommunicationBaseDeDonnees();
    bool connecter();
    void deconnecter();
    bool estConnecte() const;

  private:
    QSqlDatabase baseDeDonnees;

  signals:
};

#endif // COMMUNICATIONBASEDEDONNEES_H
