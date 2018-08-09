#include <QtCore>
#include <QtGui>

#include "window.h"

#include "quill.h"

using namespace Quill;

#include "trianglestrokeraster.h"
#include "solidfillstroker.h"

template <typename S>
void doStroke(S*stroker)
{
    stroker->width = 15.116408;
    stroker->moveTo(365.798920, 1176.752075);
    stroker->width = 15.116408;
    stroker->lineTo(365.798920, 1176.752075);
    stroker->width = 15.116408;
    stroker->lineTo(365.798920, 1176.752075);
    stroker->width = 14.875897;
    stroker->lineTo(365.798920, 1176.752075);
    stroker->width = 14.880536;
    stroker->lineTo(365.798920, 1176.752075);
    stroker->width = 14.894423;
    stroker->lineTo(365.798920, 1176.752075);
    stroker->width = 14.921822;
    stroker->lineTo(362.690430, 1175.219238);
    stroker->width = 14.964133;
    stroker->lineTo(361.978546, 1174.087280);
    stroker->width = 15.020984;
    stroker->lineTo(361.219360, 1172.015503);
    stroker->width = 15.093009;
    stroker->lineTo(360.290771, 1169.609985);
    stroker->width = 15.181452;
    stroker->lineTo(359.420898, 1166.949707);
    stroker->width = 15.286211;
    stroker->lineTo(358.763275, 1163.604004);
    stroker->width = 15.404083;
    stroker->lineTo(358.085449, 1159.588745);
    stroker->width = 15.531270;
    stroker->lineTo(357.284729, 1155.291626);
    stroker->width = 15.665442;
    stroker->lineTo(356.755249, 1150.672607);
    stroker->width = 15.804381;
    stroker->lineTo(356.530457, 1145.314941);
    stroker->width = 15.947255;
    stroker->lineTo(356.348022, 1139.316406);
    stroker->width = 16.093901;
    stroker->lineTo(356.479614, 1132.549072);
    stroker->width = 16.243864;
    stroker->lineTo(356.756317, 1124.902588);
    stroker->width = 16.399746;
    stroker->lineTo(357.298004, 1116.891235);
    stroker->width = 16.564974;
    stroker->lineTo(358.413055, 1108.087524);
    stroker->width = 16.738821;
    stroker->lineTo(359.724487, 1098.086182);
    stroker->width = 16.917633;
    stroker->lineTo(361.841766, 1086.741699);
    stroker->width = 17.094816;
    stroker->lineTo(364.878204, 1073.999878);
    stroker->width = 17.264227;
    stroker->lineTo(368.997284, 1059.999268);
    stroker->width = 17.715519;
    stroker->lineTo(374.691406, 1044.767456);
    stroker->width = 18.039478;
    stroker->lineTo(382.086884, 1028.587280);
    stroker->width = 18.297293;
    stroker->lineTo(390.697876, 1011.487244);
    stroker->width = 18.524187;
    stroker->lineTo(400.716339, 993.392700);
    stroker->width = 18.734333;
    stroker->lineTo(413.143738, 974.763000);
    stroker->width = 18.929934;
    stroker->lineTo(427.326141, 955.122009);
    stroker->width = 19.107025;
    stroker->lineTo(442.882782, 934.123169);
    stroker->width = 19.258553;
    stroker->lineTo(461.276520, 912.769958);
    stroker->width = 19.378649;
    stroker->lineTo(482.799927, 891.125244);
    stroker->width = 19.465794;
    stroker->lineTo(505.982117, 869.311890);
    stroker->width = 19.516184;
    stroker->lineTo(531.610413, 848.804565);
    stroker->width = 19.250252;
    stroker->lineTo(558.736267, 829.296692);
    stroker->width = 19.075712;
    stroker->lineTo(586.686096, 810.931946);
    stroker->width = 18.918915;
    stroker->lineTo(615.534058, 794.646729);
    stroker->width = 18.755816;
    stroker->lineTo(643.981567, 779.875671);
    stroker->width = 18.589506;
    stroker->lineTo(671.517334, 766.074097);
    stroker->width = 18.428633;
    stroker->lineTo(698.025940, 753.674988);
    stroker->width = 17.866024;
    stroker->lineTo(723.033142, 743.500366);
    stroker->width = 17.516264;
    stroker->lineTo(745.863281, 734.950317);
    stroker->width = 17.301401;
    stroker->lineTo(766.635742, 727.516418);
    stroker->width = 17.487329;
    stroker->lineTo(785.734558, 721.428589);
    stroker->width = 17.601608;
    stroker->lineTo(803.140442, 716.451111);
    stroker->width = 17.292816;
    stroker->lineTo(818.521118, 712.019470);
    stroker->width = 17.163069;
    stroker->lineTo(832.146973, 708.170349);
    stroker->width = 17.123528;
    stroker->lineTo(843.937866, 705.274780);
    stroker->width = 16.315058;
    stroker->lineTo(853.231567, 703.175964);
    stroker->width = 15.673010;
    stroker->lineTo(860.310608, 702.234070);
    stroker->width = 14.781355;
    stroker->lineTo(860.310608, 702.234070);
}


class MyWindow : public Window
{
public:
    void render(QPainter *p, RenderOptions options) override;
};

void MyWindow::render(QPainter *p, RenderOptions options)
{
    if (options.fill) {

        RasterBuffer buffer;
        buffer.allocate(options.width, options.height);
        buffer.fill(0xff000000);

        SolidFillStroker stroker;
        stroker.initialize(buffer);
        stroker.setFillValue(0xff404050);
        stroker.setScale(options.scale, options.scale);
        stroker.setTranslate(options.dx, options.dy);

        doStroke(&stroker);

        p->drawImage(0, 0, QImage((uchar *) buffer.data,
                                  buffer.width,
                                  buffer.height,
                                  QImage::Format_ARGB32_Premultiplied));

    }

    if (options.stroke) {
        Stroker<TriangleStrokeRaster> stroker;

        stroker.raster.dx = options.dx;
        stroker.raster.dy = options.dy;
        stroker.raster.sx = options.scale;
        stroker.raster.sy = options.scale;

        stroker.raster.stroker.raster.raster.fill.value = 0xffffffff;
        stroker.raster.allocate(options.width, options.height, 0x00000000);

        doStroke(&stroker);
        p->drawImage(0, 0, QImage((uchar *) stroker.raster.buffer(),
                                  stroker.raster.width(),
                                  stroker.raster.height(),
                                  QImage::Format_ARGB32_Premultiplied));
    }

}


int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    MyWindow window;
    window.resize(800, 800);
    window.show();

    return app.exec();
}
