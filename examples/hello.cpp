#include "quill.h"

using namespace Quill;

struct Consumer
{

};

int main(int argc, char **argv)
{
    Stroker<Consumer> stroker;

    stroker.moveTo(100, 100);
    stroker.lineTo(200, 100);
    stroker.lineTo(300, 200);
    stroker.finish();

    return 0;
}