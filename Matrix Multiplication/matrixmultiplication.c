#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Defines
#define X(i, n)					((i)%(n))
#define Y(i, n)					((i)/(n))
#define INDEX_M(i, l, n)		(X(i, n) + Y(i, n)*(l))
#define INDEX(i)				INDEX_M(i, linha, n)

// Typedefs
typedef unsigned char byte;
typedef unsigned char err;
typedef unsigned int uint;

// Funções
void limpa(int *M, uint n);
void printam(int *M, uint n, char *label);
err soma(int *A, int *B, int *C, uint n);
err somaR(int *A, int *B, int *C, uint linha, uint n);
err subt(int *A, int *B, int *C, uint n);
err subtR(int *A, int *B, int *C, uint linha, uint n);
err mult(int *A, int *B, int *C, uint n);
err multR(int *A, int *B, int *C, uint linha, uint n);
err multS(int *A, int *B, int *C, uint linha, uint n, byte op);

// Configurações
int *A, *B, *C, *C2, *O;
uint n;

int stoi(char *input) {
	int num = 0;
	byte i = 0, flag = input[0]-'-' ? 1 : (i++, 0x81);
	while (flag&1) {
		char c = input[i++];
		if (!c) {
			flag &= ~1;
		} else if (c >= '0' && c <= '9') {
			num = num*10 + c - '0';
		}
	}
	return flag&0x80? -num : num;
}

err usage(char *argv0) {
	printf("Usage: %s <argumento> <numero>\n\n", argv0);
	puts("<argumeto> é um '-' seguido de uma letra. Possíveis argumentos:\n");

	puts("\t\t-n: o <numero> é o tamanho da matriz. <numero> é um número na forma 2^n, para qualquer n natural positivo e menor que 32, caso não seja da forma 2^n, vai ser escolhido um número próximo.\n");

	puts("\t\t-e: <numero> é o n em 2^n, e 2^n é o tamanho da matriz.");
	return 0;
}

int main(char argc, char **argv) {
	// Parsing das flags
	if (argc - 3) {
		usage(argv[0]);
		return 1;
	} else {
		if (argv[1][0] == '-' && !(argv[1][2])) {
			uint i = 0;
			switch(argv[1][1]) {
				case 'n':
					n = stoi(argv[2]);
					while (!(n & (1<<(31-(i++)))));
					i = 1 << (32-i);
					if (n-i) {
						printf("Número lido (%d) não está na forma 2^n vamos usar %d.\n", n, i);
						n = i;
					}
					break;
				case 'e':
					i = stoi(argv[2]);
					if (i >= 32) {
						printf("Número lido (%d) é muito grande, vamos usar 31.\n", i);
						i = 31;
					}
					n = 1 << i;
					break;
				default:
					printf("Argumento não reconhecido: %s", argv[1]);
					usage(argv[0]);
					return 3;
			}
		} else {
			puts("Argumentos invalidos");
			usage(argv[0]);
			return 3;
		}
	}

	time_t time, time2;

	A = malloc(sizeof(*A)*n*n);
	B = malloc(sizeof(*B)*n*n);
	C = malloc(sizeof(*C)*n*n);
	C2 = malloc(sizeof(*C2)*n*n);
	O = malloc(sizeof(*O)*n*n);

	for (register uint i = 0; i < n*n; i++) {
		A[i] = i;
		B[i] = n*n - i;
		O[i] = 0;
	}
	
	//printam(A, n, "A:");
	//printam(B, n, "B:");

	limpa(C2, n);

	time = clock();

	mult(A, B, C2, n);

	time = clock() - time;
	time2 = time;
	printf("A operação iterativa levou:\n%ld clocks\n%ld segundos\n\n", time, time/CLOCKS_PER_SEC);

	limpa(C, n);

	time = clock();

	multS(A, B, C, n, n, 0);

	time = clock() - time;
	printf("A operação mágica levou:\n%ld clocks\n%ld segundos\n\n", time, time/CLOCKS_PER_SEC);

	//printam(C, n, "MultS C:");

	// Calcula diferenças e só mostra se estiver diferente
	for (register uint i = 0, f = 1; i < n; i++) {
		if (C2[i] - C[i]) {
			if (f && f--) printf("Diff:\n");
			printf("C[%d][%d]\tTarget: %d\t Real: %d\n", i%n, i/n, C[i], C2[i]);
		}
	}

	time2 = time-time2;
	printf("A diferença de tempo:\n%ld clocks\n%ld segundos\n\n", time2, time2/CLOCKS_PER_SEC);
	printf("Obs:\nValores positivos favorecem a versão iterativa.\nValores negativos favorecem a versão mágica.\n\n");

	free(A);
	free(B);
	free(C);
	free(C2);
	free(O);

	return 0;
}

void limpa(int *M, uint n) {
	for (register uint i = 0; i < n*n; i++) {
		M[i] = 0;
	}
}

void printam(int *M, uint n, char *label) {
	printf("%s\n", label);
	for (register uint i = 0; i < n*n; i++) {
		printf("%d%c", M[i], (i+1)%n?9:10);
	}
	putchar(10);
}

err soma(int *A, int *B, int *C, uint n) {
	for (register uint i = 0; i < n*n; i++) {
		C[i] = A[i] + B[i];
	}
	return 0;
}

err somaR(int *A, int *B, int *C, uint linha, uint n) {
	for (register uint i = 0; i < n*n; i++) {
		C[INDEX(i)] = A[INDEX(i)] + B[INDEX(i)];
	}
	return 0;
}

err somaS(int *A, int *B, int *C, uint linha, uint n) {
	for (register uint i = 0; i < n*n; i++) {
		C[i] = A[INDEX(i)] + B[INDEX(i)];
	}
	return 0;
}

err subt(int *A, int *B, int *C, uint n) {
	for (register uint i = 0; i < n*n; i++) {
		C[i] = A[i] - B[i];
	}
	return 0;
}

err subtR(int *A, int *B, int *C, uint linha, uint n) {
	for (register uint i = 0; i < n*n; i++) {
		C[INDEX(i)] = A[INDEX(i)] - B[INDEX(i)];
	}
	return 0;
}

err subtS(int *A, int *B, int *C, uint linha, uint n) {
	for (register uint i = 0; i < n*n; i++) {
		C[i] = A[INDEX(i)] - B[INDEX(i)];
	}
	return 0;
}


err mult(int *A, int *B, int *C, uint n) {
	for (register uint i = 0; i < n; i++) {
		for (register uint j = 0; j < n; j++) {
			for (register uint k = 0; k < n; k++) {
				C[j+i*n] += A[k+i*n] * B[j+k*n];
			}
		}
	}
	return 0;
}

err multR(int *A, int *B, int *C, uint linha, uint n) {
	if (n <= 2) {
		C[INDEX(0)] += A[INDEX(0)] * B[INDEX(0)] + A[INDEX(1)] * B[INDEX(2)];
		C[INDEX(1)] += A[INDEX(0)] * B[INDEX(1)] + A[INDEX(1)] * B[INDEX(3)];
		C[INDEX(2)] += A[INDEX(2)] * B[INDEX(0)] + A[INDEX(3)] * B[INDEX(2)];
		C[INDEX(3)] += A[INDEX(2)] * B[INDEX(1)] + A[INDEX(3)] * B[INDEX(3)];
		return 0;
	}

	for (register uint i = 0; i < 4; i++) {
		uint novoComecoC = (i/2)*linha*(n/2)+(i&1)*(n/2);

		multR(A+(i/2)*linha*(n/2), B+(i&1)*(n/2), C+novoComecoC, linha, n/2);
		multR(A+(i/2)*linha*(n/2)+n/2, B+linha*(n/2)+(i&1)*(n/2), C+novoComecoC, linha, n/2);

	}

	return 0;
}

err copyS(int *From, int *To, uint linha, uint n) {
	for (register uint i = 0; i < n*n; i++) {
		To[INDEX(i)] = From[i];
	}
	return 0;
}

err multS(int *A, int *B, int *C, uint linha, uint n, byte op) {
	if (n <= 2) {
		C[op?0:INDEX(0)] = A[INDEX(0)] * B[INDEX(0)] + A[INDEX(1)] * B[INDEX(2)];
		C[op?1:INDEX(1)] = A[INDEX(0)] * B[INDEX(1)] + A[INDEX(1)] * B[INDEX(3)];
		C[op?2:INDEX(2)] = A[INDEX(2)] * B[INDEX(0)] + A[INDEX(3)] * B[INDEX(2)];
		C[op?3:INDEX(3)] = A[INDEX(2)] * B[INDEX(1)] + A[INDEX(3)] * B[INDEX(3)];
		return 0;
	}

	int *M[7], *tmp[2];

	{ // Pedir memória
	uint newsize = (n/2)*(n/2);
	for (register uint i = 0; i < 7; i++) {
		M[i] = malloc(sizeof(*(M[i]))*newsize);
		if (!M[i]) {
			printf("multS: n = %u: M[%u]: Not enought memory\n", n, i);
		}
	}

	for (register uint i = 0; i < 2; i++) {
		tmp[i] = malloc(sizeof(*(tmp[i]))*newsize);
		if (!tmp[i]) {
			printf("multS: n = %u: tmp[%u]: Not enought memory\n", n, i);
		}
	}

	}

	somaS(A, A+linha*(n/2)+(n/2), tmp[0], linha, n/2);
	somaS(B, B+linha*(n/2)+(n/2), tmp[1], linha, n/2);
	multS(tmp[0], tmp[1], M[0], n/2, n/2, 1);

	somaS(A+linha*(n/2), A+linha*(n/2)+(n/2), tmp[0], linha, n/2);
	somaS(B, O, tmp[1], linha, n/2);
	multS(tmp[0], tmp[1], M[1], n/2, n/2, 1);

	somaS(A, O, tmp[0], linha, n/2);
	subtS(B+(n/2), B+linha*(n/2)+(n/2), tmp[1], linha, n/2);
	multS(tmp[0], tmp[1], M[2], n/2, n/2, 1);

	somaS(A+linha*(n/2)+(n/2), O, tmp[0], linha, n/2);
	subtS(B+linha*(n/2), B, tmp[1], linha, n/2);
	multS(tmp[0], tmp[1], M[3], n/2, n/2, 1);

	somaS(A, A+(n/2), tmp[0], linha, n/2);
	somaS(B+linha*(n/2)+(n/2), O, tmp[1], linha, n/2);
	multS(tmp[0], tmp[1], M[4], n/2, n/2, 1);

	subtS(A+linha*(n/2), A, tmp[0], linha, n/2);
	somaS(B, B+(n/2), tmp[1], linha, n/2);
	multS(tmp[0], tmp[1], M[5], n/2, n/2, 1);

	subtS(A+(n/2), A+linha*(n/2)+(n/2), tmp[0], linha, n/2);
	somaS(B+linha*(n/2), B+linha*(n/2)+(n/2), tmp[1], linha, n/2);
	multS(tmp[0], tmp[1], M[6], n/2, n/2, 1);

	// Aqui para frente não tem diferença entre somaS e somaR
	somaS(M[0], M[3], tmp[0], n/2, n/2);
	subtS(M[6], M[4], tmp[1], n/2, n/2);
	somaS(tmp[0], tmp[1], tmp[0], n/2, n/2);
	copyS(tmp[0], C, linha, n/2);

	somaS(M[2], M[4], tmp[0], n/2, n/2);
	copyS(tmp[0], C+(n/2), linha, n/2);

	somaS(M[1], M[3], tmp[0], n/2, n/2);
	copyS(tmp[0], C+linha*(n/2), linha, n/2);

	subtS(M[0], M[1], tmp[0], n/2, n/2);
	somaS(M[2], M[5], tmp[1], n/2, n/2);
	somaS(tmp[0], tmp[1], tmp[0], n/2, n/2);
	copyS(tmp[0], C+linha*(n/2)+(n/2), linha, n/2);

	//printam(C, n, "MultS:");

	// Devolver memória
	for (register uint i = 0; i < 7; i++) {
		free(M[i]);
	}

	for (register uint i = 0; i < 2; i++) {
		free(tmp[i]);
	}

	return 0;
}
