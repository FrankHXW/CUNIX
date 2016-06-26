#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QMessageBox>
#include <QStackedWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include "qcustomplot.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <arpa/inet.h>

#define SENSORS_DATA_MAX_SIZE 100   //must be 4*n
#define SENSORS_DATA_FRAME_SIZE 80  //must be 4*n
#define SENSORS_DEVICE_MAXIMUM_NUMBER 20

using namespace std;

struct SensorDataStruct{
    int32_t _machineId;
    string _machineIp;
    double _timeStamp;
    int32_t _sendCount;
    float _accelermeter_X;
    float _accelermeter_Y;
    float _accelermeter_Z;
    float _gyroscope_X;
    float _gyroscope_Y;
    float _gyroscope_Z;
    float _magnetic_X;
    float _magnetic_Y;
    float _magnetic_Z;
    float _temperature;
    float _light;
};




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QWidget *mainWidget;
    QWidget *leftWidget;                //used for setting parameters
    QWidget *rightWidget;               //used for displaying plot

    //server connect options
    QLabel *serverConnectLabel;
    QLabel *serverIpLabel;
    QLineEdit *serverIpLineEdit;
    QLabel *serverPortLabel;
    QLineEdit *serverPortLineEdit;
    QPushButton *connectButton;
    QPushButton *disconnectButton;

    //device select
    QLabel *deviceSelectWidgetLabel;
    QLabel *deviceOnlinesLabel;
    QTextEdit *deviceOnlineTextEdit;
    //select device
    QLabel *deviceSelectLabel;
    QComboBox *deviceSelectComboBox;

    //Data to plot select
    QLabel *dataSelectLabel;
    //accelermeter
    QLabel *accelermeterXLabel;
    QCheckBox *accelermeterXCheckBox;
    QLabel *accelermeterYLabel;
    QCheckBox *accelermeterYCheckBox;
    QLabel *accelermeterZLabel;
    QCheckBox *accelermeterZCheckBox;
    //gyroscope
    QLabel *gyroscopeXLabel;
    QCheckBox *gyroscopeXCheckBox;
    QLabel *gyroscopeYLabel;
    QCheckBox *gyroscopeYCheckBox;
    QLabel *gyroscopeZLabel;
    QCheckBox *gyroscopeZCheckBox;
    //magnetic
    QLabel *magneticXLabel;
    QCheckBox *magneticXCheckBox;
    QLabel *magneticYLabel;
    QCheckBox *magneticYCheckBox;
    QLabel *magneticZLabel;
    QCheckBox *magneticZCheckBox;

    //Others
    QLabel *temperatureLabel;
    QCheckBox *temperatureCheckBox;
    QLabel *lightLabel;
    QCheckBox *lightCheckBox;

    QMessageBox *messageBox;

    QCustomPlot *curvePlot;             //plot device's sensor data
    QTimer *plotTimer;                  //used for plot

    QString serverIP="166.111.64.129";
    quint16 serverPort=12346;

    QTcpSocket *qSocket;

    char qSocketReadedBuffer[SENSORS_DATA_MAX_SIZE];
    int32_t  qSocketIntArray[SENSORS_DATA_MAX_SIZE/4];
    struct SensorDataStruct sensorDataStruct;

    std::vector<std::vector<struct SensorDataStruct> > sensorsDataVector;


public slots:
    void connectServer(void);
    void disconnectServer(void);

    void serverConnectd(void);
    void serverDisconnectd(void);
    void readServerData(void);
    void connectServerFailed(void);

     void updateDataToPlotFlag(void);

    void realtimeDataPlot1(void);
    void realtimeDataPlot(void);

private:
   void initializeLeftWidget(void);
   void initializeRightWidget1(void);

   QRgb rgb[11]={0xff000000,0xff6f0000,0xffaf0000,0xffff0000,0xff006f00,0xff00af00,
                0xff00ff00,0xffffff00,0xffff00ff,0xff0000ff,0xff00ffff,};
   QString legendName[11]={"acc_x","acc_y","acc_z","gyr_x","gyr_y","gyr_z","mag_x","mag_y","mag_z","tempe","light"};
   bool dataToPlotFlag[11];

   QCustomPlot *curvePlotArray[SENSORS_DEVICE_MAXIMUM_NUMBER];
   QWidget *curvePlotWidgetArray[SENSORS_DEVICE_MAXIMUM_NUMBER];
   QStackedWidget *stackedWidget;
   void initializeRightWidget(void);

   void plotSensorData(int _deviceId);



   void setDefaultValue(void);

   int convertByteStremToIntArray(void);

   void saveInputData(void);

};

#endif // MAINWINDOW_H









