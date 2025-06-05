#include "pageguide.h"
#include "config.h"
#include "blocfaq.h"
#include <QDebug>

PageGuide::PageGuide(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* layoutPage  = new QVBoxLayout(this);
    QHBoxLayout* layoutTitre = new QHBoxLayout;

    QLabel* titre = new QLabel("<h1>Guide</h1><br>");

    QFont police;
    police.setPointSize(TAILLE_POLICE);
    titre->setFont(police);

    layoutTitre->addWidget(titre);
    layoutPage->addLayout(layoutTitre);

    BlocFAQ* faq1 = new BlocFAQ(
      "À quoi sert cette application ?",
      "Cette application permet aux utilisateurs de contrôler l'intensité "
      "lumineuse de la salle via la sélection de scénario. Exemple : le "
      "scénario 'Salle de classe' offre une luminosité de 300 lux pour la "
      "salle.<br> Il est également possible de visualiser la puissance fournie "
      "par les segments de lumière de la salle.");
    BlocFAQ* faq2 = new BlocFAQ(
      "Comment faire pour ajouter un scénario ?",
      "Sur la page de gestion (<img src='./ressources/gestion.png' width='24' "
      "height='24' style='vertical-align: middle;'>), dans la partie création "
      ": Entrez un nom, l'intensité lumineuse voulue puis confirmez en "
      "appuyant sur valider (<img src='./ressources/valider.png' width='24' "
      "height='24' style='vertical-align: middle;'>).");
    BlocFAQ* faq3 = new BlocFAQ(
      "Comment faire pour modifier un scénario ?",
      "Sur la page de gestion (<img src='./ressources/gestion.png' width='24' "
      "height='24' style='vertical-align: middle;'>), dans la partie "
      "modification & suppression : Sélectionnez le scénario voulu, en dessous "
      "entrez un nom, l'intensité lumineuse voulue puis confirmez en appuyant "
      "sur valider (<img src='./ressources/valider.png' width='24' height='24' "
      "style='vertical-align: middle;'>).");
    BlocFAQ* faq4 =
      new BlocFAQ("Comment faire pour supprimer un scénario ?",
                  "Sur la page de gestion (<img src='./ressources/gestion.png' "
                  "width='24' height='24' style='vertical-align: middle;'>), "
                  "dans la partie modification & suppression : Sélectionnez le "
                  "scénario voulu puis supprimez le appuyant sur de "
                  "suppression (<img src='./ressources/poubelle.png' "
                  "width='24' height='24' style='vertical-align: middle;'>).");

    layoutPage->addWidget(faq1);
    layoutPage->addWidget(faq2);
    layoutPage->addWidget(faq3);
    layoutPage->addWidget(faq4);
    layoutPage->addStretch();

    QList<BlocFAQ*> toutesLesFAQs;

    toutesLesFAQs << faq1 << faq2 << faq3 << faq4;

    for(BlocFAQ* faq: toutesLesFAQs)
    {
        connect(faq, &BlocFAQ::demandeOuverture, this, [=](BlocFAQ* source) {
            for(BlocFAQ* autreFaq: toutesLesFAQs)
            {
                if(autreFaq != source)
                    autreFaq->fermer();
            }
            source->basculer();
        });
    }
}
