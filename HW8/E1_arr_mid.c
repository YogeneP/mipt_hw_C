#include <stdio.h>
#define ARR_SIZE 5

int arr[ARR_SIZE] = {0};

int main()
{
    char c = 0;
    char c_prev = ' ';
    int a = 0;
    int i = 0;
    float mid = .0;
    while (((c = getchar()) != '\n') && (c != EOF) && (c != '\0')) {
        if ((c >= '0') && (c <= '9')) {
            a = a*10 + (c-'0');
            if (c_prev == '-') a *= -1;
            c_prev = c;
            continue;
        } 
        
        if ((c == '-') && (c_prev == ' ')) {
            c_prev = c;
            continue;
        }
        
        if (c == ' ') {
            if (c_prev == ' ') continue;
            c_prev = c; 
            arr[i] = a;
            a = 0;
            mid += arr[i];
            i++;
            if (i >= ARR_SIZE) break;
            continue;
        }
        
        return 1;
    }
    if (i >= ARR_SIZE-1) {
        if ((i == ARR_SIZE-1) && (c_prev != ' ')) {
            arr[i] = a; // may be omitted, summing a directly
            mid += arr[i];
        } else { 
            return 2; 
        }
        mid /= (ARR_SIZE + .0);
        printf("%.3f", mid);
        return 0;
    } 
return 2;
}
