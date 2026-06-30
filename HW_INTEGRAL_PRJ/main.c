#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

//#define DEBUG  
#define INV_ARG_QUIT do { print_help(); exit(0); } while(0); 
#define EPSI_DEF 0.0001
#define EPSR_DEF 0.0001
#define MAX_F_NUM 4
#define A_DEF -10.0
#define B_DEF 10.0
#define GET_INTEGRAL_TASK !!(1 & task_flags)
#define GET_INTERSECTION_TASK !!(2 & task_flags)
#define GET_ITERATIONS_TASK !!(4 & task_flags)
#define GET_POINTS_TASK !!(8 & task_flags)
#define SET_INTEGRAL_TASK task_flags |= 1; 
#define SET_INTERSECTION_TASK task_flags |= 2;
#define SET_ITERATIONS_TASK task_flags |= 4;
#define SET_POINTS_TASK task_flags |= 8;
#define R_INIT_SIZE 2
#define R_MAX_SIZE 255

typedef struct { 
    int f_num;
    int g_num;
    float x;
    float y;
} rpoint_t;

rpoint_t* r_points;
size_t r_points_size;

/*
task_flags bits assignment:
0: single integral calculation
1: single intersection point calculation
2: print iteration steps
3: print intersection points
*/
unsigned short int task_flags = 0;

float f0(float x);
float f1(float x);
float f2(float x);
float f3(float x);
float fx2(float x);
int storeIntersectionPoint(rpoint_t* r_points_arr, int r_i, float (*f[])(float), int fi, int gi, float a, float b, float e);
float getIntersectionX(float(*f)(float), float(*g)(float), float a, float b, float eps1);
float getIntegral(float(*f)(float), float a, float b, float eps2);
float ifnparse(int argc, char** argv, int *i);
float fparse(int argc, char** argv, int *i);
void checkParam(float param, char argn);
void fswap(float* a, float* b);
void swap(int* a, int* b);
void print_help(void);
int comp_points(const void* vp1, const void* vp2);
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

    float epsi = 0; // integral calculation accuracy
    float epsr = 0; // intersection point search accuracy
    float eps = 0; // shape area calculation accuracy
    float a = FLT_MAX; //left limit
    float b = FLT_MAX; //right limit
    int f_num = __INT16_MAX__; // first selected function number
    int g_num = __INT16_MAX__; // second selected function number

    while(i < argc) {
        if (argv[i][0] == '-') {
            #ifdef DEBUG
                printf("Argument processing: %s\n", argv[i]);
            #endif
            switch (argv[i][1]) {
                case 'a':   // set left range limit
                    a = fparse(argc,argv,&i);
                    checkParam(a, 'a');
                    break;
                case 'b':   // set right range limit 
                    b = fparse(argc,argv,&i);
                    checkParam(b, 'b');
                    break;
                case 'f':   // select first f(x) option
                    float fn = ifnparse(argc,argv,&i);
                    checkParam(fn, 'f');
                    f_num = (int)fn;
                    break;
                case 'g':   // select second g(x) option
                    float gn = ifnparse(argc,argv,&i);
                    checkParam(gn, 'g');
                    g_num = (int)gn;
                    break;
                case 'p':   // print all intersection points of complex shape area calculation
                    SET_POINTS_TASK;
                    break;
                case 'n':   // print iterations proceeded on calculations
                    SET_ITERATIONS_TASK;
                    break;
                case 'i':   // enable integral of f(x) on [a, b] range calculation
//**** CHECK epsi and epsr assignment - runs infinitely if specified*/
                    SET_INTEGRAL_TASK;
                    epsi = fparse(argc,argv,&i);
                    if(epsi == -FLT_MAX) { 
                        epsi = FLT_MAX; 
                    }
                    break;
                case 'r':   // enable f(x)^g(x) intersection on [a, b] range calculation
                    SET_INTERSECTION_TASK;
                    epsr = fparse(argc,argv,&i);
                    if(epsr == -FLT_MAX) { 
                        epsr = FLT_MAX; 
                    }
                    break;
                case 'e':   // set shape area calculation accuracy
                    eps = fparse(argc,argv,&i);
                    checkParam(eps, 'e');
                    break;
                default:
                    printf("Invalid key: -%c", argv[i][1]);                    
                    INV_ARG_QUIT;
            } 
        i++;
        }
    }

    if(a == FLT_MAX) {
        printf("Limit a is not specified, proceeding with default a = %.4f\n", A_DEF);
        a = A_DEF;
    }
    if(b == FLT_MAX) {
        printf("Limit b is not specified, proceeding with default b = %.4f\n", B_DEF);
        b = B_DEF;
    }

//    if((f_num > MAX_F_NUM) && (g_num > MAX_F_NUM)) {
//        goto AREA;
//    }

    if(GET_INTEGRAL_TASK) {
        if(f_num <= MAX_F_NUM) {
            printf("\nCalculating integral value of f(x)=%s in the range of [%.4g,%.4g].\n", f_str[f_num], a < b ? a : b, a < b ? b : a);
            if(epsi == FLT_MAX) {
                printf("Target accuracy is not specified, proceeding with default eps = %.4g\n", EPSI_DEF);
                epsi = EPSI_DEF;
            } else printf ("Target accuracy: %.4g \n", epsi);
            float res = getIntegral(f[f_num],a,b,epsi);
            printf("Result: %.4g\n", res);
        } else printf("No function f(x) selected for integral value calculation\n");
    }

    if(GET_INTERSECTION_TASK) {
        if((f_num > MAX_F_NUM) || (g_num > MAX_F_NUM)) {
            printf("To search an intersection point both of functions f(x) and g(x) must be specified.\n");
            exit(1);
        } 
        if(f_num == g_num) {
            printf("Can't find an intersection point of the function with itself (f(x)[%d] = g(x)[%d])\n",f_num,g_num);
            exit(1);
        }
        
        if(epsr == FLT_MAX) {
            printf("Target accuracy is not specified, proceeding with default eps = %.4g\n", EPSR_DEF);
            epsr = EPSI_DEF;
        }
        printf("\nSearching of the intersection point of f(x)=%s and g(x)=%s in the range of [%.4g,%.4g]. Accuracy: %.4g \n", f_str[f_num], f_str[g_num], a, b, epsr);
        float resx = getIntersectionX(f[f_num],f[g_num],a,b,epsr);
        printf("The point coordinates: (%.4g, %.4g)\n", resx, f[f_num](resx));
    }

    /****  Shape area calculation ****/
    if(GET_INTEGRAL_TASK || GET_INTERSECTION_TASK) exit(0);
    printf("\n");
    if(a > 0 || b < 0) {
        printf("The calculation proceeds in two ranges [a;0] and [0;b].\nThere must be a<0 and b>0.");
        exit(1);
    }

    r_points_size = R_INIT_SIZE;
    r_points = (rpoint_t*) malloc((sizeof(rpoint_t)*r_points_size));
    unsigned int p_num = 0;
    unsigned int inter_res = 0;

    // Fill the points array with intersection ponts (incl f(x) = 0)) in negative and positive ranges.
    // Next sort them
    // Proceed from left to right
    if (GET_ITERATIONS_TASK) printf("\nIntersections search:\n");
    for (int fi1 = 1; fi1 < MAX_F_NUM-1; fi1++) {
        for (int fi2 = fi1 + 1; fi2 < MAX_F_NUM; fi2++) {
            if ((p_num + 2) > r_points_size)  {
                r_points_size = 2 * r_points_size; 
                if(r_points_size > R_MAX_SIZE) {
                    printf("Maximal intersection points count exceeded (%d)!\n", R_MAX_SIZE);
                    exit(1);
                }
                void* temp_p;
                if (
                    temp_p = realloc(r_points, sizeof(rpoint_t) * r_points_size)
                ) { 
                    r_points = (rpoint_t*)temp_p;
                } else {
                    printf("Memory allocation error!\n");
                    exit(-1);
                } 
            }
            if (GET_ITERATIONS_TASK) {
                printf("Intersection of ");
                printf("f(x) = %s [%d]; g(x) = %s [%d] at ", f_str[fi1], fi1, f_str[fi2], fi2);
                printf("[%.2f, %.2f]: ", (float)a, (float)0);
            }
            inter_res = storeIntersectionPoint(r_points, p_num, f, fi1, fi2, a, 0, eps);
            if (inter_res) {
                if(GET_ITERATIONS_TASK) { printf("found at x = %.4f\n", r_points[p_num].x); }
                p_num += inter_res;
            } else if(GET_ITERATIONS_TASK) printf("not found\n");
            if (GET_ITERATIONS_TASK) {
                printf("Intersection of ");
                printf("f(x) = %s [%d]; g(x) = %s [%d] at ", f_str[fi1], fi1, f_str[fi2], fi2);
                printf("[%.2f, %.2f]: ", (float)0, (float)b);
            }
            inter_res = storeIntersectionPoint(r_points, p_num, f, fi1, fi2, 0, b, eps);
            if (inter_res) {
                if(GET_ITERATIONS_TASK) { printf("found at x = %.4f\n", r_points[p_num].x); }
                p_num += inter_res;
            } else if(GET_ITERATIONS_TASK) printf("not found\n");
        }
    }

    if(p_num < 2) {
        printf("Too few intersection points to form a shape found!");
        exit(1);
    }

    qsort(r_points, p_num, sizeof(rpoint_t), comp_points);
    printf("Intersection points found: %d\n", p_num);

    printf("\nArea calculation:\n");
    float area_total = 0.0;
    int fi = r_points[0].f_num; //must be top border of the shape segment 
    int gi = r_points[0].g_num; //must be bottom border of the shape segment
    for(int p_i = 0; p_i < p_num - 1; p_i++) {
        if(GET_ITERATIONS_TASK) {
            printf("Segment %d: \n", p_i);
            printf("Top border: f(x)=%s; bottom border: g(x)=%s \n", f_str[fi], f_str[gi]);
            printf("Limits: [%.2f; %.2f]\n", r_points[p_i].x, r_points[p_i+1].x);
        }
        #ifdef DEBUG 
            printf("p_i = %d; fi = %d; fi[p_i] = %d; gi = %d; gi[p_i] = %d\n", p_i, fi, r_points[p_i].f_num, gi, r_points[p_i].g_num);
        #endif
        if(fi == r_points[p_i].f_num && gi == r_points[p_i].g_num && p_i > 0) { //if top border crosses bottom border => end of shape
            printf("End of shape reached with internal crossing detected on point %d.\n", p_i+1); //means end of shape, even if more intersection points left on the right o_O
            break;
        }

        float x_mid = (r_points[p_i].x + r_points[p_i+1].x)/2;
        if(f[fi](x_mid) < f[gi](x_mid)) swap(&fi,&gi); //ensure f[fi]() on top, f[gi]() - in the bottom  
        #ifdef DEBUG 
            printf("p_i = %d; fi = %d; fi[p_i] = %d; gi = %d; gi[p_i] = %d\n", p_i, fi, r_points[p_i].f_num, gi, r_points[p_i].g_num);
        #endif
        float a1 = getIntegral(f[fi], r_points[p_i].x, r_points[p_i+1].x, eps);
        float a2 = getIntegral(f[gi], r_points[p_i].x, r_points[p_i+1].x, eps);
        float a = a1-a2;
        area_total += a;

        if(fi > gi) swap(&fi, &gi); //in r_points f_num < g_num; arranging to avoid the comparison of every of fi and gi with every of f_num, g_num 
        if(fi == r_points[p_i+1].f_num) fi = r_points[p_i+1].g_num;
        if(gi == r_points[p_i+1].g_num) gi = r_points[p_i+1].f_num;
        if(fi > gi) swap(&fi, &gi);
        printf("Segment %d area: %f \n\n", p_i, a);
    }

    printf("Total area of the shape: %f \n\n", area_total);

    exit(0);
}

int storeIntersectionPoint(rpoint_t* r_points_arr, int r_i, float (*f[])(float), int fi, int gi, float a, float b, float e) {
    r_points_arr[r_i].x = getIntersectionX(f[fi], f[gi], a, b, e);
    if (r_points_arr[r_i].x < FLT_MAX) {
        r_points_arr[r_i].f_num = fi; 
        r_points_arr[r_i].g_num = gi;
        r_points_arr[r_i].y = f[fi](r_points_arr[r_i].x);
        return 1;
    }
    return 0;
}

float f0(float x) {
    return .0;
}

float f1(float x) {
    return 0.6*x + 3;
}

float f2(float x) {
    return (x-2)*(x-2)*(x-2) - 1;
}

float f3(float x) {
    return 3/x;
}

float fx2(float x) {
    return x*x*x;
}

// safe calculation of function *f e.g. 1/x regarding break points
// appr - calculation x approach
// doesn't work with not defined f(x) (e.g. sqrt(x) at x<0) and with f(x) infinite on a range  
float safe_f(float(*f)(float), float x, int appr) {
    float fx = f(x);
    if( !isinf(fx) || appr == 0) {
        return fx;
    }  
    return appr > 0 ? FLT_MAX : -FLT_MAX;
}

float fparse(int argc, char** argv, int *i) {
    (*i)++;
    if(*i >= argc) { 
        return -FLT_MAX; 
    }
    char* f_end_ch = 0;
    float n = strtof(argv[*i], &f_end_ch);
    if(f_end_ch == argv[*i]) { 
        (*i)--;
        return FLT_MAX; 
    }
    return n;
}

float ifnparse(int argc, char** argv, int *i) {
    (*i)++;
    if(*i >= argc) { 
        return -FLT_MAX; // value missing
    }
    if(argv[*i][0] < '0' || argv[*i][0] > MAX_F_NUM + '0' || argv[*i][1] != 0) { 
        (*i)--;
        return FLT_MAX; // invalid value
    }
    return (float)(argv[*i][0] - '0');
}

void checkParam(float param, char argn) {
    if(param == -FLT_MAX) {
        printf("Parameter value missing: -%c\n", argn);
        INV_ARG_QUIT;
    } 
    if(param == FLT_MAX) {
        printf("Invalid parameter value format: -%c\n", argn);
        INV_ARG_QUIT;
    }
}

float getIntersectionX(float (*f)(float), float (*g)(float), float a, float b, float eps1) {
    if(a == b) return FLT_MAX;
    if(b < a) fswap(&a,&b);
    float fga = safe_f(f,a,1) - safe_f(g,a,1);
    float fgb = safe_f(f,b,-1) - safe_f(g,b,-1);
    if (((fga > .0) && (fgb > .0)) || ((fga < .0) && (fgb < .0))) {
//        if (GET_ITERATIONS_TASK) {
//            printf("No or multiple intersections in the range: \n f(a) - g(a) = %f - %f = %f\n f(b) - g(b) = %f - %f = %f\n", f(a), g(a), fga, f(b), g(b), fgb);
//       }
        return FLT_MAX;
    }
    if (fga == .0) return a;
    if (fgb == .0) return b;

    #ifdef DEBUG
        unsigned int i;
    #endif

    float x = (a+b) / 2;
    float x_prev = FLT_MAX;
    float fgx = FLT_MAX;
    while (fabs(x_prev - x) > eps1) {
        float fx = f(x);
        fgx = safe_f(f,x,-1) - safe_f(g,x,-1); 
        #ifdef DEBUG
            printf("%d. a = %f; b = %f; x = %f; fga = %f; fgb = %f; fgx = %f \n", i++, a, b, x, fga, fgb, fgx);
        #endif
        if(fgx == .0) {
            if(GET_ITERATIONS_TASK) printf("f(x) - g(x) = 0, exact match at x = %.4g \n", x);
            break;
        }
        x_prev = x;
        if((fgx > .0 && fga < .0) || (fgx < .0 && fga > .0)) {
            b = x;
            #ifdef DEBUG
                fgb = fgx;
            #endif
        } else {
            a = x;
            #ifdef DEBUG
                fga = fgx;
            #endif
        }
        x = (a+b) / 2;
    }
    #ifdef DEBUG
        printf(" a = %f; b = %f; x = %f; fga = %f; fgb = %f; fgx = %f \n", a, b, x, fga, fgb, fgx);
    #endif
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
            #ifdef DEBUG 
//                printf("%d. step: %f; f(%f) = %f; res = %f\n", i, step, x, f(x), res);
            #endif
            x += step;
        }
        if(GET_ITERATIONS_TASK) printf("%d. step: %f; result: %f\n", i, step, res);
        i++;
        eps_prev = eps;
        eps = fabs(res_prev - res);
        if (eps > eps_prev) {
            if(GET_ITERATIONS_TASK) printf("critical accuracy for the implemented calculation methode reached: %f\n", eps_prev);
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

void swap(int* a, int* b) {
    int t = *a;
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
    printf("To proceed with area calculation omit any -i or -r tasks \n\n");
    printf("Additional options control:\n");
    printf("\t-f N: select first function f(x) to process\n");
    printf("\t-g N: select second function g(x) to process\n");
    printf("\t-i ACC: calculate integral f(x) on [a,b] range\n");
    printf("\t\t f(x) must be selected with -f key\n");
    printf("\t\t optional ACC value specifies target accuracy\n");
    printf("\t-r ACC: find intesection point of f(x) and g(x) located inside [a, b] range.\n");
    printf("\t\t f(x) and g(x) must be selected by -f and -g keys accordingly\n");
    printf("\t\t optional ACC value specifies target accuracy\n");
    printf("\t-a: set left border of calculation range\n");
    printf("\t-b: set right border of calculation range\n");
    printf("\t-n: on every calculation print the run of iterations\n");
}

void cleanup(void) {
    free(r_points);
}