/*
    Copyright (c) 2019, reMarkable AS <technology@remarkable.no>
    Copyright (c) 2019, Gunnar Sletta <gunnar@crimson.no>
    All rights reserved.


    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
    THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QtGui>

#include "quill.h"

#include "rasterbuffer.h"
#include "solidcolorfill.h"

using namespace Quill;


struct SimpleFill
{
    typedef Varying3D Varyings;

    RasterBuffer buffer;
    void operator()(int x, int y, int length, Varying3D v, Varying3D dv) {
        unsigned int *dst = buffer.scanline(y) + x;
        for (int i=0; i<length; ++i) {
            dst[i] = 0xff000000
                     | (int(v.x * 255) << 16)
                     | (int(v.y * 255) << 8)
                     | (int(v.z * 255));
            v = v + dv;
        }
    }
};

class Window : public QRasterWindow
{
public:
    Window()
        : m_mouseDown(false)
    {
        m_clipRect = QRect(200, 200, 100, 100);
        resize(500, 500);
    }

    void paintEvent(QPaintEvent *)
    {
        ClipRaster<LerpRaster<SimpleFill>> clipper;

        QRect clip = m_clipRect.translated(m_mouseDelta);
        clip = clip & QRect(0, 0, width(), height());

        clipper.x0 = clip.x();
        clipper.y0 = clip.y();
        clipper.x1 = clip.x() + clip.width();
        clipper.y1 = clip.y() + clip.height();

        RasterBuffer *buffer = &clipper.raster.fill.buffer;
        buffer->allocate(500, 500);
        buffer->fill(0xff000000);

        Triangle t1(200, 100, 400, 200, 100, 400);

        Varying3D gray(0.1f, 0.1f, 0.1f);
        Varying3D blue(0, 0, 1);
        Varying3D red(1, 0, 0);
        Varying3D green(0, 1, 0);

        // draw the whole triangle
        clipper.raster(t1, gray, gray, gray);

        // the draw the clipped rectangle.
        clipper(t1, blue, red, green);

        QImage image((uchar *) buffer->data, buffer->width, buffer->height, QImage::Format_RGB32);

        QPainter p(this);
        p.drawImage(0, 0, image);

        p.setOpacity(0.2);
        p.setPen(Qt::white);
        p.drawRect(clip.adjusted(0, 0, -1, -1));
    }

    void mousePressEvent(QMouseEvent *e) {
        m_mouseDown = true;
        m_mouseDownPos = e->pos();
        update();
    }

    void mouseMoveEvent(QMouseEvent *e) {
        if (m_mouseDown) {
            m_mouseDelta = e->pos() - m_mouseDownPos;
            update();
        }
    }

    void mouseReleaseEvent(QMouseEvent *) {
        if (m_mouseDown) {
            m_clipRect = m_clipRect.translated(m_mouseDelta);
            m_mouseDelta = QPoint();
            m_mouseDownPos = QPoint();
            update();
            m_mouseDown = false;
        }
    }

    void wheelEvent(QWheelEvent *e) {
        if (e->angleDelta().y() > 0)
            m_clipRect = m_clipRect.adjusted(1, 1, -1, -1);
        else
            m_clipRect = m_clipRect.adjusted(-1, -1, 1, 1);
        update();
    }

private:
    QRect m_clipRect;
    QPoint m_mouseDownPos;
    QPoint m_mouseDelta;
    bool m_mouseDown;
};

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    Window window;
    window.show();

    return app.exec();
}
