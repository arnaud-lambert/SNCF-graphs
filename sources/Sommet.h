#ifndef SOMMET_H
#define SOMMET_H
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include "svg/svgfile.h"
#include <queue>

class Arete;

class Sommet
{
    public:
        Sommet(int id, std::string nom, double x, double y);
        Sommet(Sommet&source);
        virtual ~Sommet();
        void affichage()const;
        int getId() { return m_id; }
        void setId(int id) { m_id=id; }
        std::vector<std::pair<Sommet*,Arete*>> getAdjacents() { return m_adjacents; }
        void ajouterAdjacent(Sommet* adjacent, Arete* arete) { m_adjacents.push_back({adjacent,arete}); }
        double getX() const { return m_x; }
        double getY() const { return m_y; }
        void setX(double x) { m_x = x; }
        void setY(double y) { m_y = y; }
        void dessiner(Svgfile&svgout, HSL couleur);
        std::string getNom () const { return m_nom; }
        void suppAdjacent (Sommet*adjacent);
        void dfsReverse(std::vector<bool>& sommetCouleur, std::vector<std::vector<int>> reverseAdjacents, std::vector<int>& composanteFortementConnexe, std::vector<Sommet*> m_sommets);
        void dfs(std::vector<bool>& sommetCouleur, std::vector<int>& ordreSommet);
        bool bfs(std::vector<std::vector<int>> matriceAdjacence, int puit, std::vector<int>& chemin);
        double fordFulkerson(std::vector<std::vector<int>> matriceAdjacence, int puit, int sommetn, double& flotSommetn);

    private:
        int m_id;
        std::string m_nom;
        double m_x;
        double m_y;
        std::vector<std::pair<Sommet*,Arete*>> m_adjacents;

};

#endif // SOMMET_H
