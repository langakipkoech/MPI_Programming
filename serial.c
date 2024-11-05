//this is a serial code that evaluates trapezoidal rule

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){

    float a,b, x, h;
    int n; 
    float f(float x);

    float intergal, my_correct;

    //request user to input vlue of a and b
    printf("please enter value of a, b and n\n");
    scanf("%f %f %d", &a, &b, &n);
    
    //initialize the value of the integral
    intergal = (f(a) + f(b))/2;

    //compute the step size h
    h = (b-a)/n;

    //evaluate fore remaining subintervals
    for (float i = a + h ; i < b - h ; i += h ){
        intergal = intergal + f(i);
    }

    my_correct = intergal * h;
    //serial output
    printf(" the area under the curve is %f \n ", my_correct);



    return 0;
}

//implement the function f 

float f(float x){
    //determine return value
    float my_float;

    my_float = x*x;

    return my_float;

}