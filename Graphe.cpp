#include "Graphe.h"

///Lecture du fichier via le constructeur de Graphe
Graphe::Graphe(std::string nomFichier)
{
    ///On ouvre le fichier
    std::ifstream ifs{nomFichier};
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );
    ///S'il n'y a pas d'erreur
    else
    {
        ifs>>m_orientation;
        ///On récupère l'odre
        ifs>>m_ordre;
        int indice, x, y;
        char nom;
        ///On crée un nombre de sommet égal à l'odre du graphe
        for(int i=0; i<m_ordre; i++)
        {
            ifs>>indice>>nom>>x>>y;
            Sommet* nouv=new Sommet(indice, nom, x, y);
            m_som.push_back(nouv);
        }
        ///On récupère la taille du graphe
        ifs>>m_taille;
        int extremite1, extremite2;
        for(int i=0; i<m_taille; i++)
        {
            ifs>>indice>>extremite1>>extremite2;
            Arete* nouv=new Arete(indice, m_som[extremite1], m_som[extremite2]);
            m_arr.push_back(nouv);
        }
    }
}

Graphe::~Graphe()
{
    //dtor
}

void Graphe::affichage()const
{
    std::cout<<m_orientation<<std::endl<<m_ordre;
    for(size_t i=0; i<m_som.size(); i++)
        m_som[i]->affichage();
    std::cout<<std::endl<<m_taille;
    for(size_t i=0; i<m_arr.size(); i++)
        m_arr[i]->affichage();
}
