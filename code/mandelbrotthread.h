/**
 * MandelBrotThread.h
 * Auteurs : Marc Labie & Christophe Joyet
 * Date    : 19.03.2018
 *
 * Classe permettant d'implémenter des thread chargé de calculer une courbe de
 * MandleBrot sur une certaines zones d'une image (un certain nombre de ligne)
 * contenant un ensemble de pixels.
 *
 */

#ifndef MANDELBROTTHREAD_H
#define MANDELBROTTHREAD_H
#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include <QImage>
#include <QMutex>


class mandelbrotthread : public QThread
{
private:

    int halfWidth;
    int halfHeight;
    int MaxIterations;
    int width;

    int y_high;
    int y_low;

    bool* restart;
    bool* abort;

    QImage* image;
    QSize resultSize;
    QMutex mutex;

    double scaleFactor;
    double centerX;
    double centerY;

    uint* colormap;
    int ColormapSize;



public:

    /**
     * @brief mandelbrotthread : constructeur du thread
     * @param int    - halfHeight
     * @param int    - halfWidth
     * @param bool*  - restart
     * @param bool*  - abort
     * @param QSize  - resultSize
     * @param double - scaleFactor
     * @param double - centerX
     * @param double - centerY
     * @param uint*  - colormap
     * @param int    - ColormapSize
     * @param int    - y_low            : coordonnée y de la première ligne à
     *                                    calculer.
     * @param int    - y_high           : coordonnée y de la dernières ligne
     *                                    à calculer.
     */
    mandelbrotthread(int halfHeight,
                     int halfWidth,
                     bool* restart,
                     bool* abort,
                     QSize resultSize,
                     double scaleFactor,
                     double centerX,
                     double centerY,
                     uint* colormap,
                     int ColormapSize,
                     int y_low,
                     int y_high);

    /**
     * @brief Destructeur du thread.
     */
    ~mandelbrotthread();

    /**
     * @brief setMaxIteration : Donne une valeur à l'attribut maxIteration
     * @param int - MaxIterations
     */
    void setMaxIteration(int MaxIterations);

    /**
     * @brief setImage : permet à chaque thread d'avoir un pointeur vers l'image
     *                   à générer.
     * @param QImage - image : image de mandelbrot
     */
    void setImage(QImage *image);

    /**
     * \brief run Méthode lancée par le thread. Celle-ci calcule la courbe
     * de mandleBrot dans une zone de mémoire définit à la construction du
     * Thread.
     */
    void run() Q_DECL_OVERRIDE;
};

#endif // MANDELBROTTHREAD_H
