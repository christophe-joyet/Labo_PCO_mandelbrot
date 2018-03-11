#include "mandelbrotthread.h"
#include <QtWidgets>

mandelbrotthread::mandelbrotthread(int halfHeight, int halfWidth, bool restart, bool abort, QImage image,
                                   QSize resultSize, double scaleFactor, double centerX, double centerY,
                                   int MaxIterations,uint* colormap,int ColormapSize)
{
    this->halfHeight   = halfHeight;
    this->halfWidth    = halfWidth;
    this->restart      = restart;
    this->abort        = abort;
    this->image        = image;
    this->resultSize   = resultSize;
    this->scaleFactor  = scaleFactor;
    this->centerX      = centerX;
    this->centerY      = centerY;
    this->MaxIterations = MaxIterations;
    this->colormap     = colormap;
    this->ColormapSize = ColormapSize;

}

void mandelbrotthread::run()
{
    const int Limit = 4;

    for (int y = -halfHeight; y < halfHeight; ++y) {
        if (restart)
            break;
        if (abort)
            return;

        QRgb *scanLine = reinterpret_cast<QRgb *>(image.scanLine(y + halfHeight));
        double ay = centerY + (y * scaleFactor);

        for (int x = -halfWidth; x < halfWidth; ++x) {
            double ax = centerX + (x * scaleFactor);
            double a1 = ax;
            double b1 = ay;
            int numIterations = 0;

            do {
                ++numIterations;
                double a2 = (a1 * a1) - (b1 * b1) + ax;
                double b2 = (2 * a1 * b1) + ay;
                if ((a2 * a2) + (b2 * b2) > Limit)
                    break;

                ++numIterations;
                a1 = (a2 * a2) - (b2 * b2) + ax;
                b1 = (2 * a2 * b2) + ay;
                if ((a1 * a1) + (b1 * b1) > Limit)
                    break;
            } while (numIterations < MaxIterations);

            if (numIterations < MaxIterations) {
                *scanLine++ = colormap[numIterations % ColormapSize];
            } else {
                *scanLine++ = qRgb(0, 0, 0);
            }
        }
    }

}


