#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

using namespace std;

int picture[10][10] = { {19, 27, 35, 41, 56, 62, 77, 86, 10, 99}, 
                        {69, 73, 11, 10, 12, 23, 15, 88, 80, 63},
                        {19, 27, 35, 41, 56, 62, 77, 86, 10, 99}, 
                        {69, 73, 11, 10, 12, 23, 15, 88, 80, 63},
                        {19, 27, 35, 41, 56, 62, 77, 86, 10, 99} ,
                        {69, 73, 11, 10, 12, 23, 15, 88, 80, 63},
                        {19, 27, 35 ,41, 56, 62, 77, 86, 10, 99}, 
                        {69, 73, 11, 10, 12, 23, 15 ,88, 80, 63},
                        {19, 27, 35, 41 ,56 ,62 ,77 ,86, 10, 99}, 
                        {69, 73, 11 ,10, 12, 23, 15, 88 ,80 ,63}};

const double mask[5][5] =  {{0,   0,  -1,   0,  0},
                            {0 , -1 , -2 , -1 , 0},
                            {-1, -2 , 16,  -2 ,-1},
                            {0,  -1,  -2 , -1 , 0},
                            {0,  0 ,  -1 ,  0 , 0}};

void write_ip(const int height, const int width, const int border1, const int l1, const int picture[][10], const double mask[][5]);
int double2int32 (const double d);

int width = 10;
int height = 10;
int border1 = 3;
int l1 = 5;


int main ()
{
    write_ip(height, width, border1, l1, picture, mask);

    return 0;
}

void write_ip(const int height, const int width, const int border1, const int l1, const int picture[][10], const double mask[][5])
{
    FILE *laplas;
    laplas = fopen("/dev/xlnx,laplas", "w");

    fprintf(laplas, "%d, %d, %d, %d\n",height, width, border1, l1);
    printf(" [APP] %d, %d, %d, %d\n",height, width, border1, l1);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fprintf(laplas, "%d", picture[i][j]);
            printf(" [APP][Picture]: = %d, ", picture[i][j]);
        }
    }
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            fprintf(laplas, "%d", double2int32(mask[i][j]));
               printf(" [APP][Mask]: = %.3lf \n", mask[i][j]);
        }
    }

    
    fclose(laplas);
}

int double2int32 (const double d)
{
  double dd = d;
  dd *= 1 << 16;
  return (int) dd;
}