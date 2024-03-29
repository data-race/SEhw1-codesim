#include<iostream>
# include<algorithm>
# include <math.h>

int gptb2(float a, float b,  float * x1,float c, float * x2) {
  float del;
  del = b * b - (4 * a * c);
  if (del < 0) {
    return -1;
  } else { * x1 = (-b - sqrt(del)) / (2 * a); * x2 = (-b + sqrt(del)) / (2 * a);
  }
  return 1;
}
int main() {
  float a, b, c, x1, x2;
  scanf("%f%f%f", & a, & b, & c);
  fflush(stdin);
  if (a == 0) {
    printf("%.2f", -c / b);
  } else {
    if (gptb2(a, b, c, & x1, & x2) == -1) {
      printf("NaN");
    } else {
      printf("%.2f", x1);
      printf("\n%.2f", x2);
    }
  }
  getch();
  return 0;
}
