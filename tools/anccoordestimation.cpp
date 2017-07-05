#include "anccoordestimation.h"
#define CYCLE_TIME 1000
#define SQR(x) ((x)*(x))
#include <QDebug>

#define CALI_3ANCHTIME 4
AncCoordEstimation::AncCoordEstimation()
{

}

/****************calibrate 3 point**********************/
float AncCoordEstimation::F_1(float x_1){
    return (SQR(x_1)-SQR(a_1));
    //aa
};

float AncCoordEstimation::F_2(float x_2,float y_2){
    //return SQR(x_2-x_1)+SQR(y_2)-SQR(a_2);
    return SQR(x_2) + SQR(y_2) - SQR(a_2);
};

float AncCoordEstimation::F_3(float x_1,float x_2,float y_2){
    //return SQR(x_3-x_2)+SQR(y_3-y_2)-SQR(a_3);
    return SQR(x_2 - x_1) + SQR(y_2) - SQR(a_3);
};



//float AncCoordEstimation::F_4(float x_3,float y_3){
    //return (SQR(x_3)+SQR(y_3)-SQR(a_4));
//};

float AncCoordEstimation::F_5(float x_t,float y_t){
    return (SQR(x_t)+SQR(y_t)-SQR(b_1));
};

float AncCoordEstimation::F_6(float x_1,float x_t,float y_t){
    return (SQR(x_t-x_1)+SQR(y_t)-SQR(b_2));
};

float AncCoordEstimation::F_7(float x_2,float x_t,float y_2,float y_t){
    return (SQR(x_t-x_2)+SQR(y_t-y_2)-SQR(b_3));
};

//float AncCoordEstimation::F_8(float x_t,float x_3,float y_t,float y_3){
  //  return (SQR(x_t-x_3)+SQR(y_t-y_3)-SQR(b_4));
//};


//float AncCoordEstimation::F_9(float x_1,float y_2){
//    p_t = (a_1+a_2+a_3)/2;
//    return(0.25f*SQR(x_1)*SQR(y_2)-p_t*(p_t-a_1)*(p_t-a_2)*(p_t-a_3));

//};


float AncCoordEstimation::diff_F1_x_1(float x_1){
         return (2*x_1);
}

float AncCoordEstimation::diff_F3_x_1(float x_1,float x_2){
     return (2*x_1 - 2*x_2);
}

float AncCoordEstimation::diff_F6_x_1(float x_1,float x_t){
     return (2*x_1 - 2*x_t);
}

float AncCoordEstimation::diff_F2_x_2(float x_2){
         return (2*x_2);
}

float AncCoordEstimation::diff_F3_x_2(float x_1,float x_2){
         return (2*x_2-2*x_1);
}

float AncCoordEstimation::diff_F7_x_2(float x_2,float x_t){
         return (2*x_2-2*x_t);
}

float AncCoordEstimation::diff_F5_x_t(float x_t){
         return (2*x_t);
}

float AncCoordEstimation::diff_F6_x_t(float x_1,float x_t){
         return (2*x_t-2*x_1);
}

float AncCoordEstimation::diff_F7_x_t(float x_2,float x_t){
         return (2*x_t-2*x_2);
}

float AncCoordEstimation::diff_F2_y_2(float y_2){
         return (2*y_2);
}

float AncCoordEstimation::diff_F3_y_2(float y_2){
         return (2*y_2);
}

float AncCoordEstimation::diff_F7_y_2(float y_2,float y_t){
         return (2*y_2-2*y_t);
}

float AncCoordEstimation::diff_F5_y_t(float y_t){
         return (2*y_t);
}

float AncCoordEstimation::diff_F6_y_t(float y_t){
         return (2*y_t);
}

float AncCoordEstimation::diff_F7_y_t(float y_2,float y_t){
         return (2*y_t-2*y_2);
}

void AncCoordEstimation::setInitCoord(float coord[7])
{
    x_1 = coord[0];
    x_2 = coord[1];
    x_3 = coord[2];
    x_t = coord[3];
    y_2 = coord[4];
    y_3 = coord[5];
    y_t = coord[6];

    //qDebug()<<"AncCoordEstimation::setInitCoord x1_x2_x3_xt_y2_y3_yt"<<x_1<<","<<x_2<<","<<x_3<<","<<x_t<<","<<y_2<<","<<y_3<<","<<y_t;
}

void AncCoordEstimation::setInitA(float a[3])
{
  //  //qDebug()<<"AncCoordEstimation::setInitA "<<a[0]<<","<<a[1]<<","<<a[2];
    a_1 = a[0]/1000;
    a_2 = a[1]/1000;
    a_3 = a[2]/1000;
    qDebug()<<"AncCoordEstimation::setInitA_a1_a2_a3 "<<a_1<<","<<a_2<<","<<a_3;

}

void AncCoordEstimation::setInitB(float b[4])
{
  //  //qDebug()<<"AncCoordEstimation::setInitB "<<b[0]<<","<<b[1]<<","<<b[2]<<","<<b[3];
    b_1 = b[0]/1000;
    b_2 = b[1]/1000;
    b_3 = b[2]/1000;
    b_4 = b[3]/1000;
    qDebug()<<"AncCoordEstimation::setInitB_b1_b2_b3_b4 "<<b_1<<","<<b_2<<","<<b_3<<","<<b_4;

}

int AncCoordEstimation::getUwbEstimate(float *delta,int *COUNT)
{
    float x_1_last,x_2_last,x_t_last,y_2_last,y_t_last;//坐标点，待求
    float hope_delta = 0;
    float hope_delta_last =10000;
    float F_1_value;
    float F_2_value;
    float F_3_value;
    float F_5_value;
    float F_6_value;
    float F_7_value;

    float F_1_last_value;
    float F_2_last_value;
    float F_3_last_value;
    float F_5_last_value;
    float F_6_last_value;
    float F_7_last_value;

    int count = 0;
    float DELTA =0.0001f;

    float d_F1_x_1_v;
    float d_F3_x_1_v;
    float d_F6_x_1_v;
    float d_F2_x_2_v;
    float d_F3_x_2_v;
    float d_F7_x_2_v;
    float d_F5_x_t_v;
    float d_F6_x_t_v;
    float d_F7_x_t_v;
    float d_F2_y_2_v;
    float d_F3_y_2_v;
    float d_F7_y_2_v;
    float d_F5_y_t_v;
    float d_F6_y_t_v;
    float d_F7_y_t_v;

     while(count<CYCLE_TIME){

        //此处读取a_1,a_2，a_3，a_4，b_1，b_2，b_3,b_4;
        count++;
        x_1_last = x_1;
        x_2_last = x_2;
       // x_3_last = x_3;
        x_t_last = x_t;
        y_2_last = y_2;
       // y_3_last = y_3;
        y_t_last = y_t;

        d_F1_x_1_v = diff_F1_x_1(x_1_last);
        d_F3_x_1_v = diff_F3_x_1(x_1_last,x_2_last);
        d_F6_x_1_v = diff_F6_x_1(x_1_last,x_t_last);
        d_F2_x_2_v = diff_F2_x_2(x_2_last);
        d_F3_x_2_v = diff_F3_x_2(x_1_last,x_2_last);
        d_F7_x_2_v = diff_F7_x_2(x_2_last,x_t_last);
        d_F5_x_t_v = diff_F5_x_t(x_t_last);
        d_F6_x_t_v = diff_F6_x_t(x_1_last,x_t_last);
        d_F7_x_t_v = diff_F7_x_t(x_2_last,x_t_last);
        d_F2_y_2_v = diff_F2_y_2(y_2_last);
        d_F3_y_2_v = diff_F3_y_2(y_2_last);
        d_F7_y_2_v = diff_F7_y_2(y_2_last,y_t_last);
        d_F5_y_t_v = diff_F5_y_t(y_t_last);
        d_F6_y_t_v = diff_F6_y_t(y_t_last);
        d_F7_y_t_v = diff_F7_y_t(y_2_last,y_t_last);

        F_1_last_value = F_1(x_1_last);
        F_2_last_value = F_2(x_2_last,y_2_last);
        F_3_last_value = F_3(x_1_last,x_2_last,y_2_last);
        F_5_last_value = F_5(x_t_last,y_t_last);
        F_6_last_value = F_6(x_1_last,x_t_last,y_t_last);
        F_7_last_value = F_7(x_2_last,x_t_last,y_2_last,y_t_last);

        x_1 = x_1_last - (F_1_last_value*d_F1_x_1_v+F_3_last_value*d_F3_x_1_v+F_6_last_value*d_F6_x_1_v)*2*DELTA;
        x_2 = x_2_last - (F_2_last_value*d_F2_x_2_v+F_3_last_value*d_F3_x_2_v+F_7_last_value*d_F7_x_2_v)*2*DELTA;
        x_t = x_t_last - (F_5_last_value*d_F5_x_t_v+F_6_last_value*d_F6_x_t_v+F_7_last_value*d_F7_x_t_v)*2*DELTA;
        y_2 = y_2_last - (F_2_last_value*d_F2_y_2_v+F_3_last_value*d_F3_y_2_v+F_7_last_value*d_F7_y_2_v)*2*DELTA;
        y_t = y_t_last - (F_5_last_value*d_F5_y_t_v+F_6_last_value*d_F6_y_t_v+F_7_last_value*d_F7_y_t_v)*2*DELTA;

      //  qDebug()<<"diff "<<d_F1_x_1_v<<","<<d_F3_x_1_v<<","<<d_F6_x_1_v<<","<<d_F2_x_2_v<<","<<d_F3_x_2_v;
     //   qDebug()<<"x1-x2-xt,y2-yt "<<x_1<<","<<x_2<<","<<x_t<<","<<y_2<<","<<y_t;
        //qDebug()<<"a1-a2-a3-b1-b2-b3-b4"<<a_1<<","<<a_2<<","<<a_3<<","<<b_1<<","<<b_2<<","<<b_3<<","<<b_4;

        F_1_value = F_1(x_1);
        F_2_value = F_2(x_2,y_2);
        F_3_value = F_3(x_1,x_2,y_2);
       //F_4_value = F_4(x_3,y_3);
        F_5_value = F_5(x_t,y_t);
        F_6_value = F_6(x_1,x_t,y_t);
        F_7_value = F_7(x_2,x_t,y_2,y_t);

        hope_delta = SQR(F_1_value)+SQR(F_2_value)+SQR(F_3_value)+SQR(F_5_value)+SQR(F_6_value)+SQR(F_7_value);//+SQR(F_9_value);//+SQR(F_8_value);

     //   qDebug()<<"hope_delta = "<<hope_delta;

        if(fabs(hope_delta_last-hope_delta)<0.01){

            DELTA = 0.00001f;
            qDebug()<<"de_count_delta = "<<"DELTA"<<DELTA;
           }

        if(fabs(hope_delta_last-hope_delta)<0.001){
           qDebug()<<"ok_delta = "<<fabs(hope_delta_last-hope_delta);
           break;
        }

        hope_delta_last = hope_delta;

    }

    *delta = hope_delta;
    *COUNT = count;

     if (count == CYCLE_TIME)
    {
        return -1;
    }
    else
    {

        return 0;
    }


}
/****************calibrate 3 point**********************/
/****************calibrate 4 point**********************/
/* start f3 calibrate*/

float AncCoordEstimation::F3_Ft(float x_3,float y_3,float x_t_ave,float y_t_ave,float r_ave){
         return (SQR(x_3-x_t_ave)+SQR(y_3-y_t_ave)-SQR(r_ave));
}
float AncCoordEstimation::F3_Ft_diff_x_3(float x_3,float x_t_ave){
         return (2*(x_3-x_t_ave));
}
float AncCoordEstimation::F3_Ft_diff_y_3(float y_3,float y_t_ave){
         return (2*(y_3-y_t_ave));
}

int AncCoordEstimation::getUwbEstimate_x3_y3(float x_3_tmp,float y_3_tmp,vec3d avgArry[CALI_3ANCHTIME],float *delta,int *COUNT_3)
{
    x_3 = x_3_tmp;
    y_3 = y_3_tmp;

    float x_3_last=x_3_tmp;
    float y_3_last=y_3_tmp;

    float hope_delta = 0;
    float hope_delta_last =10000;
    float F3_Ft_value[4];
    float F3_Ft_last_value[4];
    float d_F3_Ft_x3[4];
    float d_F3_Ft_y3[4];
    int count_3= 0;
    float DELTA_3 =0.001f;

    while(count_3<CYCLE_TIME){

        count_3++;
        hope_delta =0;

        x_3_last = x_3;
        y_3_last = y_3;

        for(int i=0;i<CALI_3ANCHTIME;i++){
          d_F3_Ft_x3[i] = F3_Ft_diff_x_3(x_3_last,avgArry[i].x);
          d_F3_Ft_y3[i] = F3_Ft_diff_y_3(y_3_last,avgArry[i].y);
        }

        for(int i=0;i<CALI_3ANCHTIME;i++){
          F3_Ft_last_value[i] = F3_Ft(x_3_last,y_3_last,avgArry[i].x,avgArry[i].y,avgArry[i].z);
        }

        x_3= x_3_last - (F3_Ft_last_value[0]*d_F3_Ft_x3[0]+F3_Ft_last_value[1]*d_F3_Ft_x3[1]+F3_Ft_last_value[2]*d_F3_Ft_x3[2]+F3_Ft_last_value[3]*d_F3_Ft_x3[3])*2*DELTA_3;
        y_3= y_3_last - (F3_Ft_last_value[0]*d_F3_Ft_y3[0]+F3_Ft_last_value[1]*d_F3_Ft_y3[1]+F3_Ft_last_value[2]*d_F3_Ft_y3[2]+F3_Ft_last_value[3]*d_F3_Ft_y3[3])*2*DELTA_3;


        for(int i=0;i<CALI_3ANCHTIME;i++){
          F3_Ft_value[i] = F3_Ft(x_3,y_3,avgArry[i].x,avgArry[i].y,avgArry[i].z);
          hope_delta+=SQR(F3_Ft_value[i]);
        }

        if(fabs(hope_delta_last-hope_delta)<0.01){
            DELTA_3 = 0.0001f;
        }

        if(fabs(hope_delta_last-hope_delta)<0.001){
           qDebug()<<"ok_delta = "<<fabs(hope_delta_last-hope_delta);
           break;
        }

        hope_delta_last = hope_delta;
    }

    *delta = hope_delta;
    *COUNT_3 = count_3;

    if (count_3 == CYCLE_TIME)
    {
        return -1;
    }
    else
    {
        qDebug()<<"bb-x_3 = "<<x_3 <<"y_3 = "<<y_3<<"count_3"<<count_3;
        return 0;
    }


}

