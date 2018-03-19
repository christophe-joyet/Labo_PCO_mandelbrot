/**
 * MandelBrotThread.cpp
 * Auteurs : Marc Labie & Christophe Joyet
 * Date    : 19.03.2018
 */

#include "mandelbrotthread.h"
#include <QtWidgets>

/**
 * @brief Constructeur du thread
 */
mandelbrotthread::mandelbrotthread(int halfHeight, int halfWidth, bool* restart, bool* abort,
                 QSize resultSize, double scaleFactor, double centerX, double centerY,
                 uint* colormap, int ColormapSize, int y_low, int y_high)
{
    this->halfHeight    = halfHeight;
    this->halfWidth     = halfWidth;
    this->restart       = restart;
    this->abort         = abort;
    this->image         = nullptr; //sera initialisé dans la boucle
    this->resultSize    = resultSize;
    this->scaleFactor   = scaleFactor;
    this->centerX       = centerX;
    this->centerY       = centerY;
    this->MaxIterations = 0;       //sera initialisé dans la boucle
    this->colormap      = colormap;
    this->ColormapSize  = ColormapSize;
    this->y_high        = y_high; //délimite la partie que le thread doit afficher
    this->y_low         = y_low;
}

/**
 * @brief setMaxIteration : Donner une valeur à l'attribut MaxIterations
 * @param MaxIterations   : int
 */
void mandelbrotthread::setMaxIteration(int MaxIterations){
    this->MaxIterations = MaxIterations;
}

/**
 * @brief setImage  : Donner une pointeur sur une QImage
 * @param image     : pointeur sur une QImage
 */
void mandelbrotthread::setImage(QImage *image){
    this->image = image;
}


/**
 * @brief run   : Méthode appelée par start() pour lancer un thread
 */
void mandelbrotthread::run()
{

    const int Limit = 4;

    //ici on délimite l'image que le thread va afficher grâce aux valeurs y_low et y_high
    for (int y = y_low; y < y_high; ++y) {
        if (*restart)
            break;
        if (*abort)
            return;

        QRgb *scanLine = reinterpret_cast<QRgb *>(image->scanLine(y + halfHeight));
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


