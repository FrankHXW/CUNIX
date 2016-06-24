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
    for(int i=0;i<maximumSensorDevices;++i){
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
    accelermeterXLabel->setStyleSheet("QLabel{font:11pt}");
    accelermeterXCheckBox=new QCheckBox;
    accelermeterXCheckBox->setText("display");
    accelermeterXCheckBox->setStyleSheet("QCheckBox{font:11pt}");
    accelermeterXLayout->addWidget(accelermeterXLabel);
    accelermeterXLayout->addWidget(accelermeterXCheckBox);
    accelermeterXLayout->addStretch(1);
    accelermeterXLayout->setSpacing(5);
    accelermeterXLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *accelermeterYLayout=new QHBoxLayout;
    accelermeterYLabel=new QLabel;
    accelermeterYLabel->setText(QString("accelermeter_Y:"));
    accelermeterYLabel->setFixedWidth(120);
    accelermeterYLabel->setStyleSheet("QLabel{font:11pt}");
    accelermeterYCheckBox=new QCheckBox;
    accelermeterYCheckBox->setText("display");
    accelermeterYCheckBox->setStyleSheet("QCheckBox{font:11pt}");
    accelermeterYLayout->addWidget(accelermeterYLabel);
    accelermeterYLayout->addWidget(accelermeterYCheckBox);
    accelermeterYLayout->addStretch(1);
    accelermeterYLayout->setSpacing(5);
    accelermeterYLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *accelermeterZLayout=new QHBoxLayout;
    accelermeterZLabel=new QLabel;
    accelermeterZLabel->setText(QString("accelermeter_Z:"));
    accelermeterZLabel->setFixedWidth(120);
    accelermeterZLabel->setStyleSheet("QLabel{font:11pt}");
    accelermeterZCheckBox=new QCheckBox;
    accelermeterZCheckBox->setText("display");
    accelermeterZCheckBox->setStyleSheet("QCheckBox{font:11pt}");
    accelermeterZLayout->addWidget(accelermeterZLabel);
    accelermeterZLayout->addWidget(accelermeterZCheckBox);
    accelermeterZLayout->addStretch(1);
    accelermeterZLayout->setSpacing(5);
    accelermeterZLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *gyroscopeXLayout=new QHBoxLayout;
    gyroscopeXLabel=new QLabel;
    gyroscopeXLabel->setText(QString("gyroscope_X:"));
    gyroscopeXLabel->setFixedWidth(120);
    gyroscopeXLabel->setStyleSheet("QLabel{font:11pt}");
    gyroscopeXCheckBox=new QCheckBox;
    gyroscopeXCheckBox->setText("display");
    gyroscopeXCheckBox->setStyleSheet("QCheckBox{font:11pt}");
    gyroscopeXLayout->addWidget(gyroscopeXLabel);
    gyroscopeXLayout->addWidget(gyroscopeXCheckBox);
    gyroscopeXLayout->addStretch(1);
    gyroscopeXLayout->setSpacing(5);
    gyroscopeXLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *gyroscopeYLayout=new QHBoxLayout;
    gyroscopeYLabel=new QLabel;
    gyroscopeYLabel->setText(QString("gyroscope_Y:"));
    gyroscopeYLabel->setFixedWidth(120);
    gyroscopeYLabel->setStyleSheet("QLabel{font:11pt}");
    gyroscopeYCheckBox=new QCheckBox;
    gyroscopeYCheckBox->setText("display");
    gyroscopeYCheckBox->setStyleSheet("QCheckBox{font:11pt}");
    gyroscopeYLayout->addWidget(gyroscopeYLabel);
    gyroscopeYLayout->addWidget(gyroscopeYCheckBox);
    gyroscopeYLayout->addStretch(1);
    gyroscopeYLayout->setSpacing(5);
    gyroscopeYLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *gyroscopeZLayout=new QHBoxLayout;
    gyroscopeZLabel=new QLabel;
    gyroscopeZLabel->setText(QString("gyroscope_Z:"));
    gyroscopeZLabel->setFixedWidth(120);
    gyroscopeZLabel->setStyleSheet("QLabel{font:11pt}");
    gyroscopeZCheckBox=new QCheckBox;
    gyroscopeZCheckBox->setText("display");
    gyroscopeZCheckBox->setStyleSheet("QCheckBox{font:11pt}");
    gyroscopeZLayout->addWidget(gyroscopeZLabel);
    gyroscopeZLayout->addWidget(gyroscopeZCheckBox);
    gyroscopeZLayout->addStretch(1);
    gyroscopeZLayout->setSpacing(5);
    gyroscopeZLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *magneticXLayout=new QHBoxLayout;
    magneticXLabel=new QLabel;
    magneticXLabel->setText(QString("magnetic_X:"));
    magneticXLabel->setFixedWidth(120);
    magneticXLabel->setStyleSheet("QLabel{font:11pt}");
    magneticXCheckBox=new QCheckBox;
    magneticXCheckBox->setText("display");
    magneticXCheckBox->setStyleSheet("QCheckBox{font:11pt}");
    magneticXLayout->addWidget(magneticXLabel);
    magneticXLayout->addWidget(magneticXCheckBox);
    magneticXLayout->addStretch(1);
    magneticXLayout->setSpacing(5);
    magneticXLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *magneticYLayout=new QHBoxLayout;
    magneticYLabel=new QLabel;
    magneticYLabel->setText(QString("magnetic_Y:"));
    magneticYLabel->setFixedWidth(120);
    magneticYLabel->setStyleSheet("QLabel{font:11pt}");
    magneticYCheckBox=new QCheckBox;
    magneticYCheckBox->setText("display");
    magneticYCheckBox->setStyleSheet("QCheckBox{font:11pt}");
    magneticYLayout->addWidget(magneticYLabel);
    magneticYLayout->addWidget(magneticYCheckBox);
    magneticYLayout->addStretch(1);
    magneticYLayout->setSpacing(5);
    magneticYLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *magneticZLayout=new QHBoxLayout;
    magneticZLabel=new QLabel;
    magneticZLabel->setText(QString("magnetic_Z:"));
    magneticZLabel->setFixedWidth(120);
    magneticZLabel->setStyleSheet("QLabel{font:11pt}");
    magneticZCheckBox=new QCheckBox;
    magneticZCheckBox->setText("display");
    magneticZCheckBox->setStyleSheet("QCheckBox{font:11pt}");
    magneticZLayout->addWidget(magneticZLabel);
    magneticZLayout->addWidget(magneticZCheckBox);
    magneticZLayout->addStretch(1);
    magneticZLayout->setSpacing(5);
    magneticZLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *temperatureLayout=new QHBoxLayout;
    temperatureLabel=new QLabel;
    temperatureLabel->setText(QString("temperature:"));
    temperatureLabel->setFixedWidth(120);
    temperatureLabel->setStyleSheet("QLabel{font:11pt}");
    temperatureCheckBox=new QCheckBox;
    temperatureCheckBox->setText("display");
    temperatureCheckBox->setStyleSheet("QCheckBox{font:11pt}");
    temperatureLayout->addWidget(temperatureLabel);
    temperatureLayout->addWidget(temperatureCheckBox);
    temperatureLayout->addStretch(1);
    temperatureLayout->setSpacing(5);
    temperatureLayout->setContentsMargins(10,0,0,0);

    QHBoxLayout *lightLayout=new QHBoxLayout;
    lightLabel=new QLabel;
    lightLabel->setText(QString("light:"));
    lightLabel->setFixedWidth(120);
    lightLabel->setStyleSheet("QLabel{font:11pt}");
    lightCheckBox=new QCheckBox;
    lightCheckBox->setText("display");
    lightCheckBox->setStyleSheet("QCheckBox{font:11pt}");
    lightLayout->addWidget(lightLabel);
    lightLayout->addWidget(lightCheckBox);
    lightLayout->addStretch(1);
    lightLayout->setSpacing(5);
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

}



void MainWindow::initializeRightWidget()
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
    connect(plotTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    plotTimer->start(100); // Interval 0 means to refresh as fast as possible
}

void MainWindow::setDefaultValue(void)
{
    //initialize default value
    maximumSensorDevices=20;

    serverIP="127.0.0.1";
    serverPort=12345;
    serverIpLineEdit->setText(serverIP);
    serverPortLineEdit->setText(QString::number(serverPort));
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


void MainWindow::realtimeDataSlot(){
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
 //     curvePlot->graph(0)->rescaleValueAxis(true);
 //     curvePlot->graph(1)->rescaleValueAxis(true);
      curvePlot->yAxis->setRange(0,2.5,Qt::AlignTop);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    curvePlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    curvePlot->replot();
}



void MainWindow::serverConnectd(void)
{
    std::ostringstream streamTmp;
    streamTmp.str()="";
    streamTmp<<"connect to server success!";
    messageBox->setText(QString::fromStdString(streamTmp.str()));
    messageBox->exec();
}

void MainWindow::serverDisconnectd(void)
{
    std::ostringstream streamTmp;
    streamTmp.str()="";
    streamTmp<<"server is disconnected!";
    messageBox->setText(QString::fromStdString(streamTmp.str()));
    messageBox->exec();
}

void MainWindow::readServerData(void)
{

}

void MainWindow::connectServerFailed(void)
{
    std::ostringstream streamTmp;
    streamTmp.str()="";
    streamTmp<<"connect to server failed!";
    messageBox->setText(QString::fromStdString(streamTmp.str()));
    messageBox->exec();
}


