#ifndef COMMUNICATIONBASEDEDONNEES_H
#define COMMUNICATIONBASEDEDONNEES_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>

/**
 * @def HOSTNAME
 * @brief Définit l'adresse du serveur MySQL
 */
#define HOSTNAME "127.0.0.1"

/**
 * @def USERNAME
 * @brief Définit le nom d'utilisateur par défaut
 */
#define USERNAME "user_eLight"

/**
 * @def PASSWORD
 * @brief Définit le mot de passe par défaut
 */
#define PASSWORD "lasalle84"

/**
 * @def DATABASENAME
 * @brief Définit le nom de la base de données par défaut
 */
#define DATABASENAME "eLight"

class CommunicationBaseDeDonnees : public QObject
{
    Q_OBJECT
  public:
    explicit CommunicationBaseDeDonnees(QObject* parent       = nullptr,
                                        QString  dataBaseName = DATABASENAME,
                                        QString  userName     = USERNAME,
                                        QString  password     = PASSWORD,
                                        QString  hostName     = HOSTNAME);
    ~CommunicationBaseDeDonnees();

    bool connecter();
    void deconnecter();
    bool estConnecte() const;

  private:
    QSqlDatabase baseDeDonnees;

  signals:
};

#endif // COMMUNICATIONBASEDEDONNEES_H
