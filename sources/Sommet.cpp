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

/*void Sommet::affichage()const
{
    std::cout << m_id << " " << m_nom << " " << m_x << " " << m_y << " adj: ";

    for(auto &i : m_adjacents)
        std::cout << i.first->getId() << "=" << i.second->getPoids() << " ";
}*/

void Sommet::dessiner(Svgfile&svgout, HSL couleur)
{
    RGB color = HSLToRGB(couleur);
    std::string cooouleur= svgout.makeRGB((int)color.getR(),(int)color.getG(),(int)color.getB());
    //std::cout<<"COULEUR : "<<cooouleur<<std::endl;
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

///On fait un dfs
void Sommet::dfs(std::vector<bool>& sommetCouleur, std::vector<int>& ordreSommet)
{
    ///On marque le sommet
    sommetCouleur[m_id]=true;

    ///On parcourt ses adjacents
    for(size_t i=0; i<m_adjacents.size(); i++)
    {
        ///S'ils ne sont pas marqués, on fait un DFS avec eux
        if(sommetCouleur[m_adjacents[i].first->m_id]==false)
            m_adjacents[i].first->dfs(sommetCouleur, ordreSommet);
    }
    ///On range les sommets en fonction de leur ordre de post découverte
    ordreSommet.push_back(m_id);
}

///DFS avec l'orientation inversé
void Sommet::dfsReverse(std::vector<bool>& sommetCouleur, std::vector<std::vector<int>> reverseAdjacents, std::vector<int>& composanteFortementConnexe, std::vector<Sommet*> m_sommets)
{
    ///On marque le sommet
    sommetCouleur[m_id]=true;

    ///On parcourt ses "nouveaux" adjacents
    for(size_t i=0; i<reverseAdjacents[m_id].size(); i++)
    {
        ///S'ils ne sont pas amrqué, on fait un reverse DFS avec eux
        if(sommetCouleur[reverseAdjacents[m_id][i]]==false)
            m_sommets[reverseAdjacents[m_id][i]]->dfsReverse(sommetCouleur, reverseAdjacents, composanteFortementConnexe, m_sommets);
    }
    ///On range les sommets par ordre de post découverte
    composanteFortementConnexe.push_back(m_id);
}

///BFS
bool Sommet::bfs(std::vector<std::vector<int>> grapheResiduel, int puit, std::vector<int>& parent)
{
    ///On met le parent du sommet à -1
    parent[m_id]=-1;
    ///On crée un vecteur pour marqué nos sommets
    std::vector<bool> sommetCouleur((int)grapheResiduel.size(), false);
    ///On marque le sommet
    sommetCouleur[m_id]=true;
    ///On cree une file
    std::queue<int> file;
    ///On push le sommet
    file.push(m_id);

    ///Tant que la file n'est pas vide
    while(!file.empty())
    {
        ///On stocke l'id du premier de file
        int sommetTop=file.front();
        ///On le pop
        file.pop();
        ///On parcourt tous les sommets
        for(size_t i=0; i<grapheResiduel.size(); i++)
        {
            ///On vérifie dans le graphe résiduel si l'adjacent de notre sommet est marqué
            if(sommetCouleur[i]==false && grapheResiduel[sommetTop][i]>0)
            {
                ///On le push, on le marque et on dit que son parent est notre sommet
                file.push(i);
                parent[i]=sommetTop;
                sommetCouleur[i]=true;
            }
        }
    }
    return (sommetCouleur[puit]==true);
}

///Ford-Fulkerson
double Sommet::fordFulkerson(std::vector<std::vector<int>> matriceAdjacence, int puit, int sommetn, double& flotSommetn)
{
    ///On déclare le flot max
    double flotMax=0;
    ///On crée un vecteur parent pour connaitre l'ordre de parcourt
    std::vector<int> parent((int)matriceAdjacence.size(), -1);

    ///On crée un graphe résiduel = matrice d'adjacence
    std::vector<std::vector<int>> grapheResiduel((int)matriceAdjacence.size(), std::vector<int> ((int)matriceAdjacence.size(), 0));
    for(size_t i=0; i<matriceAdjacence.size(); i++)
        for(size_t j=0; j<matriceAdjacence[i].size(); j++)
            grapheResiduel[i][j]=matriceAdjacence[i][j];

    ///Tant que le bfs renvoie true = on peut renvoyer du flot
    while(bfs(grapheResiduel, puit, parent))
    {
        ///On initialise le chemin du flot à "infini"
        int cheminFlot=RAND_MAX;
        ///On initialise une variable qui va chercher le source
        int chercheSource=puit;

        ///Tant qu'on ne l'a pas trouvé
        while(chercheSource!=m_id)
        {
            ///On parcourt l'ordre
            int predecesseur=parent[chercheSource];
            ///On donne au chemin flot la plus petite valeur en son ancienne et celle du graphe résiduel
            cheminFlot=std::min(cheminFlot, grapheResiduel[predecesseur][chercheSource]);
            ///On change de sommet
            chercheSource=predecesseur;
        }
        ///On réinitialise
        chercheSource=puit;
        ///Tant qu'on a pas trouvé la source
        while(chercheSource!=m_id)
        {
            ///On parcourt l'ordre
            int predecesseur=parent[chercheSource];
            ///On baisse le flot pour l'envoyer autre part
            grapheResiduel[predecesseur][chercheSource]-=cheminFlot;
            ///On l'envoie autre part
            grapheResiduel[chercheSource][predecesseur]+=cheminFlot;
            ///On change de sommet
            chercheSource=predecesseur;
        }
        ///On incrémente le flot max par le flot du chemin
        flotMax+=cheminFlot;
    }

    ///On vérifie si notre sommet intermédiaire est un puit
    int verifPuit=0;
    for(size_t i=0; i<grapheResiduel[sommetn].size(); i++)
            verifPuit+=grapheResiduel[sommetn][i];


    ///Si ca ne l'est pas, on incrémente son flot par le flot qui lui transite
    if(verifPuit!=0)
        for(size_t i=0; i<grapheResiduel.size(); i++)
            flotSommetn+=grapheResiduel[i][sommetn];

    return flotMax;
}
