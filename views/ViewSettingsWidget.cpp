// -------------------------------------------------------------------------------------------------------------------
//
//  File: ViewSettingsWidget.cpp
//
//  Copyright 2016 (c) Decawave Ltd, Dublin, Ireland.
//
//  All rights reserved.
//
//  Author:
//
// -------------------------------------------------------------------------------------------------------------------

#include "ViewSettingsWidget.h"
#include "ui_ViewSettingsWidget.h"

#include "RTLSDisplayApplication.h"
#include "QPropertyModel.h"
#include "ViewSettings.h"
#include "OriginTool.h"
#include "ScaleTool.h"
#include "GraphicsView.h"
#include "GraphicsWidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

ViewSettingsWidget::ViewSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewSettingsWidget),
    _floorplanOpen(false)
{
    ui->setupUi(this);

    //实现fence围栏功能-关联围栏范围调节控件，存入数据到文件中
    QObject::connect(ui->fenceX_1, SIGNAL(valueChanged(double)), this, SLOT(onFenceX1ValueChange(double)));
    QObject::connect(ui->fenceY_1, SIGNAL(valueChanged(double)), this, SLOT(onFenceY1ValueChange(double)));
    QObject::connect(ui->fenceW_1, SIGNAL(valueChanged(double)), this, SLOT(onFenceW1ValueChange(double)));
    QObject::connect(ui->fenceH_1, SIGNAL(valueChanged(double)), this, SLOT(onFenceH1ValueChange(double)));

    QObject::connect(ui->fenceX_2, SIGNAL(valueChanged(double)), this, SLOT(onFenceX2ValueChange(double)));
    QObject::connect(ui->fenceY_2, SIGNAL(valueChanged(double)), this, SLOT(onFenceY2ValueChange(double)));
    QObject::connect(ui->fenceW_2, SIGNAL(valueChanged(double)), this, SLOT(onFenceW2ValueChange(double)));
    QObject::connect(ui->fenceH_2, SIGNAL(valueChanged(double)), this, SLOT(onFenceH2ValueChange(double)));

    QObject::connect(ui->fenceX_3, SIGNAL(valueChanged(double)), this, SLOT(onFenceX3ValueChange(double)));
    QObject::connect(ui->fenceY_3, SIGNAL(valueChanged(double)), this, SLOT(onFenceY3ValueChange(double)));
    QObject::connect(ui->fenceW_3, SIGNAL(valueChanged(double)), this, SLOT(onFenceW3ValueChange(double)));
    QObject::connect(ui->fenceH_3, SIGNAL(valueChanged(double)), this, SLOT(onFenceH3ValueChange(double)));

    QObject::connect(ui->fenceX_4, SIGNAL(valueChanged(double)), this, SLOT(onFenceX4ValueChange(double)));
    QObject::connect(ui->fenceY_4, SIGNAL(valueChanged(double)), this, SLOT(onFenceY4ValueChange(double)));
    QObject::connect(ui->fenceW_4, SIGNAL(valueChanged(double)), this, SLOT(onFenceW4ValueChange(double)));
    QObject::connect(ui->fenceH_4, SIGNAL(valueChanged(double)), this, SLOT(onFenceH4ValueChange(double)));
    //实现fence围栏功能-关联围栏范围调节控件，存入数据到文件中

    /**
     *function-实现坐标系、tag自动对齐
     *Sam_Yu
     */

    QObject::connect(ui->_tagAutoPos, SIGNAL(clicked(bool)), this, SLOT(onTagAutoPos()));
    QObjetc:connect(ui->_absissaAutoPos, SIGNAL(clicked(bool)), this, SLOT(onAbsissaAutoPos()));

    /**
     *function-实现坐标系、tag自动对齐
     *Sam_Yu
     */


    QObject::connect(ui->floorplanOpen_pb, SIGNAL(clicked()), this, SLOT(floorplanOpenClicked()));

    QObject::connect(ui->scaleX_pb, SIGNAL(clicked()), this, SLOT(scaleClicked()));
    QObject::connect(ui->scaleY_pb, SIGNAL(clicked()), this, SLOT(scaleClicked()));
    QObject::connect(ui->origin_pb, SIGNAL(clicked()), this, SLOT(originClicked()));

    QObject::connect(ui->saveFP, SIGNAL(clicked()), this, SLOT(saveFPClicked()));
    QObject::connect(ui->gridShow, SIGNAL(clicked()), this, SLOT(gridShowClicked()));
    QObject::connect(ui->showOrigin, SIGNAL(clicked()), this, SLOT(originShowClicked()));
    QObject::connect(ui->showTagHistory, SIGNAL(clicked()), this, SLOT(tagHistoryShowClicked()));
    QObject::connect(ui->showGeoFencingMode, SIGNAL(clicked()), this, SLOT(showGeoFencingModeClicked()));
    QObject::connect(ui->showNavigationMode, SIGNAL(clicked()), this, SLOT(showNavigationModeClicked()));

    QObject::connect(ui->useAutoPos, SIGNAL(clicked()), this, SLOT(useAutoPosClicked()));
    QObject::connect(ui->showTagTable, SIGNAL(clicked()), this, SLOT(tagAncTableShowClicked()));
    QObject::connect(ui->showAnchorTable, SIGNAL(clicked()), this, SLOT(tagAncTableShowClicked()));
    QObject::connect(ui->showAnchorTagCorrectionTable, SIGNAL(clicked()), this, SLOT(tagAncTableShowClicked()));

    QObject::connect(ui->zone1, SIGNAL(editingFinished()), this, SLOT(zone1EditFinished()));
    QObject::connect(ui->zone2, SIGNAL(editingFinished()), this, SLOT(zone2EditFinished()));

    QObject::connect(ui->zone1, SIGNAL(valueChanged(double)), this, SLOT(zone1ValueChanged(double)));
    QObject::connect(ui->zone2, SIGNAL(valueChanged(double)), this, SLOT(zone2ValueChanged(double)));
    QObject::connect(ui->inAlarm, SIGNAL(clicked()), this, SLOT(alarmSetClicked()));
    QObject::connect(ui->outAlarm, SIGNAL(clicked()), this, SLOT(alarmSetClicked()));

    QObject::connect(ui->tagHistoryN, SIGNAL(valueChanged(int)), this, SLOT(tagHistoryNumberValueChanged(int)));

    QObject::connect(RTLSDisplayApplication::viewSettings(), SIGNAL(showSave(bool)), this, SLOT(showSave(bool)));
    QObject::connect(RTLSDisplayApplication::viewSettings(), SIGNAL(showGO(bool, bool)), this, SLOT(showOriginGrid(bool, bool)));
    QObject::connect(RTLSDisplayApplication::viewSettings(), SIGNAL(setFloorPlanPic()), this, SLOT(getFloorPlanPic()));
    QObject::connect(RTLSDisplayApplication::client(), SIGNAL(enableFiltering()), this, SLOT(enableFiltering()));

    QObject::connect(ui->logging_pb, SIGNAL(clicked()), this, SLOT(loggingClicked()));

    _logging = false ;

    ui->label_logfile->setText("");
    if(_logging)
    {
        ui->logging_pb->setText("停止");
        ui->label_logingstatus->setText("日志可用.");
    }
    else
    {
        ui->logging_pb->setText("开始");
        ui->label_logingstatus->setText("日志不可用");
    }

    RTLSDisplayApplication::connectReady(this, "onReady()");
}

void ViewSettingsWidget::onReady()
{
    //实现围栏功能--从配置文件中获取围栏范围调节控件的初始值
    QSettings file("fence.ini", QSettings::IniFormat);
    ui->fenceX_1->setValue(file.value("X1").toDouble());
    ui->fenceY_1->setValue(file.value("Y1").toDouble());
    ui->fenceW_1->setValue(file.value("W1").toDouble());
    ui->fenceH_1->setValue(file.value("H1").toDouble());

    ui->fenceX_2->setValue(file.value("X2").toDouble());
    ui->fenceY_2->setValue(file.value("Y2").toDouble());
    ui->fenceW_2->setValue(file.value("W2").toDouble());
    ui->fenceH_2->setValue(file.value("H2").toDouble());

    ui->fenceX_3->setValue(file.value("X3").toDouble());
    ui->fenceY_3->setValue(file.value("Y3").toDouble());
    ui->fenceW_3->setValue(file.value("W3").toDouble());
    ui->fenceH_3->setValue(file.value("H3").toDouble());

    ui->fenceX_4->setValue(file.value("X4").toDouble());
    ui->fenceY_4->setValue(file.value("Y4").toDouble());
    ui->fenceW_4->setValue(file.value("W4").toDouble());
    ui->fenceH_4->setValue(file.value("H4").toDouble());

    //实现围栏功能--报警提示初始化
    redlight.load(":/icons/redlight.svg");
    greenlight.load(":/icons/greenlight.svg");

    timer = new QTimer(this);
    timer->setInterval(1000);

    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onEnterFence()));

    timer->start();

    //实现围栏功能--报警提示音
    alarmMusic = new QMediaPlayer();
    runPath = QCoreApplication::applicationDirPath(); //获取当前程序的运行目录
    currenPos = 0;

    alarPath = runPath + "/info.mp3";

    alarPath.replace(QString("/"), QString("\\"));

    alarmMusic->setMedia(QUrl::fromLocalFile(alarPath));
    alarmMusic->setVolume(50);

    //自动对齐坐标系初始化
    QSettings filePos("autopos.ini", QSettings::IniFormat);
    filePos.setValue("tag", 0);
    filePos.setValue("absiss", 0);
    filePos.setValue("updateanchor", 0);

    ui->_cabValue->setValue(1.00);
    ui->_cakValue->setValue(0.00);


    QPropertyDataWidgetMapper *mapper = QPropertyModel::newMapper(RTLSDisplayApplication::viewSettings(), this);
    mapper->addMapping(ui->gridWidth_sb, "gridWidth");
    mapper->addMapping(ui->gridHeight_sb, "gridHeight");

    mapper->addMapping(ui->floorplanFlipX_cb, "floorplanFlipX", "checked");
    mapper->addMapping(ui->floorplanFlipY_cb, "floorplanFlipY", "checked");
    mapper->addMapping(ui->gridShow, "showGrid", "checked");
    mapper->addMapping(ui->showOrigin, "showOrigin", "checked");

    mapper->addMapping(ui->floorplanXOff_sb, "floorplanXOffset");
    mapper->addMapping(ui->floorplanYOff_sb, "floorplanYOffset");

    mapper->addMapping(ui->floorplanXScale_sb, "floorplanXScale");
    mapper->addMapping(ui->floorplanYScale_sb, "floorplanYScale");
    mapper->toFirst();

    QObject::connect(ui->floorplanFlipX_cb, SIGNAL(clicked()), mapper, SLOT(submit())); // Bug with QDataWidgetMapper (QTBUG-1818)
    QObject::connect(ui->floorplanFlipY_cb, SIGNAL(clicked()), mapper, SLOT(submit()));
    QObject::connect(ui->gridShow, SIGNAL(clicked()), mapper, SLOT(submit())); // Bug with QDataWidgetMapper (QTBUG-1818)
    QObject::connect(ui->showOrigin, SIGNAL(clicked()), mapper, SLOT(submit()));

    //by default the Geo-Fencing is OFF

    ui->showTagHistory->setChecked(true);

    ui->zone1->setDisabled(true);
    ui->zone2->setDisabled(true);
    ui->label_z1->setDisabled(true);
    ui->label_z2->setDisabled(true);
    ui->outAlarm->setDisabled(true);
    ui->inAlarm->setDisabled(true);

    ui->tabWidget->setCurrentIndex(0);

    RTLSDisplayApplication::graphicsWidget()->zone1Value(ui->zone1->value());
    RTLSDisplayApplication::graphicsWidget()->zone2Value(ui->zone2->value());
    RTLSDisplayApplication::graphicsWidget()->setAlarm(ui->inAlarm->isChecked(), ui->outAlarm->isChecked());

    ui->filtering->setEnabled(false);
    ui->filtering->addItems(RTLSDisplayApplication::client()->getLocationFilters());
    QObject::connect(ui->filtering, SIGNAL(currentIndexChanged(int)), this, SLOT(updateLocationFilter(int)));
}

ViewSettingsWidget::~ViewSettingsWidget()
{
    delete ui;
    delete timer;
    delete alarmMusic;
}


void ViewSettingsWidget::enableFiltering(void)
{
     ui->filtering->setEnabled(true);
}

void ViewSettingsWidget::updateLocationFilter(int index)
{
     RTLSDisplayApplication::client()->setLocationFilter(index);
}

int ViewSettingsWidget::applyFloorPlanPic(const QString &path)
{
    QPixmap pm(path);

    if (pm.isNull())
    {
        //QMessageBox::critical(this, "Could not load floor plan", QString("Failed to load image : %1").arg(path));
        return -1;
    }

    ui->floorplanPath_lb->setText(QFileInfo(path).fileName());
    RTLSDisplayApplication::viewSettings()->setFloorplanPixmap(pm);
    _floorplanOpen = true;
    ui->floorplanOpen_pb->setText("Clear");

    return 0;
}

void ViewSettingsWidget::getFloorPlanPic()
{
    applyFloorPlanPic(RTLSDisplayApplication::viewSettings()->getFloorplanPath());
}

void ViewSettingsWidget::floorplanOpenClicked()
{
    if(_floorplanOpen == false)
    {
        QString path = QFileDialog::getOpenFileName(this, "Open Bitmap", QString(), "Image (*.png *.jpg *.jpeg *.bmp)");
        if (path.isNull()) return;

        if(applyFloorPlanPic(path) == 0) //if OK set/save the path string
        {
            RTLSDisplayApplication::viewSettings()->floorplanShow(true);
            RTLSDisplayApplication::viewSettings()->setFloorplanPath(path);
        }
        _floorplanOpen = true;
        ui->floorplanOpen_pb->setText("Clear");
    }
    else
    {
       RTLSDisplayApplication::viewSettings()->floorplanShow(false);
       RTLSDisplayApplication::viewSettings()->clearSettings();
       _floorplanOpen = false;
       ui->floorplanOpen_pb->setText("Open");
       ui->floorplanFlipX_cb->setChecked(false);
       ui->floorplanFlipY_cb->setChecked(false);
       ui->floorplanXScale_sb->setValue(0.0);
       ui->floorplanYScale_sb->setValue(0.0);
       ui->floorplanXOff_sb->setValue(0.0);
       ui->floorplanYOff_sb->setValue(0.0);
       ui->floorplanPath_lb->setText("");
    }
}

void ViewSettingsWidget::showOriginGrid(bool orig, bool grid)
{
    Q_UNUSED(orig)

    ui->gridShow->setChecked(grid);
    ui->showOrigin->setChecked(orig);
}

void ViewSettingsWidget::gridShowClicked()
{
    RTLSDisplayApplication::viewSettings()->setShowGrid(ui->gridShow->isChecked());
}

void ViewSettingsWidget::originShowClicked()
{
    RTLSDisplayApplication::viewSettings()->setShowOrigin(ui->showOrigin->isChecked());
}

void ViewSettingsWidget::showNavigationModeClicked()
{
    if(ui->showNavigationMode->isChecked())
    {
        ui->showGeoFencingMode->setChecked(false);
        showGeoFencingModeClicked();
    }
    else
    {
        ui->showGeoFencingMode->setChecked(true);
        showGeoFencingModeClicked();
    }
}


void ViewSettingsWidget::showGeoFencingModeClicked()
{
    RTLSDisplayApplication::graphicsWidget()->showGeoFencingMode(ui->showGeoFencingMode->isChecked());

    if(ui->showGeoFencingMode->isChecked())
    {
        ui->showTagHistory->setDisabled(true);
        ui->tagHistoryN->setDisabled(true);

        ui->zone1->setEnabled(true);
        ui->zone2->setEnabled(true);
        ui->label_z1->setEnabled(true);
        ui->label_z2->setEnabled(true);
        ui->outAlarm->setEnabled(true);
        ui->inAlarm->setEnabled(true);

        ui->showNavigationMode->setChecked(false);
        //set auto positioning to off when in geo-fencing mode
        RTLSDisplayApplication::client()->setUseAutoPos(false);
        ui->useAutoPos->setDisabled(true);
    }
    else
    {
        ui->showTagHistory->setDisabled(false);
        ui->tagHistoryN->setDisabled(false);

        ui->zone1->setDisabled(true);
        ui->zone2->setDisabled(true);
        ui->label_z1->setDisabled(true);
        ui->label_z2->setDisabled(true);
        ui->outAlarm->setDisabled(true);
        ui->inAlarm->setDisabled(true);

        ui->useAutoPos->setDisabled(false);
        useAutoPosClicked();

        ui->showNavigationMode->setChecked(true);
    }

}

void ViewSettingsWidget::tagHistoryNumberValueChanged(int value)
{
    RTLSDisplayApplication::graphicsWidget()->tagHistoryNumber(value);
}

void ViewSettingsWidget::zone1EditFinished(void)
{
    RTLSDisplayApplication::graphicsWidget()->zone1Value(ui->zone1->value());
}

void ViewSettingsWidget::zone2EditFinished(void)
{
    RTLSDisplayApplication::graphicsWidget()->zone2Value(ui->zone2->value());
}

void ViewSettingsWidget::zone1ValueChanged(double value)
{
    RTLSDisplayApplication::graphicsWidget()->zone1Value(value);
}

void ViewSettingsWidget::zone2ValueChanged(double value)
{
    RTLSDisplayApplication::graphicsWidget()->zone2Value(value);
}

void ViewSettingsWidget::useAutoPosClicked()
{
    RTLSDisplayApplication::client()->setUseAutoPos(ui->useAutoPos->isChecked());

    RTLSDisplayApplication::graphicsWidget()->anchTableEditing(!ui->useAutoPos->isChecked());
}

void ViewSettingsWidget::tagAncTableShowClicked()
{
    RTLSDisplayApplication::graphicsWidget()->setShowTagAncTable(ui->showAnchorTable->isChecked(),
                                                                 ui->showTagTable->isChecked(),
                                                                 ui->showAnchorTagCorrectionTable->isChecked());
}

void ViewSettingsWidget::setTagHistory(int h)
{
    ui->tagHistoryN->setValue(h);
}


void ViewSettingsWidget::tagHistoryShowClicked()
{
    RTLSDisplayApplication::graphicsWidget()->setShowTagHistory(ui->showTagHistory->isChecked());
}


void ViewSettingsWidget::loggingClicked(void)
{
    if(_logging == false)
    {
        _logging = true ;
        RTLSDisplayApplication::client()->openLogFile("");
        ui->logging_pb->setText("Stop");
        ui->label_logingstatus->setText("Logging enabled.");
        ui->label_logfile->setText(RTLSDisplayApplication::client()->getLogFilePath());
    }
    else
    {
        RTLSDisplayApplication::client()->closeLogFile();
        ui->logging_pb->setText("Start");
        ui->label_logingstatus->setText("Logging disabled.");
        ui->label_logfile->setText("");
        ui->saveFP->setChecked(false);
        _logging = false ;
    }
}


void ViewSettingsWidget::alarmSetClicked()
{
    RTLSDisplayApplication::graphicsWidget()->setAlarm(ui->inAlarm->isChecked(), ui->outAlarm->isChecked());
}

void ViewSettingsWidget::saveFPClicked()
{
    RTLSDisplayApplication::viewSettings()->setSaveFP(ui->saveFP->isChecked());

    if(ui->saveFP->isChecked())
    {
        //save the current settings when clicked
       emit saveViewSettings();
    }
}

void ViewSettingsWidget::showSave(bool save)
{
    ui->saveFP->setChecked(save);
}

void ViewSettingsWidget::originClicked()
{
    OriginTool *tool = new OriginTool(this);
    QObject::connect(tool, SIGNAL(done()), tool, SLOT(deleteLater()));
    RTLSDisplayApplication::graphicsView()->setTool(tool);
}

void ViewSettingsWidget::scaleClicked()
{
    ScaleTool *tool = NULL;

    if (QObject::sender() == ui->scaleX_pb)
        tool = new ScaleTool(ScaleTool::XAxis, this);
    else if (QObject::sender() == ui->scaleY_pb)
        tool = new ScaleTool(ScaleTool::YAxis, this);

    QObject::connect(tool, SIGNAL(done()), tool, SLOT(deleteLater()));
    RTLSDisplayApplication::graphicsView()->setTool(tool);
}

void ViewSettingsWidget::onFenceX1ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("X1", data);
}

void ViewSettingsWidget::onFenceY1ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("Y1", data);
}

void ViewSettingsWidget::onFenceW1ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("W1", data);
}

void ViewSettingsWidget::onFenceH1ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("H1", data);
}

void ViewSettingsWidget::onFenceX2ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("X2", data);
}

void ViewSettingsWidget::onFenceY2ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("Y2", data);
}

void ViewSettingsWidget::onFenceW2ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("W2", data);
}

void ViewSettingsWidget::onFenceH2ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("H2", data);
}

void ViewSettingsWidget::onFenceX3ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("X3", data);
}

void ViewSettingsWidget::onFenceY3ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("Y3", data);
}

void ViewSettingsWidget::onFenceW3ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("W3", data);
}

void ViewSettingsWidget::onFenceH3ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("H3", data);
}

void ViewSettingsWidget::onFenceX4ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("X4", data);
}

void ViewSettingsWidget::onFenceY4ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("Y4", data);
}

void ViewSettingsWidget::onFenceW4ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("W4", data);
}

void ViewSettingsWidget::onFenceH4ValueChange(double data)
{
    QSettings file("fence.ini", QSettings::IniFormat);
    file.setValue("H4", data);
}

void ViewSettingsWidget::onEnterFence()
{
    QSettings file("fence.ini", QSettings::IniFormat);

    if(file.value("Alarm1").toInt() == 1)
    {
        qDebug()<<"进入危险区，报警提示";
        ui->fenceAlarm_1->setPixmap(redlight);
        alarmMusic->play();
        currenPos = 1;
    }
    else
    {
        ui->fenceAlarm_1->setPixmap(greenlight);

        if(currenPos == 1)
        {
            alarmMusic->stop();
            currenPos = 0;
        }
    }

    if(file.value("Alarm2").toInt() == 1)
    {
        qDebug()<<"进入危险区，报警提示";
        ui->fenceAlarm_2->setPixmap(redlight);
        alarmMusic->play();
        currenPos = 2;
    }
    else
    {
        ui->fenceAlarm_2->setPixmap(greenlight);

        if(currenPos == 2)
        {
            alarmMusic->stop();
            currenPos = 0;
        }
    }

    if(file.value("Alarm3").toInt() == 1)
    {
        qDebug()<<"进入危险区，报警提示";
        ui->fenceAlarm_3->setPixmap(redlight);
        alarmMusic->play();
        currenPos = 3;
    }
    else
    {
        ui->fenceAlarm_3->setPixmap(greenlight);

        if(currenPos == 3)
        {
            alarmMusic->stop();
            currenPos = 0;
        }
    }

    if(file.value("Alarm4").toInt() == 1)
    {
        qDebug()<<"进入危险区，报警提示";
        ui->fenceAlarm_4->setPixmap(redlight);
        alarmMusic->play();
        currenPos = 4;
    }
    else
    {
        ui->fenceAlarm_4->setPixmap(greenlight);

        if(currenPos == 4)
        {
            alarmMusic->stop();
            currenPos = 0;
        }
    }


}

void ViewSettingsWidget::onTagAutoPos()
{
    QSettings file("autopos.ini", QSettings::IniFormat);

    double cab = ui->_cabValue->value();
    double cak = ui->_cakValue->value();

    if(ui->_tagAutoPos->text() == "Tag点校准")
    {
        file.setValue("cab", cab);
        file.setValue("cak", cak);

        file.setValue("tag", 1);

        ui->_tagAutoPos->setText("取消");
    }
    else if(ui->_tagAutoPos->text() == "取消")
    {
        file.setValue("tag", 0);

         ui->_tagAutoPos->setText("Tag点校准");
    }


}

void ViewSettingsWidget::onAbsissaAutoPos()
{
    QSettings file("autopos.ini", QSettings::IniFormat);

    double cab, cak;

    cab = ui->_cabValue->value();
    cak = ui->_cakValue->value();

    file.setValue("cab", cab);
    file.setValue("cak", cak);

    file.setValue("absiss", 1);

    qDebug()<<"对齐坐标系按钮按下";
}


