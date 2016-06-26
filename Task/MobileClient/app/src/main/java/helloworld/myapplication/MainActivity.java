package helloworld.myapplication;


import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Inet4Address;
import java.net.Socket;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.TimerTask;
import java.util.Timer;
import java.util.Enumeration;


import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import android.view.View;
import android.view.Gravity;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Button;
import android.widget.Toast;
import android.graphics.Color;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Handler;
import android.os.Message;
import android.util.Log;



public class MainActivity extends AppCompatActivity implements SensorEventListener {

    private SensorManager sManager;
    private Sensor mSensorAccelermeter;
    private Sensor mSensorGyroscope;
    private Sensor mSensorMagnetic;
    private Sensor mSensorTemperature;
    private Sensor mSensorLight;
    private Sensor mSensorProximity;

    private TextView accelermeterX;
    private TextView accelermeterY;
    private TextView accelermeterZ;

    private TextView gyroscopeX;
    private TextView gyroscopeY;
    private TextView gyroscopeZ;

    private TextView magneticX;
    private TextView magneticY;
    private TextView magneticZ;

    private TextView temperatureValue;
    private TextView lightValue;
    private TextView proximityValue;

    private EditText serverIpText;
    private EditText serverPortText;
    private EditText sendInterval;
    private EditText machineId;

    private Button connectButton;
    private Button disconnectButton;

    /* 单网卡名称*/
    private static final String NETWORK_CARD = "eth0";

    /** 绑定网卡名称*/
    private static final String NETWORK_CARD_BAND = "bond0";

    private Socket socket;
    private String defalutServerIp="166.111.64.219";
    private String defaultServerPort="12345";
    private String mLocalHostName;
    private String mLocalMachineIp;


    Integer mSensorSampleRate=30000;       //us

    Integer mMachineId=0;

    Integer mSendTimeInterval=500;          //ms

    Integer mSendSocketDataCount=0;

    volatile  boolean mEnableSendFlag=false;

    private DataInputStream mDataInStream;
    private DataOutputStream mDataOutStream;

    private StringBuffer mDataToSendString=null;
    private StringBuffer mDataReadedString=null;

    //发送格式为本机ip,时间戳,各数据，发送计数
    private final Integer mDataArraySize=20;
    volatile Integer [] mDataToSendIntArray=new Integer[mDataArraySize];
    byte [] mDataToSendByteArray=new byte[mDataArraySize*4];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mainlayout);
        sManager = (SensorManager) getSystemService(SENSOR_SERVICE);

        mSensorAccelermeter=sManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sManager.registerListener(this,mSensorAccelermeter,mSensorSampleRate);

        mSensorGyroscope=sManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
        sManager.registerListener(this,mSensorGyroscope,mSensorSampleRate);

        mSensorMagnetic=sManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
        sManager.registerListener(this,mSensorMagnetic,mSensorSampleRate);

        mSensorTemperature=sManager.getDefaultSensor(Sensor.TYPE_AMBIENT_TEMPERATURE);
        sManager.registerListener(this,mSensorTemperature,mSensorSampleRate);

        mSensorLight=sManager.getDefaultSensor(Sensor.TYPE_LIGHT);
        sManager.registerListener(this,mSensorLight,mSensorSampleRate);

        mSensorProximity=sManager.getDefaultSensor(Sensor.TYPE_PRESSURE);
        sManager.registerListener(this,mSensorProximity,mSensorSampleRate);

        bindViews();

        //处理连接按钮按下事件
        connectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //新建进程
                new Thread() {
                    public void run(){
                        connectServer();
                        Message message = new Message();
                        message.what = 1;
                        handler.sendMessage(message);
                        try{
                            while(mEnableSendFlag){
                                //打包并发送数据
                                packageDataToSend();
                                mDataOutStream.write(mDataToSendByteArray);
                                mDataOutStream.flush();
                                mSendSocketDataCount++;
                                Message message1 = new Message();
                                message1.what = 3;
                                handler.sendMessage(message1);
                                Thread.sleep(mSendTimeInterval);
                            }
                            disconnectServer();
                            Message message2 = new Message();
                            message2.what = 2;
                            handler.sendMessage(message2);
                        }catch (Exception e) {
                            mEnableSendFlag=false;
                            e.printStackTrace();
                        }
                    }
                }.start();
                }
        });

        //处理断开按钮按下事件
        disconnectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mEnableSendFlag = false;
            }
        });

    }


    //运行在主线程的消息处理句柄
    Handler handler = new Handler() {
        public void handleMessage(Message msg) {

  //          super.handleMessage(msg);
            //socket连接出错消息
            if(msg.what==1){
                //注意子线程中无法修改GUI
                proximityValue.setText("ip: "+mLocalMachineIp);
                if (mEnableSendFlag) {
                    midToast("connect to server success!");
                    connectButton.setEnabled(false);
                    connectButton.setBackgroundColor(Color.GREEN);
                    disconnectButton.setEnabled(true);
                    disconnectButton.setBackgroundColor(Color.LTGRAY);
                    serverIpText.setEnabled(false);
                    serverPortText.setEnabled(false);
                    machineId.setEnabled(false);
                    sendInterval.setEnabled(false);
                    mSendSocketDataCount = 0;
                }
                else{
                    midToast("can not connect to server!");
                    connectButton.setEnabled(true);
                    connectButton.setBackgroundColor(Color.LTGRAY);
                    disconnectButton.setEnabled(false);
                    disconnectButton.setBackgroundColor(Color.RED);
                    serverIpText.setEnabled(true);
                    serverPortText.setEnabled(true);
                    machineId.setEnabled(true);
                    sendInterval.setEnabled(true);
                    mSendSocketDataCount = 0;
                }
            }
            //socket断开出错信息
            else if(msg.what==2){
                //注意子线程中无法修改GUI
                if (mEnableSendFlag) {
                    midToast("can not disconnect server!");
                    connectButton.setEnabled(false);
                    connectButton.setBackgroundColor(Color.GREEN);
                    disconnectButton.setEnabled(true);
                    disconnectButton.setBackgroundColor(Color.LTGRAY);
                    serverIpText.setEnabled(false);
                    serverPortText.setEnabled(false);
                    machineId.setEnabled(false);
                    sendInterval.setEnabled(false);
                    mSendSocketDataCount = 0;
                }
                else{
                    midToast("disconnect server success!");
                    connectButton.setEnabled(true);
                    connectButton.setBackgroundColor(Color.LTGRAY);
                    disconnectButton.setEnabled(false);
                    disconnectButton.setBackgroundColor(Color.RED);
                    serverIpText.setEnabled(true);
                    serverPortText.setEnabled(true);
                    machineId.setEnabled(true);
                    sendInterval.setEnabled(true);
                    mSendSocketDataCount = 0;
                }
            }
            //数据发送成功消息
            else if(msg.what==3) {
                proximityValue.setText("send count: "+mSendSocketDataCount);

            }

            else{
                proximityValue.setText("oh shit! "+msg.what);
            }


        }
    };


    private void bindViews() {
        accelermeterX=(TextView)findViewById(R.id.accelerometerX);
        accelermeterY=(TextView)findViewById(R.id.accelerometerY);
        accelermeterZ=(TextView)findViewById(R.id.accelerometerZ);

        gyroscopeX=(TextView)findViewById(R.id.gyroscopeX);
        gyroscopeY=(TextView)findViewById(R.id.gyroscopeY);
        gyroscopeZ=(TextView)findViewById(R.id.gyroscopeZ);

        magneticX=(TextView)findViewById(R.id.magneticX);
        magneticY=(TextView)findViewById(R.id.magneticY);
        magneticZ=(TextView)findViewById(R.id.magneticZ);

        temperatureValue=(TextView)findViewById(R.id.temperatureValue);
        lightValue=(TextView)findViewById(R.id.lightValue);
        proximityValue=(TextView)findViewById(R.id.proximityValue);

        serverIpText=(EditText)findViewById(R.id.serverIP);
        serverPortText=(EditText)findViewById(R.id.serverPort);
        machineId=(EditText)findViewById(R.id.machineId);
        sendInterval=(EditText)findViewById(R.id.sendInterval);

        //设置默认值
        serverIpText.setText(defalutServerIp);
        serverPortText.setText(defaultServerPort);
        machineId.setText(Integer.toString(mMachineId));
        sendInterval.setText(Integer.toString(mSendTimeInterval));

        connectButton=(Button)findViewById(R.id.connectButton);
        disconnectButton=(Button)findViewById(R.id.disconnectButton);

        //清空发送缓存
        for(int i=0;i<mDataToSendIntArray.length;++i){
            mDataToSendIntArray[i]=0;
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        Sensor sensor=event.sensor;
        switch(sensor.getType()){
            case Sensor.TYPE_ACCELEROMETER:
                accelermeterX.setText("x axis:  "+(float)(Math.round(event.values[0]*100))/100);
                accelermeterY.setText("y axis:  "+(float)(Math.round(event.values[1]*100))/100);
                accelermeterZ.setText("z axis:  "+(float)(Math.round(event.values[2]*100))/100);
                mDataToSendIntArray[6]=Math.round(event.values[0]*1000);
                mDataToSendIntArray[7]=Math.round(event.values[1]*1000);
                mDataToSendIntArray[8]=Math.round(event.values[2]*1000);
                break;

            case Sensor.TYPE_GYROSCOPE:
                gyroscopeX.setText("x axis:  "+(float)(Math.round(event.values[0]*100))/100);
                gyroscopeY.setText("y axis:  "+(float)(Math.round(event.values[1]*100))/100);
                gyroscopeZ.setText("z axis:  "+(float)(Math.round(event.values[2]*100))/100);
                mDataToSendIntArray[9]=Math.round(event.values[0]*1000);
                mDataToSendIntArray[10]=Math.round(event.values[1]*1000);
                mDataToSendIntArray[11]=Math.round(event.values[2]*1000);
                break;

            case Sensor.TYPE_MAGNETIC_FIELD:
                magneticX.setText("x axis:  "+(float)(Math.round(event.values[0]*100))/100);
                magneticY.setText("y axis:  "+(float)(Math.round(event.values[1]*100))/100);
                magneticZ.setText("z axis:  "+(float)(Math.round(event.values[2]*100))/100);
                mDataToSendIntArray[12]=Math.round(event.values[0]*1000);
                mDataToSendIntArray[13]=Math.round(event.values[1]*1000);
                mDataToSendIntArray[14]=Math.round(event.values[2]*1000);
                break;

            case Sensor.TYPE_AMBIENT_TEMPERATURE:
                temperatureValue.setText("temper: "+(float)(Math.round(event.values[0]*100))/100);
                mDataToSendIntArray[15]=Math.round(event.values[0]*1000);
                break;

            case Sensor.TYPE_LIGHT:
                lightValue.setText("light:    "+(float)(Math.round(event.values[0]*100))/1000);
                mDataToSendIntArray[16]=Math.round(event.values[0]*1000/10);        //note that light is too big
                break;
            case Sensor.TYPE_PROXIMITY:
                proximityValue.setText("proximity:"+(float)(Math.round(event.values[0]*100))/100);
                mDataToSendIntArray[17]=Math.round(event.values[0]*1000);
                break;
            default:
                break;
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    @Override
    public void onBackPressed()
    {
        System.exit(0);
    }

    //用于显示提示消息
    private void midToast(String str)
    {
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, str, Toast.LENGTH_SHORT);
        toast.setGravity(Gravity.CENTER_VERTICAL|Gravity.CENTER_HORIZONTAL , 0, 0);  //设置显示位置
        TextView v = (TextView) toast.getView().findViewById(android.R.id.message);
        v.setTextColor(Color.WHITE);     //设置字体颜色
        toast.show();
    }

    private void connectServer() {
        try{
            InetAddress addr=InetAddress.getLocalHost();
            mLocalMachineIp=getLocalIP();
            mLocalHostName=addr.getHostName();
        }catch(Exception e){
            mLocalMachineIp="";
            mLocalHostName="";
        }
        try {
        //读取设置的选项
        String serverIp = serverIpText.getText().toString();
        int serverPort = Integer.parseInt(serverPortText.getText().toString());
        mMachineId=Integer.parseInt(machineId.getText().toString());
        mSendTimeInterval=Integer.parseInt(sendInterval.getText().toString());
            int i = 3;
            while ((i--) > 0) {
                //     socket.close();
                socket = new Socket(serverIp, serverPort);
                socket.setReuseAddress(true);
                socket.setTcpNoDelay(true);
                if(socket.isConnected()){
                    mDataInStream=new DataInputStream(socket.getInputStream());
                    mDataOutStream=new DataOutputStream(socket.getOutputStream());
                    mEnableSendFlag=true;
                    break;
                }
                else{
                    mEnableSendFlag=false;
                }
            }
        } catch (Exception e) {
            mEnableSendFlag=false;
            e.printStackTrace();
        }
    }

    private void disconnectServer(){
        try{
            mDataInStream.close();
            mDataOutStream.close();
            socket.close();
            mEnableSendFlag=false;
        }catch(IOException e){
            mEnableSendFlag=true;
            Log.d("disconnect server: ", e.toString());
        }
    }

    public static String getLocalIP()
    {
        String ipaddress = "";
        try {
            Enumeration<NetworkInterface> en = NetworkInterface
                    .getNetworkInterfaces();
            // 遍历所用的网络接口
            while (en.hasMoreElements())
            {
                NetworkInterface nif = en.nextElement();// 得到每一个网络接口绑定的所有ip
                Enumeration<InetAddress> inet = nif.getInetAddresses();
                // 遍历每一个接口绑定的所有ip
                while (inet.hasMoreElements()) {
                    InetAddress ip = inet.nextElement();
                    if (!ip.isLoopbackAddress() && (ip instanceof Inet4Address)) {
                        return ipaddress = ip.getHostAddress();
                    }
                }
            }
        }
        catch (SocketException e) {
            Log.e("feige", "获取本地ip地址失败");
            e.printStackTrace();
        }
        return ipaddress;
    }

    //有关数据协议的格式是在这里实现的
    private void packageDataToSend(){
        //第0位是机器Id
        mDataToSendIntArray[0]=mMachineId;

        //第1位是ip的整型
        mDataToSendIntArray[1]=IPv4IntTransformer.ip2Integer(mLocalMachineIp);
        //第2位是时间戳高位
        Long s=System.currentTimeMillis();
        mDataToSendIntArray[2]=(int)(long)(s>>32&0XFFFFFFFF);
        //第3位位时间戳低位
        mDataToSendIntArray[3]=(int)(long)(s&0XFFFFFFFF);

        //第4位为发送计数
        mDataToSendIntArray[4]=mSendSocketDataCount;

        //第5位空闲

        //第6~20位为传感器数据，在传感器函数里更新

        //将int 数组转为 byte数组
        for(int i=0;i<mDataToSendIntArray.length;++i){
            mDataToSendByteArray[i*4]=(byte)(mDataToSendIntArray[i]&0xff);
            mDataToSendByteArray[i*4+1]=(byte)(mDataToSendIntArray[i]>>8&0xff);
            mDataToSendByteArray[i*4+2]=(byte)(mDataToSendIntArray[i]>>16&0xff);
            mDataToSendByteArray[i*4+3]=(byte)(mDataToSendIntArray[i]>>24&0xff);
        }
    }

}

