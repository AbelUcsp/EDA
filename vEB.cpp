#include <cmath>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <sstream>
using namespace std;


template <typename T>
class Node {
public:
	T val;
	Node *nodes[2];
	int  altura;
	int etiqueta;
	Node() {
		nodes[0] = nullptr;
		nodes[1] = nullptr;
		altura = 1;
		etiqueta = 0;
	}
	Node(T x) {
		val = x;
		nodes[0] = nodes[1] = nullptr;
		altura = 1;
		etiqueta = 0;
	}
};

template <typename T>
class AVLTree {


public:
	Node<T> *raiz;

	typedef pair<Node<T>**, bool> pairPtrLado;
	AVLTree();
	//~AVLTree(){}
	bool find(T x, Node<T> **&ptr, stack<pairPtrLado> &camino);
	bool insert(T x);
	bool remove(T x);

	void print();
	int revalt(Node<T>* nodo);
	int difAltura(Node<T>* nodo);
	void balanceo(stack<pairPtrLado > &camino);
	void balanceo2(stack<pairPtrLado > &camino);
	void cambioLRyRL(pairPtrLado hijo, pairPtrLado &padre, pairPtrLado abuelo);
	void cambioLLyRR(pairPtrLado padre, pairPtrLado abuelo);

	int size(Node<T> *ptr);
	int size();
	int tamanho(Node<T> *ptr);
	void print2(queue<Node<T> *> cola, int n);

	/** EDA */
    bool Find(T x, Node<T> ** &p);
    void printEtiquetados();
	void DFS();
	void BFS();
	void vEB();
	int NBloque(T valor, int bloque);
};

template <typename T>
bool AVLTree<T>::Find(T x, Node<T> ** &p) {								  //for
	for (p = &raiz; (*p) && (*p)->val != x; p = &( (*p)->nodes[ (*p)->val < x] )  );
	return (*p)!=nullptr;
};


template <typename T>
int AVLTree<T>::NBloque(T valor, int bloque){
    Node<T> **Ptr;
    if (Find(valor, Ptr)){
        return  ceil( (*Ptr)->etiqueta/static_cast<float>(bloque) );
    }
    return -1;
}

template <typename T>
void AVLTree<T>::DFS(){
    int eti = 0;
    stack<Node<T>* > pila;
    pila.push(raiz);
    while(!pila.empty()){
        if(pila.top()){
            //cout << pila.top()->val << " ";
            pila.top()->etiqueta = ++eti;
            Node<T> *tmp = pila.top();
            pila.pop();
            pila.push(tmp->nodes[1]);
            pila.push(tmp->nodes[0]);
        }
        else{
            pila.pop();
        }
    }
    cout << endl;
}

template <typename T>
void AVLTree<T>::BFS() { // AMPLITUD
    int eti = 0;
    queue<Node<T>* > cola;
    cola.push(raiz);
    while(!cola.empty()){
    if(cola.front()){
        cola.front()->etiqueta = ++eti;
        cola.push(cola.front()->nodes[0]);
        cola.push(cola.front()->nodes[1]);
    }
    cola.pop();
    }
}


template <typename T>
void AVLTree<T>::printEtiquetados() { // AMPLITUD
    int eti = 0;
    queue<Node<T>* > cola;
    cola.push(raiz);
    while(!cola.empty()){
    if(cola.front()){
        cout << cola.front()->val << " " <<cola.front()->etiqueta<<endl;
        cola.push(cola.front()->nodes[0]);
        cola.push(cola.front()->nodes[1]);
    }
    cola.pop();
    }
}


template <typename T>
void AVLTree<T>::vEB() { // AMPLITUD
    int eti = 0;
    queue<Node<T>* > cola;
    cola.push(raiz);
    raiz->etiqueta = ++eti;
    raiz->nodes[0]->etiqueta = ++eti;
    raiz->nodes[1]->etiqueta = ++eti;

    while(!cola.empty()){
    if(cola.front()){
        if(cola.front()->etiqueta == 0)
            cola.front()->etiqueta = ++eti;
        if(cola.front()->nodes[0] && cola.front()->nodes[0]->etiqueta == 0){
            cola.front()->nodes[0]->etiqueta = ++eti;

            if(cola.front()->nodes[0]->nodes[0] && cola.front()->nodes[0]->nodes[0]->etiqueta == 0)
                cola.front()->nodes[0]->nodes[0]->etiqueta = ++eti;
            if(cola.front()->nodes[0]->nodes[1] && cola.front()->nodes[0]->nodes[1]->etiqueta == 0)
                cola.front()->nodes[0]->nodes[1]->etiqueta = ++eti;
        }cola.push(cola.front()->nodes[0]);


        if(cola.front()->nodes[1] && cola.front()->nodes[1]->etiqueta == 0){
            cola.front()->nodes[1]->etiqueta = ++eti;

            if(cola.front()->nodes[1]->nodes[0] && cola.front()->nodes[1]->nodes[0]->etiqueta == 0)
                cola.front()->nodes[1]->nodes[0]->etiqueta = ++eti;
            if(cola.front()->nodes[1]->nodes[1] && cola.front()->nodes[1]->nodes[1]->etiqueta == 0)
                cola.front()->nodes[1]->nodes[1]->etiqueta = ++eti;
        }cola.push(cola.front()->nodes[1]);
    }
    cola.pop();
    }
}


template <typename T>
AVLTree<T>::AVLTree() {
	raiz = nullptr;
}

template<typename T>
bool AVLTree<T>::find(T x, Node<T>** &ptr, stack<pairPtrLado> &camino) {
	ptr = &raiz;
	bool lrPos = 0;
	camino.push(make_pair(ptr, lrPos));
	while (*ptr && (*ptr)->val != x) {
		lrPos = (*ptr)->val < x;
		ptr = &((*ptr)->nodes[lrPos]);
		camino.push(make_pair(ptr, lrPos));
	}
	return *ptr;
}

template<typename T>
void AVLTree<T>::cambioLLyRR(pairPtrLado padre, pairPtrLado abuelo) {
	(*abuelo.first)->altura--;

	Node<T> *tmp = *abuelo.first;
	*abuelo.first = *padre.first;
	*padre.first = (*padre.first)->nodes[!padre.second];
	(*abuelo.first)->nodes[!padre.second] = tmp;
}

template<typename T>
void AVLTree<T>::cambioLRyRL(pairPtrLado hijo, pairPtrLado &padre, pairPtrLado abuelo) {
	(*abuelo.first)->altura--;
	(*padre.first)->altura--;
	(*hijo.first)->altura++;

	Node<T> *tmpA = *abuelo.first;
	Node<T> *tmpB = *padre.first;
	*abuelo.first = *hijo.first;

	*padre.first = (*abuelo.first)->nodes[hijo.second];
	*hijo.first = (*abuelo.first)->nodes[padre.second];

	(*abuelo.first)->nodes[padre.second] = tmpB;
	(*abuelo.first)->nodes[hijo.second] = tmpA;

	// Arreglo el doble ptr del pair
	padre.first = &((*abuelo.first)->nodes[padre.second]);
}

template<typename T>
int AVLTree<T>::difAltura(Node<T>* nodo) {
	if (nodo->nodes[0] == nullptr && nodo->nodes[1] == nullptr) {
		return 0;
	}

	if (nodo->nodes[0] && nodo->nodes[1])
		return nodo->nodes[0]->altura - nodo->nodes[1]->altura;
	if (nodo->nodes[1] != nullptr) {
		return 0 - nodo->nodes[1]->altura;
	}
	return nodo->nodes[0]->altura;
}

template<typename T>
void AVLTree<T>::balanceo(stack<pairPtrLado > &camino) {
	pairPtrLado hijo, padre;
	int diferencia;
	while (camino.size() > 1) {
		padre = camino.top();
		camino.pop();
		diferencia = difAltura(*camino.top().first);
		if (diferencia == 0) return;

		if (diferencia == 1 || diferencia == -1) {
			if ((*padre.first)->altura + 1 > (*camino.top().first)->altura)
				(*camino.top().first)->altura++;
			hijo = padre;
		}
		else if (diferencia == 2 || diferencia == -2) {
			if (hijo.second == padre.second)
				cambioLLyRR(padre, camino.top());
			else {
				cambioLRyRL(hijo, padre, camino.top());
				hijo = padre;
			}
		}

	}
}

template<typename T>
void AVLTree<T>::balanceo2(stack<pairPtrLado > &camino) {
	pairPtrLado hijo, padre, nieto;
	int dif1;
	bool qt = true;
	Node<T> *tem;

	while (camino.size() >= 1) {
		padre = camino.top();

		dif1 = difAltura(*(padre.first));
		if (dif1 == 0) {
			if (!qt) {
				return;
			}
			else {
				(*(padre.first))->altura--;
				hijo = padre;
			}
		}
		else if (dif1 == 1 || dif1 == -1) {
			return;

		}
		else if (dif1 == 2 || dif1 == -2) {


			hijo = make_pair(&((*(padre.first))->nodes[dif1 < 0]), dif1 < 0);
			int dif2 = difAltura(*(hijo.first));
			if ((dif1 < 0 && dif2 < 0) || (dif1 > 0 && dif2 > 0)) {
				cambioLLyRR(hijo, padre);
				revalt(*(hijo.first));

			}
			else {
				nieto = make_pair(&((*(hijo.first))->nodes[dif1 > 0]), dif1 > 0);
				cambioLRyRL(nieto, hijo, padre);
				revalt(*(padre.first));
				nieto = hijo;

			}
		}
		camino.pop();

	}
}


template<typename T>
int AVLTree<T>::size(Node<T> *ptr) {
	if (!ptr) return 0;
	return max(size(ptr->nodes[0]) + 1, size(ptr->nodes[1]) + 1);
}


template<typename T>
int AVLTree<T>::tamanho(Node<T> *ptr) {
	return ptr->altura;
}


template<typename T>
int AVLTree<T>::size() {
	return size(raiz);
}

template<typename T>
bool AVLTree<T>::insert(T x) {
	Node<T> **ptr;
	stack<pairPtrLado > camino;

	if (find(x, ptr, camino)) return 0;
	*ptr = new Node<T>(x);
	if (camino.size() > 1)
		balanceo(camino);
	return 1;
}

template<typename T>
bool AVLTree<T>::remove(T x) {
	Node<T> **ptr;
	stack<pairPtrLado> camino, camino2;
	if (!find(x, ptr, camino)) return 0;

	if ((*ptr)->nodes[0] && (*ptr)->nodes[1]) {
		bool random = true;

		Node<T> **tmp = ptr;
		tmp = &((*tmp)->nodes[random]);
		camino.push(make_pair(tmp, random));

		while ((*tmp)->nodes[!random]) {
			tmp = &((*tmp)->nodes[!random]);
			camino.push(make_pair(tmp, !random));
		}

		(*ptr)->val = (*tmp)->val;

		delete *tmp;
		*tmp = nullptr;


		camino.pop();


		balanceo2(camino);

		return 1;

	}


	Node<T> *tmp = *ptr;
	*ptr = (*ptr)->nodes[(*ptr)->nodes[1] != nullptr];
	delete tmp;

	camino.pop();

	balanceo2(camino);


	return 1;
}

string convertirSINT(int entero)
{   string cadena;
    stringstream convert;
    convert << entero;
    cadena = convert.str(); ///asigna cadena Al contenido del flujo
    return cadena;
}

void agregar0(string &palabra, int maxlen)
{
    while(palabra.size() != maxlen)
        palabra.insert(0,"0");
}

template <typename T>
void AVLTree<T>::print() {
	vector <Node<T>* > c;
	c.push_back(raiz);
	int len;
	int tamanho = size();
	while (c.size() != 0) {
		len = c.size();
		for (int i = 0; i < len; i++) {
			for (int j = 0; j < tamanho; j++) {
				cout << " ";
			}

			if (c[i]) {
				cout << c[i]->val << " ";
				c.push_back(c[i]->nodes[0]);
				c.push_back(c[i]->nodes[1]);
			}
			else
				cout << "*";

			tamanho--;
		}
		cout << endl;
		c.erase(c.begin(), c.begin() + len);
	}
}

template<typename T>
int AVLTree<T>::revalt(Node<T>* nodo)
{
	if (nodo->nodes[0] == nullptr && nodo->nodes[1] == nullptr) {
		nodo->altura = 1;
		return 1;
	}
	if (nodo->nodes[0] == nullptr) {
		nodo->altura = revalt(nodo->nodes[1]) + 1;

		return nodo->altura;
	}
	if (nodo->nodes[1] == nullptr) {
		nodo->altura = revalt(nodo->nodes[0]) + 1;

		return nodo->altura;
	}
	nodo->altura = max(revalt(nodo->nodes[0]), revalt(nodo->nodes[1])) + 1;
	return nodo->altura;

}

template <typename T>
void AVLTree<T>::print2(queue<Node<T> *> cola, int n){
    if(n<0)
        return;
    cout << endl;
    queue<Node<T> *> temp;
    for(int i=0; i< (pow(2,n)-1); i++)
        cout << "---";

    while(!cola.empty()){
        if(cola.front()){
            string imprimir = convertirSINT(cola.front()->val);
            agregar0(imprimir,3);
            cout << imprimir;
            for(int i=0; i< (pow(2,n+1)-1); i++)
                cout << "---";
            temp.push(cola.front()->nodes[0]);  temp.push(cola.front()->nodes[1]);
        }
        else{
            temp.push(new Node<int>(0));        temp.push(new Node<int>(0));
            for(int i=0; i< (pow(2,n-1)-1); i++)
                cout << "---";
            cout << "000";
            for(int j=0; j< (pow(2,n+1)-1); j++)
                cout << "---";
        }
        cola.pop();
    }
    print2(temp,  n-1);
}



int main() {

	AVLTree<int> arbol;
    for(int i=1; i<16; ++i)
        arbol.insert(i);

    queue<Node<int> *> cola;
    cola.push(arbol.raiz);
    arbol.print2(cola, arbol.tamanho(arbol.raiz)-1);
    cout << endl << endl;
    ///arbol.print();




    arbol.vEB();
    ///arbol.DFS();
    ///arbol.BFS();
    //arbol.printEtiquetados();
    cout << arbol.NBloque(1, 3) <<endl;
    cout << arbol.NBloque(6, 3) <<endl;


	cout << endl << endl << endl << endl << endl << endl;
	return true;
}

