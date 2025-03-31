#ifndef COMMUNICATIONBASEDEDONNEES_H
#define COMMUNICATIONBASEDEDONNEES_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>

/**
 * @def NOM_HOTE
 * @brief Définit l'adresse du serveur MySQL
 */
#define NOM_HOTE "127.0.0.1"

/**
 * @def NOM_UTILISATEUR
 * @brief Définit le nom d'utilisateur par défaut
 */
#define NOM_UTILISATEUR "user_eLight"

/**
 * @def MOT_DE_PASSE
 * @brief Définit le mot de passe par défaut
 */
#define MOT_DE_PASSE "lasalle84"

/**
 * @def NOM_BASE_DE_DONNEES
 * @brief Définit le nom de la base de données par défaut
 */
#define NOM_BASE_DE_DONNEES "eLight"

class CommunicationBaseDeDonnees : public QObject
{
    Q_OBJECT
  public:
    explicit CommunicationBaseDeDonnees(QObject* parent = nullptr);
    ~CommunicationBaseDeDonnees();

    bool connecter(QString nomBaseDeDonnees = NOM_BASE_DE_DONNEES,
                   QString nomUtilisateur   = NOM_UTILISATEUR,
                   QString motDePasse       = MOT_DE_PASSE,
                   QString nomHote          = NOM_HOTE);
    void deconnecter();
    bool estConnecte() const;
    void chargerConfiguration(QString& nomHote,
                              QString& nomBaseDeDonnees,
                              QString& nomUtilisateur,
                              QString& motDePasse);

  private:
    QSqlDatabase baseDeDonnees;

  signals:
};

#endif // COMMUNICATIONBASEDEDONNEES_H
