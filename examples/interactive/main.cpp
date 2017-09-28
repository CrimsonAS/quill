#include <QtGui>
#include <QtWidgets>

#include "quill.h"

#include "rasterbuffer.h"
#include "solidcolorfill.h"

using namespace Quill;

class Widget : public QWidget
{
public:
    Widget()
    {
        m_timer.start();
    }

    void paintEvent(QPaintEvent *)
    {
        Stroker<Clipper<MonoRasterizer<SolidColorFill>>> stroker;

        stroker.rasterizer.x1 = width();
        stroker.rasterizer.y1 = height();

        SolidColorFill *fill = &stroker.rasterizer.raster.fill;
        fill->value = 0xffffffff;

        RasterBuffer *buffer = &fill->buffer;
        buffer->allocate(width(), height());
        buffer->fill(0xff000000);

        float cx = width() / 2.0f;
        float cy = height() / 2.0f;

        stroker.width = 10;
        stroker.joinStyle = RoundJoin;

        stroker.moveTo(cx, cy);

        float t = m_timer.nsecsElapsed() / 1000000000.0;
        float r = std::min(cx, cy) * 0.8;
        float x = r * cos(t) + cx;
        float y = r * sin(t) + cy;
        stroker.lineTo(x, y);
        stroker.lineTo(100, 100);

        QImage image((uchar *) buffer->data, buffer->width, buffer->height, QImage::Format_RGB32);

        QPainter p(this);
        p.drawImage(0, 0, image);

        update();
    }

private:
    QElapsedTimer m_timer;

};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Widget widget;
    widget.show();

    return app.exec();
}