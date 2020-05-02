#include "Arete.h"
#include "svg/svgfile.h"

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

void Sommet::dessiner(Svgfile&svgout, HSL couleur)
{
    RGB color=HSLToRGB(couleur);
    std::string cooouleur= svgout.makeRGB((int)color.getR(),(int)color.getG(),(int)color.getB());
    std::cout<<"COULEUR : "<<cooouleur<<std::endl;
    svgout.addDisk( m_x*100, m_y*100, 5, cooouleur);
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

bool Sommet::bfs(std::vector<std::vector<int>> grapheResiduel, int puit, std::vector<int>& parent)
{
    parent[m_id]=-1;
    std::vector<bool> sommetCouleur((int)grapheResiduel.size(), false);
    sommetCouleur[m_id]=true;
    std::queue<int> file;
    file.push(m_id);

    while(!file.empty())
    {
        int sommetTop=file.front();
        file.pop();
        for(size_t i=0; i<grapheResiduel.size(); i++)
        {
            if(sommetCouleur[i]==false && grapheResiduel[sommetTop][i]>0)
            {
                file.push(i);
                parent[i]=sommetTop;
                sommetCouleur[i]=true;
            }
        }
    }
    return (sommetCouleur[puit]==true);
}

double Sommet::fordFulkerson(std::vector<std::vector<int>> matriceAdjacence, int puit, int sommetn, double& flotSommetn)
{
    double flotMax=0;
    std::vector<int> parent((int)matriceAdjacence.size(), -1);

    std::vector<std::vector<int>> grapheResiduel((int)matriceAdjacence.size(), std::vector<int> ((int)matriceAdjacence.size(), 0));
    for(size_t i=0; i<matriceAdjacence.size(); i++)
        for(size_t j=0; j<matriceAdjacence[i].size(); j++)
            grapheResiduel[i][j]=matriceAdjacence[i][j];

    while(bfs(grapheResiduel, puit, parent))
    {
        int cheminFlot=RAND_MAX;
        int chercheSource=puit;

        while(chercheSource!=m_id)
        {
            int predecesseur=parent[chercheSource];
            cheminFlot=std::min(cheminFlot, grapheResiduel[predecesseur][chercheSource]);
            chercheSource=predecesseur;
        }
        chercheSource=puit;
        while(chercheSource!=m_id)
        {
            int predecesseur=parent[chercheSource];
            grapheResiduel[predecesseur][chercheSource]-=cheminFlot;
            grapheResiduel[chercheSource][predecesseur]+=cheminFlot;
            chercheSource=predecesseur;
        }
        flotMax+=cheminFlot;
    }

    int verifPuit=0;
    for(size_t i=0; i<grapheResiduel[sommetn].size(); i++)
            verifPuit+=grapheResiduel[sommetn][i];


    if(verifPuit!=0)
        for(size_t i=0; i<grapheResiduel.size(); i++)
            flotSommetn+=grapheResiduel[i][sommetn];

    return flotMax;
}
