#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct Pixel {
    int r;
    int g;
    int b;
};

void setPixel(vector<vector<Pixel>>& image, int x, int y, int r, int g, int b) {
    int hauteur = image.size();
    int largeur = image[0].size();

    if (x >= 0 && x < largeur && y >= 0 && y < hauteur) {
        image[y][x].r = r;
        image[y][x].g = g;
        image[y][x].b = b;
    }
}

void drawLine(vector<vector<Pixel>>& image, int x0, int y0, int x1, int y1, int r, int g, int b) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx;
    if (x0 < x1) sx = 1;
    else sx = -1;

    int sy;
    if (y0 < y1) sy = 1;
    else sy = -1;

    int err = dx - dy;

    while (true) {
        setPixel(image, x0, y0, r, g, b);

        if (x0 == x1 && y0 == y1) {
            break;
        }

        int e2 = 2 * err;

        if (e2 > -dy) {
            err = err - dy;
            x0 = x0 + sx;
        }

        if (e2 < dx) {
            err = err + dx;
            y0 = y0 + sy;
        }
    }
}

void savePPM(const vector<vector<Pixel>>& image, const string& nom) {
    ofstream file(nom);

    int hauteur = image.size();
    int largeur = image[0].size();

    file << "P3\n";
    file << largeur << " " << hauteur << "\n";
    file << 255 << "\n";

    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            file << image[y][x].r << " "
                 << image[y][x].g << " "
                 << image[y][x].b << " ";
        }
        file << "\n";
    }

    file.close();
}

int main() {
    int largeur = 100;
    int hauteur = 100;

    vector<vector<Pixel>> image(hauteur, vector<Pixel>(largeur));

    // fond noir
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            image[y][x].r = 0;
            image[y][x].g = 0;
            image[y][x].b = 0;
        }
    }

    // liste des points
    vector<pair<int,int>> points;

    // points alignés
    points.push_back({0, 1});
    points.push_back({1, 2});
    points.push_back({2, 3});
    points.push_back({3, 4});
    points.push_back({4, 5});

    // points parasites
    points.push_back({1, 7});
    points.push_back({5, 2});

    // affichage des points en bleu
    for (int i = 0; i < points.size(); i++) {
        int x = points[i].first * 10;
        int y = points[i].second * 10;

        // petit carré 3x3 pour mieux voir les points
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                setPixel(image, x + dx, y + dy, 0, 0, 255);
            }
        }
    }

    // droite rouge
    drawLine(image, 10, 20, 80, 70, 255, 0, 0);

    savePPM(image, "ligne.ppm");

    cout << "Image creee : ligne.ppm" << endl;

    return 0;
}