#include "pageparametres.h"
#include <QDebug>

PageParametres::PageParametres(QWidget* parent) :
    QWidget(parent), baseDeDonnees(CommunicationBaseDeDonnees::getInstance())
{
    QLabel* titreParametres = new QLabel(this);
    boutonRetourParametres  = new QPushButton(this);

    QLabel* texteIp              = new QLabel(this);
    boiteIp                      = new QLineEdit(this);
    QPushButton* boutonValiderIp = new QPushButton(this);

    titreParametres->setText("<h1>Paramètres</h1>");
    boutonRetourParametres->setText("Retour");

    texteIp->setText("Ip du segment : ");
    boiteIp->setPlaceholderText("xxx.xxx.xxx.xxx");
    boutonValiderIp->setText("Valider");

    QVBoxLayout* layoutVertical = new QVBoxLayout(this);

    QHBoxLayout* layoutEntete = new QHBoxLayout();
    QHBoxLayout* layoutSalle  = new QHBoxLayout();
    QHBoxLayout* layoutIp     = new QHBoxLayout();

    layoutVertical->addLayout(layoutEntete);
    layoutVertical->addStretch();
    layoutVertical->addLayout(layoutSalle);
    layoutVertical->addLayout(layoutIp);

    layoutEntete->addWidget(titreParametres);
    layoutEntete->addWidget(boutonRetourParametres);

    layoutIp->addWidget(texteIp);
    layoutIp->addWidget(boiteIp);
    layoutIp->addWidget(boutonValiderIp);

    chargerIpBoitierDepuisBDD();

    connect(boutonValiderIp, &QPushButton::clicked, this, [=] {
        validerParametreIp();
    });
}

QPushButton* PageParametres::getBoutonRetourParametre() const
{
    return boutonRetourParametres;
}

void PageParametres::validerParametreIp()
{
    QString nouvelleIp = boiteIp->text();

    if(!nouvelleIp.isEmpty())
    {
        QString nomSalle;
        if(recupererNomSalle(nomSalle))
        {
            QSqlQuery requete;
            requete.prepare("UPDATE salle SET ip_boitier = :nouvelle_ip WHERE "
                            "nom_salle = :nom_salle");
            requete.bindValue(":nouvelle_ip", nouvelleIp);
            requete.bindValue(":nom_salle", nomSalle);

            if(!requete.exec())
            {
                qDebug() << Q_FUNC_INFO << "Erreur SQL"
                         << requete.lastError().text();
            }
            else
            {
                chargerIpBoitierDepuisBDD();
                qDebug() << Q_FUNC_INFO << "nouvelleIp" << nouvelleIp;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << "Erreur récupération nom de la salle";
        }
    }
}

bool PageParametres::recupererNomSalle(QString& nomSalle)
{
    QString cheminConfiguration =
      QCoreApplication::applicationDirPath() + "/config.ini";

    if(QFile::exists(cheminConfiguration))
    {
        qDebug() << Q_FUNC_INFO << "cheminConfiguration" << cheminConfiguration;

        QSettings parametres(cheminConfiguration, QSettings::IniFormat);
        nomSalle = parametres.value("Salle/nom").toString();

        return true;
    }
    else
    {
        return false;
    }
}

void PageParametres::chargerIpBoitierDepuisBDD()
{
    QSqlQuery requete;
    QString   nomSalle;
    if(recupererNomSalle(nomSalle))
    {
        requete.prepare(
          "SELECT ip_boitier FROM salle WHERE nom_salle = :nom_salle");
        requete.bindValue(":nom_salle", nomSalle);

        if(!requete.exec())
        {
            qDebug() << Q_FUNC_INFO << "Erreur SQL"
                     << requete.lastError().text();
            return;
        }

        while(requete.next())
        {
            boiteIp->setPlaceholderText(requete.value(0).toString());
        }
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "Erreur récupération nom de la salle";
    }
}
