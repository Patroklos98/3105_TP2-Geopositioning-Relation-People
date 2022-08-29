/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP2                                       *
 *  AUTEUR(S):  
 *  Salcedo, Renzo - SALR02089408                        *
 *  Marcotte-Gourde, Yannick - MARY07088906              *
 *                          							 *
 *                                                       */

#include <algorithm>
#include <cassert>
#include "personne.h"

int const NB_LIEUX_MINIMUM = 2;
int const NB_TEMPS_MINIMUM = 120;

std::string Personne::getNom() const {
    return nom;
}

std::string Personne::getTypeRelation(const Personne &autre) const {
    if (positions->vide() || autre.positions->vide()) {
        return INCONNUS;
    }
    return determinerRelation(autre);
}

std::string Personne::determinerRelation(const Personne &autre) const {
    const Rencontres rencontres = determinerRencontres(autre);
    if (rencontres.duree_rencontre < NB_TEMPS_MINIMUM) {
        return INCONNUS;
    } else if (rencontres.nb_lieux < NB_LIEUX_MINIMUM) {
        return CONNAISSANCES;
    } else {
        return AMIS;
    }
}

Personne::Rencontres Personne::determinerRencontres(const Personne &autre) const {
    Rencontres rencontres;
    short moment = positionnerLesPersonnes(autre);
    while (moment < NB_MINUTE_JOURNEE) {
        const Coordonnees& c1 = (*positions)[position]->espace;
        const Coordonnees& c2 = (*autre.positions)[autre.position]->espace;
        const bool au_meme_endroit = c1.estPresDe(c2);
        const short moment2 = trouverProchainMoment(autre);
        if (au_meme_endroit) {
            rencontres.nb_lieux++;
            rencontres.duree_rencontre += moment2 - moment;
        }
        moment = moment2;
    }
    return rencontres;
}

short Personne::positionnerLesPersonnes(const Personne &autre) const {
    short moment = -1;
    position = -1;
    autre.position = -1;
    // On avance le temps jusqu'à-ce-que les deux personnes aient une position.
    while (position < 0 || autre.position < 0) {
        moment = trouverProchainMoment(autre);
    }
    return moment;
}

short Personne::trouverProchainMoment(const Personne &autre) const {
    const bool fin = position == (*positions).taille() - 1;
    const bool fin_autre = autre.position == (*autre.positions).taille() - 1;
    if (fin) {
        if (fin_autre) {
            return NB_MINUTE_JOURNEE;
        } else {
            autre.position++;
            return (*autre.positions)[autre.position]->temps;
        }
    } else if (fin_autre) {
        position++;
        return (*positions)[position]->temps;
    }

    return incrementerMoment(autre);
}

short Personne::incrementerMoment(const Personne &autre) const {
    short momentMinimum = 0;
    const short moment = (*positions)[position + 1]->temps;
    const short momentAutre = (*autre.positions)[autre.position + 1]->temps;
    if (moment == momentAutre) {
        momentMinimum = moment;
        position++;
        autre.position++;
    } else if (moment < momentAutre) {
        momentMinimum = moment;
        position++;
    } else {
        momentMinimum = momentAutre;
        autre.position++;
    }
    return momentMinimum;
}

std::istream &operator>>(std::istream &is, Personne &p) {
    std::string nom;
    is >> nom;
    if (nom.empty() || is.eof()) return is;
    p.nom = nom;
    while (true) {
        char c;
        is >> c;
        if (c == '-') {
            assert(is.get() == '-');
            break;
        } else {
            is.unget();
        }
        Personne::Position* position = new Personne::Position();
        is >> position->espace >> position->temps >> c;
        assert(c == ';');
        p.positions->ajouter(position);
    }
    return is;
}

Personne::~Personne() {
    for (int i = 0; i < positions->taille(); ++i) {
        delete (*positions)[i];
    }
    delete positions;
}

Personne::Personne() : positions(new Tableau<Position*>) {}
