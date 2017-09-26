#include "quill.h"

using namespace Quill;

#include "rasterbuffer.h"
#include "solidcolorfiller.h"

#include <QtCore>
#include <QtGui>

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
    Stroker<LerpRaster<SolidColorFiller>> stroker;
    stroker.rasterizer.fill.value = 0xffffffff;

    RasterBuffer *buffer = &stroker.rasterizer.fill.buffer;
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

        if (counter % 100 == 0)
            if (timer.elapsed() > RUNNING_TIME)
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
    Stroker<LerpRaster<SolidColorFiller>> stroker;
    stroker.rasterizer.fill.value = 0xffffffff;

    RasterBuffer *buffer = &stroker.rasterizer.fill.buffer;
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

        if (counter % 100 == 0)
            if (timer.elapsed() > RUNNING_TIME)
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

        painter.setPen(QPen(Qt::white, PEN_WIDTH, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
        painter.drawPath(path);

        ++counter;

        if (counter % 100 == 0)
            if (timer.elapsed() > RUNNING_TIME)
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
                float width = PEN_WIDTH + 2 * ((i >> 1) & 0x1);
                painter.setPen(QPen(Qt::white, width, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
                first = QPointF(x, y);
            } else {
                painter.drawLine(first, QPointF(x, y));
            }
        }

        ++counter;

        if (counter % 100 == 0)
            if (timer.elapsed() > RUNNING_TIME)
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

        if (counter % 100 == 0)
            if (timer.elapsed() > RUNNING_TIME)
                break;
    }

    printf("Qt - line textured segments, AA=%d, %d elements, %d iterations in %dms, %f ops / msec      \n",
           antialiased,
           segments,
           counter,
           (int) timer.elapsed(),
           counter / float(timer.elapsed())
          );

    painter.end();
    image.save(QStringLiteral("qt_segments_textured_%2_%1.png").arg(segments).arg(antialiased ? "antialiased" : "aliased"));
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    // runQuillBenchmark_continuous(100);
    // runQuillBenchmark_continuous(1000);
    // runQuillBenchmark_continuous(10000);

    runQuillBenchmark_segments(100);
    runQuillBenchmark_segments(1000);
    runQuillBenchmark_segments(10000);

    // runQtBenchmark_continuous(100);
    // runQtBenchmark_continuous(1000);
    // runQtBenchmark_continuous(10000);

    // runQtBenchmark_segments(100);
    // runQtBenchmark_segments(1000);
    // runQtBenchmark_segments(10000);

    // runQtBenchmark_segments_textured(100);
    // runQtBenchmark_segments_textured(1000);
    // runQtBenchmark_segments_textured(10000);

    // runQtBenchmark_segments_textured(100, true);
    // runQtBenchmark_segments_textured(1000, true);
    // runQtBenchmark_segments_textured(10000, true);

    return 0;
}

