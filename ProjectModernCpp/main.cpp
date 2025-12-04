#include "ConsoleUI.h"

int main()
{
    // 1. Instanțiem interfața consolei
    ConsoleUI ui;

    // 2. Pornim jocul (aceasta intră în bucla infinită de meniuri și joc)
    ui.run();

    return 0;
}