#include "calcuwbtag.h"
#define SQR(x) ((x)*(x))
#include <math.h>
#include <QDebug>
CalcUwbTag::CalcUwbTag()
{

}

void CalcUwbTag::initA(float a[6])
{


}
void CalcUwbTag::initB(int b[4])
{

}
float CalcUwbTag::F1(float x_t,float y_t,float z_t)
{
   return (SQR(x_t)+SQR(y_t)+ SQR(z_t)-SQR(b_1));
}

float  CalcUwbTag::F2(float x_t,float y_t,float z_t){
    return (SQR(x_t-p_1.x)+SQR(y_t)+SQR(z_t)-SQR(b_2));
}
float  CalcUwbTag::F3(float x_t,float y_t,float z_t){
    return (SQR(x_t-p_2.x)+SQR(y_t-p_2.y)+SQR(z_t)-SQR(b_3));
}
float  CalcUwbTag::F4(float x_t, float  y_t,float z_t)
{
    return (SQR(x_t-p_3.x)+SQR(y_t-p_3.y)+SQR(z_t)-SQR(b_4));
}

float CalcUwbTag::diff_F_1_x_t(float x_t)
{
    return (2*x_t);
}
float CalcUwbTag::diff_F_2_x_t(float x_t){
    return (2*(x_t-p_1.x));
}
float CalcUwbTag::diff_F_3_x_t (float x_t){
    return (2*(x_t-p_2.x));
}
float CalcUwbTag::diff_F_4_x_t (float x_t){
    return (2*(x_t-p_3.x));
}
float CalcUwbTag::diff_F_1_y_t(float y_t){
    return ( 2*y_t);
}
float  CalcUwbTag::diff_F_2_y_t (float y_t){
    return ( 2*y_t);
}
float CalcUwbTag::diff_F_3_y_t (float y_t){
    return (2*(y_t-p_2.y));
}
float CalcUwbTag::diff_F_4_y_t(float y_t)
{
    return (2*(y_t-p_3.y));
}
float CalcUwbTag::diff_F_1_z_t(float z_t)
{
    return (2*z_t);
}
float CalcUwbTag::diff_F_2_z_t(float z_t)
{
    return (2*z_t);
}
float CalcUwbTag::diff_F_3_z_t(float z_t)
{
    return (2*z_t);
}
float CalcUwbTag::diff_F_4_z_t(float z_t)
{
    return (2*z_t);
}
void CalcUwbTag::getResult(double *x,double *y,double *z)
{
    *x = x_t;
    *y = y_t;
    *z = z_t;
}
int CalcUwbTag::getTagPostion(vec3d *best_solution, int use4thAnchor, vec3d* anchorArray, int *distanceArray)
//int CalcUwbTag::getTagPostion(double *x)
{
    //float F = F1^2+F2^2+F3^2+F4^2

    vec3d p_r;

    float delta=0.001;
    float x_t_last = 0;
    float y_t_last = 0;
    float z_t_last = 0;
    int cycle = 0;
    float hope_delta = 0;
    float hope_delta_last = 0;

   /* x_1 = anchorArray[1].x;
    x_2 = anchorArray[2].x;		    y_2 = anchorArray[2].y;
    x_3 = anchorArray[3].x;		    y_3 = anchorArray[3].y;
*/
    p_1.x = anchorArray[1].x;
    p_2.x = anchorArray[2].x;    p_2.y = anchorArray[2].y;
    p_3.x = anchorArray[3].x;    p_2.y = anchorArray[3].y;

  qDebug()<<"x_1 y_1 z_2 ";
    b_1 = (double) distanceArray[0] / 1000.0;
    b_2 = (double) distanceArray[1] / 1000.0;
    b_3 = (double) distanceArray[2] / 1000.0;
    b_4 = (double) distanceArray[3] / 1000.0;

    x_t = best_solution->x;
    y_t = best_solution->y;
    z_t = best_solution->z;

    while(cycle <100){
  qDebug()<<"cycle "<<cycle;
        x_t_last = x_t;
        y_t_last = y_t;
        z_t_last = z_t;

        float d_f1_xt=diff_F_1_x_t (x_t_last);
        float d_f2_xt=diff_F_2_x_t (x_t_last);
        float d_f3_xt=diff_F_3_x_t (x_t_last);
        float d_f4_xt=diff_F_4_x_t (x_t_last);


        float d_f1_yt=diff_F_1_y_t (y_t_last);
        float d_f2_yt=diff_F_2_y_t (y_t_last);
        float d_f3_yt=diff_F_3_y_t (y_t_last);
        float d_f4_yt=diff_F_4_y_t (y_t_last);

        float d_f1_zt=diff_F_1_z_t (z_t_last);
        float d_f2_zt=diff_F_2_z_t (z_t_last);
        float d_f3_zt=diff_F_3_z_t (z_t_last);
        float d_f4_zt=diff_F_4_z_t (z_t_last);


        float f1_last = F1(x_t_last,y_t_last,z_t_last);
        float f2_last = F2(x_t_last,y_t_last,z_t_last);
        float f3_last = F3(x_t_last,y_t_last,z_t_last);
        float f4_last = F4(x_t_last,y_t_last,z_t_last);

        x_t = x_t_last-(d_f1_xt*f1_last+d_f2_xt*f2_last+d_f3_xt*f3_last+d_f4_xt*f4_last)*2*delta;
        y_t = y_t_last-(d_f1_yt*f1_last+d_f2_yt*f2_last+d_f3_yt*f3_last+d_f4_yt*f4_last)*2*delta;
        z_t = z_t_last-(d_f1_zt*f1_last+d_f2_zt*f2_last+d_f3_zt*f3_last+d_f4_zt*f4_last)*2*delta;

        float f1 = F1(x_t ,y_t,z_t);
        float f2 = F2(x_t ,y_t,z_t);
        float f3 = F3(x_t ,y_t,z_t);
        float f4 = F4(x_t ,y_t,z_t);

        hope_delta = SQR(f1)+SQR(f2)+SQR(f3)+SQR(f4);

        qDebug()<<"hope_delta"<<hope_delta<<"delta_hope"<<fabs(hope_delta_last-hope_delta);
        if(fabs(hope_delta_last-hope_delta)<0.01){
            //DELTA = 0.00001f;
            delta=0.0001f;
        }

        if(fabs(hope_delta_last-hope_delta)<0.001){
            break;
        }

        hope_delta_last = hope_delta;

        cycle++;
    }

    best_solution->x = x_t;
    best_solution->y = y_t;
    best_solution->z = z_t;

}


