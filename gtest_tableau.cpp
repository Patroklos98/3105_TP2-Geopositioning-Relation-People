#include "tableau.h"
#include <gtest/gtest.h>

TEST(TestsTableau, Vide) {
    Tableau<int> t;
    t.ajouter(1);
    EXPECT_FALSE(t.vide());
}


TEST(TestsTableau, Ajouts) {
    Tableau<int> t;
    int n = 256 * 256;
    for (int i = 0; i < n; i++) {
        t.ajouter(i * 2);
    }

    EXPECT_EQ(t.taille(), n);

    for (int i = 0; i < t.taille(); i++) {
        EXPECT_EQ(t[i], i * 2);
    }
}

TEST(TestsTableau, PointeursEtNull) {
    Tableau<int *> t;
    int n = 256 * 256;
    int *ints = new int[n];
    int *iterateur = ints;
    for (int i = 0; i < n; i++) {
        *iterateur = i * 2;
        t.ajouter(iterateur);
        iterateur++;
    }

    t.ajouter(nullptr);
    t.creer_index();

    EXPECT_EQ(t.taille(), n + 1);

    for (int i = 0; i < t.taille(); i++) {
        if (t[i] != nullptr) {
            EXPECT_EQ(*(t[i]), i * 2);
        }
    }

    delete[] ints;
}

TEST(TestsTableau, tableauDeMemeValeur) {
    Tableau<int> t;
    int nombre_de_valeur = 10;
    for (int i = 0; i < nombre_de_valeur; i++) {
        t.ajouter(0);
    }
    t.creer_index();
    int index = t.chercher(0);
    EXPECT_EQ(0, index);
}

TEST(TestsTableau, valeurNegative) {
    Tableau<int> t;
    int nombre_de_valeur = 100;
    for (int i = 0; i < nombre_de_valeur; i++) {
        t.ajouter(-i);
    }
    t.creer_index();
    int index = t.chercher(-1);
    EXPECT_EQ(1, index);
}

TEST(TestsTableau, Assignation) {
    Tableau<int> t1;
    for (int i = 0; i < 10; i++) {
        t1.ajouter(i);
    }

    Tableau<int> t2;
    t2 = t1;

    // On test que t2 n'est pas affecté par les opérations sur t1.
    for (int i = 0; i < 10; i++) {
        t1[i] = 0;
    }

    t1.ajouter(0);

    EXPECT_EQ(t2.taille(), 10);

    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(t2[i], i);
    }
}

TEST(TestsTableau, ViderEtAssigner) {
    Tableau<int> t1;
    for (int i = 0; i < 10; i++) {
        t1.ajouter(i);
    }

    Tableau<int> t2;
    t2 = t1;
    t1.vider();
    EXPECT_EQ(t1.taille(), 0);
    EXPECT_EQ(t2.taille(), 10);

    for (int i = 0; i < 100; i++) {
        t1.ajouter(i + 1);
    }

    t2 = t1;
    EXPECT_EQ(t2.taille(), 100);

    for (int i = 0; i < t2.taille(); i++) {
        EXPECT_EQ(t2[i], i + 1);
    }


    Tableau<int> t3;
    for (int i = 0; i < 100; i++) {
        t3.ajouter(i);
    }

    Tableau<int> t4;
    t4 = t3;
    t3.vider();
    for (int i = 0; i < 10; i++)
        t3.ajouter(i + 1);
    t4 = t3;

    EXPECT_EQ(t4.taille(), 10);

    for (int i = 0; i < t4.taille(); i++)
        EXPECT_EQ(t4[i], i + 1);
}

TEST(TestsTableau, ViderTableauVide) {
    Tableau<int> t;
    EXPECT_NO_THROW(t.vider());
}

TEST(TestsTableau, AutoAssignation) {
    Tableau<int> t;
    for (int i = 0; i < 100; i++) {
        t.ajouter(i);
    }
    t = t; // devrait être sans effet.
    EXPECT_EQ(t.taille(), 100);

    for (int i = 0; i < 100; i++)
        EXPECT_EQ(t[i], i);
}

TEST(TestsTableau, ConstructeurParCopie) {
    Tableau<int> t1;
    for (int i = 0; i < 10; i++) {
        t1.ajouter(i);
    }
    Tableau<int> t2(t1);

    EXPECT_EQ(t2.taille(), 10);

    for (int i = 0; i < t2.taille(); i++) {
        EXPECT_EQ(t2[i], i);
    }
}

TEST(TestsTableau, OperateurEgalite) {
    Tableau<int> t1;
    Tableau<int> t2;
    for (int i = 0; i < 1024; i++) {
        t1.ajouter(i);
        t2.ajouter(i);
    }

    t1.ajouter(3);

    EXPECT_NE(t1, t2) << "#1";
    EXPECT_NE(t2, t1) << "#2";
    t2.ajouter(3);
    EXPECT_EQ(t1, t2) << "#3";
    EXPECT_EQ(t2, t1) << "#4";

    EXPECT_EQ(t1, t1) << "#5";

    Tableau<int> t3;
    EXPECT_NE(t1, t3) << "#6";
    t3 = t2;
    EXPECT_EQ(t1, t3) << "#7";
}

TEST(TestsTableau, Insertion) {
    Tableau<int> t1;
    Tableau<int> t2;
    for (int i = 0; i < 16; i++) {
        t1.ajouter(i + 1);
        t2.ajouter(i);
    }

    t1.inserer(0);
    t2.ajouter(16);

    EXPECT_EQ(t1, t2) << "#1";

    Tableau<int> t3;
    for (int i = 4; i < 17; i++) {
        t3.ajouter(i);
    }
    t3.inserer(1, 0);
    t3.inserer(0, 0);
    t3.inserer(3, 2);
    t3.inserer(2, 2);

    EXPECT_EQ(t1, t3) << "#2";
}

TEST(TestsTableau, InsererIndiceNegatif) {
    Tableau<int> t1;
    EXPECT_THROW(t1.inserer(0, -1), std::out_of_range);
}

TEST(TestsTableau, InsererPositionDefaut) {
    Tableau<int> t1;
    t1.ajouter(1);
    t1.inserer(0);
    EXPECT_EQ(t1[0], 0);
    EXPECT_EQ(t1[1], 1);
}

TEST(TestsTableau, InsererDebut) {
    Tableau<int> t1;
    t1.inserer(1, 0);
    t1.inserer(0, 0);
    EXPECT_EQ(t1[0], 0);
    EXPECT_EQ(t1[1], 1);
}

TEST(TestsTableau, InsererFin) {
    Tableau<int> t1;
    t1.ajouter(0);
    t1.inserer(1, 1);
    EXPECT_EQ(t1[0], 0);
    EXPECT_EQ(t1[1], 1);
}

TEST(TestsTableau, InsererIndiceTropGrand) {
    Tableau<int> t1;
    t1.ajouter(0);
    EXPECT_THROW(t1.inserer(1, 2), std::out_of_range);
}


TEST(TestsTableau, Enlevement) {
    Tableau<int> t1;
    for (int i = 0; i < 16; i++) {
        t1.ajouter(i);
    }

    t1.enlever(5);
    Tableau<int> t2;
    for (int i = 0; i < 5; i++) {
        t2.ajouter(i);
    }
    for (int i = 6; i < 16; i++) {
        t2.ajouter(i);
    }

    EXPECT_EQ(t1, t2);
}

TEST(TestsTableau, EnleverDernier) {
    Tableau<int> t1;
    for (int i = 0; i < 16; i++) {
        t1.ajouter(i);
    }

    for (int i = 0; i < 16; i++) {
        t1.enlever_dernier();
    }
    Tableau<int> t2;
    EXPECT_EQ(t1, t2);
}

TEST(TestsTableau, EnleverIndiceNegatif) {
    Tableau<int> t1;
    t1.ajouter(1);
    EXPECT_THROW(t1.enlever(-1), std::out_of_range);
}

TEST(TestsTableau, EnleverIndiceTropGrand) {
    Tableau<int> t1;
    t1.ajouter(1);
    EXPECT_THROW(t1.enlever(1), std::out_of_range);
}

TEST(TestsTableau, EnleverDernierTableauVide) {
    Tableau<int> t;
    EXPECT_THROW(t.enlever_dernier(), std::out_of_range);
}

TEST(TestsTableau, ChercherInt) {
    int n = 100;
    Tableau<int> t1;
    for (int i = 0; i < n; i++) {
        t1.ajouter(i * 2);
    }

    for (int i = 0; i < n; i++) {
        int index = t1.chercher(i * 2);
        EXPECT_EQ(index, i);
        index = t1.chercher(i * 2 + 1);
        EXPECT_EQ(index, -1);
    }
}

TEST(TestsTableau, ChercherIntIndexe) {
    int n = 100;
    Tableau<int> t1;
    for (int i = 0; i < n; i++) {
        t1.ajouter(i * 2);
    }
    t1.creer_index();
    for (int i = 0; i < n; i++) {
        int index = t1.chercher(i * 2);
        EXPECT_EQ(index, i);
        index = t1.chercher(i * 2 + 1);
        EXPECT_EQ(index, -1);
    }
}

class Point {
public:
    Point(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}

    bool operator==(const Point &autre) const {
        return x == autre.x && y == autre.y;
    }

    bool operator<(const Point &o) const {
        return x + y < o.x + o.y;
    }

private:
    double x, y;
};

TEST(TestsTableau, ChercherPoint) {
    Tableau<Point> points;
    points.ajouter(Point());
    points.ajouter(Point(2, 3));
    points.ajouter(Point(4, 5));
    points.ajouter(Point(3, 4));
    EXPECT_EQ(points.chercher(Point(4, 5)), 2);
    EXPECT_EQ(points.chercher(Point(-1, -1)), -1);
    Tableau<Point> points2;
    points2 = points;
    EXPECT_EQ(points2.chercher(Point(4, 5)), 2);
}

TEST(TestsTableau, CopierPoints) {
    Tableau<Point> points;
    points.ajouter(Point());
    points.ajouter(Point(2, 3));
    points.ajouter(Point(4, 5));
    points.ajouter(Point(3, 4));
    Tableau<Point> points2;
    points2 = points;
    EXPECT_TRUE(points2 == points);
}

TEST(TestsTableau, ChercherPointIndexe) {
    Tableau<Point> points;
    points.ajouter(Point());
    points.ajouter(Point(2, 3));
    points.ajouter(Point(4, 5));
    points.ajouter(Point(3, 4));
    points.creer_index();
    EXPECT_EQ(points.chercher(Point(4, 5)), 2);
    EXPECT_EQ(points.chercher(Point(-1, -1)), -1);
    Tableau<Point> points2;
    points2 = points;
    EXPECT_EQ(points2.chercher(Point(4, 5)), 2);
}

TEST(TestsTableau, ChercherTableauVide) {
    Tableau<int> t;
    t.creer_index();
    EXPECT_EQ(t.chercher(1), -1);
}

TEST(TestsTableau, ContientTableauVide) {
    Tableau<int> t;
    t.creer_index();
    EXPECT_EQ(t.compter(1), 0);
}

TEST(TestsTableau, CompterTableauVide) {
    Tableau<int> t;
    t.creer_index();
    EXPECT_EQ(t.contient(1), false);
}


TEST(TestsTableau, Contient) {
    Tableau<int> t1;
    for (int i = 0; i < 100; i++) {
        t1.ajouter(i * 2);
    }

    for (int i = 0; i < 100; i++) {
        bool estPresent = t1.contient(i * 2);
        EXPECT_EQ(estPresent, true);
        estPresent = t1.contient(i * 2 + 1);
        EXPECT_EQ(estPresent, false);
    }
}


TEST(TestsTableau, ContientIndexe) {
    Tableau<int> t1;
    for (int i = 0; i < 100; i++) {
        t1.ajouter(i * 2);
    }
    t1.creer_index();
    for (int i = 0; i < 100; i++) {
        bool estPresent = t1.contient(i * 2);
        EXPECT_EQ(estPresent, true);
        estPresent = t1.contient(i * 2 + 1);
        EXPECT_EQ(estPresent, false);
    }
}

TEST(TestTableau, Compter) {
    Tableau<int> t;
    int N = 20, M = 1000;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            t.ajouter(j);
        }
    }
    for (int j = 0; j < N; j++) {
        EXPECT_EQ(t.compter(j), M);
    }
}

TEST(TestTableau, CompterIndexe) {
    Tableau<int> t;
    int N = 20, M = 1000;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            t.ajouter(j);
        }
    }
    t.creer_index();
    for (int j = 0; j < N; j++) {
        EXPECT_EQ(t.compter(j), M);
    }
}

TEST(TestsTableau, SubscriptNegatif) {
    Tableau<int> t1;
    t1.ajouter(1);
    EXPECT_THROW(t1[-1], std::out_of_range);
}

TEST(TestsTableau, SubscriptTropGrand) {
    Tableau<int> t1;
    t1.ajouter(1);
    EXPECT_THROW(t1[1], std::out_of_range);
}


