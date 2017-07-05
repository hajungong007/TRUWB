#ifndef CALCUWBTAG_H
#define CALCUWBTAG_H

#include "trilateration.h"

class CalcUwbTag
{
public:
    CalcUwbTag();
    void initA(float a[6]);
    void initB(int b[4]);
    float F1(float x_t,float y_t,float z_t);

    float  F2(float x_t,float y_t,float z_t);
    float  F3(float x_t,float y_t,float z_t);
    float  F4(float x_t, float  y_t,float z_t);
    float diff_F_1_x_t(float x_t);
    float diff_F_2_x_t(float x_t);
    float diff_F_3_x_t (float x_t);
    float diff_F_4_x_t (float x_t);
    float diff_F_1_y_t(float y_t);
    float  diff_F_2_y_t (float y_t);
    float diff_F_3_y_t (float y_t);
    float diff_F_4_y_t(float y_t);
    float diff_F_1_z_t(float z_t);
    float diff_F_2_z_t(float z_t);
    float diff_F_3_z_t(float z_t);
    float diff_F_4_z_t(float z_t);
    void getResult(double *x,double *y,double *z);
    int getTagPostion(vec3d *best_solution, int use4thAnchor, vec3d* anchorArray, int *distanceArray);
private:
    float x_t;
    float y_t;
    float z_t;
    double b_1;
    double b_2;
    double b_3;
    double b_4;
    vec3d p_0;
    vec3d p_1;
    vec3d p_2;
    vec3d p_3;
};

#endif // CALCUWBTAG_H
