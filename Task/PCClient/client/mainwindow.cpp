#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    mainWidget = new QWidget(this);

    initializeLeftWidget();
    initializeRightWidget();


    QHBoxLayout *mainLayout=new QHBoxLayout;
    mainLayout->addWidget(leftWidget);
    mainLayout->addWidget(rightWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainWidget->setMinimumSize(960,640);
    mainWidget->setLayout(mainLayout);

    this->setCentralWidget( mainWidget );

    qSocket=new QTcpSocket(this);

    connect(qSocket,SIGNAL(connected()),this,SLOT(serverConnectd()));
    connect(qSocket,SIGNAL(disconnected()),this,SLOT(serverDisconnectd()));
    connect(qSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connectServerFailed()));
    connect(qSocket,SIGNAL(readyRead()),this,SLOT(readServerData()));

    messageBox=new QMessageBox(mainWidget);
    messageBox->setStyleSheet("QMessageBox{background-color:rgb(225,225,233)}");

    setDefaultValue();
}

MainWindow::~MainWindow()
{

}


void MainWindow::initializeLeftWidget()
{
    //left widget:server connect options
    serverConnectLabel=new QLabel;
    serverConnectLabel->setText(QString("Server options"));
    serverConnectLabel->setFixedWidth(180);
    serverConnectLabel->setStyleSheet("QLabel{font:bold,11pt}");

    QHBoxLayout *serverIpLayout=new QHBoxLayout;
    serverIpLabel=new QLabel;
    serverIpLabel->setText(QString("server ip:"));
    serverIpLabel->setFixedWidth(80);
    serverIpLabel->setStyleSheet("QLabel{font:11pt}");
    serverIpLineEdit=new QLineEdit;
    serverIpLineEdit->setFixedWidth(120);
 //   serverIpLineEdit->setPlaceholderText("166.111.64.219");
    serverIpLineEdit->setInputMask("000.000.000.000;");
    serverIpLayout->addWidget(serverIpLabel);
    serverIpLayout->addWidget(serverIpLineEdit);
    serverIpLayout->setSpacing(5);
    serverIpLayout->setContentsMargins(5,0,5,0);

    QHBoxLayout *serverPortLayout=new QHBoxLayout;
    serverPortLabel=new QLabel;
    serverPortLabel->setText(QString("server port:"));
    serverPortLabel->setFixedWidth(80);
    serverPortLabel->setStyleSheet("QLabel{font:11pt}");
    serverPortLineEdit=new QLineEdit;
    serverPortLineEdit->setFixedWidth(120);
 //   serverPortLineEdit->setPlaceholderText("12346");
    serverPortLineEdit->setInputMask("00000");
    serverPortLayout->addWidget(serverPortLabel);
    serverPortLayout->addWidget(serverPortLineEdit);
    serverPortLayout->setSpacing(5);
    serverPortLayout->setContentsMargins(5,0,5,0);

    QHBoxLayout *connectButtonLayout=new QHBoxLayout;
    connectButton=new QPushButton;
    connectButton->setFixedWidth(80);
    connectButton->setText(QString("connect"));
    connectButton->setStyleSheet("QPushButton{font:11pt}");
    disconnectButton=new QPushButton;
    disconnectButton->setFixedWidth(80);
    disconnectButton->setText(QString("disconnect"));
    disconnectButton->setStyleSheet("QPushButton{font:11pt}");
    connectButtonLayout->addWidget(connectButton);
    connectButtonLayout->addWidget(disconnectButton);
    connectButtonLayout->setSpacing(5);
    connectButtonLayout->setContentsMargins(10,0,5,0);

    QVBoxLayout *serverConnectLayout=new QVBoxLayout;
    serverConnectLayout->addWidget(serverConnectLabel);
    serverConnectLayout->addLayout(serverIpLayout);
    serverConnectLayout->addLayout(serverPortLayout);
    serverConnectLayout->addLayout(connectButtonLayout);
    serverConnectLayout->setSpacing(5);
    serverConnectLayout->setContentsMargins(5,5,0,0);

    //left widget:device select options
    deviceSelectWidgetLabel=new QLabel;
    deviceSelectWidgetLabel->setText(QString("Device select"));
    deviceSelectWidgetLabel->setFixedWidth(180);
    deviceSelectWidgetLabel->setStyleSheet("QLabel{font:bold,11pt}");

    QVBoxLayout *deviceOnlineLayout=new QVBoxLayout;
    deviceOnlinesLabel=new QLabel;
    deviceOnlinesLabel->setFixedWidth(120);
    deviceOnlinesLabel->setText("device online:");
    deviceOnlinesLabel->setStyleSheet("QLabel{font:11pt}");
    deviceOnlineTextEdit=new QTextEdit;
    deviceOnlineTextEdit->setFixedSize(220,80);
    deviceOnlineTextEdit->setReadOnly(true);
    deviceOnlineLayout->addWidget(deviceOnlinesLabel);
    deviceOnlineLayout->addWidget(deviceOnlineTextEdit);
    deviceOnlineLayout->addStretch(0);
    deviceOnlineLayout->setSpacing(5);
    deviceOnlineLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *deviceSelectLayout=new QHBoxLayout;
    deviceSelectLabel=new QLabel;
    deviceSelectLabel->setText(QString("device select:"));
    deviceSelectLabel->setFixedWidth(120);
    deviceSelectLabel->setStyleSheet("QLabel{font:11pt}");
    deviceSelectComboBox=new QComboBox;
    deviceSelectComboBox->setFixedWidth(120);
    for(int i=0;i<SENSORS_DEVICE_MAXIMUM_NUMBER;++i){
        deviceSelectComboBox->addItem("device_"+QString::number(i));
    }
    deviceSelectLayout->addWidget(deviceSelectLabel);
    deviceSelectLayout->addWidget(deviceSelectComboBox);
    deviceSelectLayout->setSpacing(5);
    deviceSelectLayout->setContentsMargins(10,0,0,0);


    QVBoxLayout *deviceSelectWidgetLayout=new QVBoxLayout;
    deviceSelectWidgetLayout->addWidget(deviceSelectWidgetLabel);
    deviceSelectWidgetLayout->addLayout(deviceOnlineLayout);
    deviceSelectWidgetLayout->addLayout(deviceSelectLayout);
    deviceSelectWidgetLayout->addStretch(0);
    deviceSelectWidgetLayout->setSpacing(5);
    deviceSelectWidgetLayout->setContentsMargins(5,15,5,0);

    //left widget:data to plot select
    dataSelectLabel=new QLabel;
    dataSelectLabel->setText(QString("Data select"));
    dataSelectLabel->setFixedWidth(180);
    dataSelectLabel->setStyleSheet("QLabel{font:bold,11pt}");

    QHBoxLayout *accelermeterXLayout=new QHBoxLayout;
    accelermeterXLabel=new QLabel;
    accelermeterXLabel->setText(QString("accelermeter_X:"));
    accelermeterXLabel->setFixedWidth(120);
    accelermeterXLabel->setStyleSheet("QLabel{font:11pt;background-color:rgba(0%,0%,0%,60%)}");
    accelermeterXCheckBox=new QCheckBox;
    accelermeterXCheckBox->setText("display");
    accelermeterXCheckBox->setStyleSheet("QCheckBox{font:11pt;background-color:rgba(0%,0%,0%,60%)}");
    accelermeterXLayout->addWidget(accelermeterXLabel);
    accelermeterXLayout->addWidget(accelermeterXCheckBox);
    accelermeterXLayout->addStretch(1);
    accelermeterXLayout->setSpacing(0);
    accelermeterXLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *accelermeterYLayout=new QHBoxLayout;
    accelermeterYLabel=new QLabel;
    accelermeterYLabel->setText(QString("accelermeter_Y:"));
    accelermeterYLabel->setFixedWidth(120);
    accelermeterYLabel->setStyleSheet("QLabel{font:11pt;background-color:rgba(30%,0%,0%,60%)}");
    accelermeterYCheckBox=new QCheckBox;
    accelermeterYCheckBox->setText("display");
    accelermeterYCheckBox->setStyleSheet("QCheckBox{font:11pt;background-color:rgba(30%,0%,0%,60%)}");
    accelermeterYLayout->addWidget(accelermeterYLabel);
    accelermeterYLayout->addWidget(accelermeterYCheckBox);
    accelermeterYLayout->addStretch(1);
    accelermeterYLayout->setSpacing(0);
    accelermeterYLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *accelermeterZLayout=new QHBoxLayout;
    accelermeterZLabel=new QLabel;
    accelermeterZLabel->setText(QString("accelermeter_Z:"));
    accelermeterZLabel->setFixedWidth(120);
    accelermeterZLabel->setStyleSheet("QLabel{font:11pt;background-color:rgba(60%,0%,0%,60%)}");
    accelermeterZCheckBox=new QCheckBox;
    accelermeterZCheckBox->setText("display");
    accelermeterZCheckBox->setStyleSheet("QCheckBox{font:11pt;background-color:rgba(60%,0%,0%,60%)}");
    accelermeterZLayout->addWidget(accelermeterZLabel);
    accelermeterZLayout->addWidget(accelermeterZCheckBox);
    accelermeterZLayout->addStretch(1);
    accelermeterZLayout->setSpacing(0);
    accelermeterZLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *gyroscopeXLayout=new QHBoxLayout;
    gyroscopeXLabel=new QLabel;
    gyroscopeXLabel->setText(QString("gyroscope_X:"));
    gyroscopeXLabel->setFixedWidth(120);
    gyroscopeXLabel->setStyleSheet("QLabel{font:11pt;background-color:rgba(100%,0%,0%,60%)}");
    gyroscopeXCheckBox=new QCheckBox;
    gyroscopeXCheckBox->setText("display");
    gyroscopeXCheckBox->setStyleSheet("QCheckBox{font:11pt;background-color:rgba(100%,0%,0%,60%)}");
    gyroscopeXLayout->addWidget(gyroscopeXLabel);
    gyroscopeXLayout->addWidget(gyroscopeXCheckBox);
    gyroscopeXLayout->addStretch(1);
    gyroscopeXLayout->setSpacing(0);
    gyroscopeXLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *gyroscopeYLayout=new QHBoxLayout;
    gyroscopeYLabel=new QLabel;
    gyroscopeYLabel->setText(QString("gyroscope_Y:"));
    gyroscopeYLabel->setFixedWidth(120);
    gyroscopeYLabel->setStyleSheet("QLabel{font:11pt;background-color:rgba(0%,30%,0%,60%)}");
    gyroscopeYCheckBox=new QCheckBox;
    gyroscopeYCheckBox->setText("display");
    gyroscopeYCheckBox->setStyleSheet("QCheckBox{font:11pt;background-color:rgba(0%,30%,0%,60%)}");
    gyroscopeYLayout->addWidget(gyroscopeYLabel);
    gyroscopeYLayout->addWidget(gyroscopeYCheckBox);
    gyroscopeYLayout->addStretch(1);
    gyroscopeYLayout->setSpacing(0);
    gyroscopeYLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *gyroscopeZLayout=new QHBoxLayout;
    gyroscopeZLabel=new QLabel;
    gyroscopeZLabel->setText(QString("gyroscope_Z:"));
    gyroscopeZLabel->setFixedWidth(120);
    gyroscopeZLabel->setStyleSheet("QLabel{font:11pt;background-color:rgba(0%,60%,0%,60%)}");
    gyroscopeZCheckBox=new QCheckBox;
    gyroscopeZCheckBox->setText("display");
    gyroscopeZCheckBox->setStyleSheet("QCheckBox{font:11pt;background-color:rgba(0%,60%,0%,60%)}");
    gyroscopeZLayout->addWidget(gyroscopeZLabel);
    gyroscopeZLayout->addWidget(gyroscopeZCheckBox);
    gyroscopeZLayout->addStretch(1);
    gyroscopeZLayout->setSpacing(0);
    gyroscopeZLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *magneticXLayout=new QHBoxLayout;
    magneticXLabel=new QLabel;
    magneticXLabel->setText(QString("magnetic_X:"));
    magneticXLabel->setFixedWidth(120);
    magneticXLabel->setStyleSheet("QLabel{font:11pt;background-color:rgba(0%,100%,0%,60%)}");
    magneticXCheckBox=new QCheckBox;
    magneticXCheckBox->setText("display");
    magneticXCheckBox->setStyleSheet("QCheckBox{font:11pt;background-color:rgba(0%,100%,0%,60%)}");
    magneticXLayout->addWidget(magneticXLabel);
    magneticXLayout->addWidget(magneticXCheckBox);
    magneticXLayout->addStretch(1);
    magneticXLayout->setSpacing(0);
    magneticXLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *magneticYLayout=new QHBoxLayout;
    magneticYLabel=new QLabel;
    magneticYLabel->setText(QString("magnetic_Y:"));
    magneticYLabel->setFixedWidth(120);
    magneticYLabel->setStyleSheet("QLabel{font:11pt;background-color:rgba(100%,100%,0%,60%)}");
    magneticYCheckBox=new QCheckBox;
    magneticYCheckBox->setText("display");
    magneticYCheckBox->setStyleSheet("QCheckBox{font:11pt;background-color:rgba(100%,100%,0%,60%)}");
    magneticYLayout->addWidget(magneticYLabel);
    magneticYLayout->addWidget(magneticYCheckBox);
    magneticYLayout->addStretch(1);
    magneticYLayout->setSpacing(0);
    magneticYLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *magneticZLayout=new QHBoxLayout;
    magneticZLabel=new QLabel;
    magneticZLabel->setText(QString("magnetic_Z:"));
    magneticZLabel->setFixedWidth(120);
    magneticZLabel->setStyleSheet("QLabel{font:11pt;background-color:rgba(100%,0%,100%,60%)}");
    magneticZCheckBox=new QCheckBox;
    magneticZCheckBox->setText("display");
    magneticZCheckBox->setStyleSheet("QCheckBox{font:11pt;background-color:rgba(100%,0%,100%,60%)}");
    magneticZLayout->addWidget(magneticZLabel);
    magneticZLayout->addWidget(magneticZCheckBox);
    magneticZLayout->addStretch(1);
    magneticZLayout->setSpacing(0);
    magneticZLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *temperatureLayout=new QHBoxLayout;
    temperatureLabel=new QLabel;
    temperatureLabel->setText(QString("temperature:"));
    temperatureLabel->setFixedWidth(120);
    temperatureLabel->setStyleSheet("QLabel{font:11pt;background-color:rgba(0%,0%,100%,60%)}");
    temperatureCheckBox=new QCheckBox;
    temperatureCheckBox->setText("display");
    temperatureCheckBox->setStyleSheet("QCheckBox{font:11pt;background-color:rgba(0%,0%,100%,60%)}");
    temperatureLayout->addWidget(temperatureLabel);
    temperatureLayout->addWidget(temperatureCheckBox);
    temperatureLayout->addStretch(1);
    temperatureLayout->setSpacing(0);
    temperatureLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *lightLayout=new QHBoxLayout;
    lightLabel=new QLabel;
    lightLabel->setText(QString("light:"));
    lightLabel->setFixedWidth(120);
    lightLabel->setStyleSheet("QLabel{font:11pt;background-color:rgba(0%,100%,100%,60%)}");
    lightCheckBox=new QCheckBox;
    lightCheckBox->setText("display");
    lightCheckBox->setStyleSheet("QCheckBox{font:11pt;background-color:rgba(0%,100%,100%,60%)}");
    lightLayout->addWidget(lightLabel);
    lightLayout->addWidget(lightCheckBox);
    lightLayout->addStretch(1);
    lightLayout->setSpacing(0);
    lightLayout->setContentsMargins(10,0,0,0);

    QVBoxLayout *dataWidgetLayout=new QVBoxLayout;
    dataWidgetLayout->addWidget(dataSelectLabel);
    dataWidgetLayout->addLayout(accelermeterXLayout);
    dataWidgetLayout->addLayout(accelermeterYLayout);
    dataWidgetLayout->addLayout(accelermeterZLayout);
    dataWidgetLayout->addLayout(gyroscopeXLayout);
    dataWidgetLayout->addLayout(gyroscopeYLayout);
    dataWidgetLayout->addLayout(gyroscopeZLayout);
    dataWidgetLayout->addLayout(magneticXLayout);
    dataWidgetLayout->addLayout(magneticYLayout);
    dataWidgetLayout->addLayout(magneticZLayout);
    dataWidgetLayout->addLayout(temperatureLayout);
    dataWidgetLayout->addLayout(lightLayout);
    dataWidgetLayout->setSpacing(4);
    dataWidgetLayout->setContentsMargins(5,15,0,0);
    dataWidgetLayout->addStretch(0);

    //left widget
    leftWidget=new QWidget;
    leftWidget->setFixedWidth(250);
    leftWidget->setStyleSheet("QWidget{background-color:rgb(218,219,233)}");
    QVBoxLayout *leftWidgetLayout=new QVBoxLayout;
    leftWidgetLayout->addLayout(serverConnectLayout);
    leftWidgetLayout->addLayout(deviceSelectWidgetLayout);
    leftWidgetLayout->addLayout(dataWidgetLayout);
    leftWidgetLayout->addStretch(1);
    leftWidgetLayout->setSpacing(0);
    leftWidgetLayout->setContentsMargins(0,5,0,0);
    leftWidget->setLayout(leftWidgetLayout);

    //connect push button to slot
    QObject::connect(connectButton,SIGNAL(clicked(bool)),this,SLOT(connectServer()));
    QObject::connect(disconnectButton,SIGNAL(clicked(bool)),this,SLOT(disconnectServer()));

    QObject::connect(accelermeterXCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateDataToPlotFlag()));
    QObject::connect(accelermeterYCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateDataToPlotFlag()));
    QObject::connect(accelermeterZCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateDataToPlotFlag()));
    QObject::connect(gyroscopeXCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateDataToPlotFlag()));
    QObject::connect(gyroscopeYCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateDataToPlotFlag()));
    QObject::connect(gyroscopeZCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateDataToPlotFlag()));
    QObject::connect(magneticXCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateDataToPlotFlag()));
    QObject::connect(magneticYCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateDataToPlotFlag()));
    QObject::connect(magneticZCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateDataToPlotFlag()));
    QObject::connect(temperatureCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateDataToPlotFlag()));
    QObject::connect(lightCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateDataToPlotFlag()));
}



void MainWindow::initializeRightWidget1()
{
    curvePlot=new QCustomPlot;

    curvePlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
    curvePlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    curvePlot->legend->setFont(legendFont);
    curvePlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    curvePlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    curvePlot->addGraph(); // blue line
    curvePlot->graph(0)->setPen(QPen(Qt::blue));
  //  curvePlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
 //   curvePlot->graph(0)->setAntialiasedFill(false);
    curvePlot->graph(0)->setName("sin");

    curvePlot->addGraph(); // red line
    curvePlot->graph(1)->setPen(QPen(Qt::red));
    curvePlot->graph(1)->setName("cos");
//    curvePlot->graph(0)->setChannelFillGraph(curvePlot->graph(1));

 //   curvePlot->addGraph(); // blue dot
//    curvePlot->graph(2)->setPen(QPen(Qt::blue));
//    curvePlot->graph(2)->setLineStyle(QCPGraph::lsNone);
//    curvePlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
//   curvePlot->graph(2)->setName("sin");
//    curvePlot->addGraph(); // red dot
//    curvePlot->graph(3)->setPen(QPen(Qt::red));
//    curvePlot->graph(3)->setLineStyle(QCPGraph::lsNone);
//    curvePlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

    curvePlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    curvePlot->xAxis->setDateTimeFormat("hh:mm:ss");
    curvePlot->xAxis->setAutoTickStep(true);
    curvePlot->xAxis->setTickStep(1);
    curvePlot->axisRect()->setupFullAxesBox();

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(curvePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), curvePlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(curvePlot->yAxis, SIGNAL(rangeChanged(QCPRange)), curvePlot->yAxis2, SLOT(setRange(QCPRange)));


    QHBoxLayout *rightLayout=new QHBoxLayout();
    rightLayout->addWidget(curvePlot);
    rightLayout->setSpacing(5);
    rightLayout->setMargin(0);
    rightWidget=new QWidget();
    rightWidget->setMinimumWidth(400);
    rightWidget->setLayout(rightLayout);

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    plotTimer=new QTimer(this);
    connect(plotTimer, SIGNAL(timeout()), this, SLOT(realtimeDataPlot1()));
    plotTimer->start(100); // Interval 0 means to refresh as fast as possible
}

void MainWindow::initializeRightWidget(void)
{
    stackedWidget=new QStackedWidget;
    for(int i=0;i<SENSORS_DEVICE_MAXIMUM_NUMBER;++i){
        curvePlotArray[i]=new QCustomPlot;
        curvePlotArray[i]->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
        curvePlotArray[i]->legend->setVisible(true);
        QFont legendFont = font();  // start out with MainWindow's font..
        legendFont.setPointSize(9); // and make a bit smaller for legend
        curvePlotArray[i]->legend->setFont(legendFont);
        curvePlotArray[i]->legend->setBrush(QBrush(QColor(255,255,255,200)));
        curvePlotArray[i]->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

        for(int j=0;j<11;++j){
            curvePlotArray[i]->addGraph(); // blue line
            curvePlotArray[i]->graph(j)->setPen(QPen(rgb[j]));
            curvePlotArray[i]->graph(j)->setName(legendName[j]);
        }

        curvePlotArray[i]->xAxis->setTickLabelType(QCPAxis::ltDateTime);
        curvePlotArray[i]->xAxis->setDateTimeFormat("hh:mm:ss");
        curvePlotArray[i]->xAxis->setAutoTickStep(true);
        curvePlotArray[i]->xAxis->setTickStep(1);
        curvePlotArray[i]->axisRect()->setupFullAxesBox();

        // make left and bottom axes transfer their ranges to right and top axes:
        connect(curvePlotArray[i]->xAxis, SIGNAL(rangeChanged(QCPRange)), curvePlotArray[i]->xAxis2, SLOT(setRange(QCPRange)));
        connect(curvePlotArray[i]->yAxis, SIGNAL(rangeChanged(QCPRange)), curvePlotArray[i]->yAxis2, SLOT(setRange(QCPRange)));

        QHBoxLayout *layoutTmp=new QHBoxLayout();
        layoutTmp->addWidget(curvePlotArray[i]);
        layoutTmp->setSpacing(0);
        layoutTmp->setMargin(0);
        curvePlotWidgetArray[i]=new QWidget();
        curvePlotWidgetArray[i]->setMinimumWidth(400);
        curvePlotWidgetArray[i]->setLayout(layoutTmp);
        stackedWidget->addWidget(curvePlotWidgetArray[i]);
    }

    QHBoxLayout *rightLayout=new QHBoxLayout();
    rightLayout->addWidget(stackedWidget);
    rightLayout->setSpacing(5);
    rightLayout->setMargin(0);
    rightWidget=new QWidget();
    rightWidget->setMinimumWidth(400);
    rightWidget->setLayout(rightLayout);

    connect(deviceSelectComboBox,SIGNAL(activated(int)),stackedWidget, SLOT(setCurrentIndex(int)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    plotTimer=new QTimer(this);
 //   connect(plotTimer, SIGNAL(timeout()), this, SLOT(realtimeDataPlot()));
    plotTimer->start(10); // Interval 0 means to refresh as fast as possible

}


void MainWindow::setDefaultValue(void)
{
    //initialize default value
    serverIP="45.78.14.139";
    serverPort=12346;
    serverIpLineEdit->setText(serverIP);
    serverPortLineEdit->setText(QString::number(serverPort));
    sensorsDataVector.clear();
    sensorsDataVector.resize(SENSORS_DEVICE_MAXIMUM_NUMBER);
    accelermeterXCheckBox->setChecked(true);
    accelermeterYCheckBox->setChecked(true);
    accelermeterZCheckBox->setChecked(true);
    gyroscopeXCheckBox->setChecked(true);
    gyroscopeYCheckBox->setChecked(true);
    gyroscopeZCheckBox->setChecked(true);
    magneticXCheckBox->setChecked(true);
    magneticYCheckBox->setChecked(true);
    magneticZCheckBox->setChecked(true);
    temperatureCheckBox->setChecked(true);
    lightCheckBox->setChecked(true);

}


void MainWindow::connectServer(void)
{
    serverIP=serverIpLineEdit->text();
    serverPort=serverPortLineEdit->text().toUShort();
    qSocket->connectToHost(QHostAddress(serverIpLineEdit->text()),serverPort);
}


void MainWindow::disconnectServer(void)
{
   qSocket->close();
}


void MainWindow::realtimeDataPlot1(){
    // calculate two new data points:
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.001) // at most add point every 10 ms
    {
      double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
      double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
      // add data to lines:
      curvePlot->graph(0)->addData(key, value0);
      curvePlot->graph(1)->addData(key, value1);
      // set data of dots:
//     curvePlot->graph(2)->clearData();
//      curvePlot->graph(2)->addData(key, value0);
//      curvePlot->graph(3)->clearData();
//      curvePlot->graph(3)->addData(key, value1);
      // remove data of lines that's outside visible range:
      curvePlot->graph(0)->removeDataBefore(key-8);
      curvePlot->graph(1)->removeDataBefore(key-8);
      // rescale value (vertical) axis to fit the current data:
      curvePlot->graph(0)->rescaleValueAxis(true);
      curvePlot->graph(1)->rescaleValueAxis(true);
 //    curvePlot->yAxis->setRange(0,2.5,Qt::AlignTop);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    curvePlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    curvePlot->replot();
}



void MainWindow::realtimeDataPlot()
{
    // calculate two new data points:
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.001){
      for(int i=0;i<SENSORS_DEVICE_MAXIMUM_NUMBER;++i){
          double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
          double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
          // add data to lines:
          curvePlotArray[i]->graph(0)->addData(key, value0+i/10.0);
          curvePlotArray[i]->graph(1)->addData(key, value1+i/10.0);
          // set data of dots:
          // remove data of lines that's outside visible range:
          curvePlotArray[i]->graph(0)->removeDataBefore(key-8);
          curvePlotArray[i]->graph(1)->removeDataBefore(key-8);
          // rescale value (vertical) axis to fit the current data:
          curvePlotArray[i]->graph(0)->rescaleValueAxis(true);
          curvePlotArray[i]->graph(1)->rescaleValueAxis(true);
//          curvePlotArray[i]->yAxis->setRange(0,2.5,Qt::AlignTop);

          // make key axis range scroll with the data (at a constant range size of 8):
          curvePlotArray[i]->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
          curvePlotArray[i]->replot();
        }
        lastPointKey = key;
    }
}


void MainWindow::serverConnectd(void)
{
    std::ostringstream streamTmp;
    streamTmp.str()="";
    streamTmp<<"connect to server success!";
    connectButton->setEnabled(false);
    connectButton->setStyleSheet("QPushButton{background-color:rgb(0,160,0);}");
    disconnectButton->setEnabled(true);
    disconnectButton->setStyleSheet("QPushButton{background-color:rgb(200,200,200);}");
    messageBox->setText(QString::fromStdString(streamTmp.str()));
    messageBox->exec();
}

void MainWindow::serverDisconnectd(void)
{
    std::ostringstream streamTmp;
    streamTmp.str()="";
    streamTmp<<"server is disconnected!";
    connectButton->setEnabled(true);
    connectButton->setStyleSheet("QPushButton{background-color:rgb(200,200,200);}");
    disconnectButton->setEnabled(false);
    disconnectButton->setStyleSheet("QPushButton{background-color:rgb(200,0,0);}");
    messageBox->setText(QString::fromStdString(streamTmp.str()));
    messageBox->exec();
    qSocket->close();
}

void MainWindow::readServerData(void)
{
    std::ostringstream streamTmp;
    streamTmp.str()="";
    memset(qSocketReadedBuffer,'\0',SENSORS_DATA_MAX_SIZE);
    int nRead=qSocket->read(qSocketReadedBuffer,SENSORS_DATA_MAX_SIZE);
    if(nRead==SENSORS_DATA_FRAME_SIZE){
        convertByteStremToIntArray();
//        deviceOnlineTextEdit->setText(QString::fromLocal8Bit(qSocketReadedBuffer,SENSORS_DATA_FRAME_SIZE));
        streamTmp<<sensorDataStruct._machineId<<": "<<(long)sensorDataStruct._timeStamp;
        deviceOnlineTextEdit->setText(QString::fromStdString(streamTmp.str()));
    }
}

void MainWindow::connectServerFailed(void)
{
    std::ostringstream streamTmp;
    streamTmp.str()="";
    streamTmp<<"connect to server failed!";
    connectButton->setEnabled(true);
    connectButton->setStyleSheet("QPushButton{background-color:rgb(200,200,200);}");
    disconnectButton->setEnabled(true);
    disconnectButton->setStyleSheet("QPushButton{background-color:rgb(200,200,200);}");
    messageBox->setText(QString::fromStdString(streamTmp.str()));
    messageBox->exec();
    qSocket->close();
}

int MainWindow::convertByteStremToIntArray(void)
{
    if((SENSORS_DATA_MAX_SIZE%4!=0)||(SENSORS_DATA_FRAME_SIZE%4!=0)){
        qDebug()<<"SENSORS_DATA_MAX_SIZE and SENSORS_DATA_FRAME_SIZE must be 4*n";
        return -1;
    }
    memset(qSocketIntArray,0,SENSORS_DATA_MAX_SIZE/4);
    int tmp[4]={0,0,0,0};
    for(int i=0;i<SENSORS_DATA_MAX_SIZE/4;++i){
        tmp[0]=qSocketReadedBuffer[4*i]&0xff;
        tmp[1]=qSocketReadedBuffer[4*i+1]&0xff;
        tmp[2]=qSocketReadedBuffer[4*i+2]&0xff;
        tmp[3]=qSocketReadedBuffer[4*i+3]&0xff;
        qSocketIntArray[i]=(tmp[0])|(tmp[1]<<8)|(tmp[2]<<16)|(tmp[3]<<24);
    }
    saveInputData();
    return 0;
}


void MainWindow::saveInputData(void)
{
    sensorDataStruct._machineId=qSocketIntArray[0];

    struct in_addr ipAddr;
    ipAddr.s_addr=ntohl(qSocketIntArray[1]);
    char *ipString=inet_ntoa(ipAddr);
    sensorDataStruct._machineIp=ipString;

    long tmp=qSocketIntArray[2]&0xffffffff;
    sensorDataStruct._timeStamp=(tmp<<32)|(qSocketIntArray[3]&0xffffffff);

    sensorDataStruct._sendCount=qSocketIntArray[4];

    sensorDataStruct._accelermeter_X=qSocketIntArray[6]/1000.0;
    sensorDataStruct._accelermeter_Y=qSocketIntArray[7]/1000.0;
    sensorDataStruct._accelermeter_Z=qSocketIntArray[8]/1000.0;

    sensorDataStruct._gyroscope_X=qSocketIntArray[9]/1000.0;
    sensorDataStruct._gyroscope_Y=qSocketIntArray[10]/1000.0;
    sensorDataStruct._gyroscope_Z=qSocketIntArray[11]/1000.0;

    sensorDataStruct._magnetic_X=qSocketIntArray[12]/1000.0;
    sensorDataStruct._magnetic_Y=qSocketIntArray[13]/1000.0;
    sensorDataStruct._magnetic_Z=qSocketIntArray[14]/1000.0;

    sensorDataStruct._temperature=qSocketIntArray[15]/1000.0;

    sensorDataStruct._light=qSocketIntArray[16]/1000.0;

    int id=sensorDataStruct._machineId;
    if((id>=0)&&(id<SENSORS_DEVICE_MAXIMUM_NUMBER)){
        sensorsDataVector[id].push_back(sensorDataStruct);
        plotSensorData(id);
    }
}

 void MainWindow::updateDataToPlotFlag(void)
 {
     dataToPlotFlag[0]=accelermeterXCheckBox->isChecked();
     dataToPlotFlag[1]=accelermeterYCheckBox->isChecked();
     dataToPlotFlag[2]=accelermeterZCheckBox->isChecked();
     dataToPlotFlag[3]=gyroscopeXCheckBox->isChecked();
     dataToPlotFlag[4]=gyroscopeYCheckBox->isChecked();
     dataToPlotFlag[5]=gyroscopeZCheckBox->isChecked();
     dataToPlotFlag[6]=magneticXCheckBox->isChecked();
     dataToPlotFlag[7]=magneticYCheckBox->isChecked();
     dataToPlotFlag[8]=magneticZCheckBox->isChecked();
     dataToPlotFlag[9]=temperatureCheckBox->isChecked();
     dataToPlotFlag[10]=lightCheckBox->isChecked();
 }

void MainWindow::plotSensorData(int _deviceId)
{
    int i=_deviceId;
    if((i>=0)&&(i<SENSORS_DEVICE_MAXIMUM_NUMBER)){
        double timeInSeconds=sensorDataStruct._timeStamp/1000.0;
        static double lastPlotTime[11]={0};
        if(((timeInSeconds-lastPlotTime[i])>=0.05)||(abs(lastPlotTime[i])<0.1)){
            for(int j=0;j<11;++j){
                if(dataToPlotFlag[j]){
                    curvePlotArray[i]->graph(j)->addData(timeInSeconds,qSocketIntArray[6+j]/1000.0);
                    curvePlotArray[i]->graph(j)->removeDataBefore(timeInSeconds-10);
                    curvePlotArray[i]->graph(j)->rescaleValueAxis(true);
//                    curvePlotArray[i]->yAxis->setRange(0,100,Qt::AlignTop);
                }
            }
            curvePlotArray[i]->xAxis->setRange(timeInSeconds+0.25,10,Qt::AlignRight);
            curvePlotArray[i]->replot();
            lastPlotTime[i]=timeInSeconds;
        }
    }

}





