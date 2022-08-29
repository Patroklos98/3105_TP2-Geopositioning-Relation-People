/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP2                                       *
 *  AUTEUR(S):  
 *  Salcedo, Renzo - SALR02089408                        *
 *  Marcotte-Gourde, Yannick - MARY07088906              *
 *                          							 *
 *                                                       */

#if !defined(_COORDONNEES__H_)
#define _COORDONNEES__H_

#include <iostream>

static const int SEUIL_PROXIMITE = 25;

static const int NOMBRE_DE_SEUILS = 901;

static const int LIMITE_DE_PRECISION_DES_SEUILS = 887;

class Coordonnees {
public:
    Coordonnees() {}

    Coordonnees(float latitude_, float longitude_);

    Coordonnees(const Coordonnees &);

    float distance(const Coordonnees &) const;

    bool estPresDe(const Coordonnees &coor) const;

    static void calculerSeuils();
    static float calculerDeltaLongitudeEnRadians(float);

private:
    short dixiemesDeLatitude;
    float latitudeRadians{};
    float longitudeRadians{};

    bool estApproximativementPresDe(const Coordonnees &coor) const;

    friend std::ostream &operator<<(std::ostream &, const Coordonnees &);

    friend std::istream &operator>>(std::istream &, Coordonnees &);
};

#endif

