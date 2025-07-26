#include "mainwindow2.h"
#include "ui_mainwindow2.h"
QStringList lines ;
QNetworkReply *reply;
MainWindow2::MainWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2)
{
    ui->setupUi(this);
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(func()));
    timer->start(0);
}
MainWindow2::~MainWindow2()
{
    delete ui;
}
void MainWindow2::func()
{
    QString Instruction="";
    QDateTime issueDate = QDateTime::currentDateTime();
    CurrentTime=issueDate.toString("yyyy-MM-dd HH:mm:ss");
    d.dropReservationId(CurrentTime); // If current date is matched then drop the row
    d.get_php_bitset(&Instruction,&params[0],&params[1],&params[2],&params[3],&params[4],&params[5],&params[6],&params[7],&params[8]);
    if(Instruction=="CancelReservation")
    {
        CancelReservation_php();
    }
    else if(Instruction=="ChangeAvailability")
    {
        ChangeAvailability_php();
    }
    if(Instruction=="ChangeConfiguration")
    {
        ChangeConfiguration_php();
    }
    else if(Instruction=="ClearCache")
    {
        ClearCache_php();
    }
    if(Instruction=="DataTransfer")
    {
        DataTransfer_php();
    }
    else if(Instruction=="GetConfiguration")
    {
        GetConfiguration_php();
    }
    if(Instruction=="GetDiagnostics")
    {
        GetDiagnostics_php();
    }
    else if(Instruction=="GetLocalListVersion")
    {
        GetLocalListVersion_php();
    }
    else if(Instruction=="RemoteStartTransaction")
    {
        RemoteStartTransaction_php();
    }
    if(Instruction=="RemoteStopTransaction")
    {
        RemoteStopTransaction_php();
    }
    else if(Instruction=="ReserveNow")
    {
        ReserveNow_php();
    }
    if(Instruction=="Reset")
    {
        Reset_php();
    }
    else if(Instruction=="SendLocalList")
    {
        SendLocalList_php();
    }
    else if(Instruction=="TriggerMessage")
    {
        TriggerMessage_php();
    }
    if(Instruction=="UnlockConnector")
    {
        UnlockConnector_php();
    }
    else if(Instruction=="UpdateFirmware")
    {
        UpdateFirmware_php();
    }


}
void MainWindow2::on_pushButton_StartChargePoint_clicked()
{
        ui->pushButton_StartChargePoint->setEnabled(false);
        ui->pushButton_StopChargePoint->setEnabled(true);
        ip=ui->comboBox_ip->currentText();
        QString port=ui->lineEdit_port_2->text();
        QString Url="ws://"+ip+":"+port+"/ocppj16/CentralSystem"; // Websocket Url to connect to charge point server
        qDebug() << "WebSocket server:" << Url;
        m_webSocket.open(QUrl(Url)); // Open Websocket
        connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &MainWindow2::Received); // If Text message received go to received loop
}
void MainWindow2::on_pushButton_StopChargePoint_clicked()
{
    ui->pushButton_StartChargePoint->setEnabled(true);
    ui->pushButton_StopChargePoint->setEnabled(false);
    m_webSocket.close(); //  Close websocket
}
void MainWindow2::Received(QString message)
{
    if(message!="")
    {
    qDebug() << "message Received" <<message;
    lines = message.split(',');
    msg_Id =lines.at(0);
    uniq_Id=lines.at(1);
    PayloadHead =lines.at(2);
    Payload=lines.at(3);
        if(msg_Id=="3")
        {
            Sub3();
        }
    }
}
void MainWindow2::Sub3()
{
    if(PayloadHead=="CancelReservation")
    {
        lines = Payload.split('|');
        QString x=lines.at(0);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = x2.split('}');
        x2=lines.at(0);
        if(x1=="Status")
        {
            Status.append(x2);
            ui->lineEdit_Status_CancelReservation_Response->setText(Status);
            Status="";
        }
    }
    if(PayloadHead=="ChangeAvailability")
    {
        lines = Payload.split('|');
         QString x=lines.at(0);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = x2.split('}');
        x2=lines.at(0);
        if(x1=="Status")
        {
            Status.append(x2);
            ui->lineEdit_Status_Change_Availability_Response->setText(Status);
            Status="";
        }
    }
    if(PayloadHead=="ChangeConfiguration")
    {
        lines = Payload.split('|');
         QString x=lines.at(0);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = x2.split('}');
        x2=lines.at(0);
        if(x1=="Status")
        {
            Status.append(x2);
            ui->lineEdit_Status_Change_Configuration_Response->setText(Status);
            Status="";
        }
    }
    if(PayloadHead=="ClearCache")
    {
        lines = Payload.split('|');
         QString x=lines.at(0);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = x2.split('}');
        x2=lines.at(0);
        if(x1=="Status")
        {
            Status.append(x2);
            ui->lineEdit_Status_Clear_Cache_Response->setText(Status);
            Status="";
        }
    }
    /*if(PayloadHead=="ClearChargingProfile")
    {
        lines = Payload.split('|');
         QString x=lines.at(0);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = x2.split('}');
        x2=lines.at(0);
        if(x1=="Status")
        {
            Status.append(x2);
            ui->lineEdit_Status_Clear_Changing_Profile_Response->setText(Status);
            Status="";
        }
    }*/
    if(PayloadHead=="DataTransfer")
    {
    }
    /*if(PayloadHead=="GetCompositeSchedule")
    {
        GetCompositeSchedule();
    }*/
    if(PayloadHead=="GetConfiguration")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = b2.split('}');
        b2=lines.at(0);
        if(a1=="Key")
        {
            Key.append(a2);
            NewValue.append(b2);
            ui->lineEdit_Key_Getconfig_response->setText(Key);
            ui->lineEdit_Value_GetConfig_response->setText(NewValue);
            Key="";NewValue="";
        }
    }
    if(PayloadHead=="GetDiagnostics")
    {
        lines = Payload.split('|');
        QString x=lines.at(0);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = x2.split('}');
        x2=lines.at(0);
        if(x1=="FileName")
        {
            FileName.append(x2);
            ui->lineEdit_FileName_Get_Diagnostics_Response->setText(FileName);
            tcp->~serversocket();
            FileName="";
        }
    }
    if(PayloadHead=="GetLocalListVersion")
    {
        QString LocalListVersion;
        lines = Payload.split('|');
        QString x=lines.at(0);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = x2.split('}');
        x2=lines.at(0);
        if(x1=="LocalListVersion")
        {
            LocalListVersion.append(x2);
            ui->lineEdit_LocalListVersion_Response->setText(LocalListVersion);
            LocalListVersion="";
        }
    }
    if(PayloadHead=="RemoteStartTransaction")
    {
        lines = Payload.split('|');
        QString x=lines.at(0);
       lines = x.split('=');
       QString x1=lines.at(0);
       QString x2=lines.at(1);
       lines=x1.split('{');
       x1=lines.at(1);
       lines = x2.split('}');
       x2=lines.at(0);
       if(x1=="Status")
       {
           Status.append(x2);
           ui->lineEdit_Status_Remote_Start_Transaction_Response->setText(Status);
           Status="";
       }
    }
    if(PayloadHead=="RemoteStopTransaction")
    {
        lines = Payload.split('|');
        QString x=lines.at(0);
       lines = x.split('=');
       QString x1=lines.at(0);
       QString x2=lines.at(1);
       lines=x1.split('{');
       x1=lines.at(1);
       lines = x2.split('}');
       x2=lines.at(0);
       if(x1=="Status")
       {
           Status.append(x2);
           ui->lineEdit_Status_Remote_Stop_Transaction_Response->setText(Status);
           Status="";
       }
    }
    if(PayloadHead=="ReserveNow")
    {
        lines = Payload.split('|');
        QString x=lines.at(0);
       lines = x.split('=');
       QString x1=lines.at(0);
       QString x2=lines.at(1);
       lines=x1.split('{');
       x1=lines.at(1);
       lines = x2.split('}');
       x2=lines.at(0);
       if(x1=="Status")
       {
           Status.append(x2);
           ui->lineEdit_Status_ReserveNow_Response->setText(Status);
           Status="";
       }
    }
    if(PayloadHead=="Reset")
    {
        lines = Payload.split('|');
         QString x=lines.at(0);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = x2.split('}');
        x2=lines.at(0);
        if(x1=="Status")
        {
            Status.append(x2);
            ui->lineEdit_Status_Reset_Response->setText(Status);
            Status="";
        }
    }
    if(PayloadHead=="SendLocalList")
    {
        lines = Payload.split('|');
         QString x=lines.at(0);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = x2.split('}');
        x2=lines.at(0);
        if(x1=="Status")
        {
            Status.append(x2);
            ui->lineEdit_Status_SendLocalList_Response->setText(Status);
            Status="";
        }
    }
    /*if(PayloadHead=="SetChargingProfile")
    {
        lines = Payload.split('|');
         QString x=lines.at(0);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = x2.split('}');
        x2=lines.at(0);
        if(x1=="Status")
        {
            Status.append(x2);
            ui->lineEdit_Status_Set_Charging_Profile_Response->setText(Status);
        }
    }*/
    if(PayloadHead=="TriggerMessage")
    {
        lines = Payload.split('|');
        QString x=lines.at(0);
       lines = x.split('=');
       QString x1=lines.at(0);
       QString x2=lines.at(1);
       lines=x1.split('{');
       x1=lines.at(1);
       lines = x2.split('}');
       x2=lines.at(0);
       if(x1=="Status")
       {
           Status.append(x2);
           ui->lineEdit_Status_TriggerMessage_Response->setText(Status);
           Status="";

        }
    }
    if(PayloadHead=="UnlockConnector")
    {
        lines = Payload.split('|');
        QString x=lines.at(0);
       lines = x.split('=');
       QString x1=lines.at(0);
       QString x2=lines.at(1);
       lines=x1.split('{');
       x1=lines.at(1);
       lines = x2.split('}');
       x2=lines.at(0);
       if(x1=="Status")
       {
           Status.append(x2);
           ui->lineEdit_Status_UnlockConnector_Response->setText(Status);
           Status="";
       }
    }
    if(PayloadHead=="UpdateFirmware")
    {
        lines = Payload.split('}');
        QString u=lines.at(0);
        lines=u.split('{');
        u=lines.at(1);
        if(u=="Acknowledgementfromserver")
        {
           tcp->~serversocket();
           qDebug()<<u;
        }
    }
}
void MainWindow2::CancelReservation()
{
    ReservationId=ui->lineEdit_ReservationID_CancelReservation_Request->text();
    result="2,"+ip+",CancelReservation,{ReservationId="+ReservationId+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ReservationId="";result="";
}
void MainWindow2::ChangeAvailability()
{
    ConnectorId=ui->lineEdit_ConnectorID_Change_Availability_Request->text();
    Type=ui->comboBox_Type_Change_Availability_Request->currentText();
    result="2,"+ip+",ChangeAvailability,{ConnectorId="+ConnectorId+"|Type="+Type+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ConnectorId="";Type="";result="";
}
void MainWindow2::ChangeConfiguration()
{
    Key=ui->lineEdit_Key_Change_Configuration_Request->text();
    NewValue=ui->lineEdit_NewValue_Change_Configuration_Request->text();
    result="2,"+ip+",ChangeConfiguration,{Key="+Key+"|NewValue="+NewValue+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    Key="";NewValue="";result="";
}
void MainWindow2::ClearCache()
{
    result="2,"+ip+",ClearCache,{RequestClearCache}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    result="";
}
/*void MainWindow2::ClearChargingProfile()
{
    ID=ui->lineEdit_Id_Clear_Changing_Profile_Request->text();
    ConnectorId=ui->lineEdit_ConnectorID_Clear_Changing_Profile_Request->text();
    PurposeType=ui->comboBox_Chargingprofilepurpose_Clear_Changing_Profile_Request->currentText();
    result="2,"+ip+",ClearChargingProfile,{ID="+ID+"|ConnectorId="+ConnectorId+"|PurposeType="+PurposeType+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ID="";ConnectorId="";PurposeType="";result="";
}*/
void MainWindow2::DataTransfer()
{
    VendorId=ui->lineEdit_vendorId_DataTransfer_Request->text();
    MessageId=ui->lineEdit_MessageID_DataTransfer_Request->text();
    Data=ui->lineEdit_DataReceived_DataTransfer_Reqeust->text();
    result="2,"+ip+",DataTransfer,{VendorId="+VendorId+"|MessageId="+MessageId+"|Data="+Data+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    VendorId="";MessageId="";Data="";result="";
}
/*void MainWindow2::GetCompositeSchedule()
{
    ConnectorId=ui->lineEdit_ConnectorID_Get_Composite_Schedule_Request->text();
    Duration=ui->lineEdit_Duration_Get_Composite_Schedule_Request->text();
    ChargingUnitType=ui->combox_ChargingRateUnit_Get_Composite_Schedule_Request->currentText();
    result="2,"+ip+",GetCompositeSchedule,{ConnectorId="+ConnectorId+"|Duration="+Duration+"|ChargingUnitType="+ChargingUnitType+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ConnectorId="";Duration="";ChargingUnitType="";result="";
}*/
void MainWindow2::GetConfiguration()
{
    Key=ui->lineEdit_Key_GetConfig_request->text();
    result="2,"+ip+",GetConfiguration,{Key="+Key+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    Key="";
}
void MainWindow2::GetDiagnostics()
{
    Location=ui->lineEdit_Location_Get_Diagnostics_Request->text();
    Retries=ui->lineEdit_Retries__Get_Diagnostics_Request->text();
    RetryInterval=ui->lineEdit_RetryInterval__Get_Diagnostics_Request->text();
    QDateTime starttime=ui->dateTimeEdit_Starttime_Get_Diagnostics_Request->dateTime();
    QDateTime stoptime=ui->dateTimeEdit_StopTime_Get_Diagnostics_Request->dateTime();
    StartTime =starttime.toString("yyyy-MM-dd HH:mm:ss");
    StopTime =stoptime.toString("yyyy-MM-dd HH:mm:ss");
    tcp->incomingConnection(-1,Location);
    result="2,"+ip+",GetDiagnostics,{Location="+Location+"|Retries="+Retries+"|RetryInterval="+RetryInterval+"|StartTime="+StartTime+"|StopTime="+StopTime+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    Location="";Retries="";RetryInterval="";StartTime="";StopTime="";result="";
}
void MainWindow2::GetLocalListVersion()
{
    result="2,"+ip+",GetLoaclListVersion,{RequestLocalListVersion}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    result="";
}
void MainWindow2::RemoteStartTransaction()
{
           ConnectorId=ui->lineEdit_ConnectorID_Remote_Start_Transaction_Request->text();
           d.get_client_changeavailability(&avail,ConnectorId); // check Availability Status of that connector
           if(avail==1)
           {
           IdTag=ui->lineEdit_IdTag_Remote_Start_Transaction_Request->text();
           //QString ChargingProfileID=ui->lineEdit_ChargingProfileID_Remote_Start_Transaction_Request->text();
           quint16 Transaction = QRandomGenerator::global()->generate();
           TransactionId = QString::number(Transaction);
           StackLevel=ui->lineEdit_StackLevel_Remote_Start_Transaction_Request->text();
           //QString purpose=ui->comboBox_Purpose_Remote_Start_Transaction_Request->currentText();
           //QString ProfileType=ui->comboBox_ProfileType_Remote_Start_Transaction_Request->currentText();
           //QString RecurenceType=ui->comboBox_RecurrencyType_Remote_Start_Transaction_Request->currentText();
           //QString Validfrom=ui->lineEdit_Validfrom_Remote_Start_Transaction_Request->text();
           //QString ValidTo=ui->lineEdit_Validto_Remote_Start_Transaction_Request->text();
           //QString Duration=ui->lineEdit_Duration_Remote_Start_Transaction_Request->text();
           //QString Startschedule=ui->lineEdit_StartSchedule_Remote_Start_Transaction_Request->text();
           UnitType = ui->comboBox_UnitType_Remote_Start_Transaction_Request->currentText();
           MinChargingRate=ui->lineEdit_MinChargingRate_Remote_Start_Transaction_Request->text();
           StartPeriod = ui->lineEdit_StartPeriod_Remote_Start_Transaction_Request->text();
           Limit=ui->lineEdit_Limit_Remote_Start_Transaction_Request->text();
           NumberPhases=ui->lineEdit_NumberPhases_Remote_Start_Transaction_Request->text();
           result="2,"+ip+",RemoteStartTransaction,{ConnectorId="+ConnectorId+"|IdTag="+IdTag+"|TransactionId="+TransactionId+"|StackLevel="+StackLevel+"|UnitType="+UnitType+"|MinChargingrate="+MinChargingRate+"|StartPeriod="+StartPeriod+"|Limit="+Limit+"|NumberPhases="+NumberPhases+"}";
           m_webSocket.sendTextMessage(result);
           qDebug()<<"Message sent :"<<result;
           ConnectorId="";IdTag="";TransactionId="";StackLevel="";UnitType="";MinChargingRate="";StartPeriod="";Limit="";NumberPhases="";result="";
           }
           else
           {
               Status="Rejected";
           }
  }
void MainWindow2::RemoteStopTransaction()
{
    TransactionId=ui->lineEdit_TransactionID_Remote_Stop_Transaction_Request->text();
    result="2,"+ip+",RemoteStopTransaction,{TransactionId="+TransactionId+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    TransactionId="";result="";
}
void MainWindow2::ReserveNow()
{
    ConnectorId=ui->lineEdit_ConnectorID_ReserveNow_Request->text();
    IdTag=ui->lineEdit_IdTag_ReserveNow_Request->text();
    QDateTime reservedTime=ui->dateTimeEdit_ReservedTime_reservenow->dateTime();
    QDateTime expiryTime=ui->dateTimeEdit_ExpiryTime_reservenow->dateTime();
    qint64 secondsDiff = expiryTime.secsTo(reservedTime);
    QString ReservedTime=reservedTime.toString("yyyy-MM-dd HH:mm:ss");
    QString ExpiryTime=expiryTime.toString("yyyy-MM-dd HH:mm:ss");
    QString Duration=QDateTime::fromTime_t(secondsDiff).toString("HH:mm:ss");
    d.Get_Authorize(&ExpiryDate,&ParentIdTag,IdTag);// Get from user details using id tag
    quint16 Reservation = QRandomGenerator::global()->generate();
    ReservationId= QString::number(Reservation);
    result="2,"+ip+",ReserveNow,{ConnectorId="+ConnectorId+"|IdTag="+IdTag+"|ParentIdTag="+ParentIdTag+"|ReservedTime="+ReservedTime+"|ExpiryTime="+ExpiryTime+"|Duration="+Duration+"|ReservationId="+ReservationId+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ReservationId="";ParentIdTag="";IdTag="";ExpiryDate="";ConnectorId="";result="";Duration="";
}
void MainWindow2::Reset()
{
    Type=ui->comboBox_Type_Reset_Request->currentText();
    result="2,"+ip+",Reset,{Type="+Type+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    Type="";result="";
}
void MainWindow2::SendLocalList(QString stream,int LocalListVersion)
{
    UpdateType=ui->comboBox_UpdateType_SendLocalList_Request->currentText();
    ListVersion=QString::number(LocalListVersion);
    result="2,"+ip+",SendLocalList,{ListVersion="+ListVersion+"|UpdateType="+UpdateType+"|Stream="+stream+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ListVersion="";UpdateType="";stream="";result="";
}
/*void MainWindow2::SetChargingProfile()
{
    ConnectorId=ui->lineEdit_ConnectorID_Remote_Start_Transaction_Request->text();
    IdTag=ui->lineEdit_IdTag_Remote_Start_Transaction_Request->text();
    //QString ChargingProfileID=ui->lineEdit_ChargingProfileID_Remote_Start_Transaction_Request->text();
    //QString TransactionId=ui->lineEdit_TransactionID_Remote_Start_Transaction_Request->text();
    StackLevel=ui->lineEdit_StackLevel_Remote_Start_Transaction_Request->text();
    //QString purpose=ui->comboBox_Purpose_Remote_Start_Transaction_Request->currentText();
    //QString ProfileType=ui->comboBox_ProfileType_Remote_Start_Transaction_Request->currentText();
    //QString RecurenceType=ui->comboBox_RecurrencyType_Remote_Start_Transaction_Request->currentText();
    //QString Validfrom=ui->lineEdit_Validfrom_Remote_Start_Transaction_Request->text();
    //QString ValidTo=ui->lineEdit_Validto_Remote_Start_Transaction_Request->text();
    //QString Duration=ui->lineEdit_Duration_Remote_Start_Transaction_Request->text();
    //QString Startschedule=ui->lineEdit_StartSchedule_Remote_Start_Transaction_Request->text();
    UnitType = ui->comboBox_UnitType_Remote_Start_Transaction_Request->currentText();
    MinChargingRate=ui->lineEdit_MinChargingRate_Remote_Start_Transaction_Request->text();
    StartPeriod = ui->lineEdit_StartPeriod_Remote_Start_Transaction_Request->text();
    Limit=ui->lineEdit_Limit_Remote_Start_Transaction_Request->text();
    NumberPhases=ui->lineEdit_NumberPhases_Remote_Start_Transaction_Request->text();
    result="2,"+ip+",RemoteStartTransaction,{ConnectorId="+ConnectorId+"|IdTag="+IdTag+"|StackLevel="+StackLevel+"|UnitType="+UnitType+"|MinChargingrate="+MinChargingRate+"|StartPeriod="+StartPeriod+"|Limit="+Limit+"|NumberPhases="+NumberPhases+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ConnectorId="";IdTag="";StackLevel="";UnitType="";MinChargingRate="";StartPeriod="";Limit="";NumberPhases="";result="";
}*/
void MainWindow2::TriggerMessage()
{
    ConnectorId=ui->lineEdit_ConnectorId_TriggerMessage_Request->text();
    RequestMessage=ui->comboBox_RequestedMessage_TriggerMessage_Request->currentText();
    result="2,"+ip+",TriggerMessage,{RequestMessage="+RequestMessage+"|ConnectorId="+ConnectorId+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ConnectorId="";RequestMessage="";result="";
}
void MainWindow2::UnlockConnector()
{
    ConnectorId=ui->lineEdit_ConnectorId_UnlockConnector_Request->text();
    result="2,"+ip+",UnlockConnector,{ConnectorId="+ConnectorId+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ConnectorId="";result="";
}
void MainWindow2::UpdateFirmware()
{
    Location=ui->lineEdit_Location_UpdateFirmware_Request->text();
    Retries=ui->lineEdit_Retires_UpdateFirmware_Request->text();
    QDateTime retrievedate=ui->dateTimeEdit_RetrieveDate_UpdateFirmware_Request->dateTime();
    RetrieveDate=retrievedate.toString("yyyy-MM-dd HH:mm:ss");
    RetryInterval=ui->lineEdit_RetryInterval_UpdateFirmware_Request->text();
    tcp->incomingConnection(-1,Location);
    result="2,"+ip+",UpdateFirmware,{Location="+Location+"|Retries="+Retries+"|RetrieveDate="+RetrieveDate+"|RetryInterval="+RetryInterval+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    Location="";Retries="";RetryInterval="";RetrieveDate="";result="";
}




void MainWindow2::CancelReservation_php()
{
    ReservationId=params[0];
    result="2,"+ip+",CancelReservation,{ReservationId="+ReservationId+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ReservationId="";result="";
}
void MainWindow2::ChangeAvailability_php()
{
    ConnectorId=params[0];
    Type=params[1];
    result="2,"+ip+",ChangeAvailability,{ConnectorId="+ConnectorId+"|Type="+Type+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ConnectorId="";Type="";result="";
}
void MainWindow2::ChangeConfiguration_php()
{
    Key=params[0];
    NewValue=params[1];
    result="2,"+ip+",ChangeConfiguration,{Key="+Key+"|NewValue="+NewValue+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    Key="";NewValue="";result="";
}
void MainWindow2::ClearCache_php()
{
    result="2,"+ip+",ClearCache,{RequestClearCache}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    result="";
}
void MainWindow2::DataTransfer_php()
{
    VendorId=params[0];
    MessageId=params[1];
    Data=params[2];
    result="2,"+ip+",DataTransfer,{VendorId="+VendorId+"|MessageId="+MessageId+"|Data="+Data+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    VendorId="";MessageId="";Data="";result="";
}
void MainWindow2::GetConfiguration_php()
{
    Key=params[0];
    result="2,"+ip+",GetConfiguration,{Key="+Key+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    Key="";
}
void MainWindow2::GetDiagnostics_php()
{
    Location=params[0];
    Retries=params[1];
    RetryInterval=params[2];
    StartTime=params[3];
    StopTime =params[4];
    tcp->incomingConnection(-1,Location);
    result="2,"+ip+",GetDiagnostics,{Location="+Location+"|Retries="+Retries+"|RetryInterval="+RetryInterval+"|StartTime="+StartTime+"|StopTime="+StopTime+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    Location="";Retries="";RetryInterval="";StartTime="";StopTime="";result="";
}
void MainWindow2::GetLocalListVersion_php()
{
    result="2,"+ip+",GetLoaclListVersion,{RequestLocalListVersion}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    result="";
}
void MainWindow2::RemoteStartTransaction_php()
{
           ConnectorId=params[0];
           d.get_client_changeavailability(&avail,ConnectorId); // check Availability Status of that connector
           if(avail==1)
           {
           IdTag=params[1];
           //QString ChargingProfileID=ui->lineEdit_ChargingProfileID_Remote_Start_Transaction_Request->text();
           quint16 Transaction = QRandomGenerator::global()->generate();
           TransactionId = QString::number(Transaction);
           StackLevel=params[2];
           //QString purpose=ui->comboBox_Purpose_Remote_Start_Transaction_Request->currentText();
           //QString ProfileType=ui->comboBox_ProfileType_Remote_Start_Transaction_Request->currentText();
           //QString RecurenceType=ui->comboBox_RecurrencyType_Remote_Start_Transaction_Request->currentText();
           //QString Validfrom=ui->lineEdit_Validfrom_Remote_Start_Transaction_Request->text();
           //QString ValidTo=ui->lineEdit_Validto_Remote_Start_Transaction_Request->text();
           //QString Duration=ui->lineEdit_Duration_Remote_Start_Transaction_Request->text();
           //QString Startschedule=ui->lineEdit_StartSchedule_Remote_Start_Transaction_Request->text();
           UnitType = params[3];
           MinChargingRate=params[4];
           StartPeriod =params[5];
           Limit=params[6];
           NumberPhases=params[7];
           result="2,"+ip+",RemoteStartTransaction,{ConnectorId="+ConnectorId+"|IdTag="+IdTag+"|TransactionId="+TransactionId+"|StackLevel="+StackLevel+"|UnitType="+UnitType+"|MinChargingrate="+MinChargingRate+"|StartPeriod="+StartPeriod+"|Limit="+Limit+"|NumberPhases="+NumberPhases+"}";
           m_webSocket.sendTextMessage(result);
           qDebug()<<"Message sent :"<<result;
           ConnectorId="";IdTag="";TransactionId="";StackLevel="";UnitType="";MinChargingRate="";StartPeriod="";Limit="";NumberPhases="";result="";
           }
           else
           {
               Status="Rejected";
           }
  }
void MainWindow2::RemoteStopTransaction_php()
{
    TransactionId=params[0];
    result="2,"+ip+",RemoteStopTransaction,{TransactionId="+TransactionId+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    TransactionId="";result="";
}
void MainWindow2::ReserveNow_php()
{
    ConnectorId=params[0];
    IdTag=params[3];
    QDateTime reservedTime=ui->dateTimeEdit_ReservedTime_reservenow->dateTime();
    QDateTime expiryTime=ui->dateTimeEdit_ExpiryTime_reservenow->dateTime();
    qint64 secondsDiff = expiryTime.secsTo(reservedTime);
    QString ReservedTime=params[1];
    QString ExpiryTime=params[2];
    QString Duration=QDateTime::fromTime_t(secondsDiff).toString("HH:mm:ss");
    d.Get_Authorize(&ExpiryDate,&ParentIdTag,IdTag);// Get from user details using id tag
    quint16 Reservation = QRandomGenerator::global()->generate();
    ReservationId= QString::number(Reservation);
    result="2,"+ip+",ReserveNow,{ConnectorId="+ConnectorId+"|IdTag="+IdTag+"|ParentIdTag="+ParentIdTag+"|ReservedTime="+ReservedTime+"|ExpiryTime="+ExpiryTime+"|Duration="+Duration+"|ReservationId="+ReservationId+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ReservationId="";ParentIdTag="";IdTag="";ExpiryDate="";ConnectorId="";result="";Duration="";
}
void MainWindow2::Reset_php()
{
    Type=params[0];
    result="2,"+ip+",Reset,{Type="+Type+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    Type="";result="";
}
void MainWindow2::SendLocalList_php()
{
    UpdateType=params[1];
    QString stream=params[2];
    ListVersion=params[0];
    result="2,"+ip+",SendLocalList,{ListVersion="+ListVersion+"|UpdateType="+UpdateType+"|Stream="+stream+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ListVersion="";UpdateType="";stream="";result="";
}
void MainWindow2::TriggerMessage_php()
{
    ConnectorId=params[0];
    RequestMessage=params[1];
    result="2,"+ip+",TriggerMessage,{RequestMessage="+RequestMessage+"|ConnectorId="+ConnectorId+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ConnectorId="";RequestMessage="";result="";
}
void MainWindow2::UnlockConnector_php()
{
    ConnectorId=params[0];
    result="2,"+ip+",UnlockConnector,{ConnectorId="+ConnectorId+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    ConnectorId="";result="";
}
void MainWindow2::UpdateFirmware_php()
{
    Location=params[0];
    Retries=params[1];
    RetrieveDate=params[3];
    RetryInterval=params[2];
    tcp->incomingConnection(-1,Location);
    result="2,"+ip+",UpdateFirmware,{Location="+Location+"|Retries="+Retries+"|RetrieveDate="+RetrieveDate+"|RetryInterval="+RetryInterval+"}";
    m_webSocket.sendTextMessage(result);
    qDebug()<<"Message sent :"<<result;
    Location="";Retries="";RetryInterval="";RetrieveDate="";result="";
}




void MainWindow2::on_pushButton_Send_cancelreseravtion_clicked()
{
    CancelReservation();    
}
void MainWindow2::on_pushButton_Send_changeavailability_clicked()
{
    ChangeAvailability();
}
void MainWindow2::on_pushButton_Send_changeconfiguration_clicked()
{
    ChangeConfiguration();
}
void MainWindow2::on_pushButton_Send_clearcache_clicked()
{
    ClearCache();
}
/*void MainWindow2::on_pushButton_Send_clearchargingprofile_clicked()
{
    ClearChargingProfile();
}*/
void MainWindow2::on_pushButton_Send_datatransfer_clicked()
{
    DataTransfer();
}
/*void MainWindow2::on_pushButton_Send_getcompositeschedule_clicked()
{
    GetCompositeSchedule();
}*/
void MainWindow2::on_pushButton_Send_getconfiguration_clicked()
{
    GetConfiguration();
}
void MainWindow2::on_pushButton_Send_getdiagnostics_clicked()
{
    GetDiagnostics();
}
void MainWindow2::on_pushButton_Send_getlocallist_clicked()
{
    GetLocalListVersion();
}
void MainWindow2::on_pushButton_Send_remotestart_clicked()
{
    RemoteStartTransaction();
}
void MainWindow2::on_pushButton_Send_remotestop_clicked()
{
    RemoteStopTransaction();
}
void MainWindow2::on_pushButton_Send_reservenow_clicked()
{
    ReserveNow();
}
void MainWindow2::on_pushButton_Send_reset_clicked()
{
    Reset();
}
void MainWindow2::on_pushButton_Send_sendlocallist_clicked()
{QString stream="";
    int numRows = ui->tableWidget_SendLocalList->rowCount();
    int numColumn = ui->tableWidget_SendLocalList->columnCount();
     for (int i=0; i<numRows; ++i)
     {for (int j=0; j<numColumn; j++)
       {
          stream=stream+ui->tableWidget_SendLocalList->item(i,j)->text() ;
          if(j!=numColumn-1){stream=stream+"\t";}
       }
       if(i!=numRows-1){stream=stream+"\n";}
    }
    SendLocalList(stream,numRows);
}
/*void MainWindow2::on_pushButton_Send_setchargingprofile_clicked()
{
    SetChargingProfile();
}*/

void MainWindow2::on_pushButton_Send_triggermesage_clicked()
{
    TriggerMessage();
}

void MainWindow2::on_pushButton_Send_unloackconnector_clicked()
{
    UnlockConnector();
}

void MainWindow2::on_pushButton_Send_updatefirmware_clicked()
{
    UpdateFirmware();
}
void MainWindow2::on_pushButton_Cancel_Reservation_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(true);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
 ui->widget_Cancel_Reservation->show();
 ui->widget_Change_Availability->hide();
 ui->widget_Change_Configuration->hide();
 ui->widget_Clear_Cache->hide();
 ui->widget_Clear_Charging_Profile->hide();
 ui->widget_Data_Transfer_CP->hide();
 ui->widget_Get_Composite_Schedule->hide();
 ui->widget_Get_Configuration->hide();
 ui->widget_Get_Diagnostics->hide();
 ui->widget_Get_Local_List_Version->hide();
 ui->widget_Remote_Start_Transaction->hide();
 ui->widget_Remote_Stop_Transaction->hide();
 ui->widget_ReserveNow->hide();
 ui->widget_Reset->hide();
 ui->widget_SendLocalList->hide();
 ui->widget_Set_Charging_Profile->hide();
 ui->widget_Trigger_Message->hide();
 ui->widget_Unlock_Connector->hide();
 ui->widget_Update_Firmware->hide();
}

void MainWindow2::on_pushButton_Change_Availability_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(true);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->show();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

void MainWindow2::on_pushButton_Change_Configuration_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(true);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->show();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

void MainWindow2::on_pushButton_Clear_Cache_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(true);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->show();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

/*void MainWindow2::on_pushButton_Clear_Charging_Profile_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(true);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->show();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}*/

void MainWindow2::on_pushButton_DataTransfer_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(true);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->show();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

/*void MainWindow2::on_pushButton_Get_Composite_Schedule_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(true);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->show();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}*/

void MainWindow2::on_pushButton_Get_Configuration_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(true);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->show();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

void MainWindow2::on_pushButton_Get_Diagnostics_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(true);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->show();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

void MainWindow2::on_pushButton_Get_Local_List_Version_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(true);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->show();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

void MainWindow2::on_pushButton_Remote_Start_Transaction_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(true);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->show();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

void MainWindow2::on_pushButton_Remote_Stop_Transaction_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(true);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->show();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

void MainWindow2::on_pushButton_Reserve_Now_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(true);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->show();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

void MainWindow2::on_pushButton_Reset_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(true);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->show();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

void MainWindow2::on_pushButton_Send_Local_List_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(true);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->show();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

/*void MainWindow2::on_pushButton_Set_Charging_Profile_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(true);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->show();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}*/

void MainWindow2::on_pushButton_Trigger_Message_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(true);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->show();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->hide();
}

void MainWindow2::on_pushButton_Unlock_Connector_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(true);
    ui->pushButton_Update_Firmware->setDisabled(false);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->show();
    ui->widget_Update_Firmware->hide();
}

void MainWindow2::on_pushButton_Update_Firmware_clicked()
{
    ui->pushButton_Cancel_Reservation->setDisabled(false);
    ui->pushButton_Change_Availability->setDisabled(false);
    ui->pushButton_Change_Configuration->setDisabled(false);
    ui->pushButton_Clear_Cache->setDisabled(false);
    ui->pushButton_Clear_Charging_Profile->setDisabled(false);
    ui->pushButton_DataTransfer->setDisabled(false);
    ui->pushButton_Get_Composite_Schedule->setDisabled(false);
    ui->pushButton_Get_Configuration->setDisabled(false);
    ui->pushButton_Get_Diagnostics->setDisabled(false);
    ui->pushButton_Get_Local_List_Version->setDisabled(false);
    ui->pushButton_Remote_Start_Transaction->setDisabled(false);
    ui->pushButton_Remote_Stop_Transaction->setDisabled(false);
    ui->pushButton_Reserve_Now->setDisabled(false);
    ui->pushButton_Reset->setDisabled(false);
    ui->pushButton_Send_Local_List->setDisabled(false);
    ui->pushButton_Set_Charging_Profile->setDisabled(false);
    ui->pushButton_Trigger_Message->setDisabled(false);
    ui->pushButton_Unlock_Connector->setDisabled(false);
    ui->pushButton_Update_Firmware->setDisabled(true);
    ui->widget_Cancel_Reservation->hide();
    ui->widget_Change_Availability->hide();
    ui->widget_Change_Configuration->hide();
    ui->widget_Clear_Cache->hide();
    ui->widget_Clear_Charging_Profile->hide();
    ui->widget_Data_Transfer_CP->hide();
    ui->widget_Get_Composite_Schedule->hide();
    ui->widget_Get_Configuration->hide();
    ui->widget_Get_Diagnostics->hide();
    ui->widget_Get_Local_List_Version->hide();
    ui->widget_Remote_Start_Transaction->hide();
    ui->widget_Remote_Stop_Transaction->hide();
    ui->widget_ReserveNow->hide();
    ui->widget_Reset->hide();
    ui->widget_SendLocalList->hide();
    ui->widget_Set_Charging_Profile->hide();
    ui->widget_Trigger_Message->hide();
    ui->widget_Unlock_Connector->hide();
    ui->widget_Update_Firmware->show();
}


void MainWindow2::on_pushButton_Add_SendLocalList_clicked()
{
    IdTag=ui->lineEdit_idtag_sendlocallist->text();
    d.Get_Authorize(&ExpiryDate,&ParentIdTag,IdTag);// Get from user details using id tag
    ui->tableWidget_SendLocalList->insertRow(ui->tableWidget_SendLocalList->rowCount());
    int row = ui->tableWidget_SendLocalList->rowCount()-1;
    ui->tableWidget_SendLocalList->setItem(row,0,new QTableWidgetItem(IdTag));
    ui->tableWidget_SendLocalList->setItem(row,1,new QTableWidgetItem(ExpiryDate));
    ui->tableWidget_SendLocalList->setItem(row,2,new QTableWidgetItem(ParentIdTag));
    qDebug()<<IdTag<<ExpiryDate<<ParentIdTag;
    IdTag="";ExpiryDate="";ParentIdTag="";
}

void MainWindow2::on_pushButton_Remove_SendLocalList_clicked()
{
    int selected=ui->tableWidget_SendLocalList->currentRow();
    ui->tableWidget_SendLocalList->removeRow(selected);
}

