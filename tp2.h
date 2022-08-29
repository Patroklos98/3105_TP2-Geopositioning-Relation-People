/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP2                                       *
 *  AUTEUR(S):  
 *  Salcedo, Renzo - SALR02089408                        *
 *  Marcotte-Gourde, Yannick - MARY07088906              *
 *                          							 *
 *                                                       */

#ifndef INF3105TP2_TP2_H
#define INF3105TP2_TP2_H

#include "personne.h"

class Tp2 {
public:
    ~Tp2();

    int analyserRelations(std::istream &);
private:
    Tableau<Personne*>* personnes = new Tableau<Personne*>();

    void lirePersonnes(std::istream &);

    void determinerEtAfficherRelations(const Tableau<Personne*> &);

    static void afficherRelation(const std::string &nom1, const std::string &nom2, const std::string &typeRelation);
};



#endif //INF3105TP2_TP2_H
