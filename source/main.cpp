#include "gui/gui.h"
#ifdef _MSC_VER
#   include <Windows.h>
int APIENTRY WinMain(HINSTANCE, HINSTANCE, PSTR, int)
#else
int main()
#endif
{
    valencia::gui::Menu app;

    app.Run();

    return 0;
}
