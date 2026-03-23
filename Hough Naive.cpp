#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

struct Point {
    int x;
    int y;
};

int main() {
    vector<Point> points;

    // points alignés sur y = x + 1
    points.push_back({0, 1});
    points.push_back({1, 2});
    points.push_back({2, 3});
    points.push_back({3, 4});
    points.push_back({4, 5});

    // parasites
    points.push_back({1, 7});
    points.push_back({2, 8});
    points.push_back({3, 9});
    points.push_back({5, 2});

    // intervalle de m
    double mMin = -10;
    double mMax = 10;
    double pasM = 0.1;

    // intervalle de b
    double bMin = -10;
    double bMax = 10;
    double pasB = 0.1;

    int nbM = (int)((mMax - mMin) / pasM) + 1;
    int nbB = (int)((bMax - bMin) / pasB) + 1;

    vector<vector<int>> accu(nbM, vector<int>(nbB, 0));

    // transformée de Hough naïve
    for (int i = 0; i < points.size(); i++) {
        int x = points[i].x;
        int y = points[i].y;

        for (int im = 0; im < nbM; im++) {
            double m = mMin + im * pasM;
            double b = y - m * x;

            int ib = (int)round((b - bMin) / pasB);

            if (ib >= 0 && ib < nbB) {
                accu[im][ib]++;
            }
        }
    }

    // chercher le maximum
    int maxVotes = 0;
    int meilleurM = 0;
    int meilleurB = 0;

    for (int im = 0; im < nbM; im++) {
        for (int ib = 0; ib < nbB; ib++) {
            if (accu[im][ib] > maxVotes) {
                maxVotes = accu[im][ib];
                meilleurM = im;
                meilleurB = ib;
            }
        }
    }

    double mTrouve = mMin + meilleurM * pasM;
    double bTrouve = bMin + meilleurB * pasB;

    cout << "y = " << mTrouve << "x + " << bTrouve << endl;
    cout << "Votes = " << maxVotes << endl;

    ofstream image("accumulateur.ppm");

    image << "P3\n";
    image << nbB << " " << nbM << "\n";
    image << 255 << "\n";

    for (int im = 0; im < nbM; im++) {
        for (int ib = 0; ib < nbB; ib++) {
            int gris = 0;

            if (maxVotes > 0) {
                gris = (accu[im][ib] * 255) / maxVotes;
            }

            image << gris << " " << gris << " " << gris << " ";
        }
        image << "\n";
    }

    image.close();
    return 0;
}
