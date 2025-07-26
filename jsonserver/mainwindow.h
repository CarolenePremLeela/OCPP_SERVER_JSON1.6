#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebSockets/QWebSocket>
#include <QWebSocketServer>
#include <database.h>
#include <QDebug>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QWebSocketServer *m_pWebSocketServer;
    database d;
public slots:

    void on_pushButton_Authorize_clicked();

    void on_pushButton_Bootnotification_clicked();

    void on_pushButton_Datatransfer_clicked();

    void on_pushButton_Diagnostics_Status_Notification_clicked();

    void on_pushButton_Firmware_Status_Notification_clicked();

    void on_pushButton_Heartbeat_clicked();

    void on_pushButton_Metervalues_clicked();

    void on_pushButton_StartTransaction_clicked();

    void on_pushButton_Status_Notification_clicked();

    void on_pushButton_Stop_Notification_clicked();

    void on_pushButton_Start_clicked();

    void on_pushButton_Stop_clicked();



signals:
    void closed();

public:

QString msg_Id,uniq_Id,Payload,result,PayloadHead,message,cp_name;
QString ExpiryDate,Status,IdTag,Interval,SerialNumber,TimeStamp_get,
Model,ChargePointSerialNumber,Vendor,FirmwareVersion,ICCID,IMSI,MeterSerialNumber,MeterType,Reason,MeterStop,VendorId,MessageId,Data,
CurrentTime,TimeStamp,TransactionId,MeterValue,ConnectorId,Value,Context,Format,Measured,Phase,Location,Unit,
MeterStart,ReservationId,ParentIdTag,StackLevel,UnitType,MinChargingRate,StartPeriod,Limit,NumberPhases;
int mON;
void onNewConnection();
void processTextMessage(QString message);
void socketDisconnected();


void sub2();
void Authorize();
void BootNotification();
void DataTransfer();
void DiagnosticsStatusNotification();
void FirmwareStatusNotification();
void HeartBeat();
void MeterValues();
void StartTransaction();
void StatusNotification();
void StopTransaction();

private:
    QList<QWebSocket *> m_clients;
    bool m_debug;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
