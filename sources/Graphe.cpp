#include "Graphe.h"
#include <set>
#include <algorithm>
#include <iterator>

///Lecture du fichier via le constructeur de Graphe
Graphe::Graphe(std::string& nomFichier)
{
    std::cout<<"Quel ";
    SetConsoleTextAttribute(texteConsole, 10);
    std::cout<<"fichier";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" voulez-vous ouvrir ? ";
    std::string nom;
    std::ifstream ifs;
    ///Vérification de l'existence du fichier
    do
    {
        SetConsoleTextAttribute(texteConsole, 10);
        std::cin>>nom;
        SetConsoleTextAttribute(texteConsole, 15);
        ifs.open("Graphes/" + nom + ".txt");
        ///S'il n'existe pas, on le mentionne
        if(!ifs)
        {
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<std::endl<<"Fichier inexistant";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<< ", veuillez ";
            SetConsoleTextAttribute(texteConsole, 9);
            std::cout<<"ressaisir";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" un nom : ";
        }
    }
    while(!ifs);
    ///On informe de la lecture du fichier
    SetConsoleTextAttribute(texteConsole, 10);
    std::cout<<std::endl<<"Lecture";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" du fichier ";
    SetConsoleTextAttribute(texteConsole, 10);
    std::cout<<nom;
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<"..."<<std::endl;
    nomFichier=nom;
    ifs>>m_orientation;
    ///On recupere l'odre
    ifs>>m_ordre;
    ///L'indice
    int indice;
    ///Les coordonnées
    double x, y;
    ///On cree un nombre de sommet égal à l'odre du graphe
    for(int i=0; i<m_ordre; ++i)
    {
        ifs>>indice>>nom>>x>>y;
        Sommet* nouv = new Sommet(indice, nom, x, y);
        m_sommets.push_back(nouv);
    }
    ///On recupere la taille du graphe
    ifs>>m_taille;
    ///Les extrémités des arêtes
    int extremite1, extremite2;
    ///On cree un nombre d'arête égal à la taille du graphe
    for(int i=0; i<m_taille; ++i)
    {
        ifs>>indice>>extremite1>>extremite2;
        Arete* nouv = new Arete(indice, m_sommets[extremite1], m_sommets[extremite2]);
        m_aretes.push_back(nouv);
        m_sommets[extremite1]->ajouterAdjacent(m_sommets[extremite2],nouv);
        ///Si le graphe est non orienté, les extrémités ont pour adjacent, leur autre extrémité
        if(!m_orientation)
            m_sommets[extremite2]->ajouterAdjacent(m_sommets[extremite1],nouv);
    }
}

//constructeur par copie
Graphe::Graphe(const Graphe&source): m_sommets{source.m_sommets}, m_aretes{source.m_aretes}, m_orientation{source.m_orientation},
    m_ponderation{source.m_ponderation}, m_ordre{source.m_ordre}, m_taille{source.m_taille}
{
    //maps pour faire correspondre adresse de la source avec adresse de la copie (pour copier valeurs)
    std::map<Sommet*,Sommet*>transpose;
    std::map<Arete*, Arete*>trans_aretes;

    for(size_t i=0; i<source.m_sommets.size(); ++i)
    {
        m_sommets[i]=new Sommet(*source.m_sommets[i]);//on crée copie sommet à partir de son double source (copie en profondeur)
        transpose[source.m_sommets[i]]=m_sommets[i];//associe nvelle adresse de chaque sommet à l'ancienne
    }

    //creations nouvelles aretes avec donnees identiques mais nvx pointeurs
    for(size_t i=0; i<source.m_aretes.size(); ++i)
    {
        m_aretes[i]=new Arete(*source.m_aretes[i]);//construction arete a partir de la source
        trans_aretes[source.m_aretes[i]]=m_aretes[i];
        m_aretes[i]->setExtremites(transpose[source.m_aretes[i]->getExtremites().first], transpose[source.m_aretes[i]->getExtremites().second]);
    }

    //on attribue les adjacents correspondant à partir de la map
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        for(auto j: source.m_sommets[i]->getAdjacents())
        {
            m_sommets[i]->ajouterAdjacent(transpose[j.first], trans_aretes[j.second]);
        }
    }

}

///On désalloue la mémoire
Graphe::~Graphe()
{
    for(auto &i: m_sommets)
        delete i;
    for(auto &i: m_aretes)
        delete i;
}

/*void Graphe::affichage()const
{
    std::cout<<std::endl<<m_orientation<<std::endl<<m_ordre<<std::endl;
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        m_sommets[i]->affichage();
        std::cout << std::endl;
    }
    std::cout<<m_taille<<std::endl;
    for(size_t i=0; i<m_aretes.size(); ++i)
        m_aretes[i]->affichage();
}*/

///Chargement du fichier de pondération
void Graphe::ponderation()
{
    int taille;
    bool verif=false;
    std::cout<<"Quel ";
    SetConsoleTextAttribute(texteConsole, 10);
    std::cout<<"fichier";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" de ";
    SetConsoleTextAttribute(texteConsole, 10);
    std::cout<<"ponderation";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" voulez-vous ouvrir ? ";
    std::string nomFichier;
    std::cin.ignore();
    ///On vérifie si le nom du fichier existe, et s'il est compatible avec le graphe chargé précédemment
    do
    {
        nomFichier="";
        SetConsoleTextAttribute(texteConsole, 10);
        std::getline(std::cin, nomFichier);
        SetConsoleTextAttribute(texteConsole, 15);
        std::ifstream ifs{"Ponderations/" + nomFichier + ".txt"};
        ///Si c'est le cas, on charge la pondération
        if(ifs)
        {
            ifs>>taille;
            if(taille==m_taille)
            {
                verif=true;
                SetConsoleTextAttribute(texteConsole, 10);
                std::cout<<std::endl<<"Lecture";
                SetConsoleTextAttribute(texteConsole, 15);
                std::cout<<" du fichier ";
                SetConsoleTextAttribute(texteConsole, 10);
                std::cout<<nomFichier;
                SetConsoleTextAttribute(texteConsole, 15);
                std::cout<<"..."<<std::endl;
                m_ponderation=true;
                int id;
                double poids;
                for(int i=0; i<taille; ++i)
                {
                    ifs>>id>>poids;
                    m_aretes[id]->setPoids(poids);
                }
            }
            ///Sinon, on informe que le fichier est incompatible et qu'il faut en ressaisir un
            else
            {
                SetConsoleTextAttribute(texteConsole, 12);
                std::cout<<std::endl<<"Fichier incompatible";
                SetConsoleTextAttribute(texteConsole, 15);
                std::cout<<" (nombre d'aretes different), veuillez ";
                SetConsoleTextAttribute(texteConsole, 9);
                std::cout<<"ressaisir";
                SetConsoleTextAttribute(texteConsole, 15);
                std::cout<<" un nom : ";
            }
        }
        ///Si on entre aucun fichier, alors on passe à la suite
        else if(nomFichier!="")
        {
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<std::endl<<"Fichier inexistant";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout << ", veuillez ";
            SetConsoleTextAttribute(texteConsole, 9);
            std::cout<<"ressaisir";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" un nom : ";
        }
    }
    while(nomFichier!="" && !verif);
    ///Si il n'y a pas de fichier, alors il n'y a pas de pondération
    if(nomFichier == "")
    {
        m_ponderation = false;
        for (auto &i : m_aretes)
            i->setPoids(1.0);
    }
}

///fonction dessin svg du graphe
void Graphe::dessiner (std::string nom_fichier, bool indices)
{
    bool even=true;
    std::vector<int> degres;
    std::vector<double> indices_aretes;
    double max_ind=0;
    //affichage sur differents svg suivant les informations
    Svgfile svgout1("SVG/output1.svg", 1400, 650);
    Svgfile svgout2("SVG/output2.svg", 1400, 650);
    Svgfile svgout3("SVG/output3.svg", 1400, 650);
    Svgfile svgout4("SVG/output4.svg", 1400, 650);
    svgout1.addGrid();

    //si les indices pour ce graphe ont deja ete calcules et sauvegardes
    if(indices==true)
    {
        indices_aretes=chargementInterAretes(nom_fichier);//charge les indices d'intermediarite des aretes à partir de sauvegarde
        //on recupere le plus petit indice
        double min_ind=indices_aretes[std::distance(indices_aretes.begin(), std::min_element(indices_aretes.begin(), indices_aretes.end()))];
        //on soustrait l'indice minimal de chaque indice (permet affichage couleurs aretes suivant min et max)
        for(size_t j=0; j<indices_aretes.size(); ++j)
            indices_aretes[j]=indices_aretes[j]-min_ind;
        //on recupere l'indice max parmis les "nouveaux" indices
        max_ind=indices_aretes[std::distance(indices_aretes.begin(), std::max_element(indices_aretes.begin(), indices_aretes.end()))];
    }

    for(size_t j=0; j<m_aretes.size(); ++j)
    {
        m_aretes[j]->dessiner(svgout1, m_orientation, even, m_ponderation, "black");
        //si indices calcules, on affiche les aretes sur tous les svg
        if(indices==true)
        {
            m_aretes[j]->dessiner(svgout2, m_orientation, even, m_ponderation, "black");
            m_aretes[j]->dessiner(svgout3, m_orientation, even, m_ponderation, "black");
            HSL couleur=HSL((indices_aretes[j]/max_ind)*300+60, 0.99f, 0.47f);//calcul couleur en fonction des indices min et max
            RGB color=HSLToRGB(couleur);
            m_aretes[j]->dessiner(svgout4, m_orientation, even, m_ponderation, svgout4.makeRGB((int)color.getR(),(int)color.getG(),(int)color.getB()));
        }

        //even true un tour sur deux(pour eviter superposition indices en oriente)
        if(even==true)
            even=false;
        else
            even=true;
    }

    ///pour chaque indice (centralite degre, intermediarite, vecteur propre et proximite):
    //on recupere valeurs puis on stocke l'indice min, on soustrait à toutes les valeurs puis recherche indice max (comme pour aretes) et dessin
    for(auto i:m_sommets)
        degres.push_back((int)i->getAdjacents().size());
    int min_degre = degres[std::distance(degres.begin(), std::min_element(degres.begin(), degres.end()))];

    for(size_t i=0; i<degres.size(); ++i)
        degres[i]=degres[i]-min_degre;
    int max_degre = degres[std::distance(degres.begin(), std::max_element(degres.begin(), degres.end()))];

    for(size_t i=0; i<m_sommets.size(); ++i)
        m_sommets[i]->dessiner(svgout1, HSL((degres[i]/(double)max_degre)*300+60, 0.99f, 0.47f));

    //affichage dans les autres svg seulement quand indices calcules
    if(indices==true)
    {
        std::vector<std::vector<double>> indicesSommets=chargementIndicesSommets(nom_fichier);//chargement des indicse a partir d'une sauvegarde
        std::vector<double> vecMax;
        std::vector<double> proxMax;
        std::vector<double> interMax;

        for(size_t i=0; i<indicesSommets.size(); ++i)
        {
            vecMax.push_back(indicesSommets[i][0]);
            proxMax.push_back(indicesSommets[i][1]);
            interMax.push_back(indicesSommets[i][2]);
        }

        //trouve min
        double min_vec=vecMax[std::distance(vecMax.begin(),std::min_element(vecMax.begin(), vecMax.end()))];
        double min_prox=proxMax[std::distance(proxMax.begin(),std::min_element(proxMax.begin(), proxMax.end()))];
        double min_inter=interMax[std::distance(interMax.begin(),std::min_element(interMax.begin(), interMax.end()))];

        //recalcul des indices avec min
        for(size_t i=0; i<vecMax.size(); ++i)
        {
            vecMax[i]=vecMax[i]-min_vec;
            proxMax[i]=proxMax[i]-min_prox;
            interMax[i]=interMax[i]-min_inter;
        }

        //trouve max
        double max_vec=vecMax[std::distance(vecMax.begin(),std::max_element(vecMax.begin(), vecMax.end()))];
        double max_prox=proxMax[std::distance(proxMax.begin(),std::max_element(proxMax.begin(), proxMax.end()))];
        double max_inter=interMax[std::distance(interMax.begin(),std::max_element(interMax.begin(), interMax.end()))];

        //dessin avec couleur adequat pour chaque sommet
        for(size_t i=0; i<m_sommets.size(); ++i)
        {
            m_sommets[i]->dessiner(svgout2, HSL((vecMax[i]/max_vec)*300+60, 0.99f, 0.47f));
            m_sommets[i]->dessiner(svgout3, HSL((proxMax[i]/max_prox)*300+60, 0.99f, 0.47f));
            m_sommets[i]->dessiner(svgout4, HSL((interMax[i]/max_inter)*300+60, 0.99f, 0.47f));
        }
    }


    ///legende
    svgout1.addRectangle(10, 610, 190, 30, 5, 5, "white");
    svgout2.addRectangle(10, 610, 190, 30, 5, 5, "white");
    svgout3.addRectangle(10, 610, 190, 30, 5, 5, "white");
    svgout4.addRectangle(10, 610, 190, 30, 5, 5, "white");
    svgout1.addText(20, 630, "indice arete", "darkorchid");
    svgout2.addText(20, 630, "indice arete", "darkorchid");
    svgout3.addText(20, 630, "indice arete", "darkorchid");
    svgout4.addText(20, 630, "indice arete", "darkorchid");
    svgout1.addText(110, 630, "poids arete", "darkgrey");
    svgout2.addText(110, 630, "poids arete", "darkgrey");
    svgout3.addText(110, 630, "poids arete", "darkgrey");
    svgout4.addText(110, 630, "poids arete", "darkgrey");

}

///Indice centralite vecteur propre
std::vector<std::pair<double, double>> Graphe::vecteurPropre()
{
    ///Vecteur qui stocke l'ensemble des indices non normalisés et normalisés
    std::vector<std::pair<double, double>> vecIndices;
    ///Vecteur qui stocke la somme des indices des adjacents de tous les sommets
    std::vector<double> vecSommeIndices;
    ///Variables utilisées pour l'algo
    double lambda1=0, lambda2, sommeIndices=0, sommeSommeIndicesCarre=0;
    ///On initialise tous les indices à 1
    for(size_t i=0; i<m_sommets.size(); ++i)
        vecIndices.push_back({0, 1});
    do
    {
        lambda2=lambda1;
        ///On calcule la somme des indices des voisins pour chaque sommet
        for(size_t i=0; i<m_sommets.size(); ++i)
        {
            for(size_t j=0; j<m_sommets[i]->getAdjacents().size(); ++j)
                sommeIndices+=vecIndices[m_sommets[i]->getAdjacents()[j].first->getId()].second;

            vecSommeIndices.push_back(sommeIndices);
            sommeIndices=0;
        }
        ///On calcule la somme de tous les indices des sommets^2
        for(size_t i=0; i<vecSommeIndices.size(); ++i)
            sommeSommeIndicesCarre+=pow(vecSommeIndices[i], 2);

        ///On fait la racine carré
        lambda1=sqrt(sommeSommeIndicesCarre);
        sommeSommeIndicesCarre=0;
        ///On normalise les indices de tous les sommets
        for(size_t i=0; i<vecIndices.size(); ++i)
        {
            vecIndices[i].first=vecSommeIndices[i];
            if(lambda1!=0)
                vecIndices[i].second=vecIndices[i].first/lambda1;
            else
                vecIndices[i].second=0;
        }
        vecSommeIndices.clear();
    }
    ///Tant que lambda "varie trop"
    while(abs(lambda1-lambda2)>0.01);

    /*std::cout<<std::endl<<"Indices des sommets selon le vecteur propre"<<std::endl;
    for(size_t i=0; i<m_sommets.size(); i++)
            std::cout<<m_sommets[i]->getId()<<" "<<vecIndices[i].first<<" "<<vecIndices[i].second<<std::endl;*/
    return vecIndices;
}


///calcul indice centralite degre
std::vector<std::pair<int, double>> Graphe::centraliteDegre ()
{
    std::vector<std::pair<int, double>> centralite_degres;
    std::pair<int, double> degres;//first est degre non normalisé et second est degré normalisé

    //on recupere le degre de chaque sommet puis on retourne valeur non normalisee et normalisee
    for(auto i: m_sommets)
    {
        degres.first = i->getAdjacents().size();//degre est nbre d'adjacents
        degres.second = ((double)i->getAdjacents().size())/((double)m_ordre-1);
        centralite_degres.push_back(degres);
    }

    return centralite_degres;
}

///recherche des composantes connexes (graphes non orientes)
int Graphe::rechercheCC ()
{
    std::queue<Sommet*> file;
    std::set<Sommet*> marques;
    std::vector<Sommet*> sommets_isoles;
    int compteur=0;

    Sommet*parcours=m_sommets[0];//sommet de depart

    //parcours bfs en marquant sommets decouverts comme appartenant a la meme composante connexe
    //on recommence tant qu'il reste des sommets non decouverts et on compte le nbre de composantes
    do
    {
        //on ajoute sommet parcouru a la file et on l'ajoute aux sommets marques
        file.push(parcours);
        marques.insert(parcours);
        ++compteur;//nvelle composante en cours de decouverte

        //tant qu'il reste des sommets decouverts mais non parcourus dans la file
        while(!file.empty())
        {
            file.pop();//on supprime tete de file

            //on decouvre sommets adjacents que l'on marque
            for(auto i: parcours->getAdjacents())
            {
                if(marques.find(i.first)==marques.end())
                {
                    marques.insert(i.first);
                    file.push(i.first);
                }
            }
            //si il y a encore des sommets dans la file, on recommence avec sommet en tete de file
            if(!file.empty())
                parcours=file.front();
        }

        //si l'on a pas marque tous les sommets du graphe
        if((int)marques.size()<m_ordre)
        {
            //on cherche un sommet qui n'a pas ete marque et on recommence le parcours bfs a partir de lui
            for(auto s: m_sommets)
            {
                if(marques.find(s)==marques.end())
                    parcours=s;
            }
        }
    }
    while((int)marques.size()<m_ordre);//tant qu'on a pas marque tous les sommets

    return compteur;//on retourne nbre composantes connexes
}

//test de la connexite du graphe apres suppression d'une ou plusieurs aretes
void Graphe::testConnexite (int nb)
{
    Graphe copie=*this;
    int cc=0;

    cc=this->rechercheCC();
    if(cc==1)
    {
        std::cout<<std::endl<<"Pour ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"le moment";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", ce graphe est ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"connexe"<<std::endl;
        SetConsoleTextAttribute(texteConsole, 15);
    }
    else
    {
        std::cout<<"Ce graphe est ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"deja non connexe";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", il contient ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<cc<<"composantes connexes"<<std::endl;
        SetConsoleTextAttribute(texteConsole, 15);
    }

    for(int i=0; i<nb; ++i)
        copie.supprimerArete();
    cc= copie.rechercheCC();

    std::cout<<std::endl<<"Le graphe est ";
    SetConsoleTextAttribute(texteConsole, 14);

    if(cc==1)
        std::cout<<"connexe"<<std::endl;

    else
    {
        std::cout<<"non connexe";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<". Il contient ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<cc<<" "<<"composantes connexes"<<std::endl;
    }

    SetConsoleTextAttribute(texteConsole, 15);

    for(auto s: copie.m_sommets)
    {
        if((s->getAdjacents()).size()==0)
        {
            SetConsoleTextAttribute(texteConsole, 14);
            std::cout<<std::endl<<s->getNom();
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" est un sommet ";
            SetConsoleTextAttribute(texteConsole, 14);
            std::cout<<"isole"<<std::endl;
            SetConsoleTextAttribute(texteConsole, 15);
        }
    }

}

void recursifIntermediarite(std::pair<std::unordered_map<Sommet*,unsigned int>,std::unordered_map<Arete*,unsigned int>> &compt, std::pair<Sommet*,Arete*> current,
                            std::unordered_map<Sommet*, std::pair<std::vector<std::pair<Sommet*,Arete*>>,double>> &predecesseurs, std::unordered_map<Sommet*, int> &nombreChemins)
{
    if (predecesseurs.find(current.first) != predecesseurs.end())
    {
        compt.second[current.second]+=nombreChemins[current.first];
        compt.first[current.first]+=nombreChemins[current.first];
        for(auto &j : predecesseurs[current.first].first)
            recursifIntermediarite(compt,j,predecesseurs,nombreChemins);
    }
    else
        ++compt.second[current.second];
}

std::pair<std::vector<std::pair<double,double>>,std::vector<std::pair<Arete*,std::pair<double,double>>>> Graphe::intermediarite()
{
    Sommet* courant;
    double longueur;
    std::vector<std::pair<double,double>> centraliteSommets(m_ordre, {0.0,0.0});
    std::vector<std::pair<Arete*,std::pair<double,double>>> centraliteAretes;
    std::unordered_map<Arete*,double> mapCentraliteAretes;

    auto compare = [](const std::pair<Sommet*,double> s1, const std::pair<Sommet*,double> s2)
    {
        return s1.second > s2.second;
    };

    std::priority_queue<std::pair<Sommet*,double>, std::vector<std::pair<Sommet*,double>>, decltype(compare)> prio(compare);
    for(auto &j : m_sommets)
    {
        std::unordered_map<Sommet*, int> nombreChemins;
        std::unordered_map<Sommet*, std::pair<std::vector<std::pair<Sommet*,Arete*>>,double>> predecesseurs;
        prio.push({j,0.0});

        while(!prio.empty())
        {
            courant = prio.top().first;
            longueur = prio.top().second;
            prio.pop();

            for(auto &i : courant->getAdjacents())
            {
                if(i.first != j)
                {
                    if(nombreChemins.find(i.first) == nombreChemins.end() || (longueur+i.second->getPoids()) < predecesseurs[i.first].second)//ecrase
                    {
                        prio.push({i.first,longueur+i.second->getPoids()});
                        predecesseurs[i.first] = {{{courant,i.second}},i.second->getPoids()+longueur};

                        if(courant == j)
                            nombreChemins[i.first] = 1;
                        else
                            nombreChemins[i.first] = nombreChemins[courant];
                    }
                    else if (longueur+i.second->getPoids() == predecesseurs[i.first].second)//autre chemin court
                    {
                        predecesseurs[i.first].second = i.second->getPoids()+longueur;
                        predecesseurs[i.first].first.push_back({courant,i.second});
                        nombreChemins[i.first] += nombreChemins[courant];
                    }
                }
            }
        }


        for(auto &k : predecesseurs)
            if(m_orientation || k.first->getId() > j->getId())
                for(auto &z : predecesseurs[k.first].first)
                {
                    std::pair<std::unordered_map<Sommet*,unsigned int>,std::unordered_map<Arete*,unsigned int>> compt;

                    recursifIntermediarite(compt,z,predecesseurs,nombreChemins);
                    for(auto &i : compt.first)
                        centraliteSommets[i.first->getId()].first += (double) i.second/nombreChemins[k.first];

                    for(auto &i : compt.second)
                        mapCentraliteAretes[i.first] += (double) i.second/nombreChemins[k.first];
                }
    }

    for(auto &i : centraliteSommets)
    {
        if(m_orientation)
            i.first /= 2.0;

        i.second =i.first *2.0/(double)((m_ordre-1)*(m_ordre-2));
        //std::cout << i.first  << " " << i.second<< std::endl;
    }

    for(auto &i : mapCentraliteAretes)
    {
        centraliteAretes.push_back({i.first,{i.second,i.second/(double)((m_taille - 1)*(m_taille))}});
        //std::cout << "ID " << i.first->getId() << ": " << i.second <<" "<< i.second*(double)2.0/((m_taille-1)*(m_taille-2)) << std::endl;
    }
    for(auto &i : m_aretes)
        if(mapCentraliteAretes.find(i) == mapCentraliteAretes.end())
            centraliteAretes.push_back({i,{0.0,0.0}});

    std::sort(centraliteAretes.begin(), centraliteAretes.end(), [](std::pair<Arete*,std::pair<double,double>> a1, std::pair<Arete*,std::pair<double,double>> a2)
    {
        return a1.first->getId() < a2.first->getId();
    });

    return {centraliteSommets,centraliteAretes};
}

///Indice centralite Proximite
std::vector<std::pair<double, double>> Graphe::vecteurProximite()
{
    ///On initialise toutes les distances à "infini"
    std::vector<double> distance(m_ordre, RAND_MAX);
    ///On copie le sommet courant
    Sommet* sommetCourant;

    ///On stocke les prédecesseurs de chaque sommet
    std::map<Sommet*, std::pair<Sommet*, double>> predecesseur;
    ///On stocke les indices non normalisés et normalisés
    std::vector<std::pair<double, double>> indiceSommets;

    ///On trie les sommets par distance
    auto comparaison=[](const std::pair<Sommet*, double> s1, const std::pair<Sommet*, double> s2)
    {
        return s1.second > s2.second;
    };

    ///Priority queue qui stocke les sommets et leur distance
    std::priority_queue<std::pair<Sommet*, double>, std::vector<std::pair<Sommet*, double>>, decltype(comparaison)> file(comparaison);

    ///On parcours tous les sommets
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        ///On initialise les indices à 0
        indiceSommets.push_back({0, 0});
        ///On push les sommets 1 par 1
        file.push({m_sommets[i], 0});
        ///On initialise la distance
        distance[m_sommets[i]->getId()]=0;
        ///Tant que la file n'est pas vide
        while(!file.empty())
        {
            ///On copie le premier de la file puis on le pop de la file
            sommetCourant=file.top().first;
            file.pop();
            ///On parcours tous ses adjacents
            for(size_t j=0; j<sommetCourant->getAdjacents().size(); j++)
            {
                ///Si la distance du sommet fils > à la distance du sommet parent + poids sommet fils-parent
                if(distance[sommetCourant->getAdjacents()[j].first->getId()] > distance[sommetCourant->getId()] + sommetCourant->getAdjacents()[j].second->getPoids())
                {
                    ///Alors on affecte la nouvelle distance au sommet fils, on le push dans la file et il devient le sucesseur de son parent
                    distance[sommetCourant->getAdjacents()[j].first->getId()] = distance[sommetCourant->getId()] + sommetCourant->getAdjacents()[j].second->getPoids();
                    file.push({sommetCourant->getAdjacents()[j].first, distance[sommetCourant->getAdjacents()[j].first->getId()]});
                    predecesseur[sommetCourant->getAdjacents()[j].first]= {sommetCourant, distance[sommetCourant->getAdjacents()[j].first->getId()]};
                }
            }
        }
        ///On fait la somme des distances (les plus courtes) passant par chaque sommet
        for(size_t j=0; j<m_sommets.size(); j++)
            indiceSommets[i].first+=distance[j];

        ///Si c'est différent de 0, on divise par la distance totale
        if(indiceSommets[i].first!=0)
            indiceSommets[i].first=1/indiceSommets[i].first;

        else
            indiceSommets[i].first=0;

        ///On normalise tous les indices
        indiceSommets[i].second=indiceSommets[i].first*(m_ordre-1);

        ///On réinitialise les variables
        predecesseur.clear();
        distance.clear();
        distance.resize(m_ordre, RAND_MAX);
    }
    /*for(size_t i=0; i<m_sommets.size(); i++)
        std::cout<<m_sommets[i]->getId()<<" "<<indiceSommets[i].first<<" "<<indiceSommets[i].second<<std::endl;*/
    return indiceSommets;
}

///Sauvegarde
void Graphe::sauvegarder(std::vector<std::pair<int, double>> centralite_degres, std::vector<std::pair<double, double>> vecteurPropre, std::vector<std::pair<double, double>> vecteurProximite, std::pair<std::vector<std::pair<double,double>>,std::vector<std::pair<Arete*,std::pair<double,double>>>> intermediarite, std::string nomFichier)
{
    ///Variable qui vérifie si le fichier save existe déja
    bool verif=false;
    ///C'est l'indice qui va apparaître à la fin du fichier save
    int occurence=0;
    ///Nom du fichier sauvegarde
    std::string fichierSauvegarde;
    ///Tant qu'on a pas trouvé de nom valide pour la sauvegarde
    while(!verif)
    {
        ///Nom du fichier sauvegarde = nom du graphe + _save + nombre de fois ou le fichier sauvegarde existe déjà
        fichierSauvegarde=nomFichier + "_save" + std::to_string(occurence);
        ///On test si le fichier save existe déjà
        std::ifstream ifs{"Saves/" + fichierSauvegarde + ".txt"};
        ///Si ce n'est pas le cas, on a trouvé un bon nom fichier pour la save
        if(!ifs)
            verif=true;
        occurence++;
    }
    ///On écrit dans le fichier
    std::ofstream ofs{"Saves/" + fichierSauvegarde + ".txt"};
    ///Si ce n'est pas possible, on le mentionne
    if(!ofs)
    {
        SetConsoleTextAttribute(texteConsole, 12);
        std::cout<<std::endl<<"Ouverture impossible";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", la sauvegarde ne peut avoir lieu"<<std::endl;
    }

    ///On informe du code couleur et des indices présentés
    else
    {
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"Indice";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" de ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"centralite";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" des sommets : ";
        SetConsoleTextAttribute(texteConsole, 3);
        std::cout<<"degre";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", ";
        SetConsoleTextAttribute(texteConsole, 12);
        std::cout<<"vecteur propre";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", ";
        SetConsoleTextAttribute(texteConsole, 11);
        std::cout<<"proximite";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", ";
        SetConsoleTextAttribute(texteConsole, 13);
        std::cout<<"intermediarite";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" :"<<std::endl;

        ///On écrit le résultat dans le fichier save
        for(size_t i=0; i<m_sommets.size(); i++)
        {
            ofs<<m_sommets[i]->getId()<<" "<<centralite_degres[i].first<<" "<<centralite_degres[i].second<<" "
               <<vecteurPropre[i].first<<" "<<vecteurPropre[i].second<<" "
               <<vecteurProximite[i].first<<" "<<vecteurProximite[i].second<<" "
               <<intermediarite.first[i].first<<" "<<intermediarite.first[i].second<<std::endl;

            ///On affiche le résultat de tous les indices pour chaque sommet
            std::cout<<std::endl<<"Sommet "<<m_sommets[i]->getId()<<" : "<<std::endl;
            SetConsoleTextAttribute(texteConsole, 3);
            std::cout<<"   "<<centralite_degres[i].first<<" "<<centralite_degres[i].second<<std::endl;
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<"   "<<vecteurPropre[i].first<<" "<<vecteurPropre[i].second<<std::endl;
            SetConsoleTextAttribute(texteConsole, 11);
            std::cout<<"   "<<vecteurProximite[i].first<<" "<<vecteurProximite[i].second<<std::endl;
            SetConsoleTextAttribute(texteConsole, 13);
            std::cout<<"   "<<intermediarite.first[i].first<<" "<<intermediarite.first[i].second<<std::endl;
            SetConsoleTextAttribute(texteConsole, 15);
        }
        ///On affiche l'indice de centralité des arêtes
        std::cout << std::endl << "Indice de ";
        SetConsoleTextAttribute(texteConsole, 13);
        std::cout<<"centralite intermediaire";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" des ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"aretes";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" : "<< std::endl << std::endl;
        ofs << std::endl << "aretes" <<std::endl;
        ///On sauvegarde l'indice de centralité des aretes dans le fichier
        for(auto &i : intermediarite.second)
        {
            ofs << i.first->getId() << " " << i.second.first << " " << i.second.second << std::endl;
            std::cout << "Arete "<<i.first->getId() << " : " << " " << i.second.first << " " << i.second.second << std::endl;
        }
        SetConsoleTextAttribute(texteConsole, 10);
        std::cout<<std::endl<<"Sauvegarde";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" du fichier ";
        SetConsoleTextAttribute(texteConsole, 10);
        std::cout<<nomFichier;
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<"..."<<std::endl;
    }
}


void Graphe::supprimerArete ()
{
    int indice=0;
    std::set<int> indices;

    for(auto a: m_aretes)
    {
        indices.insert(a->getId());
    }

    std::cout<<std::endl<<"Saisissez l'";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"indice";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" de l'";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"arete";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" a ";
    SetConsoleTextAttribute(texteConsole, 12);
    std::cout<<"supprimer";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" : ";

    do
    {
        std::cin>>indice;
        if(indices.find(indice)==indices.end())
        {
            std::cout<<std::endl<<"Cet ";
            SetConsoleTextAttribute(texteConsole, 14);
            std::cout<<"indice";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" est ";
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<"invalide";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<", veuillez le ";
            SetConsoleTextAttribute(texteConsole, 9);
            std::cout<<"ressaisir";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" : ";
        }
    }
    while(indices.find(indice)==indices.end());

    this->SuppAreteI(indice);

}

void Graphe::SuppAreteI (int indice)
{
    std::pair<Sommet*, Sommet*> extremites;

    for(auto i: m_aretes)
    {
        if(i->getId()==indice)
            extremites = i->getExtremites();
    }

    extremites.first->suppAdjacent(extremites.second);

    if(!m_orientation)
        extremites.second->suppAdjacent(extremites.first);

    for(size_t i=0; i<m_aretes.size(); ++i)
    {
        if(m_aretes[i]->getId()==indice)
        {
            delete m_aretes[i];
            m_aretes.erase(m_aretes.begin()+i);
        }

    }
    --m_taille;
}

void Graphe::supprimerSommet (Sommet*s)
{

    for(auto i: s->getAdjacents())
    {
        for(auto j: m_sommets)
        {
            if(i.first == j)
            {
                j->suppAdjacent(s);
            }
        }
    }

    int i=0;
    do
    {
        for(size_t j=0; j<m_aretes.size(); ++j)
        {
            ///
            if((m_aretes[j]->getExtremites().first->getId()==s->getId())||(m_aretes[j]->getExtremites().second->getId()==s->getId()))
            {
                m_aretes.erase(m_aretes.begin()+j);
                --m_taille;
                ++i;
                break;
            }
        }

    }
    while(i<(int)s->getAdjacents().size());


    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        ///
        if(m_sommets[i]->getId()==s->getId())
        {
            m_sommets.erase(m_sommets.begin()+i);
            --m_ordre;
        }
    }
}

void Graphe::kSommetsConnexite ()
{
    std::vector<int>nb;
    std::vector<std::unordered_set<int>> chemins_p;
    int compteur=1, h=0;
    std::vector <int> tempo;
    std::unordered_set<int>actuel;
    std::unordered_set<int> tempo2;
    std::unordered_set<int> suivant;
    std::vector<int> reference_s;

    std::map<std::pair<Sommet*,Sommet*>,std::vector<std::unordered_set<int>>> chemins = tousLesChemins();
    for(auto&i: m_sommets)
    {
        for(auto&j: m_sommets)
        {
            if((m_orientation && j!=i) || j->getId() > i->getId())
            {
                chemins_p=chemins[ {i,j}];
                //std::cout<<"PAIRE"<<std::endl;
                std::sort(chemins_p.begin(), chemins_p.end(), [](std::unordered_set<int> a, std::unordered_set<int> b)
                {
                    return a.size()<b.size();
                } );
                for(size_t y=0; y<chemins_p.size(); ++y)
                {
                    //std::cout<<"Comparaison"<<std::endl;
                    actuel=chemins_p[y];
                    if(y+1<chemins_p.size())
                    {
                        suivant=chemins_p[y+1];
                        for(auto a: m_aretes)
                        {
                            if(suivant.find(a->getId())!=suivant.end())
                            {
                                if(a->getExtremites().first!=i && a->getExtremites().first!=j)
                                    tempo.push_back(a->getExtremites().first->getId());
                                if(a->getExtremites().second!=i && a->getExtremites().second!=j)
                                    tempo.push_back(a->getExtremites().second->getId());

                            }
                            if(actuel.find(a->getId())!=actuel.end())
                            {
                                if(a->getExtremites().first!=i && a->getExtremites().first!=j)
                                    tempo2.insert(a->getExtremites().first->getId());
                                if(a->getExtremites().second!=i && a->getExtremites().second!=j)
                                    tempo2.insert(a->getExtremites().second->getId());

                            }
                        }

                        for(auto&l: tempo)
                        {
//                                std::cout<<l<<"  ";
                            if(tempo2.find(l)!=tempo2.end())
                            {
                                h=1;
                                chemins_p[y+1]=chemins_p[y];
                            }
                        }
//                        std::cout<<std::endl;
//                        for(auto&l: tempo2)
//                        {
//                                std::cout<<l<<"  ";
//                        }
//                        std::cout<<std::endl;

                        if(h==0)
                        {
                            ++compteur;
                            //tempo2.insert(tempo.begin(),tempo.end());
                        }

                        h=0;
                        tempo.clear();
                        tempo2.clear();
                    }

                }
                //std::cout<<"cpt: "<<compteur<<std::endl;
                nb.push_back(compteur);
                compteur=1;
                chemins_p.clear();
            }

        }
    }

    std::sort(nb.begin(), nb.end(), [](int a, int b)
    {
        return a < b;
    });
    std::cout<<std::endl<<"Ce graphe est ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<nb[0]<<"-sommet(s)";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" connexe"<<std::endl;
}


void Graphe::comparaisonIndices(int nb)
{
    Graphe copie=*this;

    std::vector<std::pair<int, double>> centralite_degres1 = centraliteDegre ();
    std::vector<std::pair<double, double>> vecteurPropre1=vecteurPropre();
    std::vector<std::pair<double, double>> vecteurProximite1=vecteurProximite();
    std::pair<std::vector<std::pair<double,double>>,std::vector<std::pair<Arete*,std::pair<double,double>>>> intermediarite1=intermediarite();

    for(int i=0; i<nb; ++i)
        copie.supprimerArete();

    std::vector<std::pair<int, double>> centralite_degres2 = copie.centraliteDegre ();
    std::vector<std::pair<double, double>> vecteurPropre2=copie.vecteurPropre();
    std::vector<std::pair<double, double>> vecteurProximite2=copie.vecteurProximite();
    std::pair<std::vector<std::pair<double,double>>,std::vector<std::pair<Arete*,std::pair<double,double>>>> intermediarite2=copie.intermediarite();

    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<std::endl<<"Difference";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" des ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"indices";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" de ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"centralite";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<", ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"avant";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" et ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"apres";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" la ";
    SetConsoleTextAttribute(texteConsole, 12);
    std::cout<<"suppression";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" d'arete(s)"<<std::endl;

    for(size_t i=0; i<copie.m_sommets.size(); ++i)
    {
        std::cout<<std::endl<<"Sommet "<<m_sommets[i]->getId()<<" : "<<std::endl;

        SetConsoleTextAttribute(texteConsole, 3);
        std::cout<<"   Degre: ";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<centralite_degres1[i].first-centralite_degres2[i].first<<" ";
        std::cout<<centralite_degres1[i].second-centralite_degres2[i].second<<" ";
        SetConsoleTextAttribute(texteConsole, 12);
        std::cout<<std::endl<<"   Vecteur Propre: ";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<vecteurPropre1[i].first-vecteurPropre2[i].first<<" ";
        std::cout<<vecteurPropre1[i].second-vecteurPropre2[i].second<<" ";
        SetConsoleTextAttribute(texteConsole, 11);
        std::cout<<std::endl<<"   Proximite: ";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<vecteurProximite1[i].first-vecteurProximite2[i].first<<" ";
        std::cout<<vecteurProximite1[i].second-vecteurProximite2[i].second<<" ";
        SetConsoleTextAttribute(texteConsole, 13);
        std::cout<<std::endl<<"   Intemediarite: ";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<intermediarite1.first[i].first-intermediarite2.first[i].first<<" ";
        std::cout<<intermediarite1.first[i].second-intermediarite2.first[i].second<<std::endl;
    }
}

void recursifTousLesChemins (std::vector<std::unordered_set<int>> &commun,std::pair<std::unordered_set<Sommet*>,std::unordered_set<int>> cheminUnique, std::pair<Sommet*,Arete*> current, std::pair<Sommet*,Sommet*> &debFin)
{
    if(current.first != debFin.first && cheminUnique.first.find(current.first) == cheminUnique.first.end() && cheminUnique.second.find(current.second->getId()) == cheminUnique.second.end() )
    {
        cheminUnique.second.insert(current.second->getId());
        cheminUnique.first.insert(current.first);
        if (current.first == debFin.second)
            commun.push_back(cheminUnique.second);
        else

            for (auto i : current.first->getAdjacents())
                recursifTousLesChemins(commun, cheminUnique,i,debFin);
    }
}

std::map<std::pair<Sommet*,Sommet*>,std::vector<std::unordered_set<int>>> Graphe::tousLesChemins()
{
    std::map<std::pair<Sommet*,Sommet*>,std::vector<std::unordered_set<int>>> chemins;
    for(auto &i : m_sommets)
        for(auto &j : m_sommets)
            if((m_orientation && j!=i)|| (m_orientation && j->getId() > i->getId()))
            {
                std::pair<Sommet*,Sommet*> debFin = {i,j};
                std::vector<std::unordered_set<int>> commun;

                for(auto &l : i->getAdjacents())
                {
                    std::pair<std::unordered_set<Sommet*>,std::unordered_set<int>> cheminUnique;
                    recursifTousLesChemins(commun,cheminUnique,l,debFin);
                }
                chemins[ {i,j}] = commun;
                //std::cout << "(" << i->getNom() << "," << j->getNom() << ") " << chemins[ {i,j}].size() << " chemins" << std::endl;
            }

    return chemins;
}


void Graphe::kAretesConnexe ()
{
    std::vector<int>nb;
    std::vector<std::unordered_set<int>> chemins_p;
    int compteur=1, h=0;
    std::vector <int> tempo;
    //std::vector <int> tempo2;
    std::unordered_set<int> suivant;

    std::map<std::pair<Sommet*,Sommet*>,std::vector<std::unordered_set<int>>> chemins = tousLesChemins();
    for(auto&i: m_sommets)
    {
        for(auto&j: m_sommets)
        {
            if((m_orientation && j!=i)|| (m_orientation && j->getId() > i->getId()))
            {
                chemins_p=chemins[ {i,j}];
                //std::cout<<"PAIRE"<<std::endl;
                std::sort(chemins_p.begin(), chemins_p.end(), [](std::unordered_set<int> a, std::unordered_set<int> b)
                {
                    return a.size()<b.size();
                } );
                for(size_t y=0; y<chemins_p.size(); ++y)
                {
                    //std::cout<<"Comparaison"<<std::endl;
                    tempo= {chemins_p[y].begin(), chemins_p[y].end()};
                    if(y+1<chemins_p.size())
                    {
                        suivant= {chemins_p[y+1].begin(), chemins_p[y+1].end()};
                        //tempo2={chemins_p[y+1].begin(), chemins_p[y+1].end()};
                        for(auto&l: tempo)
                        {
                            //std::cout<<l<<"  ";
                            if(suivant.find(l)!=suivant.end())
                            {
                                h=1;
                                chemins_p[y+1]=chemins_p[y];
                            }
                        }
//                        std::cout<<std::endl;
//                        for(auto&l: tempo2)
//                        {
//                                std::cout<<l<<"  ";
//                        }
//                        std::cout<<std::endl;

                        if(h==0)
                        {
                            ++compteur;
                            chemins_p[y+1].insert(chemins_p[y].begin(),chemins_p[y].end());
                        }
                        h=0;
                    }

                }
                //std::cout<<"cpt: "<<compteur<<std::endl;
                nb.push_back(compteur);
                compteur=1;
                chemins_p.clear();
            }

        }
    }

    std::sort(nb.begin(), nb.end(), [](int a, int b)
    {
        return a < b;
    });
    std::cout<<std::endl<<"Ce graphe est ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<nb[0]<<"-arete(s)";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" connexe"<<std::endl;
}

///On test la forte connexité d'un graphe orienté
void Graphe::testForteConnexite(int nb)
{
    ///On fait une copie profonde
    Graphe copie=*this;

    ///On supprime les aretes sélectionnées par l'utilisateur
    for(int i=0; i<nb; ++i)
        copie.supprimerArete();

    ///Vecteur qui permet de marqué les sommets
    std::vector<bool> sommetCouleur(m_ordre, false);
    ///Vecteur qui permet de donner l'ordre de post découverte des sommets
    std::vector<int> ordreSommet;

    ///On fait un premier DFS
    for(size_t i=0; i<copie.m_sommets.size(); i++)
    {
        if(sommetCouleur[i]==false)
            copie.m_sommets[i]->dfs(sommetCouleur, ordreSommet);
    }

    ///On renverse l'ordre de post découverte
    std::reverse(ordreSommet.begin(), ordreSommet.end());
    sommetCouleur.clear();
    sommetCouleur.resize(m_ordre, false);

    ///On crée un vecteur qui va inverser le sens des arêtes
    std::vector<std::vector<int>> reverseAdjacents(m_ordre);

    ///Pour tous les sommets, et tous les adjacents de ces sommets, on inverses l'orientation des arêtes
    for(size_t i=0; i<copie.m_sommets.size(); i++)
    {
        for(size_t j=0; j<copie.m_sommets[i]->getAdjacents().size(); j++)
            reverseAdjacents[copie.m_sommets[i]->getAdjacents()[j].first->getId()].push_back(copie.m_sommets[i]->getId());
    }

    ///Vecteur qui stocke une composante fortement connexe
    std::vector<int> composanteFortementConnexe;
    ///Vecteur qui stocke les différentes composantes fortement connexes
    std::vector<std::vector<int>> composantesFortementConnexes;

    ///On parcours tous les sommets dans l'odre inverse de post découverte
    for(size_t i=0; i<ordreSommet.size(); i++)
    {
        ///S'il n'a pas été marqué, on lance un DFS (orientation inversée), et on push chaque composante fortement connexe dans un vecteur
        if(sommetCouleur[ordreSommet[i]]==false)
        {
            copie.m_sommets[ordreSommet[i]]->dfsReverse(sommetCouleur, reverseAdjacents, composanteFortementConnexe, getSommets());
            composantesFortementConnexes.push_back(composanteFortementConnexe);
            composanteFortementConnexe.clear();
        }
    }

    ///On affiche le nb de composantes fortement connexes
    std::cout<<std::endl<<"Il y a ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<(int)composantesFortementConnexes.size()<<" composante(s) fortement connexe(s)";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" : "<<std::endl;
    ///On affiche les sommets qui composent chaque composante fortement connexe
    for(size_t i=0; i<composantesFortementConnexes.size(); i++)
    {
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<std::endl<<i+1<<" : ";
        SetConsoleTextAttribute(texteConsole, 15);
        for(size_t j=0; j<composantesFortementConnexes[i].size(); j++)
            std::cout<<composantesFortementConnexes[i][j]<<" ";
    }
    std::cout<<std::endl;
}


///On cree une matrice d'adjacence
std::vector<std::vector<int>> Graphe::creationMatriceAdjacence()
{
    ///On cree une matrice
    std::vector<std::vector<int>> matriceAdjacence(m_ordre, std::vector<int>(m_ordre, 0));
    ///On le poids de chaque arête dans la case qui lui est propre
    //for(size_t i=0; i<m_aretes.size(); i++)
    //matriceAdjacence[m_aretes[i]->getExtremites().first->getId()][m_aretes[i]->getExtremites().second->getId()]=m_aretes[i]->getPoids();

    for(size_t i=0; i<m_sommets.size(); i++)
        for(size_t j=0; j<m_sommets[i]->getAdjacents().size(); j++)
            matriceAdjacence[m_sommets[i]->getId()]
            [m_sommets[i]->getAdjacents()[j].first->getId()]
                =m_sommets[i]->getAdjacents()[j].second->getPoids();

    return matriceAdjacence;
}


std::vector<std::vector<double>> Graphe::chargementIndicesSommets(std::string nomFichier)
{
    std::vector<std::vector<double>> indices;
    bool verif=false;
    int occurence=0;
    std::string fichierSauvegarde;
    double inutile1, inutile2, indice, donnees=0;
    std::vector<double> tempo;

    while(!verif)
    {
        fichierSauvegarde=nomFichier + "_save" + std::to_string(occurence);
        std::ifstream ifs{ "Saves/" + fichierSauvegarde + ".txt"};
        if(!ifs)
        {
            verif=true;
            --occurence;
        }
        else
            ++occurence;
    }
    fichierSauvegarde=nomFichier + "_save" + std::to_string(occurence);
    std::ifstream ifs{ "Saves/" + fichierSauvegarde + ".txt"};
    if(!ifs)
    {
        SetConsoleTextAttribute(texteConsole, 12);
        std::cout<<std::endl<<"Ouverture impossible";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", le chargement ne peut avoir lieu"<<std::endl;
    }
    else
    {
        for(size_t i=0; i<m_sommets.size(); ++i)
        {
            ifs>>indice>>inutile1>>inutile2>>donnees;
            tempo.push_back(donnees);
            ifs>>inutile1>>donnees;
            tempo.push_back(donnees);
            ifs>>inutile1>>donnees>>inutile2;
            tempo.push_back(donnees);
            indices.push_back(tempo);
            tempo.clear();
        }

    }

    return indices;

}

std::vector<double> Graphe::chargementInterAretes (std::string nom_fichier)
{
    bool verif=false;
    int occurence=0;
    std::string fichierSauvegarde;
    double indice, donnees=0;
    std::vector<double> tempo;

    while(!verif)
    {
        fichierSauvegarde=nom_fichier + "_save" + std::to_string(occurence);
        std::ifstream ifs{ "Saves/" + fichierSauvegarde + ".txt"};
        if(!ifs)
        {
            verif=true;
            --occurence;
        }
        else
            ++occurence;
    }
    fichierSauvegarde=nom_fichier + "_save" + std::to_string(occurence);
    std::ifstream ifs{"Saves/" + fichierSauvegarde + ".txt"};
    if(!ifs)
    {
        SetConsoleTextAttribute(texteConsole, 12);
        std::cout<<std::endl<<"Ouverture impossible";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", le chargement ne peut avoir lieu"<<std::endl;
    }
    else
    {
        std::string ligne;
        do
        {
            std::getline(ifs, ligne);
        }
        while(ligne!="aretes");

        for(size_t i=0; i<m_aretes.size(); ++i)
        {
            ifs>>indice>>donnees>>ligne;
            //std::cout<<"DONNEES : "<<donnees<<std::endl;
            tempo.push_back(donnees);
        }


    }

    return tempo;
}

///On calcule l'indice de centralite intermédiaire avec les flots
std::vector<double> Graphe::intermediariteFlots()
{
    ///Vecteur qui stock l'indice de centralite de chaque sommet
    std::vector<double> flotsMax(m_sommets.size(), 0);
    ///On parcourt toutes les futures sources
    for(size_t i=0; i<m_sommets.size(); i++)
    {
        ///On parcourt tous les futurs puits
        for(size_t j=0; j<m_sommets.size(); j++)
        {
            ///Si le graphe est non orienté, on parcourt une seule fois chaque chemin
            if((m_orientation && m_sommets[i]!=m_sommets[j]) || m_sommets[i]->getId() < m_sommets[j]->getId())
            {
                ///Copie profonde
                Graphe b(*this);
                ///On crée un puit (ne possède aucun adjacent)
                b.m_sommets[j]->supprAllAdjacents();
                ///On reparcourt tous les sommets
                for(size_t k=0; k<m_sommets.size(); k++)
                {
                    ///Ses adjacents
                    for(size_t m=0; m<m_sommets[k]->getAdjacents().size(); m++)
                    {
                        ///Pour créer une source (il n'est l'adjacent de personne !)
                        if(m_sommets[i]==m_sommets[k]->getAdjacents()[m].first)
                            b.m_sommets[k]->suppAdjacent(b.m_sommets[i]);
                    }
                }
                ///On parcourt les sommets à étudier (flot transitant par eux)
                for(size_t n=0; n<m_sommets.size(); n++)
                {
                    ///Si il ne s'agit ni de la source, ni du puit
                    if(m_sommets[n]->getId()!=m_sommets[i]->getId() && m_sommets[n]->getId()!=m_sommets[j]->getId())
                    {
                        ///On crée une matrice d'adjacence
                        std::vector<std::vector<int>> matriceAdjacence=b.creationMatriceAdjacence();

                        ///Variable qui stocke le flot transitant par un sommet
                        double flotSommetn=0;
                        ///On lance Ford-Fulkerson pour trouver le flot MAX du graphe
                        double flotMax=b.m_sommets[i]->fordFulkerson(matriceAdjacence, b.m_sommets[j]->getId(), m_sommets[n]->getId(), flotSommetn, b.m_sommets);
                        ///Si le graphe est non orienté, on calcul l'indice de centralité intermédiaire du sommet n
                        if(flotMax!=0 && !m_orientation)
                            flotsMax[n]+=flotSommetn/flotMax;
                        ///Sinon, on le calcul en le divisant par 2
                        else if(flotMax!=0)
                            flotsMax[n]+=flotSommetn/(2*flotMax);
                    }
                }
            }
        }
    }
    return flotsMax;
}

///On compare les indices de centralité intermédiaire avant et après la suppression d'un ou plusieurs troncon(s) (flot)
void Graphe::comparaisonICIFlots(std::vector<double> flotAvant, std::vector<double>& flotApres, std::string saisie)
{
    char option='0';
    ///On demande à l'utilisateur s'il veut supprimer des troncons
    SetConsoleTextAttribute(texteConsole, 12);
    std::cout<<"Supprimer";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" des ";
    SetConsoleTextAttribute(texteConsole, 12);
    std::cout<<"troncons";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" pour ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"etudier";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" la ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"repartition";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" du flot ?     1 : OUI     2 : NON"<<std::endl<<std::endl;

    ///On blinde la saisie
    do
    {
        SetConsoleTextAttribute(texteConsole, 3);
        std::cout<<"> ";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cin>>saisie;
        if(saisie.length()>1)
        {
            option='d';
        }
        else
            option=saisie.front();

        ///On lui demande de ressaisir si sa saisie est invalide
        if(option!='1' && option!='2')
        {
            std::cout<<std::endl<<"Cette option est ";
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<"invalide";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<", veuillez ";
            SetConsoleTextAttribute(texteConsole, 9);
            std::cout<<"ressaisir";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" : ";
        }
    }
    while(option!='1' && option!='2');

    ///S'il veut supprimer des arêtes, on lui demande combien
    if(option=='1')
    {
        std::cout<<std::endl<<"Combien d'";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"aretes";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" voulez vous ";
        SetConsoleTextAttribute(texteConsole, 12);
        std::cout<<"supprimer";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" ? ";

        int nb;

        ///On blinde la saisie par rapport à la taille du graphe
        do
        {
            std::cin>>nb;
            ///On lui informe que sa saisie est invalide
            if((nb<0)||(nb>m_taille))
            {
                std::cout<<std::endl<<"Vous avez saisie un nombre ";
                SetConsoleTextAttribute(texteConsole, 12);
                std::cout<<"trop important";
                SetConsoleTextAttribute(texteConsole, 15);
                std::cout<<" ou ";
                SetConsoleTextAttribute(texteConsole, 12);
                std::cout<<"negatif";
                SetConsoleTextAttribute(texteConsole, 15);
                std::cout<<", veuillez ";
                SetConsoleTextAttribute(texteConsole, 9);
                std::cout<<"ressaisir";
                SetConsoleTextAttribute(texteConsole, 15);
                std::cout<<" : ";
            }
        }
        while((nb<0)||(nb>m_taille));

        ///Copie profonde du graphe pour supprimer les arêtes
        Graphe copie(*this);

        ///On supprime les arêtes choisis par l'utilisateur
        for(int i=0; i<nb; ++i)
            copie.supprimerArete();

        ///On calcul l'indice avec les flots après suppression de troncon(s)
        flotApres=copie.intermediariteFlots();

        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<std::endl<<"Difference";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" des ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"indices";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" de ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"centralite";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"avant";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" et ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"apres";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" la ";
        SetConsoleTextAttribute(texteConsole, 12);
        std::cout<<"suppression";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" de troncon(s)"<<std::endl<<std::endl;
    }

    else
    {
        std::cout<<std::endl<<"Indices de ";
        SetConsoleTextAttribute(texteConsole, 13);
        std::cout<<"centralite intermediaire";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" avec le flot : "<<std::endl<<std::endl;
    }

    ///On affiche la différence des indices de centralité avant et après la suppression de troncon(s)
    for(size_t i=0; i<m_sommets.size(); i++)
        std::cout<<"Sommet "<<i<<" : "<<flotAvant[i]-flotApres[i]<<std::endl;

}
