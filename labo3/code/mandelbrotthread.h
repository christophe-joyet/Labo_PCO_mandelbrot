#ifndef MANDELBROTTHREAD_H
#define MANDELBROTTHREAD_H
#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include <QImage>


class mandelbrotthread : public QThread
{
private:
    int halfWidth;
    int halfHeight;
    int MaxIterations;

    bool restart;
    bool abort;

    QImage image;
    QSize resultSize;

    double scaleFactor;
    double centerX;
    double centerY;

    uint *colormap;
    int ColormapSize;


public:
    mandelbrotthread(int halfHeight, int halfWidth, bool restart, bool abort, QImage image,
                     QSize resultSize, double scaleFactor, double centerX, double centerY,
                     int MaxIterations, uint *colormap, int ColormapSize);

    /**
     * \brief run Méthode lancée par le thread
     */
    void run() Q_DECL_OVERRIDE;
};

#endif // MANDELBROTTHREAD_H
