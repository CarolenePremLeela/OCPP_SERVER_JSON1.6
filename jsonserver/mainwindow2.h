#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H
#include "database.h"
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMessageBox>
#include <QMainWindow>
#include "mainwindow.h"
#include "serversocket.h"
namespace Ui {
class MainWindow2;
}

class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow2(QWidget *parent = 0);
    ~MainWindow2();
    database d;
    serversocket *tcp = new serversocket();
    MainWindow *w = new MainWindow();
    QTimer *timer;

private slots:

    void on_pushButton_Cancel_Reservation_clicked();
    void on_pushButton_Change_Availability_clicked();
    void on_pushButton_Change_Configuration_clicked();
    void on_pushButton_Clear_Cache_clicked();
//    void on_pushButton_Clear_Charging_Profile_clicked();
    void on_pushButton_DataTransfer_clicked();
//    void on_pushButton_Get_Composite_Schedule_clicked();
    void on_pushButton_Get_Configuration_clicked();
    void on_pushButton_Get_Diagnostics_clicked();
    void on_pushButton_Get_Local_List_Version_clicked();
    void on_pushButton_Remote_Start_Transaction_clicked();
    void on_pushButton_Remote_Stop_Transaction_clicked();
    void on_pushButton_Reserve_Now_clicked();
    void on_pushButton_Reset_clicked();
    void on_pushButton_Send_Local_List_clicked();
//    void on_pushButton_Set_Charging_Profile_clicked();
    void on_pushButton_Trigger_Message_clicked();
    void on_pushButton_Unlock_Connector_clicked();
    void on_pushButton_Update_Firmware_clicked();




    void on_pushButton_Send_cancelreseravtion_clicked();
    void on_pushButton_Send_reservenow_clicked();
    void on_pushButton_Send_changeavailability_clicked();
    void on_pushButton_Send_changeconfiguration_clicked();
    void on_pushButton_Send_clearcache_clicked();
//    void on_pushButton_Send_clearchargingprofile_clicked();
    void on_pushButton_Send_datatransfer_clicked();
//    void on_pushButton_Send_getcompositeschedule_clicked();
    void on_pushButton_Send_getconfiguration_clicked();
    void on_pushButton_Send_getdiagnostics_clicked();
    void on_pushButton_Send_getlocallist_clicked();
    void on_pushButton_Send_remotestart_clicked();
    void on_pushButton_Send_remotestop_clicked();
    void on_pushButton_Send_reset_clicked();
    void on_pushButton_Send_sendlocallist_clicked();
//    void on_pushButton_Send_setchargingprofile_clicked();
    void on_pushButton_Send_triggermesage_clicked();
    void on_pushButton_Send_unloackconnector_clicked();
    void on_pushButton_Send_updatefirmware_clicked();


    void on_pushButton_StartChargePoint_clicked();
    void on_pushButton_StopChargePoint_clicked();

    void func();


    void on_pushButton_Add_SendLocalList_clicked();

    void on_pushButton_Remove_SendLocalList_clicked();

public:
    void Received(QString message);
void Sub3();

    void CancelReservation();
    void ChangeAvailability();
    void ChangeConfiguration();
    void ClearCache();
//    void ClearChargingProfile();
    void DataTransfer();
//    void GetCompositeSchedule();
    void GetConfiguration();
    void GetDiagnostics();
    void GetLocalListVersion();
    void RemoteStartTransaction();
    void RemoteStopTransaction();
    void ReserveNow();
    void Reset();
    void SendLocalList(QString stream,int LocalListVersion);
//    void SetChargingProfile();
    void TriggerMessage();
    void UnlockConnector();
    void UpdateFirmware();

    void CancelReservation_php();
    void ChangeAvailability_php();
    void ChangeConfiguration_php();
    void ClearCache_php();
//    void ClearChargingProfile_php();
    void DataTransfer_php();
//    void GetCompositeSchedule_php();
    void GetConfiguration_php();
    void GetDiagnostics_php();
    void GetLocalListVersion_php();
    void RemoteStartTransaction_php();
    void RemoteStopTransaction_php();
    void ReserveNow_php();
    void Reset_php();
    void SendLocalList_php();
//    void SetChargingProfile_php();
    void TriggerMessage_php();
    void UnlockConnector_php();
    void UpdateFirmware_php();


QString params[10];

    QString msg_Id,uniq_Id,Payload,result,PayloadHead,CurrentTime;
    QString Status,RequestMessage,ip;
    QString ReservationId,ConnectorId,Type,Key,NewValue,ID,PurposeType,VendorId,MessageId,Data,Duration,ChargingUnitType,
    Location,Retries,RetryInterval,StartTime,StopTime,IdTag,StackLevel,UnitType,MinChargingRate,StartPeriod,FileName,
    Limit,NumberPhases,TransactionId,ExpiryDate,ListVersion,UpdateType,ParentIdTag,RetrieveDate,reservestatus,ParentIdTag_reserve;
    int avail;
private:
    QWebSocket m_webSocket;
    QUrl url;
    Ui::MainWindow2 *ui;
};

#endif // MAINWINDOW2_H
