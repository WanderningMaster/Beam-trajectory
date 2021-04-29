#include "Point.h"
#include <stdlib.h>
#include <math.h>

#define PI 3.141592654

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

    free(coef_a);
}

double get_trajectory_length(struct Point begin, struct Point end, struct Point *drops, int count){

    struct Point current_point;
    double length = 0;

    if(count == 1){

        length = sqrt( pow(end.x-begin.x, 2) + pow(end.y-begin.y, 2) );
    }
    else{

        current_point.x = begin.x;
        current_point.y = begin.y;
        for(int i = 0; i<count-1; i++){

            length += sqrt( pow(drops[i].x-current_point.x, 2) + pow(drops[i].y-current_point.y, 2) );
            current_point.x = drops[i].x;
            current_point.y = drops[i].y;
        }

        length += sqrt( pow(end.x-current_point.x, 2) + pow(end.y-current_point.y, 2) );
    }
    printf("Length: %lf\n", length);

    return length;
}

double get_corner(struct Point begin, struct Point end, struct Point *drops, int count){
    double corner;
    printf("begin(%lf, %lf), end(%lf, %lf), drop(%lf, %lf)\n", begin.x, begin.y, end.x, end.y, drops[0].x, drops[0].y);
    /*corners[0] - begin corner, corners[1] - end corner*/
    corner = atan( ( begin.y - drops[0].y ) / ( abs(drops[0].x - begin.x) ) ); 
    
    /*converting from radians to degrees*/ 
    corner = (corner * 180) / PI;

    //printf("begin(%lf, %lf), end(%lf, %lf)\n", corners[0], corners[1]);

    return corner;
}