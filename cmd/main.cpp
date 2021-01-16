
#include <iostream>
#include <string>
#include <string.h>
#include "scene.h"

int main(int argc, char *argv[])
{
    Scene scene;

    // Process command-line arguments
    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "--in") || !strcmp(argv[i], "-in") || !strcmp(argv[i], "-i"))
        {
            scene.SetScenePath(argv[++i]);
        }
        else if (!strcmp(argv[i], "--out") || !strcmp(argv[i], "-out") || !strcmp(argv[i], "-o"))
        {
            scene.SetOutputPath(argv[++i]);
        }
    }

    scene.Run();

    return 0;
}
