#include "communicationbasededonnees.h"

CommunicationBaseDeDonnees::CommunicationBaseDeDonnees(QObject* parent) :
    QObject(parent)
{
    baseDeDonnees = QSqlDatabase::addDatabase("QMYSQL");
    baseDeDonnees.setHostName("localhost");
    baseDeDonnees.setDatabaseName("eLight");
    baseDeDonnees.setUserName("user_eLight");
    baseDeDonnees.setPassword("lasalle84");
}

CommunicationBaseDeDonnees::~CommunicationBaseDeDonnees()
{
    deconnecter();
}

bool CommunicationBaseDeDonnees::connecter()
{
    if(!baseDeDonnees.open())
    {
        qDebug() << "Erreur connexion MySQL :"
                 << baseDeDonnees.lastError().text();
        return false;
    }
    qDebug() << "Connexion réussie à MySQL !";
    return true;
}

void CommunicationBaseDeDonnees::deconnecter()
{
    if(baseDeDonnees.isOpen())
    {
        baseDeDonnees.close();
        qDebug() << "Base de données MySQL déconnectée.";
    }
}

bool CommunicationBaseDeDonnees::estConnecte() const
{
    return baseDeDonnees.isOpen();
}
