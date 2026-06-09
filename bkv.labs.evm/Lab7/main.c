#include <stdio.h>

extern void compute_sici(double x, double *out_si, double *out_ci);

int main() {
    printf("Интегральные синус Si(x) и косинус Ci(x)\n");
    printf("--------------------------------------------------\n");
    printf("%-8s | %-20s | %-20s\n", "x", "Si(x)", "Ci(x)");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i <= 100; i++) {
        double x = i * 0.1;
        double si_val, ci_val;
        
        compute_sici(x, &si_val, &ci_val);
        
        printf("%-8.1f | %-20.10f | %-20.10f\n", x, si_val, ci_val);
    }
    
    return 0;
}