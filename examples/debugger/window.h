#pragma once

#include <QtCore>
#include <QtGui>

struct RenderOptions
{
    int width;
    int height;

    float dx    = 0.0f;
    float dy    = 0.0f;
    float scale = 1.0f;

    bool fill   = true;
    bool stroke = true;
};

class Window : public QRasterWindow
{
public:
    virtual void render(QPainter *p, RenderOptions options);

    void paintEvent(QPaintEvent *e) override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;

    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

    void wheelEvent(QWheelEvent *e) override;

private:
    float m_dx      = 0.0f;
    float m_dy      = 0.0f;
    float m_scale   = 1.0f;

    bool m_fill     = true;
    bool m_stroke   = true;

    bool m_mouseDown = false;
    QPoint m_lastMousePos;
};
