#include <stdio.h>
#define MAXAR 7
#define MAXARNO 5

// Xavier Martin Tapia 1748951
// Daniel Genov Draganova 1703660

typedef struct {
    unsigned int n1;
    unsigned int n2;
    unsigned int estat;
} Aresta;

typedef struct {
    unsigned int nodes[MAXAR + 1];
    unsigned int arvis[MAXAR];
    int naresvisit;
} EstructuraCami;

unsigned arestesdelnode(unsigned int node, unsigned int NAR, Aresta larestes[], unsigned int llistap[]) {
    unsigned int i, nombre = 0;
    for (i = 0; i < NAR; i++) {
		if (larestes[i].estat == 0 && (larestes[i].n1 == node || larestes[i].n2 == node)) {
		    llistap[nombre] = i;
		    nombre++;
		}
    }
    
	return nombre;
}



int main() {
	unsigned int i, arestespos[MAXARNO], npos = 0, seguir = 0;
    unsigned int NAR = 7;
    Aresta larestes[] = {
		{0, 1, 0}, // Pont entre 0 i 1
		{0, 2, 0}, // Pont entre 0 i 2
		{0, 2, 0}, // Pont entre 0 i 2
		{0, 3, 0}, // Pont entre 0 i 3
		{0, 3, 0}, // Pont entre 0 i 3
		{1, 2, 0}, // Pont entre 1 i 2
		{1, 3, 0}  // Pont entre 1 i 3
	};
	
    EstructuraCami cami;
	for (i = 0; i < MAXAR; i++) {
		cami.nodes[i] = 0;
	}
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
			printf("Node acutal: %d\n", cami.nodes[cami.naresvisit]);
		    printf("Teniu %d arestes per triar: \n", npos);

		    for (i = 0; i < npos; i++) {
				printf("%d -> [%d,%d]\n", arestespos[i],
						larestes[arestespos[i]].n1,
						larestes[arestespos[i]].n2);
		    }
		
		    printf("Trieu: ");
			unsigned int node_desti;
			scanf("%u", &node_desti);
			//scanf("%u", &cami.arvis[cami.naresvisit]);
			
			unsigned int node_actual = cami.nodes[cami.naresvisit];
			unsigned int arestes_desti[MAXARNO];
			unsigned int numero_arestes_desti = arestesdelnode(node_desti, NAR, larestes, arestes_desti);
			
			int connexio_valida = existeixConnexio(node_actual, node_desti, arestes_desti,
												   numero_arestes_desti, larestes);
			
			if (connexio_valida) {

			}

		    larestes[cami.arvis[cami.naresvisit]].estat = 1;
		    (cami.naresvisit)++;
		
		    unsigned int aresta_anterior = cami.arvis[cami.naresvisit - 1]; 
			unsigned int node_anterior = cami.nodes[cami.naresvisit - 1];

			if (larestes[aresta_anterior].n1 == node_anterior) {
				cami.nodes[cami.naresvisit] = larestes[cami.arvis[(cami.naresvisit) - 1]].n2;
		    } else {
				cami.nodes[cami.naresvisit] = larestes[cami.arvis[(cami.naresvisit) - 1]].n1;
		    }
		
		    for (i = 0; i < cami.naresvisit; i++) {
				printf("%d -(%d)-> ", cami.nodes[i], cami.arvis[i]);
		    }
		    printf("%d\n", cami.nodes[cami.naresvisit]);
		    npos = arestesdelnode(cami.nodes[cami.naresvisit], NAR, larestes,
					  arestespos);
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
    return 0;
}
