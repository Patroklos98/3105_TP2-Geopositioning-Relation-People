/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP2                                       *
 *  AUTEUR(S):  
 *  Salcedo, Renzo - SALR02089408                        *
 *  Marcotte-Gourde, Yannick - MARY07088906              *
 *                          							 *
 *                                                       */

#include <iostream>
#include <fstream>
#include "tableau.h"
#include "tp2.h"

int main(int argc, const char **argv) {
    Tp2 tp2;
    if (argc < 2)
        return tp2.analyserRelations(std::cin);
    else {
        std::ifstream fichierPositions(argv[1]);
        return tp2.analyserRelations(fichierPositions);
    }
}

int Tp2::analyserRelations(std::istream &fluxEntree) {
    Coordonnees::calculerSeuils();
    lirePersonnes(fluxEntree);
    determinerEtAfficherRelations(*personnes);
    return EXIT_SUCCESS;
}

void Tp2::lirePersonnes(std::istream &fluxEntree) {
    Personne* nouvellePersonne = nullptr;
    while (fluxEntree) {
        nouvellePersonne = new Personne();
        fluxEntree >> *nouvellePersonne;
        if (fluxEntree.eof()) break;
        personnes->ajouter(nouvellePersonne);
    };
    delete nouvellePersonne;
}

void Tp2::determinerEtAfficherRelations(const Tableau<Personne*> &personnes) {
    for (int i = 0; i < personnes.taille(); i++)
        for (int j = i + 1; j < personnes.taille(); j++) {
            const std::string typeRelation = personnes[i]->getTypeRelation(*personnes[j]);
            if (typeRelation != INCONNUS) {
                afficherRelation(personnes[i]->getNom(), personnes[j]->getNom(), typeRelation);
            }
        }
}

void Tp2::afficherRelation(const std::string &nom1, const std::string &nom2, const std::string &typeRelation) {
    std::cout << nom1 << " et " << nom2 << " sont des " << typeRelation << "." << std::endl;
}

Tp2::~Tp2() {
    for (int i = 0; i < personnes->taille(); ++i) {
        delete (*personnes)[i];
    }
    delete personnes;
}
