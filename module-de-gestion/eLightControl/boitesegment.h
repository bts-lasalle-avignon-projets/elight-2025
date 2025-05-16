#ifndef BOITESEGMENT_H
#define BOITESEGMENT_H

#include <QtWidgets>

/**
 * @def ICONE_SEGMENT
 * @brief L'icone de la lampe du segment
 */
#define ICONE_SEGMENT "lampe.png"

/**
 * @def LARGEUR_ICONE
 * @brief La largeur de l'icone de la lampe
 */
#define LARGEUR_ICONE 100

/**
 * @def HAUTEUR_ICONE
 * @brief La hauteur de l'icone de la lampe
 */
#define HAUTEUR_ICONE 100

class BoiteSegment : public QWidget
{
    Q_OBJECT
  public:
    explicit BoiteSegment(int segmentId, QWidget* parent = nullptr);
    void setPuissance(double nouvellePuissance);
    int  getIdSegment();

  private:
    int     idSegment;
    double  puissance;
    QLabel* imageLabel;
    QLabel* puissanceLabel;
    void    recupererDonnees();
};

#endif // BOITESEGMENT_H
