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
    mandelbrotthread(int halfHeight, int halfWidth, bool* restart, bool* abort,
                     QSize resultSize, double scaleFactor, double centerX, double centerY,
                     uint* colormap, int ColormapSize,int y_low, int y_high);

    void setMaxIteration(int MaxIterations);

    void setImage(QImage *image);

    /**
     * \brief run Méthode lancée par le thread
     */
    void run() Q_DECL_OVERRIDE;
};

#endif // MANDELBROTTHREAD_H
