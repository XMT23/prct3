// Xavier Martin Tapia 1748951
// Daniel Genov Draganova 1703660

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXAR 7
#define MAXARNO 5


typedef struct {
    unsigned n1;
	unsigned n2;
	unsigned estat;
} Aresta;

typedef struct {
	unsigned nodes[MAXAR + 1];
    unsigned arvis[MAXAR];
    int naresvisit;
} EstructuraCami;

typedef struct {
	Aresta *arestes;
	unsigned num_arestes;
	unsigned num_nodes;
	int valid;
} Graf;

unsigned arestesdelnode(unsigned node, unsigned NAR, Aresta larestes[],
						unsigned llistap[]) {
	unsigned i, nombre = 0;
	for (i = 0; i < NAR; i++) {
		if (larestes[i].estat == 0 && 
		   (larestes[i].n1 == node || larestes[i].n2 == node)) {
			llistap[nombre] = i;
			nombre++;
		}
	}

	return nombre;
}

int existeixConnexio(unsigned int node_actual, unsigned int node_desti,
					 unsigned int arestes_posibles[], unsigned int num_posibles,
					 Aresta arestes[], unsigned* aresta_index) {						
	for (int i = 0; i < num_posibles; i++) {
		if ((arestes[arestes_posibles[i]].n1 == node_desti &&
			 arestes[arestes_posibles[i]].n2 == node_actual) ||
			(arestes[arestes_posibles[i]].n2 ==  node_desti &&
			 arestes[arestes_posibles[i]].n1 == node_actual)) {
			
			*aresta_index = arestes_posibles[i];
			return 1;	// Si connexió
		}
	}

	return 0; // No connexió
}

int nodeValenciaMaxima(Aresta arestes[], int nombre_nodes, int nombre_arestes) {
	// El valor a la posició i-éssima de la matriu correspon a la 
	// valència de la aresta i (valencies[0] = "valència de l'aresta 0,
	// valencies[1] = "valencia de l'aresta 1, ...")
	int *valencies = calloc(nombre_nodes, sizeof(int));

	for (int i = 0; i < nombre_arestes; i++) {
		int sortida = arestes[i].n1;
		int arribada = arestes[i].n2;

		valencies[sortida]++;
		valencies[arribada]++;
	}

	int valencia_maxima = 0;
	int index_maxima = 0;
	for (int i = 0; i < nombre_nodes; i++) {
		if (valencies[i] > valencia_maxima) {
			valencia_maxima = valencies[i];
			index_maxima = i;
		}
	}

	free(valencies);
	return index_maxima;
}

Graf crearGrafFitxer(char* nom_fitxer) {
	FILE *file;
	if ((file = fopen(nom_fitxer, "r")) == NULL) {
		perror("Error obrint el fitxer");
		return (Graf) { .valid = 0 };
	}

	// Anem a suposar que els fitxers proporcionats tenen sempre la estructura següent:
	// <int> (numero de nodes)
	// <int> (numero de arestes)
	// <int> <int> (origen - destí)
	// <int> <int> (origen - destí)
	// ...

	unsigned int num_nodes;
	fscanf(file, "%u", &num_nodes);
	unsigned int num_arestes;
	fscanf(file, "%u", &num_arestes);

	Aresta *arestes = calloc(num_arestes, sizeof(Aresta));
	unsigned n1, n2;
	size_t i = 0;
	while (fscanf(file, "%i %i", &n1, &n2) != EOF && i < num_arestes) {
		Aresta nova_aresta = {
			.n1 = n1,
			.n2 = n2,
			.estat = 0,
		};

		arestes[i++] = nova_aresta; 
	}
	
	return (Graf) {
		.arestes = arestes,
		.num_nodes = num_nodes,
		// Si al fitxer el numero de arestes que s'indica és superior al total, 
		// a num_arestes només guardem el numero d'arestes guardades a arestes
		.num_arestes = i,	
		.valid = 1,
	};
}

int main(int argc, char* argv[]) {
	Graf graf;

	if (argc == 2) {
		printf("Creant graf des del fitxer %s\n", argv[1]);
		graf = crearGrafFitxer(argv[1]);
	} else {
		printf("Creant graf predeterminat: Ponts de Konigsberg\n");
		// Graf predeterminat (Ponts de Königsberg)
		// Crea un graf amb els mateixos parametres que els exercicis anteriors
		Aresta arestes[] = {
			{0, 1, 0},
			{0, 2, 0},
			{0, 2, 0},
			{0, 3, 0},
			{0, 3, 0}, 
			{1, 2, 0}, 
			{1, 3, 0},
		};

		graf = (Graf) {
			.arestes = arestes,
			.num_arestes = 7,
			.num_nodes = 4,
			.valid = 1,
		};
	}

	if (!graf.valid) {
		return -1;
	}

	printf("Numero nodes: %u\n", graf.num_nodes);
	printf("Numero arestes: %u\n", graf.num_arestes);
	
    unsigned i, arestespos[MAXARNO], npos = 0, seguir = 0, NAR = graf.num_arestes, num_nodes = graf.num_nodes;
    Aresta *larestes = graf.arestes;
    
	EstructuraCami cami;
	
	printf("Node maxima valencia: %d\n", nodeValenciaMaxima(larestes, num_nodes, NAR));
	
    cami.naresvisit = 0;
    for (i = 0; i < NAR; i++) {
		printf("Aresta %d -> [%d,%d]\n", i, larestes[i].n1, larestes[i].n2);
    }
	
    printf("Node Inici: ");
    scanf("%u", &cami.nodes[0]);
	
    npos = arestesdelnode(cami.nodes[0], NAR, larestes, arestespos);
    seguir = 1;
	
    do {
		if (npos > 0) {
			printf("Teniu %d arestes per triar: \n", npos);
		    for (i = 0; i < npos; i++) {
				printf("%d -> [%d,%d]\n", arestespos[i],
					larestes[arestespos[i]].n1,
					larestes[arestespos[i]].n2);
				}
				
				printf("Trieu: ");
				unsigned node_desti;
				scanf("%u", &node_desti);
				
				unsigned node_actual = cami.nodes[cami.naresvisit];
				unsigned idx_aresta;
				if (existeixConnexio(node_actual, node_desti, arestespos, npos, larestes, &idx_aresta)) {
					cami.arvis[cami.naresvisit] = idx_aresta;
					larestes[idx_aresta].estat = 1;
					cami.naresvisit++;
					
					cami.nodes[cami.naresvisit] = node_desti;
					
					for (i = 0; i < cami.naresvisit; i++) {
						printf("%d -(%d)-> ", cami.nodes[i], cami.arvis[i]);
					}
					printf("%d\n", cami.nodes[cami.naresvisit]);
					
					npos = arestesdelnode(cami.nodes[cami.naresvisit], NAR, larestes, arestespos);
				} else {
					printf("No existeix connexio entre els nodes %d i %d!\n", node_actual, node_desti);
				}
			} else {
				printf("No queden arestes disponibles.\n");
				printf("Arestes visitades (%d):\n", cami.naresvisit);
				for (i = 0; i < cami.naresvisit; i++) {
					printf("%d ", cami.arvis[i]);
		    }
			
		    seguir = 0;
		}
    } while (seguir);
	
    printf("\n");
	free(larestes);
	return EXIT_SUCCESS;
}

