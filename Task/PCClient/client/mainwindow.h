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
#include <QTcpSocket>
#include <QHostAddress>
#include "qcustomplot.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>


namespace Ui {
class MainWindow;
}

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

    int maximumSensorDevices=20;

    QString serverIP="166.111.64.129";
    quint16 serverPort=12346;

    QTcpSocket *qSocket;

public slots:
    void connectServer(void);
    void disconnectServer(void);

    void serverConnectd(void);
    void serverDisconnectd(void);
    void readServerData(void);
    void connectServerFailed(void);

    void realtimeDataSlot(void);

private:
   void initializeLeftWidget(void);
   void initializeRightWidget(void);

   void setDefaultValue(void);



};

#endif // MAINWINDOW_H









