#include "communicationbasededonnees.h"
#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QCoreApplication>

CommunicationBaseDeDonnees* CommunicationBaseDeDonnees::bdd     = nullptr;
int                         CommunicationBaseDeDonnees::nbAcces = 0;

CommunicationBaseDeDonnees* CommunicationBaseDeDonnees::creerInstance()
{
    if(bdd == nullptr)
    {
        bdd = new CommunicationBaseDeDonnees();
    }
    nbAcces++;
    qDebug() << Q_FUNC_INFO << "nbAcces" << nbAcces;

    return bdd;
}

void CommunicationBaseDeDonnees::detruireInstance()
{
    if(bdd != nullptr)
    {
        nbAcces--;
        qDebug() << Q_FUNC_INFO << "nbAcces" << nbAcces;
        if(nbAcces == 0)
            delete bdd;
    }
}

CommunicationBaseDeDonnees::CommunicationBaseDeDonnees()
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
  QString nomBaseDeDonnees /*= NOM_BASE_DE_DONNEES*/,
  QString nomUtilisateur /*= NOM_UTILISATEUR*/,
  QString motDePasse /*= MOT_DE_PASSE*/,
  QString nomHote /*= NOM_HOTE*/)
{
    if(!estConnecte())
    {
        qDebug() << Q_FUNC_INFO << "nomBaseDeDonnees" << nomBaseDeDonnees
                 << "nomUtilisateur" << nomUtilisateur << "motDePasse"
                 << motDePasse << "nomHote" << nomHote;

        chargerConfiguration(nomHote,
                             nomBaseDeDonnees,
                             nomUtilisateur,
                             motDePasse);

        baseDeDonnees.setHostName(nomHote);
        baseDeDonnees.setDatabaseName(nomBaseDeDonnees);
        baseDeDonnees.setUserName(nomUtilisateur);
        baseDeDonnees.setPassword(motDePasse);
        if(!baseDeDonnees.open())
        {
            qCritical() << "Erreur connexion MySQL"
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

void CommunicationBaseDeDonnees::chargerConfiguration(QString& nomHote,
                                                      QString& nomBaseDeDonnees,
                                                      QString& nomUtilisateur,
                                                      QString& motDePasse)
{
    QString cheminConfiguration =
      QCoreApplication::applicationDirPath() + "/config-base-de-donnees.ini";

    if(QFile::exists(cheminConfiguration))
    {
        qDebug() << Q_FUNC_INFO << "cheminConfiguration" << cheminConfiguration;

        QSettings parametres(cheminConfiguration, QSettings::IniFormat);

        nomHote = parametres.value("BaseDeDonnees/hote", nomHote).toString();
        nomBaseDeDonnees =
          parametres.value("BaseDeDonnees/nom", nomBaseDeDonnees).toString();
        nomUtilisateur =
          parametres.value("BaseDeDonnees/utilisateur", nomUtilisateur)
            .toString();
        motDePasse =
          parametres.value("BaseDeDonnees/motDePasse", motDePasse).toString();
    }
    else
    {
        qWarning() << Q_FUNC_INFO << "Fichier de configuration non trouvé"
                   << cheminConfiguration;
    }
}
