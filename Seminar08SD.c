#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru Heap
//avem nevoie si de numarul de elemente vizibile!!
struct Heap {
	int lungime;
	int nrElemViz;
	Masina* vector;
};
typedef struct Heap Heap;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrElemViz = 0;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int stanga = 2 * pozitieNod + 1;
	int dreapta = 2 * pozitieNod + 2;
	int pozMax = pozitieNod; //cazul ideal, cel mai mare e pe prima pozitie
	if (stanga < heap.nrElemViz && heap.vector[stanga].pret > heap.vector[pozMax].pret) //daca facem cuy min heap am modifica doar semnul de la compararea preturilor
	{
		pozMax = stanga; //marcam ca trebuie sa facem o interschimbare
	}
	if (dreapta < heap.nrElemViz && heap.vector[dreapta].pret > heap.vector[pozMax].pret)
	{
		pozMax = dreapta; //marcam ca trebuie sa facem o interschimbare
	}
	//verificam daca maximul se afla in radacina si daca e necesar sa facem interschimbarea
	if (pozMax != pozitieNod)
	{
		Masina aux;
		aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		//verificam daca pozMax e parinte si faem autoapel
		if (pozMax < (heap.nrElemViz - 2) / 2)
		{
			filtreazaHeap(heap, pozMax);
		}
	}
	//filtreaza heap-ul pentru nodul a carei pozitie o primeste ca parametru
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	//citim toate masinile din fisier si le stocam intr-un heap
	// pe care trebuie sa il filtram astfel incat sa respecte
	// principiul de MAX-HEAP sau MIN-HEAP dupa un anumit criteriu
	// sunt citite toate elementele si abia apoi este filtrat vectorul

	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	if (f)
	{
		while (!feof(f))
		{
			heap.vector[heap.nrElemViz++] = citireMasinaDinFisier(f);
		}
	}

	//filstrare
	for (int i = (heap.nrElemViz - 2) / 2; i >= 0; i--) //mergem de jos in sus deci i>=0
	{
		filtreazaHeap(heap, i);
	}


	fclose(f);
	return heap;

	//citirea si filtrarea le facem separat, iar filtrarea o facem de jos in sus!, adica plecam de la ultimul parinte
}

void afisareHeap(Heap heap) {
	//este o simpla afisare de vector
	for (int i = 0; i < heap.nrElemViz; i++)
	{
		afisareMasina(heap.vector[i]);
	}
	//afiseaza elementele vizibile din heap
}

void afiseazaHeapAscuns(Heap heap) {

	for (int i = heap.nrElemViz; i < heap.lungime; i++)
	{
		afisareMasina(heap.vector[i]);
	}
	//afiseaza elementele ascunse din heap
	//parcurgem toata lungimea
}

Masina extrageMasina(Heap* heap) {

	//nr elemente - 1 este ultimul element din vector
	//in ce conditii putem extrage?
	// cand avem elemente vizibile!
	Masina aux;
	aux.id = -1;
	if (heap->nrElemViz > 0)
	{
		heap->nrElemViz--;
		aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElemViz];
		heap->vector[heap->nrElemViz] = aux;
		filtreazaHeap(*heap, 0); //0 fiind prima pozitie
	}
	return aux;
	//extrage si returneaza masina de pe prima pozitie
	//elementul extras nu il stergem...doar il ascundem
}


void dezalocareHeap(Heap* heap) {

	for (int i = 0; i < heap->lungime; i++)
	{
		free(heap->vector[i].numeSofer);
		free(heap->vector[i].model);
	}
	free(heap->vector);

	heap->vector = NULL;
	heap->nrElemViz = 0;
	heap->lungime = 0;
}

int main() {

	Heap heap = citireHeapDeMasiniDinFisier("Masini.txt");
	afisareHeap(heap);
	printf("Extrage:\n");
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	printf("Heap ascuns:\n");
	afiseazaHeapAscuns(heap);
	dezalocareHeap(&heap);
	return 0;
}
