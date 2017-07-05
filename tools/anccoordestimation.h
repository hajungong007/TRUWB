#ifndef ANCCOORDESTIMATION_H
#define ANCCOORDESTIMATION_H

#include "trilateration.h"
class AncCoordEstimation
{
public:
    AncCoordEstimation();
    float F_1(float x_1);

    float F_2(float x_2,float y_2);

    //float F_3(float x_3,float x_2,float y_3,float y_2);
    float F_3(float x_1,float x_2,float y_2);

    //float F_4(float x_3,float y_3);

    float F_5(float x_t,float y_t);

    float F_6(float x_t,float x_1,float y_t);

    float F_7(float x_t,float x_2,float y_t,float y_2);

    //float F_8(float x_t,float x_3,float y_t,float y_3);

    //float F_9(float x_1,float y_2);


    float diff_F1_x_1(float x_1);

    float diff_F3_x_1(float x_1,float x_2);

    float diff_F6_x_1(float x_1,float x_t);

    float diff_F2_x_2(float x_2);

    float diff_F3_x_2(float x_1,float x_2);

    float diff_F7_x_2(float x_2,float x_t);

    float diff_F5_x_t(float x_t);

    float diff_F6_x_t(float x_1,float x_t);

    float diff_F7_x_t(float x_2,float x_t);

    float diff_F2_y_2(float y_2);

    float diff_F3_y_2(float y_2);

    float diff_F7_y_2(float y_2,float y_t);

    float diff_F5_y_t(float y_t);

    float diff_F6_y_t(float y_t);

    float diff_F7_y_t(float y_2,float y_t);

    void setInitCoord(float coord[7]);
    void setInitA(float a[3]);
    void setInitB(float b[4]);

    int getUwbEstimate(float *delta,int *COUNT);


    //F3 calibration
    float F3_Ft(float x_3,float y_3,float x_t_ave,float y_t_ave,float r_ave);
    float F3_Ft_diff_x_3(float x_3,float x_t);
    float F3_Ft_diff_y_3(float y_3,float y_t);

    int getUwbEstimate_x3_y3(float x_3,float y_3,vec3d avgArry[3],float *delta,int *COUNT_3);


public:
    float a_1;
    float a_2;
    float a_3;
    float a_4;
    float b_1;
    float b_2;
    float b_3;
    float b_4;


    float x_1;
    float x_2;
    float x_3;
    float x_t;
    float y_2;
    float y_3;
    float y_t;

    float x_1_tmp;
    float x_2_tmp;
    float x_3_tmp;
    float x_t_tmp;
    float y_2_tmp;
    float y_3_tmp;
    float y_t_tmp;

    float x_t_ave[3];
    float y_t_ave[3];
    float r_ave[3];

};

#endif // ANCCOORDESTIMATION_H
