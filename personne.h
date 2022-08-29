/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP2                                       *
 *  AUTEUR(S):  
 *  Salcedo, Renzo - SALR02089408                        *
 *  Marcotte-Gourde, Yannick - MARY07088906              *
 *                          							 *
 *                                                       */

#if !defined(__PERSONNE_H__)
#define __PERSONNE_H__

static const int NB_MINUTE_JOURNEE = 1440;

static const char *const INCONNUS = "";

static const char *const CONNAISSANCES = "connaissances";

static const char *const AMIS = "amis";

#include <iostream>
#include <string>
#include "coordonnees.h"
#include "tableau.h"

class Personne {
public:
    std::string getNom() const;

    std::string getTypeRelation(const Personne &autre) const;

    Personne();
    ~Personne();

private:
    struct Position {
        Coordonnees espace;
        short temps;

        bool operator<(const Position &o) const {
            return temps < o.temps;
        }

        bool operator==(const Position &o) const {
            return temps == o.temps;
        }
    };

    struct Rencontres {
        int duree_rencontre{0};
        int nb_lieux{0};
    };

    std::string nom{};
    Tableau<Position*>* positions;
    mutable int position{-1};

    short trouverProchainMoment(const Personne &autre) const;

    short incrementerMoment(const Personne &autre) const;

    std::string determinerRelation(const Personne &autre) const;

    short positionnerLesPersonnes(const Personne &autre) const;

    Rencontres determinerRencontres(const Personne &autre) const;

    friend std::istream &operator>>(std::istream &, Personne &);
};

#endif
