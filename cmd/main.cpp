
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "scene.h"

static void usage(const char *msg = nullptr) {
    if (msg)
        fprintf(stderr, "raycmd: %s\n\n", msg);

    fprintf(stderr, R"(usage: raycmd --in <filename.yml> --out <output.ppm> [<options>]
Rendering options:
  --help               Print this help text.
  --in <filename>      The input scene description in yaml format.
  --nthreads <num>     Use specified number of threads for rendering.
  --out <filename>     Write the final image to the given filename (in ppm format).
)");
    exit(msg ? 1 : 0);
}

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
        else if (!strcmp(argv[i], "--nthreads") || !strcmp(argv[i], "-nthreads")) {
            uint nThreads = std::atoi(argv[++i]);
            if (nThreads == 0) {
                usage("invalid argument for --nthreads");
            }
            scene.SetNumThreads(nThreads);
        }
        else if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-help") || !strcmp(argv[i], "-h")) {
            usage();
            return 0;
        }
    }

    scene.Run();

    return 0;
}
