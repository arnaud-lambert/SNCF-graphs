#include "Arete.h"
Sommet::Sommet(int id, std::string nom, double x, double y)
    :m_id{id}, m_nom{nom}, m_x{x}, m_y{y}
{

}

Sommet::Sommet(Sommet&source): m_id{source.m_id}, m_nom{source.m_nom}, m_x{source.m_x}, m_y{source.m_y}
{

}

Sommet::~Sommet()
{

}

void Sommet::affichage()const
{
    std::cout << m_id << " " << m_nom << " " << m_x << " " << m_y << " adj: ";

    for(auto &i : m_adjacents)
        std::cout << i.first->getId() << "=" << i.second->getPoids() << " ";
}

void Sommet::dessiner(Svgfile&svgout)
{
    std::string couleur;
    switch((int)m_adjacents.size())
    {
    case 1 :
        couleur="cyan";
        break;
    case 2 :
        couleur="green";
        break;
    case 3 :
        couleur="blue";
        break;
    case 4 :
        couleur="red";
        break;
    default :
        couleur="black";
        break;
    }
    svgout.addDisk( m_x*100, m_y*100, 5, couleur);
    svgout.addText( m_x*100 - 5, m_y*100 - 10, m_nom, "black" );
}

void Sommet::suppAdjacent(Sommet*adjacent)
{
    for(size_t i=0; i<m_adjacents.size(); ++i)
    {
        if(m_adjacents[i].first==adjacent)
        {
            m_adjacents.erase(m_adjacents.begin()+i);
        }
    }
}

void Sommet::dfs(std::vector<bool>& sommetCouleur, std::vector<int>& ordreSommet)
{
    sommetCouleur[m_id]=true;
    for(size_t i=0; i<m_adjacents.size(); i++)
    {
        if(sommetCouleur[m_adjacents[i].first->m_id]==false)
            m_adjacents[i].first->dfs(sommetCouleur, ordreSommet);
    }
    ordreSommet.push_back(m_id);
}

void Sommet::dfsReverse(std::vector<bool>& sommetCouleur, std::vector<std::vector<int>> reverseAdjacents, std::vector<int>& composanteFortementConnexe, std::vector<Sommet*> m_sommets)
{
    sommetCouleur[m_id]=true;

    for(size_t i=0; i<reverseAdjacents[m_id].size(); i++)
    {
        if(sommetCouleur[reverseAdjacents[m_id][i]]==false)
            m_sommets[reverseAdjacents[m_id][i]]->dfsReverse(sommetCouleur, reverseAdjacents, composanteFortementConnexe, m_sommets);
    }
    composanteFortementConnexe.push_back(m_id);
}
