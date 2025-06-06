#ifndef BOITESEGMENT_H
#define BOITESEGMENT_H

#include <QtWidgets>

/**
 * @def ICONE_SEGMENT
 * @brief L'icone de la lampe du segment
 */
#define ICONE_SEGMENT "lampe.png"

/**
 * @def LARGEUR_ICONE_LAMPE
 * @brief La largeur de l'icone de la lampe
 */
#define LARGEUR_ICONE_LAMPE 43

/**
 * @def HAUTEUR_ICONE_LAMPE
 * @brief La hauteur de l'icone de la lampe
 */
#define HAUTEUR_ICONE_LAMPE 100

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

  protected:
    void mousePressEvent(QMouseEvent* event) override;

  signals:
    void segmentClique(int idSegment);
};

#endif // BOITESEGMENT_H
