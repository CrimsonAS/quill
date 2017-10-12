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
        Stroker<ClipRaster<MonoRaster<SolidColorFill>>> stroker;

        stroker.raster.x1 = width();
        stroker.raster.y1 = height();

        SolidColorFill *fill = &stroker.raster.raster.fill;
        fill->value = 0xffffffff;

        RasterBuffer *buffer = &fill->buffer;
        buffer->allocate(width(), height());
        buffer->fill(0xff000000);

        float cx = width() / 2.0f;
        float cy = height() / 2.0f;

        stroker.width = 10;
        stroker.joinStyle = BevelJoin;
        stroker.capStyle = RoundCap;

        stroker.moveTo(cx, cy);

        float t = m_timer.nsecsElapsed() / 1000000000.0;
        float r = std::min(cx, cy) * 0.8;
        float x = r * cos(t) + cx;
        float y = r * sin(t) + cy;
        stroker.lineTo(x, y);
        stroker.width = 20;
        stroker.lineTo(100, 100);
        stroker.finish();

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
