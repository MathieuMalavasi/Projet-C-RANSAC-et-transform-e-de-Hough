#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Point {
    int x;
    int y;
};

struct Droite {
    double rho;
    double theta; // en degres
};

// distance point-droite en representation rho/theta
double distancePointDroite(Point p, Droite d) {
    double thetaRad = d.theta * M_PI / 180.0;
    return fabs(p.x * cos(thetaRad) + p.y * sin(thetaRad) - d.rho);
}

// / calcule le rho et theta d'une droite a partir de 2 points
Droite droiteDepuisDeuxPoints(Point p1, Point p2) {
    Droite d;

    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;

    // vecteur normal
    double a = dy;
    double b = -dx;

    double norme = sqrt(a * a + b * b);

    a = a / norme;
    b = b / norme;

    d.rho = p1.x * a + p1.y * b;
    d.theta = atan2(b, a) * 180.0 / M_PI;

    if (d.rho < 0) {
        d.rho = -d.rho;
        d.theta = d.theta + 180.0;
    }

    if (d.theta < 0) {
        d.theta = d.theta + 180.0;
    }

    return d;
}

int main() {
    srand(time(0));

    vector<Point> points;

    // points alignes
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

    // droite trouvee par Hough
    Droite droiteHough;
    droiteHough.rho = 0.7;
    droiteHough.theta = 135;

    // -------------------------
    // ETAPE 1 : construire S
    // -------------------------
    double e = 1.0; // seuil pour garder les points proches de Hough
    vector<Point> S;

    for (int i = 0; i < points.size(); i++) {
        if (distancePointDroite(points[i], droiteHough) < e) {
            S.push_back(points[i]);
        }
    }

    cout << "Nombre de points dans S : " << S.size() << endl;
    for (int i = 0; i < S.size(); i++) {
        cout << "(" << S[i].x << "," << S[i].y << ")" << endl;
    }

    if (S.size() < 2) {
        cout << "Pas assez de points pour RANSAC" << endl;
        return 0;
    }

    // -------------------------
    // ETAPE 2 : RANSAC
    // -------------------------
    int K = 100;      // nombre d'iterations
    double t = 0.5;   // seuil pour les inliers

    Droite meilleureDroite;
    int meilleurScore = -1;

    for (int i = 0; i < K; i++) {
        int i1 = rand() % S.size();
        int i2 = rand() % S.size();

        while (i2 == i1) {
            i2 = rand() % S.size();
        }

        Point p1 = S[i1];
        Point p2 = S[i2];

        Droite d = droiteDepuisDeuxPoints(p1, p2);

        int score = 0;

        for (int j = 0; j < S.size(); j++) {
            if (distancePointDroite(S[j], d) < t) {
                score++;
            }
        }

        if (score > meilleurScore) {
            meilleurScore = score;
            meilleureDroite = d;
        }
    }

    cout << "Droite finale RANSAC :" << endl;
    cout << "rho = " << meilleureDroite.rho << endl;
    cout << "theta = " << meilleureDroite.theta << " deg" << endl;
    cout << "score = " << meilleurScore << endl;

    return 0;
}