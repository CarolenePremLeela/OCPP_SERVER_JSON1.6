#include "mainwindow.h"
#include "ui_mainwindow.h"
QString Url="ws://49.206.200.225:1234/ocppj16/CentralSystem0";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pWebSocketServer(new QWebSocketServer(Url,QWebSocketServer::NonSecureMode, this)),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mdiArea->setWindowIcon(QIcon(":/file/falogo1.png"));
    ui->mdiArea->tileSubWindows();
    ui->dockWidget->setWindowTitle("ChargePoint");
    on_pushButton_Start_clicked();
}

MainWindow::~MainWindow()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
    delete ui;
}

void MainWindow::on_pushButton_Start_clicked()
{
    ui->pushButton_Start->hide();
    ui->pushButton_Stop->show();
    QString portstr=ui->lineEdit_port->text(); //fetching Port
    int port=portstr.toInt();
        if (m_pWebSocketServer->listen(QHostAddress::Any, port))// Listening Port
        {
            qDebug() << "Echoserver listening on port" << port;
            connect(m_pWebSocketServer, &QWebSocketServer::newConnection,this, &MainWindow::onNewConnection); //if New Socket Connected, go new connection
            connect(m_pWebSocketServer, &QWebSocketServer::closed,this ,&MainWindow::closed); // If Server closed, go closed
        }     
}

void MainWindow::on_pushButton_Stop_clicked()
{
    ui->pushButton_Stop->hide();
    ui->pushButton_Start->show();
    m_pWebSocketServer->close(); // Close Server
}
void MainWindow::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection(); // Connection to websocket with server
    QString url= pSocket->resourceName(); // Get the Url connected
    QStringList lines = url.split('/');
    QString ipvector=lines.at(2);
    cp_name=lines.at(4); // Charge Point Name from Url
    lines = ipvector.split(':');
    QString ip=lines.at(0);   // Ip from Url
    QString port=lines.at(1); // Port From Url
    qDebug()<<cp_name <<"is connected.";
    connect(pSocket, &QWebSocket::textMessageReceived, this, &MainWindow::processTextMessage); // If text Message received from socket, go to process text message
    connect(pSocket, &QWebSocket::disconnected, this, &MainWindow::socketDisconnected); // If disconnected, go to socket disconnected
}
void MainWindow::processTextMessage(QString message)
{
    if(message!=NULL)
    {
      QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
      qDebug() << "Message received:" << message;
      QStringList lines = message.split(',');
      msg_Id =lines.at(0);
      uniq_Id=lines.at(1);
      PayloadHead =lines.at(2);
      Payload=lines.at(3);
      if(msg_Id=="2")
       {
          sub2();
          pClient->sendTextMessage(result); // Reply to the Socket
          result="";
       }
     }
}

void MainWindow::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << cp_name <<"is Disconnected.";
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void MainWindow::sub2()  //If Message Id = 2;
{
    if(PayloadHead=="Authorize")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('}');
         y=lines.at(0);
        lines = y.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        if(x1=="IdTag")
        {
            IdTag.append(x2);
            ui->lineEdit_IdTag_Authorize_Request->setText(IdTag);
            Authorize();
        }

    }
    if(PayloadHead=="BootNotification")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        QString c=lines.at(2);
        QString e=lines.at(3);
        QString f=lines.at(4);
        QString g=lines.at(5);
        QString h=lines.at(6);
        QString i=lines.at(7);
        QString j=lines.at(8);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = c.split('=');
        QString c2=lines.at(1);
        lines = e.split('=');
        QString e2=lines.at(1);
        lines = f.split('=');
        QString f2=lines.at(1);
        lines = g.split('=');
        QString g2=lines.at(1);
        lines = h.split('=');
        QString h2=lines.at(1);
        lines = i.split('=');
        QString i2=lines.at(1);
        lines = j.split('=');
        QString j2=lines.at(1);
        lines = j2.split('}');
        j2=lines.at(0);
        if(a1=="SerialNumber")
        {
            SerialNumber.append(a2);
            Model.append(b2);
            ChargePointSerialNumber.append(c2);
            Vendor.append(e2);
            FirmwareVersion.append(f2);
            ICCID.append(g2);
            IMSI.append(h2);
            MeterSerialNumber.append(i2);
            MeterType.append(j2);
            ui->lineEdit_SerialNumber_bootnotification_Request->setText(SerialNumber);
            ui->lineEdit_Model_bootnotification_Request->setText(Model);
            ui->lineEdit_SerialNumChgpoint_bootnotification_Request->setText(ChargePointSerialNumber);
            ui->lineEdit_vendor_bootnotification_Request->setText(Vendor);
            ui->lineEdit_firmwareversion_bootnotification_Request->setText(FirmwareVersion);
            ui->lineEdit_Iccid_bootnotification_Request->setText(ICCID);
            ui->lineEdit_Imsi_bootnotification_Request->setText(IMSI);
            ui->lineEdit_MeterSerialNum_bootnotification_Request->setText(MeterSerialNumber);
            ui->lineEdit_MeterType_bootnotification_Request->setText(MeterType);
            // Update Client Table Using Client IP i.e., Uniq Id
            d.update_BootNotification(SerialNumber,Model,ChargePointSerialNumber,Vendor,FirmwareVersion,ICCID,IMSI,MeterSerialNumber,MeterType,uniq_Id);
            SerialNumber="";Model="";ChargePointSerialNumber="";Vendor="";FirmwareVersion="";ICCID="";IMSI="";MeterSerialNumber="";MeterType="";
            BootNotification();
         }
    }
    if(PayloadHead=="DataTransfer")
    {
        DataTransfer();
    }
    if(PayloadHead=="DiagnosticsStatusNotification")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('}');
         y=lines.at(0);
        lines = y.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        if(x1=="Status")
        {
            Status.append(x2);
            ui->lineEdit_Status_diagnosticsnotification_Request->setText(Status);
            Status="";
            DiagnosticsStatusNotification();
        }

    }
    if(PayloadHead=="FirmwareStatusNotification")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('}');
         y=lines.at(0);
        lines = y.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        if(x1=="Status")
        {
            Status.append(x2);
            ui->lineEdit_Status_firmwarenotification_Request->setText(Status);
            Status="";
            FirmwareStatusNotification();
        }

    }
    if(PayloadHead=="HeartBeat")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('}');
        QString u=lines.at(0);
        if(u=="RequestHeartBeat")
        {
            HeartBeat();
        }
    }
    if(PayloadHead=="MeterValue")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        QString c=lines.at(2);
        QString e=lines.at(3);
        QString f=lines.at(4);
        QString g=lines.at(5);
        QString h=lines.at(6);
        QString i=lines.at(7);
        QString j=lines.at(8);
        QString k=lines.at(9);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = c.split('=');
        QString c2=lines.at(1);
        lines = e.split('=');
        QString e2=lines.at(1);
        lines = f.split('=');
        QString f2=lines.at(1);
        lines = g.split('=');
        QString g2=lines.at(1);
        lines = h.split('=');
        QString h2=lines.at(1);
        lines = i.split('=');
        QString i2=lines.at(1);
        lines = j.split('=');
        QString j2=lines.at(1);
        lines = k.split('=');
        QString k2=lines.at(1);
        lines = k2.split('}');
        k2=lines.at(0);
        if(a1=="TimeStamp")
        {
            TimeStamp.append(a2);
            TransactionId.append(b2);
            ConnectorId.append(c2);
            Value.append(e2);
            Context.append(f2);
            Format.append(g2);
            Measured.append(h2);
            Phase.append(i2);
            Location.append(j2);
            Unit.append(k2);
            ui->lineEdit_Timestamp_metervalues->setText(TimeStamp);
            ui->lineEdit_TransactionID_Metervalue_Req->setText(TransactionId);
            ui->lineEdit_ConnectorID_Metervalue_Req->setText(ConnectorId);
            ui->lineEdit_value_Metervalue_Req->setText(MeterValue);
            ui->lineEdit_context_Metervalue_Req->setText(Context);
            ui->lineEdit_format_Metervalue_Req->setText(Format);
            ui->lineEdit_measurand_Metervalue_Req->setText(Measured);
            ui->lineEdit_phase_Metervalue_Req->setText(Phase);
            ui->lineEdit_location_Metervalue_Req->setText(Location);
            ui->lineEdit_unit_Metervalue_Req->setText(Unit);
            d.update_meterstatus(1,ConnectorId); // Update Meterstatus in connectorId details table using ConnectorId
            if(TransactionId==NULL) // If Metervalue runs after start transaction_id will be not equal to zero
            {
                // This function invokes when trigger message calls MeterValues
                d.insert_MeterValues(TimeStamp,TransactionId,ConnectorId,Value,Context,Format,Measured,Phase,Location,Unit);
            }
            else
            {
                // If Transaction Table has Transaction Id,then update Metervalues
                d.update_MeterValues(TimeStamp,TransactionId,ConnectorId,Value,Context,Format,Measured,Phase,Location,Unit);
            }
            TimeStamp="";TransactionId="";Context="";Format="";Measured="";Phase="";Location="";Unit="";
            MeterValues();
        }
    }
    if(PayloadHead=="StartTransaction")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        QString c=lines.at(2);
        QString e=lines.at(3);
        QString f=lines.at(4);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = c.split('=');
        QString c2=lines.at(1);
        lines = e.split('=');
        QString e2=lines.at(1);
        lines = f.split('=');
        QString f2=lines.at(1);
        lines = f2.split('}');
        f2=lines.at(0);
        if(a1=="ConnectedId")
        {
            ConnectorId.append(a2);
            IdTag.append(b2);
            MeterStart.append(c2);
            ReservationId.append(e2);
            TimeStamp.append(f2);
            ui->lineEdit_ConnectorID_Start_Transaction_Request->setText(ConnectorId);
            ui->lineEdit_Idtag_Start_Transaction_Request->setText(IdTag);
            ui->lineEdit_meterstart_Start_Transaction_Request->setText(MeterStart);
            ui->lineEdit_reservationid_Start_Transaction_Request->setText(ReservationId);
            ui->lineEdit_timestamp_Start_Transaction_Request->setText(TimeStamp);
//            d.check_StartTransaction(&mON,&TimeStamp_get,ConnectorId); // Meter start and timestamp from transaction table
            int remote_start_status;
            d.get_client_remotestartstatus(&remote_start_status,ConnectorId);
            if(remote_start_status==0) // Check Meter is off and Timestamp not NULL
            {
                d.insert_StartTransaction(ConnectorId,IdTag,MeterStart,ReservationId,TimeStamp); // Insert into transaction table, when start transaction
                StartTransaction();
                MeterStart="";ReservationId="";TimeStamp="";
            }
            else if(remote_start_status==1) // Check Meter is off and Timestamp is equal to Null
            {
                d.update_RemoteStart(0,ConnectorId);
                d.update_StartTransaction(TimeStamp,MeterStart,ReservationId,ConnectorId); // Update in transaction table using connector id when remote transaction start
                StartTransaction();
                MeterStart="";ReservationId="";TimeStamp="";
            }
            else
            {
                MeterStart="";ReservationId="";TimeStamp="";ConnectorId="";IdTag="";
            }
        }
    }
    if(PayloadHead=="StatusNotification")
    {
        StatusNotification();
    }
    if(PayloadHead=="StopTransaction")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        QString c=lines.at(2);
        QString e=lines.at(3);
        QString f=lines.at(4);
        QString g=lines.at(5);
        QString h=lines.at(6);
        QString i=lines.at(7);
        QString j=lines.at(8);
        QString k=lines.at(9);
        QString l=lines.at(10);
        QString m=lines.at(11);
        QString n=lines.at(12);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = c.split('=');
        QString c2=lines.at(1);
        lines = e.split('=');
        QString e2=lines.at(1);
        lines = f.split('=');
        QString f2=lines.at(1);
        lines = g.split('=');
        QString g2=lines.at(1);
        lines = h.split('=');
        QString h2=lines.at(1);
        lines = i.split('=');
        QString i2=lines.at(1);
        lines = j.split('=');
        QString j2=lines.at(1);
        lines = k.split('=');
        QString k2=lines.at(1);
        lines = l.split('=');
        QString l2=lines.at(1);
        lines = m.split('=');
        QString m2=lines.at(1);
        lines = n.split('=');
        QString n2=lines.at(1);
        lines = n2.split('}');
        n2=lines.at(0);
        if(a1=="IdTag")
        {
            IdTag.append(a2);
            Reason.append(b2);
            MeterStop.append(c2);
            TimeStamp.append(e2);
            TransactionId.append(f2);
            ConnectorId.append(g2);
            Value.append(h2);
            Context.append(i2);
            Format.append(j2);
            Measured.append(k2);
            Phase.append(l2);
            Location.append(m2);
            Unit.append(n2);
            ui->lineEdit_IdTag_StopTransaction_Req->setText(IdTag);
            ui->lineEdit_reason_StopTransaction_Req->setText(Reason);
            ui->lineEdit_Meterstop_StopTransaction_Req->setText(MeterStop);
            ui->lineEdit_Timestamp_StopTransaction_Req->setText(TimeStamp);
            ui->lineEdit_Transaction_StopTransaction_Req->setText(TransactionId);
            ui->lineEdit_ConnectorId__StopTransaction_Req->setText(ConnectorId);
            ui->lineEdit_value_StopTransaction_Req->setText(MeterValue);
            ui->lineEdit_context_StopTransaction_Req->setText(Context);
            ui->lineEdit_format_StopTransaction_Req->setText(Format);
            ui->lineEdit_measurand_StopTransaction_Req->setText(Measured);
            ui->lineEdit_phase_StopTransaction_Req->setText(Phase);
            ui->lineEdit_location_StopTransaction_Req->setText(Location);
            ui->lineEdit_unit_StopTransaction_Req->setText(Unit);
            //Insert into transaction table
            d.insert_StopTransaction(IdTag,Reason,MeterStop,TimeStamp,TransactionId,ConnectorId,Value,Context,Format,Measured,Phase,Location,Unit);
            Reason="";MeterStop="";TimeStamp="";TransactionId="";Context="";Format="";Measured="";Phase="";Location="";Unit="";
            StopTransaction();
        }
    }
}

void MainWindow::Authorize()
{
    d.Get_Authorize(&ExpiryDate,&ParentIdTag,IdTag); // Get from user details using id tag
    qDebug()<<"ExpiryDate :"<<ExpiryDate;
    if((ExpiryDate!="")&&(IdTag!=""))
    {
        Status="Accepted";
    }
    else Status="Rejected";
    result = "3,"+uniq_Id+",Authorize,{Status="+Status+"|ExpiryDate="+ExpiryDate+"|ParentIdTag="+ParentIdTag+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";ExpiryDate="";IdTag="";ParentIdTag="";
}
void MainWindow::BootNotification()
{
    d.Get_Interval(&Interval,uniq_Id); // Get from client table using uniq_id
    Status=ui->comboBox_Status_bootnotification_Response->currentText();
    QDateTime issueDate = QDateTime::currentDateTime();
    CurrentTime=issueDate.toString("yyyy-MM-dd HH:mm:ss");
    result = "3,"+uniq_Id+",BootNotification,{Interval="+Interval+"|CurrentDate="+CurrentTime+"|Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";ExpiryDate="";Interval="";
}
void MainWindow::DataTransfer()
{
    VendorId=ui->lineEdit_vendorID_Datatransfer_Request->text();
    MessageId=ui->lineEdit_MessageID_Datatransfer_Request->text();
    Data=ui->lineEdit_Data_Datatransfer_Request->text();
    result="3,"+uniq_Id+",DataTransfer,{VendorId="+VendorId+"|MessageId="+MessageId+"|Data="+Data+"}";
    qDebug()<<"Message sent :"<<result;
}
void MainWindow::DiagnosticsStatusNotification()
{
    result = "3,"+uniq_Id+",DiagnosticsStatusNotification,{Acknowledgementfromserver}";
    qDebug()<<"Message sent :"<<result;
}
void MainWindow::FirmwareStatusNotification()
{
    result = "3,"+uniq_Id+",FirmwareStatusNotification,{Acknowledgementfromserver}";
    qDebug()<<"Message sent :"<<result;
}
void MainWindow::HeartBeat()
{
    QDateTime issueDate = QDateTime::currentDateTime();
    CurrentTime=issueDate.toString("yyyy-MM-dd HH:mm:ss");
    result = "3,"+uniq_Id+",HeartBeat,{CurrentTime="+CurrentTime+"}";
    qDebug()<<"Message sent :"<<result;
    CurrentTime="";
}
void MainWindow::MeterValues()
{
    result = "3,"+uniq_Id+",MeterValue,{Acknowledgementfromserver}";
    qDebug()<<"Message sent :"<<result;
    ConnectorId="";
}
void MainWindow::StartTransaction()
{
    d.Get_Authorize(&ExpiryDate,&ParentIdTag,IdTag);// Get from user details using id tag
    d.check_TransactionId(&TransactionId,ConnectorId); // Get transaction id using connector id
    if(TransactionId=='0') // If transaction id is nUll generate transaction id
    {
    quint16 Transaction= QRandomGenerator::global()->generate();
    TransactionId = QString::number(Transaction);
    }
    Status=ui->comboBox_Status_Start_Transaction_Response->currentText();
    result = "3,"+uniq_Id+",StartTransaction,{ExpiryDate="+ExpiryDate+"|ParentIdTag="+ParentIdTag+"|Status="+Status+"|Transaction="+TransactionId+"}";
    qDebug()<<"Message sent :"<<result;
    ExpiryDate="";Status="";TransactionId="";ParentIdTag="";ConnectorId="";IdTag="";
}
void MainWindow::StatusNotification()
{
    result = "3,"+uniq_Id+",StatusNotification,{Acknowledgementfromserver}";
    qDebug()<<"Message sent :"<<result;
}
void MainWindow::StopTransaction()
{
    Status=ui->comboBox_Status_StopTransaction_Resp->currentText();
    d.Get_Authorize(&ExpiryDate,&ParentIdTag,IdTag);// Get from user details using id tag
    result = "3,"+uniq_Id+",StopTransaction,{ExpiryDate="+ExpiryDate+"|ParentIdTag="+ParentIdTag+"|Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    d.update_meterstatus(0,ConnectorId);  // Update Meterstatus in connectorId details table using ConnectorId
    Status="";ExpiryDate="";ParentIdTag="";ConnectorId="";IdTag="";
}







void MainWindow::on_pushButton_Authorize_clicked()
{
    ui->widget_authorize->show();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Bootnotification_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->show();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Datatransfer_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->show();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Diagnostics_Status_Notification_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->show();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Firmware_Status_Notification_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->show();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Heartbeat_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->show();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Metervalues_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->show();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_StartTransaction_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->show();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Status_Notification_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->show();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Stop_Notification_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->show();
}
