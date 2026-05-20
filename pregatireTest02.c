#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


struct Pilot {
	int id;
	float greutate;
	char* echipa;
	int nrPuncte;
};
typedef struct Pilot Pilot;

struct Nod {
	Pilot info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

struct Heap {
	int lungime;
	int nrElemViz;
	Pilot* vector;
};
typedef struct Heap Heap;

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrElemViz = 0;
	heap.vector = (Pilot*)malloc(sizeof(Pilot) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int stanga = 2 * pozitieNod + 1;
	int dreapta = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (stanga < heap.nrElemViz && heap.vector[stanga].nrPuncte > heap.vector[pozMax].nrPuncte) //daca facem cuy min heap am modifica doar semnul de la compararea preturilor
	{
		pozMax = stanga; //marcam ca trebuie sa facem o interschimbare
	}
	if (dreapta < heap.nrElemViz && heap.vector[dreapta].nrPuncte > heap.vector[pozMax].nrPuncte)
	{
		pozMax = dreapta; //marcam ca trebuie sa facem o interschimbare
	}
	//verificam daca maximul se afla in radacina si daca e necesar sa facem interschimbarea
	if (pozMax != pozitieNod)
	{
		Pilot aux;
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


Nod* inserareArboreBinar(Nod* radacina, Pilot pilotNou) {
	//inserare dupa id:
	if (radacina) {
		if (radacina->info.id > pilotNou.id) {
			radacina->stanga = inserareArboreBinar(radacina->stanga, pilotNou);
		}
		else {
			radacina->dreapta=inserareArboreBinar(radacina->dreapta, pilotNou);
		}
		return radacina;
	}
	else {
		Nod* nou = malloc(sizeof(Nod));
		nou->info = pilotNou;
		nou->dreapta = NULL;
		nou->stanga = NULL;
		//radacina = nou;
		//radacina = nou;
		return nou;
	}
}

Pilot citireAvionDinFisier(FILE* file) {
	char sep[4] = ",\n";
	char buffer[100];
	char* aux;
	Pilot pilot;
	fgets(buffer, 100, file);
	aux = strtok(buffer, sep);
	pilot.id = atoi(aux);
	aux = strtok(NULL, sep);
	pilot.greutate = atof(aux);
	aux = strtok(NULL, sep);
	pilot.echipa = malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy_s(pilot.echipa, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	pilot.nrPuncte = atoi(aux);
	return pilot;
}

Nod* adaugarePilotiInArboreDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(file)) {
		radacina = inserareArboreBinar(radacina,citireAvionDinFisier(file));
	}
	fclose(file);
	return radacina;
}

void afisarePilot(Pilot p) {
	printf("id:%d\n", p.id);
	printf("greutate:%.2f\n", p.greutate);
	printf("echipa:%s\n", p.echipa);
	printf("nr puncte:%d\n", p.nrPuncte);
}

void afisareArboreBinar(Nod* radacina) {
	if (radacina) {
		afisarePilot(radacina->info);
		afisareArboreBinar(radacina->stanga);
		afisareArboreBinar(radacina->dreapta);
	}
}

Pilot getById(Nod* radacina, int id) {
	
	Pilot pilot;

	if (radacina) {
		if (radacina->info.id == id) {
			pilot = radacina->info;
			pilot.echipa = malloc(sizeof(char) * (strlen(radacina->info.echipa) + 1));
			strcpy_s(pilot.echipa, strlen(radacina->info.echipa) + 1, radacina->info.echipa);
			return pilot;
		}
		else {
			if (radacina->info.id <= id) {
				pilot = getById(radacina->dreapta, id);
			}
			else {
				pilot = getById(radacina->stanga, id);
			}
		}
	}
	else {
		pilot.id = -1;
	}
	return pilot;
}


int calculeazaAparitiiPilotByEchipa(Nod* radacina, const char* echipa) {
	if (radacina == NULL) {
		return 0;
	}
	int nrCurent = 0;
	if (!(strcmp(radacina->info.echipa, echipa))) {
		nrCurent++;
	}

	int nrStanga = calculeazaAparitiiPilotByEchipa(radacina->stanga, echipa);
	int nrDreapta = calculeazaAparitiiPilotByEchipa(radacina->dreapta, echipa);

	return nrStanga + nrDreapta + nrCurent;
}

void dezalocareMemorie(Nod** radacina) {
	if (*radacina) {
		dezalocareMemorie(&(*radacina)->stanga);
		dezalocareMemorie(&(*radacina)->dreapta);
		free((*radacina)->info.echipa);
		free(*radacina);
		*radacina = NULL;
	}
}

int main(){
	Nod* arbore = adaugarePilotiInArboreDinFisier("pregatireTest02.txt");
	afisareArboreBinar(arbore);
	afisarePilot(getById(arbore, 1));
	printf("nr persoane:%d", calculeazaAparitiiPilotByEchipa(arbore, "Steluta"));
	dezalocareMemorie(&(arbore));
	afisareArboreBinar(arbore);

}
