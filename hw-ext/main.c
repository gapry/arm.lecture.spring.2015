#include <stdio.h>
#include <immintrin.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#define REXP 1000
#define M_128 134217728
#define M_1	1024
#define NTERMS 28

/*
 * Ref: http://www.sbin.org/doc/glibc/libc_19.html
 */
#define M_PI 3.14159265358979323846264338327

static volatile double* __attribute__((aligned)) elapsed_time;

typedef void fs_func(void);

fs_func* pi_calculate(void);

typedef struct {
	const char* fname;
	fs_func* sign;
	const char* desc;
} fs_list;

void ms_baseline(void);
void ms_bbp(void);
void ms_bbp_avx(void);

void v_baseline(void);
void v_bbp(void);
void v_bbp_avx(void);

double baseline(size_t dt);
float bbp(int n);
float bbp_avx(int n);

#define FSM(n, d) {.fname=#n, .sign=n, .desc=d}

fs_list fl[] = {
	FSM(ms_baseline, "To measure  the execution time of baseline()."),
	FSM(ms_bbp, "To measure the execution time of bbp()."),
	FSM(ms_bbp_avx, "To measure the execution time of bbp_avx()."),
	FSM(v_baseline, "Verify the correctness."),
	FSM(v_bbp, "Verify the correctness."),
	FSM(v_bbp_avx, "Verify the correctness."),
};

__attribute__((constructor)) void time_init() {
	elapsed_time = malloc(sizeof(double) * REXP);

	int i = 0;
	while(i++ < REXP) {
		elapsed_time[i] = 0;
	}
}

void dbg_printf(const char* msg) {
	printf("%s\n", msg);
}

void v_baseline(void) {
	dbg_printf("Start to verify baseline.");
	FILE* fopt = fopen("./correctness/baseline.txt", "w");
	double pi = 0.0;

	int i = 0;
	for(; i < M_1; ++i) {
		pi = baseline(i);
		fprintf(fopt, "%d %lf\n", i, M_PI - pi);
	}
	
	dbg_printf("Complete the verification of baseline.");
}

void v_bbp(void) {
	dbg_printf("Start to verify bbp.");
	FILE* fopt = fopen("./correctness/bbp.txt", "w");
	float pi = 0.0;

	int i = 0;
	for(; i < NTERMS; ++i) {
		pi = bbp(i);
		fprintf(fopt, "%d %lf\n", i, M_PI - pi);
	}
	
	dbg_printf("Complete the verification of bbp.");

}

void v_bbp_avx(void) {
	dbg_printf("Start to verify bbp_avx.");
	FILE* fopt = fopen("./correctness/bbp_avx.txt", "w");
	float pi = 0.0;

	int i = 0;
	for(; i < NTERMS; ++i) {
		pi = bbp_avx(i);
		fprintf(fopt, "%d %lf\n", i, M_PI - pi);
	}
	
	dbg_printf("Complete the verification of bbp_avx.");
}

void ms_baseline(void) {
	dbg_printf("Start to measure baseline.");

	FILE* fopt = fopen("./benchmark/baseline.txt", "w");
	double pi = 0.0;
	clock_t t0, t1;

	int i = 0;
	for (; i < REXP; ++i) {
		t0 = clock();
		pi = baseline(M_1);		
		t1 = clock();
		elapsed_time[i] = (double)(t1 - t0) / CLOCKS_PER_SEC;
		fprintf(fopt, "%d %lf\n", i, elapsed_time[i]);
	}
	
	dbg_printf("Complete the process of measure baseline.");
}

void ms_bbp(void) {
	dbg_printf("Start to measure bbp.");

	FILE* fopt = fopen("./benchmark/bbp.txt", "w");
	double pi = 0.0;
	clock_t t0, t1;

	int i = 0;
	for (; i < REXP; ++i) {
		t0 = clock();
		pi = bbp(NTERMS);
		t1 = clock();
		elapsed_time[i] = (double)(t1 - t0) / CLOCKS_PER_SEC;
		fprintf(fopt, "%d %lf\n", i, elapsed_time[i]);
	}
	
	dbg_printf("Complete the process of measure bbp.");

}

void ms_bbp_avx(void) {
	dbg_printf("Start to measure baseline.");

	FILE* fopt = fopen("./benchmark/bbp_avx.txt", "w");
	double pi = 0.0;
	clock_t t0, t1;

	int i = 0;
	for (; i < REXP; ++i) {
		t0 = clock();
		pi = bbp_avx(NTERMS);		
		t1 = clock();
		elapsed_time[i] = (double)(t1 - t0) / CLOCKS_PER_SEC;
		fprintf(fopt, "%d %lf\n", i, elapsed_time[i]);
	}
	
	dbg_printf("Complete the process of measure bbp_avx.");
}

fs_func* pi_calculate(void) {
	int i = 0;
	
	for(; i < sizeof(fl) / sizeof(fl[0]); ++i) {
		fl[i].sign();
	}

	return NULL;
}

double baseline(size_t dt)
{
	double pi = 0.0;
   	double delta = 1.0 / dt;
	size_t i = 0;
    for (; i < dt; i++) {
        double x = (double) i / dt;
        pi += delta / (1.0 + x * x);
    }
    return pi * 4.0;
}

float bbp(int n) {
	float pi = 0.0, p = 0.0, b = 0.0, k = 0.0, m = 0.0, l = 0.0;
	__attribute__((aligned(4))) const int c = 8;
	int i = 0;

	for (; i < n; ++i) {
		p = 1.0 / pow(16, i);
		b = 4.0 / (c * i + 1);
		k = 2.0 / (c * i + 4);
		m = 1.0 / (c * i + 5);
		n = 1.0 / (c * i + 6);
		pi += p * (b - k - m - l);		
	}

	return pi;
}

float bbp_avx(int n) {
	float pi = 3.3 - 1.0 / 6;

	__attribute__((aligned(4))) const float c = 8.0;
	float c0 = c, c1 = c;	

	float p0 = 0.0, p1 = 0.0;

	register __m256 vec8, b, k, m, p;
	
	vec8 = _mm256_setzero_ps();

	int i = 1, j = 0;
	for(; i <= n; i += 2) {	
		c0 *= i;
		c1 *= (i + 1);
		b = _mm256_set_ps(c1, c1, c1, c1, c0, c0, c0, c0);
		vec8 = _mm256_add_ps(vec8, b);

		k = _mm256_set_ps(6.0, 5.0, 4.0, 1.0, 6.0, 5.0, 4.0, 1.0);
		vec8 = _mm256_add_ps(vec8, k);

		m = _mm256_set_ps(1.0, 1.0, 2.0, 4.0, 1.0, 1.0, 2.0, 4.0);
		vec8 = _mm256_div_ps(m, vec8);

		p0 = 1.0 / pow(16, i);
		p1 = 1.0 / pow(16, i + 1);
		p = _mm256_set_ps(-1 * p1, -1 * p1, -1 * p1, p1, -1 * p0, -1 * p0, -1 * p0, p0);
		vec8 = _mm256_mul_ps(p, vec8);

		__m256 result = vec8;

		for(j = 0; j < 8; ++j) {
			pi += ((float *) &result)[j];	
		}
	}	

	return pi;
}

int main(void) {
	pi_calculate();

	return 0;
}

__attribute__((destructor)) void time_destory() {
	free(elapsed_time);
}
