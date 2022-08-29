#include "coordonnees.h"
#include <gtest/gtest.h>
#include <cstdlib>

static const float LONGITUDE_TEST = 0.00022;
static const float LONGITUDE_TEST_NEGA = 0.00022;
static const float LONGITUDE_TEST2 = 0.00027;
static const float LONGITUDE_TEST3 = 0.001;
static const float LONGITUDE_TEST4 = 0.01;
static const float LONGITUDE_TEST5 = 0.1;
static const float LONGITUDE_TEST6 = 1.1;
static const float LONGITUDE_TEST7 = 179;

bool estSetup = false;

void testerCoords(float lat1, float long1, float lat2, float long2) {
    if (!estSetup) {
        Coordonnees::calculerSeuils();
        estSetup = true;
    }
    Coordonnees c1(lat1, long1);
    Coordonnees c2(lat2, long2);
    std::cout << lat1 << " " << long2 << " " << c1.distance(c2) << " " << " "
              << c1.estPresDe(c2) << " " << (c1.distance(c2) < SEUIL_PROXIMITE) << std::endl;
    assert(c1.estPresDe(c2) == (c1.distance(c2) < SEUIL_PROXIMITE));
}

void testerLatitude(float latitude) {
    testerCoords(latitude, 0, latitude, LONGITUDE_TEST);
    testerCoords(latitude, 0, latitude, LONGITUDE_TEST_NEGA);
    testerCoords(latitude, 0, latitude, LONGITUDE_TEST2);
    testerCoords(latitude, 0, latitude, LONGITUDE_TEST3);
    testerCoords(latitude, 0, latitude, LONGITUDE_TEST4);
    testerCoords(latitude, 0, latitude, LONGITUDE_TEST5);
    testerCoords(latitude, 0, latitude, LONGITUDE_TEST6);
    testerCoords(latitude, 0, latitude, LONGITUDE_TEST7);
}

void testerCoordsSilencieux(float lat1, float long1, float lat2, float long2) {
    if (!estSetup) {
        Coordonnees::calculerSeuils();
        estSetup = true;
    }
    Coordonnees c1(lat1, long1);
    Coordonnees c2(lat2, long2);
    assert(c1.estPresDe(c2) == (c1.distance(c2) < SEUIL_PROXIMITE));
}

void testerLatitudeSilencieux(float latitude) {
    testerCoordsSilencieux(latitude, 0, latitude, LONGITUDE_TEST);
    testerCoordsSilencieux(latitude, 0, latitude, LONGITUDE_TEST_NEGA);
    testerCoordsSilencieux(latitude, 0, latitude, LONGITUDE_TEST2);
    testerCoordsSilencieux(latitude, 0, latitude, LONGITUDE_TEST3);
    testerCoordsSilencieux(latitude, 0, latitude, LONGITUDE_TEST4);
    testerCoordsSilencieux(latitude, 0, latitude, LONGITUDE_TEST5);
    testerCoordsSilencieux(latitude, 0, latitude, LONGITUDE_TEST6);
    testerCoordsSilencieux(latitude, 0, latitude, LONGITUDE_TEST7);
}

// Test de toutes les latitudes de 0 à 89.99999 avec précision de 1 mètre
TEST(estPresDe, OmegaTest) {
    for (int i = 0; i < 9000000; ++i) {
        testerLatitudeSilencieux(i * 0.00001);
    }
}

float latitudeAuHasard() {
    return -90 + (((float) rand() / RAND_MAX) * 180);
};

float longitudeAuHasard() {
    return -180 + (((float) rand() / RAND_MAX) * 360);
};

// Test de toutes les latitudes de 0 à 89.99999 avec précision de 1 mètre
TEST(estPresDe, TestAuHasard) {
    srand(123456);
    for (int i = 0; i < 90000; ++i) {
        testerCoordsSilencieux(
                latitudeAuHasard(),
                longitudeAuHasard(),
                latitudeAuHasard(),
                longitudeAuHasard());
    }
}

TEST(estPresDe, DistanceDiagonale) {
    testerCoords(45.5022, -73.5936, 45.5024, -73.5934);
}

TEST(estPresDe, TestLat0) {
    testerLatitude(0);
}

TEST(estPresDe, TestLat10) {
    testerLatitude(10);
}

TEST(estPresDe, TestLat20) {
    testerLatitude(20);
}

TEST(estPresDe, TestLat30) {
    testerLatitude(30);
}

TEST(estPresDe, TestLat40) {
    testerLatitude(40);
}

TEST(estPresDe, TestLat50) {
    testerLatitude(50);
}

TEST(estPresDe, TestLat60) {
    testerLatitude(60);
}

TEST(estPresDe, TestLat70) {
    testerLatitude(70);
}

TEST(estPresDe, TestLat79999999999) {
    testerLatitude(79.99999999);
}

TEST(estPresDe, TestLat80) {
    testerLatitude(80);
}

TEST(estPresDe, TestLat89) {
    testerLatitude(89);
}

TEST(estPresDe, TestLat89999999999) {
    testerLatitude(89.99999999);
}