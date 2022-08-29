/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP2                                       *
 *  AUTEUR(S):  
 *  Salcedo, Renzo - SALR02089408                        *
 *  Marcotte-Gourde, Yannick - MARY07088906              *
 *                          							 *
 *                                                       */

#include <cmath>
#include <cassert>
#include <limits>
#include "coordonnees.h"

static const int RAYON_TERRE_EN_KILOMETRES = 6371;
static const int RAYON_TERRE_EN_METRES = RAYON_TERRE_EN_KILOMETRES * 1000;

static const float PI = 3.14159265359;
static const float UN_RADIAN = ((float) PI / 180.0f);
static const float SEUIL_CIBLE = 0.0251;
float FACTEUR_HAVERSINE = std::pow(std::sin(SEUIL_CIBLE / RAYON_TERRE_EN_KILOMETRES / 2), 2);
static float SEUILS_DE_25M_EN_RADIANS[NOMBRE_DE_SEUILS];

Coordonnees::Coordonnees(const Coordonnees &coor) = default;

Coordonnees::Coordonnees(float latitude_, float longitude_)
        : dixiemesDeLatitude((short) (latitude_ * 10)), latitudeRadians(latitude_ * UN_RADIAN),
          longitudeRadians(longitude_ * UN_RADIAN) {
}

bool Coordonnees::estPresDe(const Coordonnees &coor) const {
    return estApproximativementPresDe(coor) && distance(coor) <= SEUIL_PROXIMITE;
}

bool Coordonnees::estApproximativementPresDe(const Coordonnees &coor) const {
    const float seuil = SEUILS_DE_25M_EN_RADIANS[abs((dixiemesDeLatitude + coor.dixiemesDeLatitude) / 2)];
    return std::abs(latitudeRadians - coor.latitudeRadians) < seuil
           && std::abs(longitudeRadians - coor.longitudeRadians) < seuil;
}

float Coordonnees::distance(const Coordonnees &coor) const {
    const float s1 = std::sin((coor.latitudeRadians - latitudeRadians) / 2);
    const float s2 = std::sin((coor.longitudeRadians - longitudeRadians) / 2);
    return 2 * RAYON_TERRE_EN_METRES *
           std::asin(std::sqrt(s1 * s1 + std::cos(latitudeRadians) * std::cos(coor.latitudeRadians) * s2 * s2));
}

std::istream &operator>>(std::istream &fluxEntree, Coordonnees &coor) {
    char parentheseOuvrante, virgule, parentheseFermante;
    fluxEntree >> parentheseOuvrante;
    if (fluxEntree) {
        fluxEntree >> coor.latitudeRadians >> virgule >> coor.longitudeRadians >> parentheseFermante;
        assert(parentheseOuvrante == '(');
        assert(virgule == ',');
        assert(parentheseFermante == ')');
        coor.dixiemesDeLatitude = (short) (coor.latitudeRadians * 10);
        coor.longitudeRadians *= UN_RADIAN;
        coor.latitudeRadians *= UN_RADIAN;
    }
    return fluxEntree;
}

std::ostream &operator<<(std::ostream &fluxSortie, const Coordonnees &coor) {
    fluxSortie << "("
               << (coor.latitudeRadians / UN_RADIAN)
               << ","
               << (coor.longitudeRadians / UN_RADIAN)
               << ")";
    return fluxSortie;
}

// On utilise des seuils de 25 mètres pré-calculés pour chaque dixième de degré de latitude à l'aide d'une
// fonction "reverse haversine".
//
// Nous avons vérifié l'approximation avec des tests exhaustifs à des intervals de latitude d'un centimètre.
void Coordonnees::calculerSeuils() {
    for (int i = 0; i < LIMITE_DE_PRECISION_DES_SEUILS; ++i) {
        SEUILS_DE_25M_EN_RADIANS[i] = calculerDeltaLongitudeEnRadians((float)i / 10.0f);
    }
    // À partir de la latitude 88.7 en montant, notre méthode ne produit plus d'estimés fiables.
    // On dit alors que le seuil est l'infini positif pour qu'on considère toujours des coordonnées à ces
    // latitudes comme candidates et qu'on calcule la vraie distance entre elles.
    for (int i = LIMITE_DE_PRECISION_DES_SEUILS; i < NOMBRE_DE_SEUILS; ++i) {
        SEUILS_DE_25M_EN_RADIANS[i] = std::numeric_limits<float>::infinity();
    }
}

/*
  Algorithme Reverse Haversine inspiré par:
    Roger Martin (https://math.stackexchange.com/users/1009736/roger-martin)
    Reverse use of Haversine formula
    URL (version: 2021-12-28): https://math.stackexchange.com/q/4343818
 */
float  Coordonnees::calculerDeltaLongitudeEnRadians(float latitude) {
    float cos1 = std::cos(latitude * UN_RADIAN);
    return 2 * std::asin(std::sqrt(FACTEUR_HAVERSINE / (cos1 * cos1)));
}
