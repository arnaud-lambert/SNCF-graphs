#include "Arete.h"
#include "svg/svgfile.h"

Sommet::Sommet(int id, std::string nom, double x, double y)
    :m_id{id}, m_nom{nom}, m_x{x}, m_y{y}
{

}

///constructeur par copie de sommet
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

///fonction dessin svg: affiche nom et sommet avec couleur generee en fonction des indices
void Sommet::dessiner(Svgfile&svgout, HSL couleur)
{
    RGB color = HSLToRGB(couleur);//on cr�e une couleur de type rgb � partir de l'hsl
    std::string cooouleur= svgout.makeRGB((int)color.getR(),(int)color.getG(),(int)color.getB());
    svgout.addDisk( m_x*100, m_y*100, 5, cooouleur);
    svgout.addText( m_x*100 - 5, m_y*100 - 10, m_nom, "black" );
}

///fonction suppression d'un adjacent envoye en parametre
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
        ///S'ils ne sont pas marqu�s, on fait un DFS avec eux
        if(sommetCouleur[m_adjacents[i].first->m_id]==false)
            m_adjacents[i].first->dfs(sommetCouleur, ordreSommet);
    }
    ///On range les sommets en fonction de leur ordre de post d�couverte
    ordreSommet.push_back(m_id);
}

///DFS avec l'orientation invers�
void Sommet::dfsReverse(std::vector<bool>& sommetCouleur, std::vector<std::vector<int>> reverseAdjacents, std::vector<int>& composanteFortementConnexe, std::vector<Sommet*> m_sommets)
{
    ///On marque le sommet
    sommetCouleur[m_id]=true;

    ///On parcourt ses "nouveaux" adjacents
    for(size_t i=0; i<reverseAdjacents[m_id].size(); i++)
    {
        ///S'ils ne sont pas amrqu�, on fait un reverse DFS avec eux
        if(sommetCouleur[reverseAdjacents[m_id][i]]==false)
            m_sommets[reverseAdjacents[m_id][i]]->dfsReverse(sommetCouleur, reverseAdjacents, composanteFortementConnexe, m_sommets);
    }
    ///On range les sommets par ordre de post d�couverte
    composanteFortementConnexe.push_back(m_id);
}

///BFS
bool Sommet::bfs(std::vector<std::vector<int>> grapheResiduel, int puit, std::vector<int>& parent)
{
    ///On met le parent du sommet � -1
    parent[m_id]=-1;
    ///On cr�e un vecteur pour marqu� nos sommets
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
            ///On v�rifie dans le graphe r�siduel si l'adjacent de notre sommet est marqu�
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

///On surpprime tous les adjacents d'un sommet
void Sommet::supprAllAdjacents()
{
    m_adjacents.clear();
}

///Ford-Fulkerson
double Sommet::fordFulkerson(std::vector<std::vector<int>> matriceAdjacence, int puit, int sommetn, double& flotSommetn, std::vector<Sommet*> m_sommets)
{
    ///On d�clare le flot max
    double flotMax=0;
    ///On cr�e un vecteur parent pour connaitre l'ordre de parcourt
    std::vector<int> parent((int)matriceAdjacence.size(), -1);

    ///On cr�e un graphe r�siduel = matrice d'adjacence
    std::vector<std::vector<int>> grapheResiduel((int)matriceAdjacence.size(), std::vector<int> ((int)matriceAdjacence.size(), 0));
    for(size_t i=0; i<matriceAdjacence.size(); i++)
        for(size_t j=0; j<matriceAdjacence[i].size(); j++)
            grapheResiduel[i][j]=matriceAdjacence[i][j];

    ///Tant que le bfs renvoie true = on peut renvoyer du flot
    while(bfs(grapheResiduel, puit, parent))
    {
        ///On initialise le chemin du flot � "infini"
        int cheminFlot=RAND_MAX;
        ///On initialise une variable qui va chercher le source
        int chercheSource=puit;

        ///Tant qu'on ne l'a pas trouv�
        while(chercheSource!=m_id)
        {
            ///On parcourt l'ordre
            int predecesseur=parent[chercheSource];
            ///On donne au chemin flot la plus petite valeur en son ancienne et celle du graphe r�siduel
            cheminFlot=std::min(cheminFlot, grapheResiduel[predecesseur][chercheSource]);
            ///On change de sommet
            chercheSource=predecesseur;
        }
        ///On r�initialise
        chercheSource=puit;
        ///Tant qu'on a pas trouv� la source
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
        ///On incr�mente le flot max par le flot du chemin
        flotMax+=cheminFlot;
    }

    ///On test si le sommet est isol�
    std::vector<int> verifIsole((int)matriceAdjacence.size(), 0);
    ///On parcourt tous les sommets, et si un n'est parent de personne, alors il est isol�
    for(size_t j=0; j<m_sommets.size(); j++)
            for(size_t k=0; k<parent.size(); k++)
                if(m_sommets[j]->getId()==parent[k])
                    verifIsole[j]+=1;


    ///On v�rifie si le sommet a un lien de parent� (m�me tr�s �loign�) avec la source
    std::vector<bool> verifChemin((int)matriceAdjacence.size(), false);
    ///On parcourt tous les sommets et on remonte l'arbre g�n�alogie tant qu'on est pas arriv� au tout premier
    for(size_t i=0; i<m_sommets.size(); i++)
    {
        int actuel=m_sommets[i]->getId();
        while(parent[actuel]!=-1)
        {
            ///Si dans son arbre il poss�de la source, qu'il n'est pas isol� et son parent le plus proche n'est pas le sommet en cours d'�tude
            if(parent[actuel]==m_id && verifIsole[m_sommets[i]->getId()]>=1 && sommetn!=parent[m_sommets[i]->getId()])
                ///Alors on consid�re qu'il est un vecteur de transmission de flot pour notre sommet �tudi�
                verifChemin[m_sommets[i]->getId()]=true;
            ///On remonte l'arbre g�n�alogique
            actuel=parent[actuel];
        }
    }
    ///On dit que la source est un vecteur de transmission de flot
    verifChemin[m_id]=true;

    ///On v�rifie si notre sommet interm�diaire est un puit
    int verifPuit=0;
    for(size_t i=0; i<grapheResiduel[sommetn].size(); i++)
        verifPuit+=grapheResiduel[sommetn][i];


    ///Si ca ne l'est pas, on incr�mente son flot par le flot qui lui transite
    if(verifPuit!=0)
        for(size_t i=0; i<grapheResiduel[sommetn].size(); i++)
        {
            ///On regarde �galement si le sommet de transmission de flot est valid�
            if(verifChemin[i])
                ///On fait la diff�rence entre le graphe r�siduel et la matrice d'adjacence
                flotSommetn+=grapheResiduel[sommetn][i]-matriceAdjacence[sommetn][i];
        }
    return flotMax;
}
