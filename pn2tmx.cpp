/**
 * Map Generator for Tiled Map Editor
 * License under GPLv3: LICENSE.txt
 * (C)2024, SUZUKI PLAN
 */
#include <string>
#include <stdint.h>
#include "perlinnoise.hpp"

extern "C" {
    extern const unsigned short vgs0_rand16[65536];
};

int main(int argc, char* argv[])
{
    uint16_t seed = 0;
    uint16_t xLimit = 64;
    uint16_t yLimit = 64;
    uint8_t oct = 0;
    int32_t width = 128;
    int32_t height = 128;
    int32_t div = 1;
    std::string img = "image.bmp";
    std::string tsx = "image.tsx";

    bool error = false;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            error = true;
            break;
        }
        auto opt = tolower(argv[i++][1]);
        if (argc <= i) {
            error = true;
            break;
        }
        switch (opt) {
            case 's': seed = (uint16_t)atoi(argv[i]); break;
            case 'x': xLimit = (uint16_t)atoi(argv[i]); break;
            case 'y': yLimit = (uint16_t)atoi(argv[i]); break;
            case 'o': oct = (uint8_t)atoi(argv[i]); break;
            case 'w': width = atoi(argv[i]); break;
            case 'h': height = atoi(argv[i]); break;
            case 'd': div = atoi(argv[i]); break;
            case 'i': img = argv[i]; break;
            case 't': tsx = argv[i]; break;
            default: error = true;
        }
        if (error) {
            break;
        }
    }
    if (error || div < 0 || 255 < div)  {
        puts("pn2tmx [-x x_limit]");
        puts("       [-y y_limit]");
        puts("       [-o octave_value]");
        puts("       [-w map_width]");
        puts("       [-h map_height]");
        puts("       [-d pattern_divison_value]");
        puts("       [-i image_file_name]");
        puts("       [-t tsx_file_name]");
        return 1;
    }

    PerlinNoise pn(vgs0_rand16, seed);
    pn.limitX(xLimit);
    pn.limitY(yLimit);

    printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<map version=\"1.4\" tiledversion=\"1.4.3\" orientation=\"orthogonal\" renderorder=\"right-down\" width=\"%d\" height=\"%d\" tilewidth=\"8\" tileheight=\"8\" infinite=\"0\" nextlayerid=\"2\" nextobjectid=\"1\">\n", width, height);
    printf("%s\n",("<tileset firstgid=\"1\" source=\"" + tsx + "\"/>").c_str());
    printf("<layer id=\"1\" name=\"Tile Layer 1\" width=\"%d\" height=\"%d\">\n",width,height);
    printf("<data encoding=\"csv\">\n");

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t n = oct ? pn.octave(oct, (uint16_t)x, (uint16_t)y) : pn.noise((uint16_t)x,(uint16_t)y);
            if (x) {
                printf(", %d", n / div);
            } else {
                printf("%d", n / div);
            }
        }
        if (y + 1 < height) {
            printf(",\n");
        } else {
            printf("\n");
        }
    }
    puts("</data>");
    puts("</layer>");
    puts("</map>");
    return 0;
}
