/*    UQAM / Département d'informatique                          *
 *    INF3105 - Structures de données et algorithmes             *
 *    Été 2022 / TP2                                             *
 *    AUTEUR(S):Salcedo, Renzo: SALR02089408 
 * 				Marcotte-Gourde, Yannick: MARY07088906           *
 *                                                               *
 *    Modifications pour TP2:                                    *
 *     - Implémenté tri_fusion en plus de tri_fusion_indices     *
 *     - Implémenté fusion en plus de fusion_indices             *
 *     - Implémenté trier                                        *
 *                                                               *
 *     Note: À cause de l'annulation du critère F, aucun des     *
 *     changements à cette classe effectué pour ce TP ne sont    *
 *     utilisés.                                                 */

/* Classe générique Tableau<T>
 * TP1 -- Tableau dynamique générique
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes - Groupe 30 - Été 2022
 * http://ericbeaudry.uqam.ca/INF3105/tp1/
 *
 * Équipe:
 * - Barbier, Benoit; BARB26069703; barbier.benoit@courrier.uqam.ca
 * - Marcotte-Gourde, Yannick; MARY07088906; marcotte-gourde.yannick@courrier.uqam.ca
 *
 * Compromis:
 *   Bien que d'autres algorithmes de tri, tel le tri d'insertion binaire, soient plus rapides dans l'exécution de
 *   tests_tp1, nous avons sélectionné le tri fusion pour sa complexité asymptotique supérieure.
 *
 * References:
 *   L'algorithme des méthodes tri_fusion_indices et fusion_indices a été adapté de l'implémentation suggérée
 *   sur https://big-o.io/algorithms/comparison/merge-sort/.
 */

#if !defined(_TABLEAU___H_)
#define _TABLEAU___H_

#include <stdexcept>

const int CAPACITE_INITIALE = 8;
const char *const INDICE_HORS_TABLEAU = "L'indice est hors du tableau.";

template<class T>
class Tableau {
public:

    explicit Tableau(int capacite_ = CAPACITE_INITIALE);

    Tableau(const Tableau<T> &);

    ~Tableau();

    void ajouter(const T &item);

    void vider();

    void inserer(const T &element, int indice = 0);

    void enlever(int indice = 0);

    void enlever_dernier();

    T &operator[](int indice);

    const T &operator[](int indice) const;

    bool vide() const;

    int taille() const;

    // Retourne indice première occurrence.
    int chercher(const T &element) const;

    bool contient(const T &element) const;

    // Retourne le nombre d'occurrences de l'élément.
    int compter(const T &element) const;

    // Cette fonction permet d'optimiser les fonctions chercher, contient et compter.
    void creer_index();

    void trier();

    Tableau<T> &operator=(const Tableau<T> &autre);

    bool operator==(const Tableau<T> &autre) const;

    bool operator!=(const Tableau<T> &autre) const;

private:

    // Contient des pointeurs triés et consolidés aux éléments du tableau pour permettre la recherche rapide.
    struct Index;

    // Métadonnées d'un élément du tableau.
    struct Entree;

    int nb_elements;
    int capacite;
    T *elements;
    Index *index;

    void cloner(const Tableau<T> &autre);

    // Copie la représentation interne de ce tableau dans un autre tableau natif.
    void copier_elements(T *&autre) const;

    // Détermine si le tableau est indexé.
    bool est_indexe() const;

    // Copie l'index d'un autre tableau et le fait pointer vers les éléments de ce tableau.
    void cloner_index(const Index *autre);

    // Détruit l'index.
    void invalider_index();

    void doubler_capacite();

    // Vérifie si les éléments du tableau passé sont équivalents aux éléments de ce tableau.
    bool est_equivalent(const Tableau<T> &autre) const;

    void tri_fusion_indices(int *indices_elements, int debut, int fin) const;

    void fusion_indices(int *indices_elements, int debut, int milieu, int fin) const;

    Index *populer_index(const int *indices_elements);

    void tri_fusion(int debut, int fin);

    void fusion(int debut, int milieu, int fin);

};


// ---------- Définitions Tableau -------------


template<class T>
Tableau<T>::Tableau(int capacite_) :
        nb_elements(0), capacite(capacite_), elements(new T[capacite]),
        index(nullptr) {}

template<class T>
Tableau<T>::Tableau(const Tableau &autre) :
        nb_elements(autre.nb_elements), capacite(autre.capacite),
        elements(new T[capacite]), index(nullptr) {
    cloner(autre);
}

template<class T>
void Tableau<T>::cloner(const Tableau<T> &autre) {
    autre.copier_elements(elements);
    if (autre.est_indexe()) {
        cloner_index(autre.index);
    }
}

template<class T>
void Tableau<T>::copier_elements(T *&autre) const {
    for (int i = 0; i < nb_elements; i++) {
        autre[i] = elements[i];
    }
}

template<class T>
bool Tableau<T>::est_indexe() const {
    return index != nullptr;
}

template<class T>
void Tableau<T>::cloner_index(const Index *autre) {
    index = new Index(autre->nb_entrees);
    for (int i = 0; i < autre->nb_entrees; i++) {
        Entree &entree = index->entrees[i];
        entree = autre->entrees[i];
        entree.element = &elements[entree.premier_indice];
    }
}

template<class T>
Tableau<T>::~Tableau() {
    delete[] elements;
    if (est_indexe()) {
        delete index;
    }
}

template<class T>
bool Tableau<T>::vide() const {
    return nb_elements == 0;
}

template<class T>
int Tableau<T>::taille() const {
    return nb_elements;
}

template<class T>
void Tableau<T>::ajouter(const T &item) {
    if (nb_elements == capacite) {
        doubler_capacite();
    }
    elements[nb_elements] = item;
    nb_elements++;
    invalider_index();
}

template<class T>
void Tableau<T>::doubler_capacite() {
    capacite *= 2;
    T *nouveaux_elements = new T[capacite];
    copier_elements(nouveaux_elements);
    delete[] elements;
    elements = nouveaux_elements;
}

template<class T>
void Tableau<T>::invalider_index() {
    if (est_indexe()) {
        delete index;
        index = nullptr;
    }
}

template<class T>
void Tableau<T>::vider() {
    invalider_index();
    nb_elements = 0;
}

template<class T>
void Tableau<T>::inserer(const T &element, const int indice) {
    if (indice < 0 || indice > nb_elements) {
        throw std::out_of_range(INDICE_HORS_TABLEAU);
    }
    T *tableau = elements;
    if (nb_elements == capacite) {
        capacite *= 2;
        tableau = new T[capacite];
        for (int i = 0; i < indice; i++) {
            tableau[i] = elements[i];
        }
    }
    for (int i = nb_elements; i > indice; i--) {
        tableau[i] = elements[i - 1];
    }
    tableau[indice] = element;
    if (tableau != elements) {
        delete[] elements;
        elements = tableau;
    }
    nb_elements++;
    invalider_index();
}

template<class T>
void Tableau<T>::enlever(const int indice) {
    if (indice < 0 || indice >= nb_elements) {
        throw std::out_of_range(INDICE_HORS_TABLEAU);
    }
    for (int i = indice; i < nb_elements - 1; i++) {
        elements[i] = elements[i + 1];
    }
    nb_elements--;
    invalider_index();
}

template<class T>
void Tableau<T>::enlever_dernier() {
    enlever(nb_elements - 1);
}

template<class T>
int Tableau<T>::chercher(const T &element) const {
    if (est_indexe()) {
        return index->chercher(element).premier_indice;
    }

    for (int i = 0; i < nb_elements; i++) {
        if (elements[i] == element) {
            return i;
        }
    }
    return -1;
}

template<class T>
bool Tableau<T>::contient(const T &element) const {
    return chercher(element) != -1;
}

template<class T>
int Tableau<T>::compter(const T &element) const {
    int compte = 0;
    if (est_indexe()) {
        return index->chercher(element).compte;
    }

    for (int i = 0; i < nb_elements; i++) {
        if (elements[i] == element) {
            compte++;
        }
    }
    return compte;
}

template<class T>
void Tableau<T>::creer_index() {
    if (!est_indexe()) {
        int *const indices_elements = new int[nb_elements];
        for (int i = 0; i < nb_elements; i++) {
            indices_elements[i] = i;
        }
        tri_fusion_indices(indices_elements, 0, nb_elements - 1);
        index = populer_index(indices_elements);
        index->dedupliquer();
        delete[] indices_elements;
    }
}

template<class T>
void Tableau<T>::tri_fusion_indices(int *const indices_elements, const int debut, const int fin) const {
    if (debut < fin) {
        int milieu = (debut + fin) / 2;

        tri_fusion_indices(indices_elements, debut, milieu);
        tri_fusion_indices(indices_elements, milieu + 1, fin);

        fusion_indices(indices_elements, debut, milieu, fin);
    }
}

template<class T>
void Tableau<T>::fusion_indices(int *const indices_elements, const int debut, const int milieu, const int fin) const {
    const int taille_gauche = milieu - debut + 1;
    const int taille_droite = fin - milieu;
    auto const gauche = new int[taille_gauche];
    auto const droite = new int[taille_droite];

    for (int i = 0; i < taille_gauche; i++) {
        gauche[i] = indices_elements[debut + i];
    }

    for (int i = 0; i < taille_droite; i++) {
        droite[i] = indices_elements[milieu + 1 + i];
    }

    int indice_gauche = 0;
    int indice_droite = 0;
    int i = debut;
    while (indice_gauche < taille_gauche && indice_droite < taille_droite) {
        if (elements[gauche[indice_gauche]] < elements[droite[indice_droite]] ||
            elements[gauche[indice_gauche]] == elements[droite[indice_droite]]) {

            indices_elements[i] = gauche[indice_gauche];
            indice_gauche++;
        } else {
            indices_elements[i] = droite[indice_droite];
            indice_droite++;
        }
        i++;
    }

    while (indice_gauche < taille_gauche) {
        indices_elements[i] = gauche[indice_gauche];
        i++;
        indice_gauche++;
    }

    while (indice_droite < taille_droite) {
        indices_elements[i] = droite[indice_droite];
        i++;
        indice_droite++;
    }

    delete[] gauche;
    delete[] droite;
}

template<class T>
typename Tableau<T>::Index *Tableau<T>::populer_index(const int *indices_elements) {
    auto *nouvel_index = new Index(this->nb_elements);
    for (int i = 0; i < this->nb_elements; i++) {
        nouvel_index->entrees[i] = {
                .element = &this->elements[indices_elements[i]],
                .premier_indice = indices_elements[i],
                .compte = 1
        };
    }
    return nouvel_index;
}

template<class T>
const T &Tableau<T>::operator[](int const indice) const {
    if (indice < 0 || indice >= nb_elements) {
        throw std::out_of_range(INDICE_HORS_TABLEAU);
    }
    return elements[indice];
}

template<class T>
T &Tableau<T>::operator[](int const indice) {
    if (indice < 0 || indice >= nb_elements) {
        throw std::out_of_range(INDICE_HORS_TABLEAU);
    }
    invalider_index();
    return elements[indice];
}

template<class T>
Tableau<T> &Tableau<T>::operator=(const Tableau<T> &autre) {
    if (this == &autre) {
        return *this;
    }
    capacite = autre.capacite;
    nb_elements = autre.nb_elements;
    delete[] elements;
    elements = new T[capacite];
    cloner(autre);
    return *this;
}

template<class T>
bool Tableau<T>::operator==(const Tableau<T> &autre) const {
    return nb_elements == autre.nb_elements && est_equivalent(autre);
}

template<class T>
bool Tableau<T>::est_equivalent(const Tableau<T> &autre) const {
    for (int i = 0; i < nb_elements; i++) {
        if (!(elements[i] == autre.elements[i])) {
            return false;
        }
    }
    return true;
}

template<class T>
bool Tableau<T>::operator!=(const Tableau<T> &autre) const {
    return !(*this == autre);
}

template<class T>
void Tableau<T>::tri_fusion(const int debut, const int fin) {
    if (debut < fin) {
        int milieu = (debut + fin) / 2;

        tri_fusion(debut, milieu);
        tri_fusion(milieu + 1, fin);

        fusion(debut, milieu, fin);
    }
}

template<class T>
void Tableau<T>::fusion(const int debut, const int milieu, const int fin) {
    const int taille_gauche = milieu - debut + 1;
    const int taille_droite = fin - milieu;
    auto const gauche = new T[taille_gauche];
    auto const droite = new T[taille_droite];

    for (int i = 0; i < taille_gauche; i++) {
        gauche[i] = elements[debut + i];
    }

    for (int i = 0; i < taille_droite; i++) {
        droite[i] = elements[milieu + 1 + i];
    }

    int indice_gauche = 0;
    int indice_droite = 0;
    int i = debut;
    while (indice_gauche < taille_gauche && indice_droite < taille_droite) {
        if (gauche[indice_gauche] < droite[indice_droite] ||
            gauche[indice_gauche] == droite[indice_droite]) {

            elements[i] = gauche[indice_gauche];
            indice_gauche++;
        } else {
            elements[i] = droite[indice_droite];
            indice_droite++;
        }
        i++;
    }

    while (indice_gauche < taille_gauche) {
        elements[i] = gauche[indice_gauche];
        i++;
        indice_gauche++;
    }

    while (indice_droite < taille_droite) {
        elements[i] = droite[indice_droite];
        i++;
        indice_droite++;
    }

    delete[] gauche;
    delete[] droite;
}


// ---------- Définitions Index -------------


template<class T>
struct Tableau<T>::Index {
    Entree *entrees;
    int nb_entrees;

    explicit Index(int capacite);

    ~Index();

    void dedupliquer();

    Entree chercher(const T &element) const;

    Entree chercher(const T &element, int debut, int fin) const;
};

template<class T>
struct Tableau<T>::Entree {
    T *element;
    int premier_indice;
    int compte;
};

template<class T>
void Tableau<T>::trier() {
    tri_fusion(0, nb_elements - 1);
    invalider_index();
}

template<class T>
Tableau<T>::Index::Index(int capacite) : entrees(new Entree[capacite]), nb_entrees(capacite) {
}

template<class T>
Tableau<T>::Index::~Index() {
    delete[] entrees;
}

template<class T>
void Tableau<T>::Index::dedupliquer() {
    int indice_reference = 0;
    int deduplications = 0;
    for (int i = 0; i < nb_entrees - 1; i++) {
        if (*(entrees[indice_reference].element) == *(entrees[i + 1].element)) {
            entrees[indice_reference].compte++;
            deduplications++;
        } else {
            indice_reference++;
            entrees[indice_reference] = entrees[i + 1];
        }
    }

    nb_entrees -= deduplications;
}

template<class T>
typename Tableau<T>::Entree Tableau<T>::Index::chercher(const T &element) const {
    return chercher(element, 0, nb_entrees - 1);
}

template<class T>
typename Tableau<T>::Entree Tableau<T>::Index::chercher(const T &element, const int debut, const int fin) const {
    if (fin < debut) {
        return {.element = nullptr, .premier_indice = -1, .compte = 0};
    }

    const int milieu = (debut + fin) / 2;
    const Entree &entree_milieu = entrees[milieu];

    if (element < *entree_milieu.element) {
        return chercher(element, debut, milieu - 1);
    } else if (*entree_milieu.element < element) {
        return chercher(element, milieu + 1, fin);
    }

    return entree_milieu;
}

#endif //define _TABLEAU___H_
