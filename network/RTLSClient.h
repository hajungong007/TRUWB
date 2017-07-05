// -------------------------------------------------------------------------------------------------------------------
//
//  File: RTLSClient.h
//
//  Copyright 2016 (c) Decawave Ltd, Dublin, Ireland.
//
//  All rights reserved.
//
//  Author:
//
// -------------------------------------------------------------------------------------------------------------------

#ifndef RTLSCLIENT_H
#define RTLSCLIENT_H

#include <QObject>

#include "SerialConnection.h"
#include "trilateration.h"
#include "anccoordestimation.h"
#include "calcuwbtag.h"

#include <stdint.h>

class QFile;
class DataAnchor;
class DataTag;

#include <armadillo>
using namespace arma;

#define PI (3.141592653589793)

#define MAX_NUM_ANCS_RNG 3 //A0-A1, A0-A2, A1-A2
#define ANC_RANGE_HIST 25
#define HIS_LENGTH 50
#define FILTER_SIZE 10  //NOTE: filter size needs to be > 2
#define FILTER_SIZE_SHORT 6

//add function of autoPos by Sam_Yu
#define MAX_COUNT_NUM 100
#define AVERAGE_NUM 1
#define CALI3ANCHTIME 4

typedef struct
{
    double x_arr[HIS_LENGTH];
    double y_arr[HIS_LENGTH];
    double z_arr[HIS_LENGTH];
    double av_x, av_y, av_z; //average
    double fx, fy, fz; //filter average
    double sqx_arr[HIS_LENGTH]; //square x
    double sqy_arr[HIS_LENGTH];
    double sqz_arr[HIS_LENGTH];
    double avsq_x, avsq_y, avsq_z; //average of squares
    double errx_arr[HIS_LENGTH]; //error x (x-av_x)
    double erry_arr[HIS_LENGTH];
    double errz_arr[HIS_LENGTH];
    double averr_x, averr_y, averr_z; //avearge error
    double variancex, variancey, variancez;
    double std_x, std_y, std_z;
    double r95;
    int id;
    int arr_idx;
    int count;
    int numberOfLEs;
    int filterReady;
    bool ready;
    int rangeSeq;
    int rangeCount[256];
    int rangeValue[256][4]; //(mm) each tag ranges to 4 anchors - it has a range number which is modulo 256
} tag_reports_t;


#define MAX_NUM_TAGS (8)
#define MAX_NUM_ANCS (4)

typedef struct
{
    double x, y, z;
    uint64_t id;
    QString label;
    int tagRangeCorection[MAX_NUM_TAGS];
} anc_struct_t;

typedef struct
{
    double x, y, z;
    uint64_t id;
} pos_report_t;

typedef struct
{
  double x;
  double y;
} vec2d;

class RTLSClient : public QObject
{
    Q_OBJECT
public:
    explicit RTLSClient(QObject *parent = 0);
    ~RTLSClient();

    int calculateTagLocation(vec3d *report, int count, int *ranges);
    void updateTagStatistics(int i, double x, double y, double z);
    void initialiseTagList(int id);
    double process_ma(double *array, int idx);
    double process_me(double *array, int idx);
    double process_avg(int idx);
    void setGWReady(bool set);
    void setUseAutoPos(bool useAutoPos);
    QStringList getLocationFilters(void);
    void setLocationFilter(int filter);
    void saveConfigFile(QString filename);
    void loadConfigFile(QString filename);

    int* getTagCorrections(int anchID);

    void addMissingAnchors(void);

    void trilaterateTag(int tid, int seq);
    void processTagRangeReport(int aid, int tid, int range, int lnum, int seq);
    void processAnchRangeReport(int aid, int tid, int range, int lnum, int seq);

    void mds(mat twrdistance, int nNodes, int viewNode, mat* transCoord);
    void angleRotation(mat transCoord, int nNodes, mat* estCoord);

    void openLogFile(QString userfilename);
    void closeLogFile(void);
    const QString &getLogFilePath();

signals:
    void anchPos(quint64 anchorId, double x, double y, double z,bool, bool);
    void tagPos(quint64 tagId, double x, double y, double z);
    void tagStats(quint64 tagId, double x, double y, double z, double r95);
    void tagRange(quint64 tagId, quint64 aId, double x);
    void statusBarMessage(QString status);

    void centerOnAnchors();
    void enableFiltering(void);
    void ancRanges(int a01, int a02, int a12);

    //add function of autoPos by Sam_Yu
    void estAvgGeted();

protected slots:
    void onReady();
    void onConnected(QString ver, QString conf);

    void updateAnchorXYZ(int id, int x, double value);
    void updateTagCorrection(int aid, int tid, int value);

private slots:
    void newData();
    void connectionStateChanged(SerialConnection::ConnectionState);

    /**
     * function 自动对齐坐标系、Tag
     * Sam_Yu
     *
     */
    void onEstAvgGeted();

private:
    bool _graphicsWidgetReady;
    bool _first;
    bool _useAutoPos;

    QList <tag_reports_t> _tagList;

    anc_struct_t _ancArray[MAX_NUM_ANCS];

    int _ancRangeCount;
    double _ancRangeArray[MAX_NUM_ANCS_RNG][ANC_RANGE_HIST]; //contains the last 50 ranges so we can calculate average

    double _ancRangeValues[MAX_NUM_ANCS][MAX_NUM_ANCS];
    double _ancRangeValuesAvg[MAX_NUM_ANCS][MAX_NUM_ANCS];

    QFile *_file;
    QFile *_fileDbg;

    QSerialPort *_serial;
    QStringList _locationFilterTypes ;
    int _usingFilter;
    uint8_t _ancRangeLastSeq;
    uint8_t _configuration;
    QString _version;
    QString _config;
    QString _logFilePath;

    int _filterSize;

    //添加自动对其坐标系、tag
    float _calibFactorK;
    float _calibFactorB;

    int _count_third;
    int _reportBCount;
    int _reportACount;
    int _reportTCount;

    bool _use_third_anc;
    bool _getAvgBFinished;
    bool _getAvgAFinished;
    bool _getAvgTFinished;

    float _sumB1;
    float _sumB2;
    float _sumB3;
    float _sumB4;
    float _avgB1;
    float _avgB2;
    float _avgB3;
    float _avgB4;

    float _sumA1;
    float _sumA2;
    float _sumA3;
    float _avgA1;
    float _avgA2;
    float _avgA3;

    float _avgTx;
    float _avgTy;
    float _sumTx;
    float _sumTy;

    vec3d  _avgR3Arry[CALI3ANCHTIME];

    int _getAvgR3Times;

    AncCoordEstimation *_ancCoordEstimation;
    CalcUwbTag *_calcuwbtag;

    float _delta;
    float _delta_3;

    int _count;
    int _count_3;

};

void r95Sort(double s[], int l, int r);

#endif // RTLSCLIENT_H
