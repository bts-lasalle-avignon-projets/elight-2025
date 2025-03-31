#include "communicationbasededonnees.h"
#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QCoreApplication>

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

        QString configPath = QCoreApplication::applicationDirPath() +
                             "/config-base-de-donnees.ini";

        if(QFile::exists(configPath))
        {
            qDebug() << Q_FUNC_INFO << "config-base-de-donnees.ini";

            QSettings settings(configPath, QSettings::IniFormat);

            hostName = settings.value("Database/host", hostName).toString();
            dataBaseName =
              settings.value("Database/name", dataBaseName).toString();
            userName = settings.value("Database/user", userName).toString();
            password = settings.value("Database/password", password).toString();
        }
        else
        {
            qWarning() << Q_FUNC_INFO
                       << "Fichier config-base-de-donnees.ini non trouvé";
        }

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
