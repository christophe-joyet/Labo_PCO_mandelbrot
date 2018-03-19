/**
 * MandelBrotThread.h
 * Auteurs : Marc Labie & Christophe Joyet
 * Date    : 19.03.2018
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
     * @param int    - y_low
     * @param int    - y_high
     */
    mandelbrotthread(int halfHeight, int halfWidth, bool* restart, bool* abort,
                     QSize resultSize, double scaleFactor, double centerX, double centerY,
                     uint* colormap, int ColormapSize,int y_low, int y_high);

    /**
     * @brief setMaxIteration : Donne une valeur à l'attribut maxIteration
     * @param int - MaxIterations
     */
    void setMaxIteration(int MaxIterations);

    /**
     * @brief setImage : permet à chaque thread d'avoir un pointeur vers l'image
     * @param QImage - image : image de mandelbrot
     */
    void setImage(QImage *image);

    /**
     * \brief run Méthode lancée par le thread
     */
    void run() Q_DECL_OVERRIDE;
};

#endif // MANDELBROTTHREAD_H
