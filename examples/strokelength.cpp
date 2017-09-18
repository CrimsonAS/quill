#include "quill.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "rasterbuffer.h"
#include "solidcolorfiller.h"

using namespace Quill;
using namespace std;

struct Dumper 
{
    void operator()(Span span) {
        std::cout << " - y=" << span.y
                  << ", x=" << span.x
                  << ", length=" << span.length
                  << ", coverage=" << span.coverage
                  << std::endl;
    }

};

struct VertexXYUV
{
    float x = 0.0f;
    float y = 0.0f;
    float u = 0.0f;
    float v = 0.0f;

    struct Varyings {
        Varyings(float u, float v) : u(u), v(v) { }
        float u;
        float v;
    };

    Varyings varyings() const { return Varyings(u, v); }
};

VertexXYUV::Varyings operator+(VertexXYUV::Varyings a, VertexXYUV::Varyings b)
{
    return VertexXYUV::Varyings(a.u + b.u, a.v + b.v);
}

VertexXYUV::Varyings operator-(VertexXYUV::Varyings a, VertexXYUV::Varyings b)
{
    return VertexXYUV::Varyings(a.u - b.u, a.v - b.v);
}

VertexXYUV::Varyings operator/(VertexXYUV::Varyings a, float div)
{
    return VertexXYUV::Varyings(a.u / div, a.v / div);
}

VertexXYUV::Varyings operator*(VertexXYUV::Varyings a, float mul)
{
    return VertexXYUV::Varyings(a.u * mul, a.v * mul);
}

void swap(VertexXYUV &a, VertexXYUV &b)
{
    VertexXYUV tmp = a;
    a = b;
    b = tmp;
}

std::ostream &operator<<(std::ostream &o, VertexXYUV v)
{
    o << "[x=" << v.x << ",y=" << v.y << ",u=" << v.u << ",v=" << v.v << "]";
    return o;
}


template <typename SpansConsumer, typename VertexType>
struct LerpRaster
{
    typedef VertexType Vertex;

    void operator()(Triangle<VertexType> t);

    SpansConsumer fill;

    // ********************
    // Internals
    //

    void iterate(float &y, float ymax, float left, float right, float leftIncr, float rightIncr);
};

template <typename SpansConsumer, typename Vertex>
void LerpRaster<SpansConsumer, Vertex>::operator()(Triangle<Vertex> t)
{
    t.sort();

    float yaFloored = std::floor(t.a.y);
    float y;

    if (t.a.y - yaFloored <= 0.5) {
        y = y0Floored + 0.5;
    } else {
        y = y0Floored + 1.5;
    }



    // std::cout << "rasterizing: " << t << std::endl;
    // std::cout << " - y=" << y << ", y0Floored=" << y0Floored << std::endl;

    typename Vertex::Varyings va = t.a.varyings();
    typename Vertex::Varyings vb = t.b.varyings();
    typename Vertex::Varyings vc = t.c.varyings();

    if (t.a.y != t.b.y) {
        float dy01 = (t.b.y - t.a.y);
        float dy02 = (t.c.y - t.a.y);
        float dxl = (t.b.x - t.a.x) / dy01;
        float dxr = (t.c.x - t.a.x) / dy02;
        float yoffset = y - t.a.y;
        float left = t.a.x + dxl * yoffset + 0.5;
        float right = t.a.x + dxr * yoffset + 0.5;

        typename Vertex::Varyings vac = (vc - va) * (dy01 / dy02);
        std::cout << "vac: " << vac.u << ", " << vac.v << endl;

        typename Vertex::Varyings viab = (vb - va) / (t.b.y - t.a.y);
        std::cout << "viab: " << viab.u << ", " << viab.v << endl;
        
        // std::cout << " - "
        //     << "dy01=" << dy01
        //     << ", dy02=" << dy02
        //     << ", dxl=" << dxl
        //     << ", dxr=" << dxr
        //     << ", yoffset=" << yoffset
        //     << ", left=" << left
        //     << ", right=" << right
        //     << std::endl;

        if (dxr < dxl) {
            std::swap(left, right);
            std::swap(dxl, dxr);
        }
        iterate(y, t.b.y, left, right, dxl, dxr);
    }

    // if (t.b.y != t.c.y) {
    //     float dy02 = (t.c.y - t.a.y);
    //     float dy12 = (t.c.y - t.b.y);
    //     float dxl = (t.c.x - t.b.x) / dy12;
    //     float dxr = (t.c.x - t.a.x) / dy02;
    //     float left = t.c.x - (t.c.y - y) * dxl + 0.5;
    //     float right = t.c.x - (t.c.y - y) * dxr + 0.5;
    //     if (left > right) {
    //         std::swap(left, right);
    //         std::swap(dxl, dxr);
    //     }
    //     iterate(y, t.c.y, left, right, dxl, dxr);
    //  }
}

// template <typename SpanConsumer, typename Vertex>
// void LerpRaster<SpanConsumer, Vertex>::iterate(float &y, float ymax, float left, float right, float leftIncr, float rightIncr)
// {

//     cout << y << " " << ymax << endl;

//     while (y < ymax) {
//         int l = (int) (left);
//         int r = (int) (right);
//         int len = r - l;
//         assert(len >= 0);
//         if (len > 0) {
//             fill(Span(int(y), l, len));
//         }
//         y += 1.0f;
//         left += leftIncr;
//         right += rightIncr;
//     }
// }

int main(int argc, char **argv)
{
    LerpRaster<Dumper, VertexXYUV> raster;

    Triangle<VertexXYUV> t = Triangle<VertexXYUV>::create(5, 0, 0, 10, 10, 20);
    t.a.u = 0.0f;
    t.a.v = 0.0f;
    t.b.u = 1.0f;
    t.b.v = 0.0f;
    t.c.u = 0.0f;
    t.c.v = 1.0f;

    raster(t);

    // Stroker<LerpRaster<SolidColorFiller, Triangle<Vertex2D>>> stroker;

    // SolidColorFiller *fill = &stroker.rasterizer.fill;
    // fill->value = 0xffe0b0a0;

    // RasterBuffer *buffer = &fill->buffer;
    // buffer->allocate(100, 100);
    // buffer->fill(0xff000000);

    // stroker.width = 16;
    // stroker.moveTo(10, 10);
    // stroker.lineTo(90, 50);
    // stroker.width = 8;
    // stroker.lineTo(10, 90);

    // stbi_write_png("strokelength.png",
    //                buffer->width,
    //                buffer->height,
    //                4,
    //                buffer->data,
    //                buffer->width * sizeof(unsigned int));
}
