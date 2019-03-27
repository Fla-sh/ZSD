#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Node {

    Node *parent;
    Node *left;
    Node *right;
    int data;
    int balance;
};

string cr, cl, cp;

//usuwanie drzewa - postorderowe
//pierwsza wartość to korzen drzewa
void remove_tree(Node *&actual) {
    if (actual) {
        remove_tree(actual->left);
        remove_tree(actual->right);
        delete actual;
        actual = NULL;
    }
}

//obrot RR
void RR_rotate(Node *&root, Node *A) {
    Node *B = A->right;
    Node *parent = A->parent;

    A->right = B->left;
    if (A->right) A->right->parent = A;

    B->left = A;
    B->parent = parent;
    A->parent = B;

    if (parent) {
        if (parent->left == A) parent->left = B;
        else parent->right = B;
    } else root = B;

    if (B->balance == -1) A->balance = B->balance = 0;
    else {
        A->balance = -1;
        B->balance = 1;
    }
}

//obrot LL
void LL_rotate(Node *&root, Node *A) {
    Node *B = A->left, *parent = A->parent;

    A->left = B->right;
    if (A->left) A->left->parent = A;

    B->right = A;
    B->parent = parent;
    A->parent = B;

    if (parent) {
        if (parent->left == A) parent->left = B;
        else parent->right = B;
    } else root = B;

    if (B->balance == 1) A->balance = B->balance = 0;
    else {
        A->balance = 1;
        B->balance = -1;
    }
}

//obrot RL
void RL_rotate(Node *&root, Node *A) {
    Node *B = A->right;
    Node *C = B->left;
    Node *parent = A->parent;

    B->left = C->right;
    if (B->left) B->left->parent = B;

    A->right = C->left;
    if (A->right) A->right->parent = A;

    C->left = A;
    C->right = B;
    A->parent = B->parent = C;
    C->parent = parent;

    if (parent) {
        if (parent->left == A) parent->left = C;
        else parent->right = C;
    } else root = C;

    if (C->balance == -1) A->balance = 1;
    else A->balance = 0;
    if (C->balance == 1) B->balance = -1;
    else B->balance = 0;

    C->balance = 0;
}

//obrot LR
void LR_rotate(Node *&root, Node *A) {
    Node *B = A->left;
    Node *C = B->right;
    Node *parent = A->parent;

    B->right = C->left;
    if (B->right) B->right->parent = B;

    A->left = C->right;
    if (A->left) A->left->parent = A;

    C->right = A;
    C->left = B;
    A->parent = B->parent = C;
    C->parent = parent;

    if (parent) {
        if (parent->left == A) parent->left = C;
        else parent->right = C;
    } else root = C;

    if (C->balance == 1) A->balance = -1;
    else A->balance = 0;
    if (C->balance == -1) B->balance = 1;
    else B->balance = 0;

    C->balance = 0;
}


void insert_node(Node *&root, int key) {
    Node *added_node;
    Node *parent_node;
    Node *parent_of_parent_node;
    bool t;

    added_node = new Node;
    added_node->left = added_node->right = added_node->parent = NULL;
    added_node->data = key;
    added_node->balance = 0;

    parent_node = root;

    // WSTAWIANIE

    if (!parent_node) root = added_node;
        //sprawdzenie czy drzewo istnieje
    else {
        while (true)
            // lewy syn ma zawsze wartosc mniejsza od ojca
            // prawy wieksza
            if (key < parent_node->data) {
                if (!parent_node->left) {
                    //jezeli wezel parent nie ma lewego syna to w jego miejsce wstawiamy nowy element
                    parent_node->left = added_node;
                    break;
                }
                //jesl nie to idziemy do lewego syna
                parent_node = parent_node->left;
            } else {
                if (!parent_node->right) {
                    // nie ma syna
                    parent_node->right = added_node;
                    break;
                }
                // ma syna
                parent_node = parent_node->right;
            }
        //ustawienie ojca dla wezla nowego wezla
        added_node->parent = parent_node;

        //ROWNOWAZENIE

        if (parent_node->balance) parent_node->balance = 0;
            // jedyne wartsci jakie moze przyjac balance to 1 albo -1,
            // jezeli taka wartosc zostala przyjeta to tylko dlatego ze w drugiej galezi jest
            // jakis elemnt, nie jest wazne jaka to galaz, wazne ze jest
            // wiec skoro dolozylismy drugiego syna do ojca to teraz obie galezie maja ta sama wysokosc
            // rowna jeden
        else {
            // w tym kroku instrukcja warunkowa sprawdzila, ze wartosc wspolczynnika rownowagi dla ojca
            // jest zerowa, dlatego wiadomo juz ze balance bedzie mialo wartosc albo 1 albo -1
            // trzeba tylko sprawdzic
            if (parent_node->left == added_node)
                parent_node->balance = 1;
            else
                parent_node->balance = -1;

            // nalezy pamietac, ze zmiana w  rodzicu nowo wstawionego dziecka moze
            // propagowac sie w gore
            // trzeba sprawdzic, czy tak sie nie stalo

            parent_of_parent_node = parent_node->parent;
            t = false;
            while (parent_of_parent_node) {
                // powtarzaj sie do kiedy bedize mozna cos jeszcze sprawdzic
                if (parent_of_parent_node->balance) {
                    // jezeli ktorys z napotkanych rodzicow byl nie zbalansowany
                    // - mosiadal wartosc balance rozna od 0
                    // to w tym momencie stal sie jeszcze bardziej nie zbalansowny

                    t = true;
                    break;
                }

                // jezeli aktualny rodzic jest zbalansowany to nalezy sprawdzic z jakiej galezi
                // przyszlismy i dopisac do balansu ojca informacje ze jedno z jego
                // poddrzew stalo sie dluzsze
                // kwestia ktore
                if (parent_of_parent_node->left == parent_node) {
                    // lewe
                    parent_of_parent_node->balance = 1;
                } else {
                    // prawe
                    parent_of_parent_node->balance = -1;
                }

                // przejdz dalej do rodzicow
                parent_node = parent_of_parent_node;
                parent_of_parent_node = parent_of_parent_node->parent;
            }

            if (t) {
                // jezeli stwierdzilismy, ze na drodze od nowo dodanego elementu do
                // korzenia znaleziono jakiegos rodzica, ktory nie jest zbalansowany
                // to nalezy sprawdzic, czy dodanie nowego elemnt go zbalansowalo
                //      - przypadek w ktorym nowy elemnt zostanie dodany do krotszej galezi
                // czy moze doprowadzilo do jeszcze wiekszego nie zbalansowania
                //      - przypdek dodania elemntu do dluzszej galezi
                if (parent_of_parent_node->balance == 1) {
                    if (parent_of_parent_node->right == parent_node) {
                        // przypadek dolozenie elemntu do krotszej galezi
                        // drzewo sie zbalansowalo
                        parent_of_parent_node->balance = 0;
                    }
                        // nalezy okreslic z jakiej podgalezi przyszlismy,
                        // skutkuje to wybraniem odpowiedniej rotacji
                    else if (parent_node->balance == -1) {
                        LR_rotate(root, parent_of_parent_node);
                    } else LL_rotate(root, parent_of_parent_node);
                } else {
                    if (parent_of_parent_node->left == parent_node) {
                        parent_of_parent_node->balance = 0;
                    } else if (parent_node->balance == 1) {
                        RL_rotate(root, parent_of_parent_node);
                    } else RR_rotate(root, parent_of_parent_node);
                }
            }
        }
    }
}


Node *predecessor_node(Node *p) {
    Node *r;

    if (p) {
        if (p->left) {
            p = p->left;
            while (p->right) p = p->right;
        } else
            do {
                r = p;
                p = p->parent;
            } while (p && p->right != r);
    }
    return p;
}

Node *find(Node *elem, int key) {
    while (elem && elem->data != key) {
        cout << elem->data << " ";
        elem = (key < elem->data) ? elem->left : elem->right;
    }
    cout << elem->data << endl;
    return elem;
}

void inorder(Node *elem) {
    if (elem->left != NULL) inorder(elem->left);
    cout << elem->data << " ";
    if (elem->right != NULL) inorder(elem->right);
}

void preorder(Node *elem) {
    cout << elem->data << " ";
    if (elem->left != NULL) preorder(elem->left);
    if (elem->right != NULL) preorder(elem->right);
}

void postorder(Node *elem) {
    if (elem->left != NULL) preorder(elem->left);
    if (elem->right != NULL) preorder(elem->right);
    cout << elem->data << endl;
}

Node *remove(Node *&root, Node *x) {
    Node *t;
    Node *y;
    Node *z;
    bool nest;

    if (x->left && x->right) {
        // sprawdzenie czy usuwany elemnt ma 2 synow
        // jesli tak to usun najmnijeszy elemnt z lewej galezi
        // jednoczesnie go zapamietujac do y
        y = remove(root, predecessor_node(x));
        nest = false;
    } else {
        // sprawdz, ktorego z synow mial ojciec
        if (x->left) {
            // jeli mial lewego to go zapamietaj i usun
            y = x->left;
            x->left = NULL;
        } else {
            // jesli prawego to rob to samo
            y = x->right;
            x->right = NULL;
        }
        // nie posiadanie syna zalicza sie pod druga opcje tyle tylko, ze y = NULL
        // w dalszej czesci progrmau zostanie to przypisane jako wartosc x
        // - skasujemy x jezeli byl lisciem

        //jesli usuwany elemnt mial tylko jedno dziecko ktore zostalo takze usuniete z drzewa
        // i przeniesione do zmiennej y to w tym momencie el. x jest zbalansowany - jest lisciem
        x->balance = 0;
        nest = true;
    }

    if (y) {
        // jesli zostalo znaleziony jakis elemnt, ktory bedzimy wstawiac za x to nalezy do niego
        // przepisac wartosci z x - te dotyczace polozenia
        y->parent = x->parent;

        y->left = x->left;
        if (y->left) y->left->parent = y;

        y->right = x->right;
        if (y->right) y->right->parent = y;

        y->balance = x->balance;
        // jezeli elemnt y mial jakichkolwiek synow, to nalezy ich zachowac
    }

    if (x->parent) {
        // nalezy podmienic informacje o potomkach dla ojca usuwanego elementu x
        if (x->parent->left == x) x->parent->left = y;
        else x->parent->right = y;
    }
        // przypdek w ktory usuwamy korzen
    else root = y;

    if (nest) {
        // nie potrzeba naprawiaz struktury ktora tego nie wymaga
        // a tak jest w przypadku gdy wywolamy funkcje usuwania dla elemntu posiadajacego
        // 2 synow
        // to dopiero kolejne wywolanie zniszczy drzewo  i bedzie wymagalo jego naprawienia
        // poniewaz wtedy zostanie faktycznie usuniety jakis elemnt
        // w tym przypadku nigdzie nie zmieni sie wartosc balance poniewaz nie zostana
        // dodane nowe elemnty

        // zachowujemy wartosc x poniewaz bedzie trzeba ja zwrocic na koncu wywolania
        // zmienne z i y beda teraz odpowiadac synowi i ojcu
        z = y;
        y = x->parent;
        while (y) {
            if (!y->balance) {
                // drzewo juz przed usunieciem bylo zbalansowane, usuniecie jednego
                // elemntu wplynie na balans tego wezla jednak nie wplynie na wysokosc calej galezi
                // poniewaz druga podgalaz zachowa swoja dlugosc

                // wiec nalezy tylko ustawic odpowiedni balans
                if (y->left == z) y->balance = -1;
                else y->balance = 1;
                // koniec naprawania
                break;
            } else {
                if (((y->balance == 1) && (y->left == z)) ||
                    ((y->balance == -1) && (y->right == z))) {
                    // zostal usuniety element z galezi dluzszej, przez co balans calej galezi sie naprawil,
                    // jednak takie usuniecie w przeciwienstwie do poprzedniego skrocilo takze cala galaz
                    // a nie tylko jedna z podgalezi
                    // trzeba posprawdzac, czy balansowania nie wymagaja wyzsze poziomy
                    y->balance = 0;
                    // przesun o pietro w gore
                    z = y;
                    y = y->parent;
                } else {
                    // skracanie krotszej galezi
                    // z zawsze przychodzi z galezi skroconej - lzejszej
                    // ustawiamy wskaznik ciezszej - t - na przeciwna niz
                    // ta  z ktorej przychodzimy
                    if (y->left == z) t = y->right;
                    else t = y->left;

                    if (!t->balance) {
                        // balans ciezszego poddrzewa jest zachowny dlatego nalezy wykonac odpowiednia
                        // rotacje LL lub RR
                        // taka rotacja nie powoduje zmiany wysokosci poddrzewa, jednak je rownowazy
                        // algorytm sie konczy
                        if (y->balance == 1) LL_rotate(root, y);
                        else RR_rotate(root, y);
                        break;
                    } else if (y->balance == t->balance) {
                        // drzewo ciezsze i aktualnie przegladany wezel maja te same wspolczynniki balansu
                        // nalezy wykonac rotacje RR lub LL
                        // niestey wykonanie takich rotacji spowoduje zmiane w wysokosci galezi
                        // algorytm musie sie kontynuowac
                        if (y->balance == 1) LL_rotate(root, y);
                        else RR_rotate(root, y);
                        z = t;
                        y = t->parent;
                    } else {
                        // z poprzednich instrukcji wiadomo, ze balansy wezla y i t nie sa sobie rowne,
                        // nie sa zerowe
                        // wiec musza byc przeciwne
                        // nalezy zastosowac jedna z rotacji LR lub RL
                        if (y->balance == 1) LR_rotate(root, y);
                        else RL_rotate(root, y);
                        z = y->parent;
                        y = z->parent;
                    }
                }
            }
        }
    }
    return x;
}

void createData(int size, int *el) {
    int i;
    int el1;
    int el2;
    int x;

    for (i = 0; i < size; i++)
        el[i] = i + 1;

    for (i = 0; i < 300; i++) {
        el1 = rand() % size;
        el2 = rand() % size;

        x = el[el1];
        el[el1] = el[el2];
        el[el2] = x;
    }
}

void shuffle(int *el, int size) {
    int el1;
    int el2;
    int i;
    int x;
    for (i = 0; i < 300; i++) {
        el1 = rand() % size;
        el2 = rand() % size;

        x = el[el1];
        el[el1] = el[el2];
        el[el2] = x;
    }
}

void printBT(string sp, string sn, Node *v) {
    string s;

    if (v) {
        s = sp;
        if (sn == cr) s[s.length() - 2] = ' ';
        printBT(s + cp, cr, v->right);

        s = s.substr(0, sp.length() - 2);
        cout << s << sn << v->data << ":" << v->balance << endl;

        s = sp;
        if (sn == cl) s[s.length() - 2] = ' ';
        printBT(s + cp, cl, v->left);
    }
}

int main() {
    bool interactive = true;
    Node *root = NULL;
    if(interactive) {
        cout << "Choose option: " << endl;
        cout << "n:     print in-order" << endl;
        cout << "p:     print post-order" << endl;
        cout << "r:     print pre-order" << endl;
        cout << "i:     add element" << endl;
        cout << "f:     find element" << endl;
        cout << "m:     remove tree" << endl;
        cout << "v      remove element" << endl;
        while (true) {
            char opt;
            opt = getchar();

            switch (opt) {
                case 'i': {
                    int key;
                    cout << "Write key: ";
                    cin >> key;
                    insert_node(root, key);
                    break;
                }
                case 'p': {
                    postorder(root);
                    break;
                }
                case 'n': {
                    inorder(root);
                    break;
                }
                case 'r': {
                    preorder(root);
                    break;
                }
                case 'f': {
                    int key;
                    cout << "Write key: ";
                    cin >> key;
                    find(root, key);
                    break;
                }
                case 'm': {
                    remove_tree(root);
                    break;
                }
                case 'v': {
                    int key;
                    cout << "Write key: ";
                    cin >> key;
                    remove(root, find(root, key));
                    break;
                }
            }
            cout << endl;
        }
    }
    else {
        const int size = 30;
        int el[size];
        int i;

        createData(size, el);

        srand(time(NULL));

        for (i = 0; i < size; i++) insert_node(root, el[i]);

        // printBT("","",root);
        // shuffle(el, size);
        // for(i = 0; i < 15; i++) remove(root,find(root,el[i]));
        inorder(root);
        cout << endl << endl;
        remove_tree(root);
    }
    return 0;
} 