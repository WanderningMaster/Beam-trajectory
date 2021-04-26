#include "Point.h"
#include <stdlib.h>

void get_drop_points(struct Point begin, struct Point end, struct Point *drops, double c, double c1, int count){

	double *coef_a;

    double a, x0 = begin.x, x = end.x, 
              y0 = begin.y, y = end.y;

    coef_a = (double *)malloc(sizeof(double)*(count));
    
    coef_a[0] = (y0-c)/(c1-c);

    int counter = count - 2;
    for(int i = 1; i <= counter; i++){

        coef_a[i] = 1;
    }
    if(count % 2 == 0){

        coef_a[count-1] = (y-c)/(c1-c);
    }
    else{

        coef_a[count-1] = (c1-y)/(c1-c);
    }
    
    double coef_sum = 0;
    for(int i = 0; i < count; i++){

        coef_sum += coef_a[i];
    }
    a = ( x - x0 ) / coef_sum;
    double curr_x = x0;
    for(int i = 0; i < count-1; i++){

        curr_x += (a * coef_a[i]);
        drops[i].x = curr_x;
        if(i % 2 == 0){

            drops[i].y = c;
        }
        else{

            drops[i].y = c1;
        }
        printf("Drop: (%lf, %lf)\n", drops[i].x, drops[i].y);
    }
}