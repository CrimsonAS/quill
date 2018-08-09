#include "window.h"

void Window::render(QPainter *p, RenderOptions options)
{
    p->fillRect(0, 0, options.width, options.height, Qt::white);
}

void Window::paintEvent(QPaintEvent *)
{

    RenderOptions options;
    options.width = width();
    options.height = height();
    options.dx = m_dx;
    options.dy = m_dy;
    options.scale = m_scale;
    options.fill = m_fill;
    options.stroke = m_stroke;

    QPainter p(this);

    p.fillRect(0, 0, width(), height(), Qt::black);
    render(&p, options);
}

void Window::mousePressEvent(QMouseEvent *e)
{
    m_mouseDown = true;
    m_lastMousePos = e->pos();
}

void Window::mouseReleaseEvent(QMouseEvent *)
{
    m_mouseDown = false;
}

void Window::mouseMoveEvent(QMouseEvent *e)
{
    if (m_mouseDown) {
        QPoint delta = e->pos() - m_lastMousePos;
        m_dx += delta.x();
        m_dy += delta.y();
        m_lastMousePos = e->pos();
        update();
    }
}

void Window::wheelEvent(QWheelEvent *e)
{
    float x = (e->x() - m_dx) / m_scale;
    float y = (e->y() - m_dy) / m_scale;


    if (e->angleDelta().y() > 0) {
        m_scale = m_scale * 1.1;
    } else {
        m_scale = m_scale / 1.1;
    }

    m_dx = e->x() - x * m_scale;
    m_dy = e->y() - y * m_scale;

    update();
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_S) {
        m_stroke = !m_stroke;
        update();
    }
    if (e->key() == Qt::Key_F) {
        m_fill = !m_fill;
        update();
    }
}

void Window::keyReleaseEvent(QKeyEvent *)
{

}
