#include "quill.h"

using namespace Quill;

#include "rasterbuffer.h"
#include "solidcolorfiller.h"

#include <QtCore>
#include <QtGui>

void runQuillBenchmark_segments(int segments)
{
    Stroker<MonoRasterizer<SolidColorFiller>> stroker;
    stroker.rasterizer.fill.value = 0xffffffff;

    RasterBuffer *buffer = &stroker.rasterizer.fill.buffer;
    buffer->fill(0xff000000);
    buffer->allocate(1000, 1000);

    QElapsedTimer timer; timer.start();

    int counter = 0;
    while (true) {

        float cx = buffer->width / 2;
        float cy = buffer->height / 2;

        stroker.reset();

        stroker.width = 10;

        for (int i=0; i<segments; ++i) {
            float t = (i / float(segments - 1));
            float x = sin(t * M_PI * 2 * 8) * t * cx * 0.8 + cx;
            float y = cos(t * M_PI * 2 * 8) * t * cy * 0.8 + cy;

            if ((i & 0x1) == 0) {
                stroker.width = 10 + 5 * ((i >> 1) & 0x1);
                stroker.moveTo(x, y);
            } else {
                stroker.lineTo(x, y);
            }
        }

        ++counter;

        if (counter % 100 == 0)
            if (timer.elapsed() > 5000)
                break;
    }

    printf("Quill - line segments, %d elements, %d iterations in %dms, %f ops / msec      \n",
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
    Stroker<MonoRasterizer<SolidColorFiller>> stroker;
    stroker.rasterizer.fill.value = 0xffffffff;

    RasterBuffer *buffer = &stroker.rasterizer.fill.buffer;
    buffer->fill(0xff000000);
    buffer->allocate(1000, 1000);

    QElapsedTimer timer; timer.start();

    int counter = 0;
    while (true) {

        float cx = buffer->width / 2;
        float cy = buffer->height / 2;

        stroker.reset();
        stroker.width = 10;
        stroker.moveTo(cx, cy);

        for (int i=0; i<segments; ++i) {
            float t = (i / float(segments - 1));
            float x = sin(t * M_PI * 2 * 8) * t * cx * 0.8 + cx;
            float y = cos(t * M_PI * 2 * 8) * t * cy * 0.8 + cy;
            stroker.lineTo(x, y);
        }

        ++counter;

        if (counter % 100 == 0)
            if (timer.elapsed() > 5000)
                break;
    }

    printf("Quill - continuous path, %d elements, %d iterations in %dms, %f ops / msec      \n",
           segments,
           counter,
           (int) timer.elapsed(),
           counter / float(timer.elapsed())
          );

    QImage image((unsigned char *) buffer->data, buffer->width, buffer->height, QImage::Format_RGB32);
    image.save(QStringLiteral("quill_continuous_%1.png").arg(segments));
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

        painter.setPen(QPen(Qt::white, 10, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
        painter.drawPath(path);

        ++counter;

        if (counter % 100 == 0)
            if (timer.elapsed() > 5000)
                break;
    }

    printf("Qt - continuous path, %d elements, %d iterations in %dms, %f ops / msec      \n",
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
                float width = 10 + 5 * ((i >> 1) & 0x1);
                painter.setPen(QPen(Qt::white, width, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
                first = QPointF(x, y);
            } else {
                painter.drawLine(first, QPointF(x, y));
            }
        }

        ++counter;

        if (counter % 100 == 0)
            if (timer.elapsed() > 5000)
                break;
    }

    printf("Qt - line segments, %d elements, %d iterations in %dms, %f ops / msec      \n",
           segments,
           counter,
           (int) timer.elapsed(),
           counter / float(timer.elapsed())
          );

    painter.end();
    image.save(QStringLiteral("qt_segments_%1.png").arg(segments));
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    runQuillBenchmark_continuous(100);
    runQuillBenchmark_continuous(1000);
    runQuillBenchmark_continuous(10000);

    runQuillBenchmark_segments(100);
    runQuillBenchmark_segments(1000);
    runQuillBenchmark_segments(10000);

    runQtBenchmark_continuous(100);
    runQtBenchmark_continuous(1000);
    runQtBenchmark_continuous(10000);

    runQtBenchmark_segments(100);
    runQtBenchmark_segments(1000);
    runQtBenchmark_segments(10000);

    return 0;
}

