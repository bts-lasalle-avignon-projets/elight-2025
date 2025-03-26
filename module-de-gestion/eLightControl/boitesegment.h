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
    void setConsommation(double nouvelleConsommation);

  private:
    int     idSegment;
    double  consommation;
    QLabel* imageLabel;
    QLabel* consommationLabel;
    void    recupererDonnees();
};

#endif // BOITESEGMENT_H
