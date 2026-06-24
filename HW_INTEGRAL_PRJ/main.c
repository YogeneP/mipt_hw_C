#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define D  
#define INV_ARG_QUIT do { print_help(); exit(0); } while(0); 
#define EPSI_DEF 0.0001
#define EPSR_DEF 0.0001
#define MAX_F_NUM 4
#define NEGATIVE_R_LIMIT -10
#define POSITIVE_R_LIMIT 10
#define GET_INTEGRAL_TASK !!(1 & task_flags)
#define GET_INTERSECTION_TASK !!(2 & task_flags)
#define GET_ITERATIONS_TASK !!(4 & task_flags)
#define GET_POINTS_TASK !!(8 & task_flags)
#define SET_INTEGRAL_TASK task_flags |= 1; 
#define SET_INTERSECTION_TASK task_flags |= 2;
#define SET_ITERATIONS_TASK task_flags |= 4;
#define SET_POINTS_TASK task_flags |= 8;
#define R_INIT_SIZE 10

typedef struct { 
    int f_num;
    int g_num;
    float a;
    float b;
    float eps;
} params_t;

typedef struct { 
    int f_num;
    int g_num;
    float x;
    float y;
} rpoint_t;

rpoint_t* r_points;
size_t r_points_size;

float f0(float x);
float f1(float x);
float f2(float x);
float f3(float x);
float fx2(float x);
float getIntersectionX(float(*f)(float), float(*g)(float), float a, float b, float eps1);
float getIntegral(float(*f)(float), float a, float b, float eps2);
float ifnparse(int argc, char** argv, int *i);
float fparse(int argc, char** argv, int *i);
int parseParameters(int ac, char** av, int index, int parse_f2, params_t params);
void checkParam(float param, char argn);
void fswap(float* a, float* b);
void print_help(void);
void cleanup(void);

int main(int argc, char** argv) {
// ***** Args and parameters parsing ******
    int i = 1;
    float(*f[])(float) = {f0, f1, f2, f3, fx2};

    char* f_str[] = {
        "0", 
        "0.6*x + 3",
        "(x-2)^3 - 1",
        "3/x",
        "x^3"
    };
    params_t params = {INT_MAX,INT_MAX,FLT_MAX,FLT_MAX,FLT_MAX}; 

    unsigned int task_flags = 0; //
    float epsi = EPSI_DEF;
    float epsr = EPSR_DEF;
    float a = FLT_MAX;
    float b = FLT_MAX;
    int fn = 0;

    while(i < argc) {
        if (argv[i][0] == '-') {
            D printf("Argument processing: %s\n", argv[i]);
            switch ((argv[i])[1]) {
                case 'a':   // set left range limit
                    params.a = fparse(argc,argv,&i);
                    checkParam(params.a, 'a');
                    break;
                case 'b':   // set right range limit 
                    params.b = fparse(argc,argv,&i);
                    checkParam(params.b, 'b');
                    break;
                case 'f':   // select first f(x) option
                    float fn = ifnparse(argc,argv,&i);
                    checkParam(fn, 'f');
                    params.f_num = (int)fn;
                    break;
                case 'g':   // select second g(x) option
                    float gn = ifnparse(argc,argv,&i);
                    checkParam(fn, 'g');
                    params.g_num = (int)fn;
                    break;
                case 'p':   // print all intersection points of complex shape area calculation
                    SET_POINTS_TASK;
                    break;
                case 'n':   // print iterations proceeded on calculations
                    SET_ITERATIONS_TASK;
                    break;
                case 'i':   // enable integral of f(x) on [a, b] range calculation
                    checkParam(epsi, 'i');
                    SET_INTEGRAL_TASK;
                    break;
                case 'r':   // enable f(x)^g(x) intersection on [a, b] range calculation
                    checkParam(epsr, 'r');
                    SET_INTERSECTION_TASK;
                    break;
                default:
                    INV_ARG_QUIT;
            } 
        i++;
        }
        INV_ARG_QUIT;
    }

    r_points_size = R_INIT_SIZE;
    r_points = (rpoint_t*) malloc((sizeof(rpoint_t)*r_points_size));
    unsigned int p_num = 0;

    // Fill the points array with intersection ponts (incl f(x) = 0)) in negative and positive ranges.
    // Next sort them
    // Proceed from left to right
        for (int fi1 = 1; fi1 < MAX_F_NUM-1; fi1++) {
        for (int fi2 = fi1 + 1; fi2 < MAX_F_NUM; fi2++) {
            if ((p_num + 2) > r_points_size)  { 
                r_points_size = 2 * r_points_size;
                realloc(r_points, sizeof(rpoint_t) * r_points_size); 
            }
            storeIntersectionPoint(r_points, f, fi1, fi2, NEGATIVE_R_LIMIT, 0, epsr); 
            p_num = storeIntersectionPoint(r_points, f, fi1, fi2, 0, POSITIVE_R_LIMIT, epsr);
        }
    }

    qsort(r_points, p_num, sizeof(rpoint_t), comp_points);
    char f_comb[][2] = {
        {1,3},
        {1,2},
        {3,2}
    }; 
    float area_total = 0.0;
    int fi = 0;
    int gi = 0;
    for(int p_i = 0; p_i < p_num - 1; p_i++) {
//GO HERE!!!

        if(r_points[p_i].f_num )
        float a1 = getIntegral(f[r_points[p_i].f_num], r_points[p_i].x, r_points[p_i+1].x, epsi);
        float a2 = getIntegral(f[r_points[p_i].g_num], r_points[p_i].x, r_points[p_i+1].x, epsi);
        area_total += abs(a1-a2);

    }

    float inter = getIntersectionX(fx2, f0, 0, 10, 0.0001);
    //    float integral = getIntegral(f2, a, b, 0.001);
        printf("f(x) = %f | g(x) = %f -> x = %f\n", fx2(inter), f0(inter), inter);
    //    printf("integral f(%f|%f) = %f\n", a, b, integral);
        exit(0);
}

unsigned int storeIntersectionPoint(rpoint_t* r_points_arr, float (*f[])(float), int fi, int gi, float a, float b, float e) {
    static unsigned int r_i = 0;
    r_points_arr[r_i].x = getIntersectionX(f[fi], f[gi], a, b, e);
    if (r_points_arr[r_i].x < FLT_MAX) {
        r_points_arr[r_i].f_num = fi;
        r_points_arr[r_i].f_num = gi;
        r_points_arr[r_i].y = f[fi](r_points_arr[r_i].x);
        r_i++;
    }
    return r_i - 1;     
}

float f0(float x) {
    return .0;
}

float f1(float x) {
    return 0.6*x + 3;
}

float f2(float x) {
    return pow(x-2,3) - 1;
}

float f3(float x) {
    return 3/x;
}

float fx2(float x) {
    return x*x*x;
}

float fparse(int argc, char** argv, int *i) {
    (*i)++;
    if(*i >= argc) { 
        return -FLT_MAX; 
    }
    char* f_end_ch = 0;
    float n = strtof(argv[*i], &f_end_ch);
    if(f_end_ch == argv[*i]) { 
        return FLT_MAX; 
    }
    return n;
}

float ifnparse(int argc, char** argv, int *i) {
    (*i)++;
    if(*i >= argc) { 
        return -FLT_MAX;
    }
    if(argv[*i][0] < '0' || argv[*i][0] > MAX_F_NUM + '0' || argv[*i][1] != 0) { 
        return FLT_MIN; 
    }
    return (float)(argv[*i][0] - '0');
}

void checkParam(float param, char argn) {
    if(param == -FLT_MIN) {
        printf("Parameter value missing: -%c\n", argn);
        INV_ARG_QUIT;
    } 
    if(param == FLT_MIN) {
        printf("Invalid parameter value format: -%c\n", argn);
        INV_ARG_QUIT;
    }
}

float getIntersectionX(float (*f)(float), float (*g)(float), float a, float b, float eps1) {
    printf("Calculating intersection point in a range [%f, %f]; \n", a, b);
    if(a == b) return FLT_MAX;
    if(b < a) fswap(&a,&b);
    float fga = f(a) - g(a);
    float fgb = f(b) - g(b);
    if (((fga > .0) && (fgb > .0)) || ((fga < .0) && (fgb < .0))) {
        printf("no intersection or even intersections count in the range: \n f(a) - g(a) = %f - %f = %f\n f(b) - g(b) = %f - %f = %f\n", f(a), g(a), fga, f(b), g(b), fgb);
        return FLT_MAX;
    }
    if (fga == .0) return a;
    if (fgb == .0) return b;

    int i = 0;
    float x = (a+b) / 2;
    float x_prev = FLT_MAX;
    float fgx = FLT_MAX;
    while (fabs(x_prev - x) > eps1) {
        fgx = f(x) - g(x); 
        printf("%d. a = %f; b = %f; x = %f; fga = %f; fgb = %f; fgx = %f \n", i++, a, b, x, fga, fgb, fgx);
        if(fgx == .0) {
            printf("fgx = 0, match");
            break;
        }
        x_prev = x;
        if((fgx > .0 && fga < .0) || (fgx < .0 && fga > .0)) {
            b = x;
            D fgb = fgx;
        } else {
            a = x;
            D fga = fgx;
        }
        x = (a+b) / 2;

    }
    return x;
}

float getIntegral(float(*f)(float), float a, float b, float eps2) {
    if(a == b) return .0;
    if(b < a) fswap(&a,&b);
    float step = (b - a) / 2;
    float res = .0;
    float res_prev = FLT_MAX;
    float eps = fabs(res_prev - res);
    float eps_prev = FLT_MAX;
    float x = .0;
    int i = 0;
    while (eps > eps2) {
        x = a + step/2;
        res_prev = res; 
        res = .0;
        while(x <= b) {
            res += step * f(x);
//            printf("%d. step: %f; f(%f) = %f; res = %f\n", i, step, x, f(x), res);
            x += step;
        }
        printf("%d. step: %f; res = %f\n", i, step, res);
        i++;
        eps_prev = eps;
        eps = fabs(res_prev - res);
        if (eps > eps_prev) {
            printf("critical accuracy for the implemented calculation methode reached: %f\n", eps_prev);
            res = res_prev;
            break;
        }
        step /= 2;
    }

    return res;
}

void fswap(float* a, float* b) {
    float t = *a;
    *a = *b;
    *b = t;
}

int comp_points(const void* vp1, const void* vp2) {
    rpoint_t* p1 = (rpoint_t*)vp1;
    rpoint_t* p2 = (rpoint_t*)vp2;
    return (p1->x < p2->x) ? -1 : ((p1->x > p2->x) ? 1 : 0);
}

 void print_help(void) {
    printf("The main feature is the calculation of the area of the shape limited by three predefined functions:\n");
    printf("\t\t\t1. f(x) = 0.6*x + 3\n");
    printf("\t\t\t2. f(x) = (x-2)^3 - 1\n");
    printf("\t\t\t3. f(x) = 3 / x\n\n");
    printf("Limitation of fuctions use: the number of the points of intersection between any pair\n of functions must be not more than one in every positive and negative ranges.\n\n");

    printf("Additional options control:\n");
    printf("\t-f: select first function to process with\n");
    printf("\t-g: select second function to process with\n");
    printf("\t-i: calculate integral f(x) on [a,b] range\n");
    printf("\t-r: find intesection point of f(x) and g(x) located inside [a, b] range.");
    printf("\t-a: set left border of calculation range\n");
    printf("\t-b: set right border of calculation range\n");
    printf("\t-n: on every calculation print the count of iterations passed");
}

void cleanup(void) {
    free(r_points);
}