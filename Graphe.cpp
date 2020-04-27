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
        int indice;
        double x, y;
        std::string nom;
        ///On crée un nombre de sommet égal à l'odre du graphe
        for(int i=0; i<m_ordre; ++i)
        {
            ifs>>indice>>nom>>x>>y;
            Sommet* nouv = new Sommet(indice, nom, x, y);
            m_sommets.push_back(nouv);
        }
        ///On récupère la taille du graphe
        ifs>>m_taille;
        int extremite1, extremite2;
        for(int i=0; i<m_taille; ++i)
        {
            ifs>>indice>>extremite1>>extremite2;
            Arete* nouv = new Arete(indice, m_sommets[extremite1], m_sommets[extremite2]);
            m_aretes.push_back(nouv);
            m_sommets[extremite1]->ajouterAdjacent(m_sommets[extremite2]);
            if(!m_orientation)
                m_sommets[extremite2]->ajouterAdjacent(m_sommets[extremite1]);
        }

    }
}

Graphe::~Graphe()
{
    //dtor
}

void Graphe::affichage()const
{
    std::cout<<m_orientation<<std::endl<<m_ordre<<std::endl;
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        m_sommets[i]->affichage();
//            std::cout << "ajd: ";
//        for(auto j : m_sommets[i]->getAdjacents())
//            std::cout << j->getId() << " ";
//        std::cout << std::endl;
    }
    std::cout<<m_taille<<std::endl;
    for(size_t i=0; i<m_aretes.size(); ++i)
        m_aretes[i]->affichage();
}

void Graphe::ponderation(std::string nomFichier)
{
    if(nomFichier=="")
        m_ponderation=false;
    else
    {
        std::ifstream ifs{nomFichier};
        if(!ifs)
            throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );
        int taille;
        ifs>>taille;
        if(taille==m_taille)
        {
            m_ponderation=true;
            int id;
            double poids;
            for(int i=0; i<taille; ++i)
            {
                ifs>>id>>poids;
                m_aretes[id]->setPoids(poids);
            }
        }
        else
        {
            m_ponderation=false;
            std::cout<<"Votre fichier est incompatible (taille incorrecte)"<<std::endl;
        }
    }

}

void Graphe::dessiner ()
{
    Svgfile svgout;
    svgout.addGrid();

    for(size_t i=0; i<m_sommets.size(); ++i)
        m_sommets[i]->dessiner(svgout);

    for(size_t j=0; j<m_aretes.size(); ++j)
        m_aretes[j]->dessiner(svgout);
}

