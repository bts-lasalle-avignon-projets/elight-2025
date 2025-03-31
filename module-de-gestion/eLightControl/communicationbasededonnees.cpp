#include "communicationbasededonnees.h"
#include <QDebug>

CommunicationBaseDeDonnees::CommunicationBaseDeDonnees(QObject* parent) :
    QObject(parent)
{
    qDebug() << Q_FUNC_INFO << this;
    baseDeDonnees = QSqlDatabase::addDatabase("QMYSQL");
}

CommunicationBaseDeDonnees::~CommunicationBaseDeDonnees()
{
    deconnecter();
    qDebug() << Q_FUNC_INFO << this;
}

bool CommunicationBaseDeDonnees::connecter(
  QString dataBaseName /*= DATABASENAME*/,
  QString userName /*= USERNAME*/,
  QString password /*= PASSWORD*/,
  QString hostName /*= HOSTNAME*/)
{
    if(!estConnecte())
    {
        qDebug() << Q_FUNC_INFO << "dataBaseName" << dataBaseName << "userName"
                 << userName << "password" << password << "hostName"
                 << hostName;
        /**
         * @todo mettre en place un fichier INI pour les paramètres de connexion
         */
        baseDeDonnees.setHostName(hostName);
        baseDeDonnees.setDatabaseName(dataBaseName);
        baseDeDonnees.setUserName(userName);
        baseDeDonnees.setPassword(password);
        if(!baseDeDonnees.open())
        {
            qCritical() << "Erreur connexion MySQL :"
                        << baseDeDonnees.lastError().text();
            return false;
        }
    }

    return true;
}

void CommunicationBaseDeDonnees::deconnecter()
{
    if(baseDeDonnees.isOpen())
    {
        baseDeDonnees.close();
    }
}

bool CommunicationBaseDeDonnees::estConnecte() const
{
    return baseDeDonnees.isOpen();
}
