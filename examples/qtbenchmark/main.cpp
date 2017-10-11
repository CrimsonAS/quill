/*
    Copyright (c) 2017, Gunnar Sletta <gunnar@crimson.no>
    Copyright (c) 2017, reMarkable AS <technology@remarkable.no>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "quill.h"

using namespace Quill;

#include "rasterbuffer.h"
#include "solidcolorfill.h"

#include <QtCore>
#include <QtGui>

#define STB_PERLIN_IMPLEMENTATION
#include "../stb_perlin.h"

static int RUNNING_TIME = 1000;

const int PEN_WIDTH = 10;

template <typename Rasterizer, typename TriangleType>
struct TriangleVsEdge
{
    typedef TriangleType Triangle;

    float x0 = 0.0f;
    float x1 = 1000.0f;
    float y0 = 0.0f;
    float y1 = 1000.0f;

    Rasterizer rasterizer;

    void operator()(Triangle t) {
        if (t.a.x < x0 || t.b.x < x0 || t.c.x < x0
            || t.a.x > x1 || t.b.x > x1 || t.c.x > x1
            || t.a.y < y0 || t.b.y < y0 || t.c.y < y0
            || t.a.y > y1 || t.b.y > y1 || t.c.y > y1)
            return;
        rasterizer(t);
    }
};

void runQuillBenchmark_segments(int segments)
{
    Stroker<MonoRaster<SolidColorFill>> stroker;
    stroker.raster.fill.value = 0xffffffff;

    RasterBuffer *buffer = &stroker.raster.fill.buffer;
    buffer->allocate(1000, 1000);
    buffer->fill(0xff000000);

    QElapsedTimer timer; timer.start();

    int counter = 0;
    while (true) {

        float cx = buffer->width / 2;
        float cy = buffer->height / 2;

        stroker.reset();

        stroker.width = PEN_WIDTH;

        for (int i=0; i<segments; ++i) {

            // const int HIT = 92;
            // if (i != HIT && i != HIT+1)
            //     continue;

            float t = (i / float(segments - 1));
            float x = sin(t * M_PI * 2 * 8) * t * cx * 0.8 + cx;
            float y = cos(t * M_PI * 2 * 8) * t * cy * 0.8 + cy;

            if ((i & 0x1) == 0) {
                stroker.width = PEN_WIDTH + 2 * ((i >> 1) & 0x1);
                stroker.moveTo(x, y);
            } else {
                stroker.lineTo(x, y);
            }
        }

        ++counter;

        if (counter % 25 == 0)
            if (timer.elapsed() > RUNNING_TIME)
                break;
    }

    printf("Quill - line segments, %5d elements, %4d iterations in %5dms, %7.3f ops / msec      \n",
           segments,
           counter,
           (int) timer.elapsed(),
           counter / float(timer.elapsed())
          );

    QImage image((unsigned char *) buffer->data, buffer->width, buffer->height, QImage::Format_RGB32);
    image.save(QStringLiteral("quill_segments_%1.png").arg(segments));
}

void runQuillBenchmark_continuous(int segments)
{
    Stroker<MonoRaster<SolidColorFill>> stroker;
    stroker.raster.fill.value = 0xffffffff;

    RasterBuffer *buffer = &stroker.raster.fill.buffer;
    buffer->allocate(1000, 1000);
    buffer->fill(0xff000000);

    QElapsedTimer timer; timer.start();

    int counter = 0;

    while (true) {

        float cx = buffer->width / 2;
        float cy = buffer->height / 2;

        stroker.reset();
        stroker.width = PEN_WIDTH;
        stroker.moveTo(cx, cy);

        for (int i=0; i<segments; ++i) {
            float t = (i / float(segments - 1));
            float x = sin(t * M_PI * 2 * 8) * t * cx * 0.8 + cx;
            float y = cos(t * M_PI * 2 * 8) * t * cy * 0.8 + cy;
            stroker.lineTo(x, y);
        }

        ++counter;

        if (counter % 25 == 0)
            if (timer.elapsed() > RUNNING_TIME)
                break;
    }

    printf("Quill - continuous path, %5d elements, %4d iterations in %5dms, %7.3f ops / msec      \n",
           segments,
           counter,
           (int) timer.elapsed(),
           counter / float(timer.elapsed())
          );

    QImage image((unsigned char *) buffer->data, buffer->width, buffer->height, QImage::Format_RGB32);
    image.save(QStringLiteral("quill_continuous_%1.png").arg(segments));
}

struct SimpleFill
{
    typedef VaryingUV Varyings;
    RasterBuffer buffer;
    void operator()(Vertex pos, int length, VaryingUV v, VaryingUV dx) {
        unsigned int *dst = buffer.scanline((int) pos.y) + (int) pos.x;
        for (int i=0; i<length; ++i) {
            float n = stb_perlin_noise3(v.u, v.v, 0.0f, 0, 0, 0);
            if (n > 0)
                dst[i] = 0xffffffff;
            v = v + dx;
        }
    }
};

void runQuillBenchmark_segments_textured(int segments)
{
    Stroker<LerpRaster<SimpleFill>, VaryingGeneratorLengthWidth> stroker;

    RasterBuffer *buffer = &stroker.raster.fill.buffer;
    buffer->allocate(1000, 1000);
    buffer->fill(0xff000000);

    QElapsedTimer timer; timer.start();
    int counter = 0;
    while (true) {

        float cx = buffer->width / 2;
        float cy = buffer->height / 2;

        QPointF first;
        float width = 0;

        for (int i=0; i<segments; ++i) {
            float t = (i / float(segments - 1));
            float x = sin(t * M_PI * 2 * 8) * t * cx * 0.8 + cx;
            float y = cos(t * M_PI * 2 * 8) * t * cy * 0.8 + cy;

            if ((i & 0x1) == 0) {
                width = PEN_WIDTH + 2 * ((i >> 1) & 0x1);
                first = QPointF(x, y);
            } else {
                stroker.width = width;
                stroker.length = i;
                stroker.moveTo(first.x(), first.y());
                stroker.lineTo(x, y);
            }
        }

        ++counter;

        if (counter % 25 == 0)
            if (timer.elapsed() > RUNNING_TIME)
                break;
    }

    printf("Quill - textured line segments, %5d elements, %4d iterations in %5dms, %7.3f ops / msec      \n",
           segments,
           counter,
           (int) timer.elapsed(),
           counter / float(timer.elapsed())
          );

    QImage image((unsigned char *) buffer->data, buffer->width, buffer->height, QImage::Format_RGB32);
    image.save(QStringLiteral("quill_segments_textured_%2.png").arg(segments));
}



void runQtBenchmark_continuous(int segments)
{
    QImage image(1000, 1000, QImage::Format_RGB32);
    image.fill(Qt::black);

    QPainter painter(&image);

    QElapsedTimer timer; timer.start();

    int counter = 0;
    while (true) {

        float cx = image.width() / 2;
        float cy = image.height() / 2;

        QPainterPath path;
        path.moveTo(cx, cy);
        for (int i=0; i<segments; ++i) {
            float t = (i / float(segments - 1));
            float x = sin(t * M_PI * 2 * 8) * t * cx * 0.8 + cx;
            float y = cos(t * M_PI * 2 * 8) * t * cy * 0.8 + cy;
            path.lineTo(x, y);
        }

        painter.setPen(QPen(Qt::white, PEN_WIDTH, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
        painter.drawPath(path);

        ++counter;

        if (counter % 25 == 0)
            if (timer.elapsed() > RUNNING_TIME)
                break;
    }

    printf("Qt    - continuous path, %5d elements, %4d iterations in %5dms, %7.3f ops / msec      \n",
           segments,
           counter,
           (int) timer.elapsed(),
           counter / float(timer.elapsed())
          );

    painter.end();
    image.save(QStringLiteral("qt_continuous_%1.png").arg(segments));
}

void runQtBenchmark_segments(int segments)
{
    QImage image(1000, 1000, QImage::Format_RGB32);
    image.fill(Qt::black);

    QPainter painter(&image);

    QElapsedTimer timer; timer.start();

    int counter = 0;
    while (true) {

        float cx = image.width() / 2;
        float cy = image.height() / 2;

        QPointF first;

        for (int i=0; i<segments; ++i) {
            float t = (i / float(segments - 1));
            float x = sin(t * M_PI * 2 * 8) * t * cx * 0.8 + cx;
            float y = cos(t * M_PI * 2 * 8) * t * cy * 0.8 + cy;

            if ((i & 0x1) == 0) {
                float width = PEN_WIDTH + 2 * ((i >> 1) & 0x1);
                painter.setPen(QPen(Qt::white, width, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
                first = QPointF(x, y);
            } else {
                painter.drawLine(first, QPointF(x, y));
            }
        }

        ++counter;

        if (counter % 25 == 0)
            if (timer.elapsed() > RUNNING_TIME)
                break;
    }

    printf("Qt    - line segments, %5d elements, %4d iterations in %5dms, %7.3f ops / msec      \n",
           segments,
           counter,
           (int) timer.elapsed(),
           counter / float(timer.elapsed())
          );

    painter.end();
    image.save(QStringLiteral("qt_segments_%1.png").arg(segments));
}

void runQtBenchmark_segments_textured(int segments, bool antialiased = false)
{
    QImage image(1000, 1000, QImage::Format_RGB32);
    image.fill(Qt::white);

    QImage texture(64, 64, QImage::Format_Mono);
    texture.fill(Qt::color0);
    for (int i=0; i<32 * 32; ++i) {
        int x = rand() % 64;
        int y = rand() % 64;
        texture.setPixel(x, y, Qt::color1);
    }
    QBitmap bitmapTexture = QBitmap::fromImage(texture);

    QPainter painter(&image);

    if (antialiased)
        painter.setRenderHint(QPainter::Antialiasing);

    QElapsedTimer timer; timer.start();

    int counter = 0;
    while (true) {

        float cx = image.width() / 2;
        float cy = image.height() / 2;

        QPointF first;

        for (int i=0; i<segments; ++i) {
            float t = (i / float(segments - 1));
            float x = sin(t * M_PI * 2 * 8) * t * cx * 0.8 + cx;
            float y = cos(t * M_PI * 2 * 8) * t * cy * 0.8 + cy;

            if ((i & 0x1) == 0) {
                float width = PEN_WIDTH + 2 * ((i >> 1) & 0x1);
                QBrush brush(bitmapTexture);
                QTransform xform;
                xform.rotate(i);
                painter.setPen(QPen(brush, width, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
                first = QPointF(x, y);
            } else {
                painter.drawLine(first, QPointF(x, y));
            }
        }

        ++counter;

        if (counter % 10 == 0)
            if (timer.elapsed() > RUNNING_TIME)
                break;
    }

    printf("Qt    - textured line segments, %5d elements, %4d iterations in %5dms, %7.3f ops / msec (AA=%d)\n",
           segments,
           counter,
           (int) timer.elapsed(),
           counter / float(timer.elapsed()),
           antialiased
          );

    painter.end();
    image.save(QStringLiteral("qt_segments_textured_%2_%1.png").arg(segments).arg(antialiased ? "antialiased" : "aliased"));
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    runQuillBenchmark_continuous(100);
    runQuillBenchmark_continuous(1000);
    runQuillBenchmark_continuous(10000);
    runQtBenchmark_continuous(100);
    runQtBenchmark_continuous(1000);
    runQtBenchmark_continuous(10000);
    std::cout << std::endl;


    runQuillBenchmark_segments(100);
    runQuillBenchmark_segments(1000);
    runQuillBenchmark_segments(10000);
    runQtBenchmark_segments(100);
    runQtBenchmark_segments(1000);
    runQtBenchmark_segments(10000);
    std::cout << std::endl;

    runQuillBenchmark_segments_textured(100);
    runQuillBenchmark_segments_textured(1000);
    runQuillBenchmark_segments_textured(10000);
    runQtBenchmark_segments_textured(100);
    runQtBenchmark_segments_textured(1000);
    runQtBenchmark_segments_textured(10000);

    // runQtBenchmark_segments_textured(100, true);
    // runQtBenchmark_segments_textured(1000, true);
    // runQtBenchmark_segments_textured(10000, true);

    return 0;
}

