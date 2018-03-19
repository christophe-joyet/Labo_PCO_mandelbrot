/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "renderthread.h"
#include "mandelbrotthread.h"

#include <QtWidgets>

#include <math.h>
#include <iostream>

RenderThread::RenderThread(QObject *parent)
    : QThread(parent)
{
    restart = false;
    abort = false;

    for (int i = 0; i < ColormapSize; ++i)
        colormap[i] = rgbFromWaveLength(380.0 + (i * 400.0 / ColormapSize));
}

RenderThread::~RenderThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void RenderThread::render(double centerX, double centerY, double scaleFactor,
                          QSize resultSize)
{
    QMutexLocker locker(&mutex);

    this->centerX = centerX;
    this->centerY = centerY;
    this->scaleFactor = scaleFactor;
    this->resultSize = resultSize;

    if (!isRunning()) {
        start(LowPriority);
    } else {
        restart = true;
        condition.wakeOne();
    }
}

void RenderThread::run()
{
    //Définit le nombre ideal de thread(s) qui peuvent tourner sur le système en cours
    int nbreThread = idealThreadCount();
    if(nbreThread > 0){
        std::cout << "Nombre de thread(s) ideal(s) pour votre machine = " << idealThreadCount() << std::endl;
    }else{
         std::cout << "Nombre de thread(s) ideal(s) non-détécé : par défaut nous mettons 1 thread" << std::endl;
         nbreThread = 1;
    }

    forever {
        mutex.lock();
        QSize resultSize   = this->resultSize;
        double scaleFactor = this->scaleFactor;
        double centerX     = this->centerX;
        double centerY     = this->centerY;
        mutex.unlock();

        int halfWidth = resultSize.width() / 2;
        int halfHeight = resultSize.height() / 2;

        const int NumPasses = 8;
        int pass = 0;
        QImage image(resultSize, QImage::Format_RGB32);

        //Séparation de la taille d'une image pour chaque thread.
        mandelbrotthread* threads[nbreThread];
        int dividedHeight = resultSize.height() / nbreThread; //on divise la hauteur en fonction du nombre de thread
        int heightCounter = -halfHeight;
        int threadIterator;

        //déclarer les thread avant le while -> évite qu'ils soient une variable locale à la boucle et soient supprimés à la fin du bloc
        for(threadIterator = 0; threadIterator < nbreThread; threadIterator++){
            threads[threadIterator] =
                    new mandelbrotthread(halfHeight,
                                         halfWidth,
                                         &restart,
                                         &abort,
                                         resultSize,
                                         scaleFactor,
                                         centerX,
                                         centerY,
                                         colormap,
                                         ColormapSize,
                                         heightCounter,
                                         (heightCounter + dividedHeight));

            heightCounter += dividedHeight;
        }


        while (pass < NumPasses) { //calcul un nombre d'itération maximum
            QImage image(resultSize, QImage::Format_RGB32); //déclaration de l'image à l'intérieur de la boucle while pour éviter les problèmes de concurrence

            //si on ferme la fenêtre, on doit attendre la fin d'execution de chaque thread
            if(abort){
                 for(threadIterator=0; threadIterator<nbreThread; threadIterator++)
                     threads[threadIterator]->wait();
                 return;
            }

            const int MaxIterations = (1 << (2 * pass + 6)) + 32;

            //initialisation des attributs des threads
            for(threadIterator=0; threadIterator<nbreThread; threadIterator++){
                threads[threadIterator]->setMaxIteration(MaxIterations);
                threads[threadIterator]->setImage(&image);
            }

            QTime startTime = QTime::currentTime();

            //lancement des thread à l'aide de la méthode start() qui appellera la méthode run()
            for(threadIterator=0; threadIterator<nbreThread; threadIterator++)
                threads[threadIterator]->start();

            //le programme principal attend que chaque thread ait terminé ses calculs
            for(threadIterator=0; threadIterator<nbreThread; threadIterator++)
                threads[threadIterator]->wait();

            QTime endTime = QTime::currentTime();
            std::cout << "Time for pass " << pass << " (in ms) : " << startTime.msecsTo(endTime) << std::endl;


                if (!restart)
                    emit renderedImage(image, scaleFactor);

                ++pass;
        }

        mutex.lock();
        if (!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();

    }


}

uint RenderThread::rgbFromWaveLength(double wave)
{
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    if (wave >= 380.0 && wave <= 440.0) {
        r = -1.0 * (wave - 440.0) / (440.0 - 380.0);
        b = 1.0;
    } else if (wave >= 440.0 && wave <= 490.0) {
        g = (wave - 440.0) / (490.0 - 440.0);
        b = 1.0;
    } else if (wave >= 490.0 && wave <= 510.0) {
        g = 1.0;
        b = -1.0 * (wave - 510.0) / (510.0 - 490.0);
    } else if (wave >= 510.0 && wave <= 580.0) {
        r = (wave - 510.0) / (580.0 - 510.0);
        g = 1.0;
    } else if (wave >= 580.0 && wave <= 645.0) {
        r = 1.0;
        g = -1.0 * (wave - 645.0) / (645.0 - 580.0);
    } else if (wave >= 645.0 && wave <= 780.0) {
        r = 1.0;
    }

    double s = 1.0;
    if (wave > 700.0)
        s = 0.3 + 0.7 * (780.0 - wave) / (780.0 - 700.0);
    else if (wave <  420.0)
        s = 0.3 + 0.7 * (wave - 380.0) / (420.0 - 380.0);

    r = pow(r * s, 0.8);
    g = pow(g * s, 0.8);
    b = pow(b * s, 0.8);
    return qRgb(int(r * 255), int(g * 255), int(b * 255));
}
