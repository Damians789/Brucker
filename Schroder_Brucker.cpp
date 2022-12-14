#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <stdexcept>
#include <memory>

using namespace std;


class wezel{
    int nr;
    int czas; //wprowadzony czas wykonania
    int root; //zmodyfikowany czas wykonania
    int real_czas; //faktyczny czas wykonania
    int m;
    int l; //opoznienie
    int stan; //0 - niewykoanne, 1 - wykonane
    vector <int> a_list;
    vector <wezel*> prev;
    vector <wezel*> next;
    vector <wezel*> nodes;
    int lmax, cmax;

    public:
    wezel(){}
    wezel(int n){
        nodes.reserve(n);
        fstream f("input.txt", ios::in);
        for (int i=0;i<n;i++) {
            wezel *w = new wezel;
            w->nr = i+1;
            w->root = 0;
            w->stan = 0;
            int x;
            if(f >> x){
                w->czas = x;
            }
            nodes.push_back(w);
        }
        f.close();
    }


    inline int rozmiar(){return nodes.size();}


    bool cykl(int id, wezel *node) {
        if (node->nr == nr) return true;
        for (vector<wezel*>::iterator it = node->next.begin(); it != node->next.end(); ++it) {
            if (cykl(id,*it)) return true;
        }
        return false;
    }

    bool cykl(wezel *node) {
        for (vector<wezel*>::iterator it = node->next.begin(); it != node->next.end(); ++it) {
            if (cykl(node->nr,*it)) return true;
        }
        return false;
    }


    void wczytaj(){
        int u = 0,v = 0;
        cout << "Podaj sciezki grafu lub -1 by zakonczyc\nOD ";
        while (cin >> u && u != -1) {
            cout << "DO ", cin >> v;
            nodes[u-1]->next.push_back(nodes[v-1]);
            if (cykl(nodes[v-1]) || nodes[u-1]->next.size() > 1) {
                nodes[u-1]->next.erase(nodes[u-1]->next.end()-1);
                cout << "Zapisanie sciezki zakonczone niepowodzeniem, wykryto cykl" << endl;
                continue;
            }
            nodes[v-1]->prev.push_back(nodes[u-1]);
            cout << "\nOD ";
        }

        cout << "\nSciezki wprowadzone!" << endl;
    }

    void wczytaj2(){
        fstream f("input2.txt", ios::in);
        int u = 0,v = 0;
        while (f >> u && u != -1) {
            f >> v;
            nodes[u-1]->next.push_back(nodes[v-1]);
            if (cykl(nodes[v-1]) || nodes[u-1]->next.size() > 1) {
                nodes[u-1]->next.erase(nodes[u-1]->next.end()-1);
                cout << "Zapisanie sciezki zakonczone niepowodzeniem, wykryto cykl" << endl;
                continue;
            }
            nodes[v-1]->prev.push_back(nodes[u-1]);
        }

        cout << "\nSciezki wprowadzone!" << endl;
    }

    void wykres(){
        ofstream f;
        f.open("crit.plt");
        f << rozmiar() << endl;
        f << "Start" << " " << "0" << endl;
        for (auto it: nodes)
            f << "Z" << it->nr << " " << it->czas << endl;
        f << "Koniec" << " " << "0" << endl;
        ifstream f2("input2.txt", ios::in);
        f << 33 << " " << 32 << endl;
        f << 33 << " " << 30 << endl;
        f << 33 << " " << 14 << endl;
        f << 33 << " " << 23 << endl;
        f << 33 << " " << 20 << endl;
        f << 33 << " " << 10 << endl;
        f << 33 << " " << 4 << endl;
        int u,v;
        while (f2 >> u && u != -1){
            f << u << " ";
            f2 >> v;
            f << v << endl;
        }
        f2.close();
        f << 1 << " " << 0 << endl;
        f << "quit" << endl;
        f << 33 << " " << 4 << " " << 3 << " " << 1 << " " << 0 << endl;
        f.close();
    }

    string exec(const char* cmd) {
    char buffer[128];
    string result = "";
    shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}


    void print() {
        cout << endl;
        cout << "Z\tM\tCzas\tZCzas\tRCzas\tL" << endl;
        cout << "______________________________________________" << endl;
        for (auto it: nodes) {
            if (it->nr > 9)
                cout << it->nr << "|\t";
            else
                cout << it->nr << " |\t";

            cout << it->m+1 << " |\t";

            if (it->czas > 9)
                cout << it->czas << "  |\t";
            else
                cout << it->czas << "   |\t";
            
            if (it->root < -9)
                cout << it->root << "  |\t";
            else if (it->root > 9)
                cout << it->root << "  |\t";
            else if (it->root < 0 && it->root > -10)
                cout << it->root << "   |\t";
            else
                cout << it->root << "    |\t";
            

            if (it->real_czas > 9)
                cout << it->real_czas << "  |\t";
            else
                cout << it->real_czas << "   |\t";


            if (it->l < -9)
                cout << it->l << "  |" << endl;
            else if (it->l > 9)
                cout << it->l << "   |" << endl;
            else if (it->l < 0 && it->l > -10)
                cout << it->l << "   |" << endl;
            else
                cout << it->l << "    |" << endl;    
        }
    }
    int dk (wezel *node){
        if (node->nr == 1) return 1 - node->czas; 
        int wiekszy = 1 - node->czas;
        for (auto it: node->next){
            if (it->root+1 > wiekszy)
                return it->root+1;
        }
        return wiekszy;
    }

    void initiate(){
        for (auto it: nodes){
            it->root = dk(it);
        }
    }
    bool wykonane(wezel* node){
        for (auto it: node->prev)
            if (it->stan == 0) return false;
        return true;
    }

    vector <wezel*> potencjalne(){
        vector <wezel*> temp;
        for (auto it: nodes)
            if (it->stan == 0 && (wykonane(it) || it->prev.size() == 0))
                temp.push_back(it);
        return temp;
    }
    
    bool wykonane(){
        for (auto it: nodes)
            if (it->stan == 0) return true;
        return false;
    }

    void maszyny(){
        vector <vector<int>> maszynka (4, vector<int> (rozmiar()));
        vector <wezel*> A;
        int najwiecej, n, z, j, x;
        while(wykonane()){
            n = 4;
            A = potencjalne();
            if (A.size() < n) n = A.size();
            j = 0;
            for (int i=0; i<n; i++){
                najwiecej = -9999;
                for (auto it: A){
                    if (it->stan == 0 && it->root > najwiecej){
                        najwiecej = it->root;
                        z = it->nr;
                    }
                }
                x = j;
                do{
                    x++;
                }while (maszynka[j][x]);
               
                maszynka[j][x] = 1;
                nodes[z-1]->m = j;
                nodes[z-1]->stan = 1;
                nodes[z-1]->real_czas = x - nodes[z-1]->m;
                nodes[z-1]->l = x - nodes[z-1]->czas;
                j++;
            }
        }
        
        lmax = -9999, cmax = -9999;
        for (auto it: nodes){
            if (it->l > lmax) 
                lmax = it->l;
            if (it->real_czas > cmax)
                cmax = it->real_czas;
        }
        cout << "lmax: " << lmax << ", cmax: " << cmax << endl;
    }


    void prettyprint() {
        int m = 4;
        int szer = 100;
        int szerokosc = szer * cmax + 10;
        int height = 50;

        ofstream plik;
        plik.open("rysunek.svg");
        plik << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";
        for (auto it: nodes) {
            plik << "<g>";

            plik << "<rect x=\"" << szer*(it->real_czas-1) << "\" y=\"" << it->m*height << "\" width=\"" << szer << "\" height=\"" << height << "\" style=\"fill:white;stroke-width:1;stroke:black\" >" << "</rect>" << endl;
            plik << "<text x=\"" << szer*(it->real_czas-1) + (szer/3) << "\" y=\"" << (height/2)+height*it->m << "\" font-family=\"Arial\" font-size=\"16\" fill=\"black\">Z" << it->nr << "</text>" << endl;

            plik << "</g>" << endl;
        }

        plik << "<line x1=\"0\" y1=\"" << (m)*height + 15 << "\" x2=\"" << szerokosc << "\" y2=\"" << (m)*height + 15 << "\" style=\"stroke:black;stroke-width:2\"/>" << endl;
        for (int i=0;i<cmax+1;i++) {
            plik << "<line x1=\"" << szer*i << "\" y1=\"" << (m)*height + 10 << "\" x2=\"" << szer*i << "\" y2=\"" << (m)*height + 20 << "\" style=\"stroke:black;stroke-width:2\"/>" << endl;
        }
        plik << "<text x=\"" << szerokosc << "\" y=\"" << (m)*height + 10 << "\" font-family=\"Arial\" font-size=\"16\" fill=\"black\">" << cmax << "</text>" << endl;
        plik << "</svg>" << endl;
        plik.close();
        // cout << "\nZapisano 'rysunek.svg'.\n";
    }
};


int main() {

    wezel w(32);

    // w.wczytaj();
    w.wczytaj2();
    w.initiate();
    w.maszyny();
    w.print();
    // w.wykres();
    // w.prettyprint();
    // w.exec("python3 crit.py < crit.plt");


    return 0;
}