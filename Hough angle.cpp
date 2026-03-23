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

    // points alignés
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

    // theta
    double thetaMin = 0;
    double thetaMax = 179;
    double pastheta = 1;

    // rho
    double rhoMin = -20;
    double rhoMax = 20;
    double pasrho = 0.01;

    int nbtheta = (int)((thetaMax - thetaMin) / pastheta) + 1;
    int nbrho = (int)((rhoMax - rhoMin) / pasrho) + 1;

    vector<vector<int>> accu(nbtheta, vector<int>(nbrho, 0));

    // Hough
    for (int i = 0; i < points.size(); i++) {
        int x = points[i].x;
        int y = points[i].y;

        for (int ang = 0; ang < nbtheta; ang++) {

            double theta = (thetaMin + ang * pastheta) * M_PI / 180.0;

            double rho = x * cos(theta) + y * sin(theta);

            int irho = (int)round((rho - rhoMin) / pasrho);

            if (irho >= 0 && irho < nbrho) {
                accu[ang][irho]++;
            }
        }
    }

    // max
    int maxVotes = 0;
    int meilleurtheta = 0;
    int meilleurrho = 0;

    for (int ang = 0; ang < nbtheta; ang++) {
        for (int irho = 0; irho < nbrho; irho++) {
            if (accu[ang][irho] > maxVotes) {
                maxVotes = accu[ang][irho];
                meilleurtheta = ang;
                meilleurrho = irho;
            }
        }
    }

    double thetaTrouve = thetaMin + meilleurtheta * pastheta;
    double rhoTrouve = rhoMin + meilleurrho * pasrho;

    cout << "rho = " << rhoTrouve << endl;
    cout << "theta = " << thetaTrouve - 90 << " deg" << endl;
    cout << "Votes = " << maxVotes << endl;

    ofstream image("accu_rho_theta.ppm");

    image << "P3\n";
    image << nbtheta << " " << nbrho << "\n";
    image << 255 << "\n";

    for (int irho = 0; irho < nbrho; irho++) {
        for (int itheta = 0; itheta < nbtheta; itheta++) {

            int gris = 0;

            if (maxVotes > 0) {
                // version améliorée pour voir les détails
                gris = (accu[itheta][irho] * 255) / maxVotes;
            }
        image << gris << " " << gris << " " << gris << " ";
        }
        image << "\n";
    }

    image.close();

    return 0;
}
