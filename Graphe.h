#ifndef GRAPHE_H
#define GRAPHE_H
#include "Arete.h"
#include <fstream>
#include <math.h>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <queue>
#include <windows.h>

///Varibale globale qui permet de mettre de la couleur sur le texte de la console
extern HANDLE texteConsole;

class Graphe
{
    public:
        Graphe(std::string& nomFichier);
        Graphe(const Graphe&source);
        virtual ~Graphe();
        void affichage()const;
        void ponderation();
        void dessiner ();
        std::vector<std::pair<double, double>> vecteurPropre();
        std::vector<Sommet*> getSommets() { return m_sommets;}
        int getTaille() {return m_taille; }
        std::vector<std::pair<int, double>> centraliteDegre ();
        int rechercheCC ();
        void SuppAreteI (int indice);
        void supprimerArete ();
        void testConnexite (int nb);
        void sauvegarder(std::vector<std::pair<int, double>> centralite_degres, std::vector<std::pair<double, double>> vecteurPropre, std::vector<std::pair<double, double>> vecteurProximite, std::pair<std::vector<std::pair<double,double>>,std::vector<std::pair<Arete*,std::pair<double,double>>>> intermediarite, std::string nomFichier);
        std::pair<std::vector<std::pair<double,double>>,std::vector<std::pair<Arete*,std::pair<double,double>>>> intermediarite();
        std::vector<std::pair<double, double>> vecteurProximite();
        void kSommetsConnexite ();
        void supprimerSommet (Sommet*s);
        void comparaisonIndices (int nb);
        void testForteConnexite();
        std::map<std::pair<Sommet*,Sommet*>,std::vector<std::unordered_set<Arete*>>> tousLesChemins();
        void kAretesConnexe ();


    private:
        std::vector<Sommet*> m_sommets;
        std::vector<Arete*> m_aretes;
        bool m_orientation;
        bool m_ponderation;
        int m_ordre;
        int m_taille;
};

void recursifIntermediarite (std::pair<std::unordered_map<Sommet*,unsigned int> ,std::unordered_map<Arete*,unsigned int>> &compt, Sommet* current, std::unordered_map<Sommet*, std::pair<std::vector<std::pair<Sommet*,Arete*>>,double>> &predecesseurs);
void recursifTousLesChemins (std::vector<std::unordered_set<Arete*>> &commun, std::unordered_set<Arete*> cheminUnique, std::pair<Sommet*,Arete*> current, std::pair<Sommet*,Sommet*> &debFin);
#endif // GRAPHE_H
