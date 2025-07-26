#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include<iostream>

#include<QtSql>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlDriver>
#include<QtSql/QSqlQuery>


class database : public QObject
{
    Q_OBJECT
public:
    explicit database(QObject *parent = 0);
    ~database();
    bool createConnection();
    void get_php_bitset(QString *Operation, QString *para_1, QString *para_2, QString *para_3, QString *para_4, QString *para_5, QString *para_6, QString *para_7, QString *para_8, QString *para_9);


    void update_BootNotification(QString SerialNumber, QString Model, QString ChargePointSerialNumber, QString Vendor, QString FirmwareVersion, QString ICCID, QString IMSI, QString MeterSerialNumber, QString MeterType, QString ip);
    void insert_MeterValues(QString TimeStamp,QString TransactionId,QString ConnectorId,QString Value,QString Context,QString Format,QString Measured,QString Phase,QString Location,QString Unit);
    void update_MeterValues(QString TimeStamp, QString TransactionId, QString ConnectorId, QString Value, QString Context, QString Format, QString Measured, QString Phase, QString Location, QString Unit);
    void update_meterstatus(int boot, QString ConnectorId);
    void check_StartTransaction(int *mON, QString *timestamp, QString ConnectorId);
    void insert_StartTransaction(QString ConnectedId, QString IdTag, QString MeterStart, QString ReservationId, QString TimeStamp);
    void update_StartTransaction(QString TimeStamp, QString MeterStart, QString ReservationId, QString ConnectorId);
    void insert_StopTransaction(QString IdTag, QString Reason, QString MeterStop, QString TimeStamp, QString TransactionId, QString ConnectorId, QString Value, QString Context, QString Format, QString Measured, QString Phase, QString Location, QString Unit);


    void Get_Authorize(QString *ExpiryDate, QString *ParentIdTag, QString IdTag);
    void Get_Interval(QString *Interval, QString ClientIp);
    void check_TransactionId(QString *TransactionId,QString ConnectorId);
    void dropReservationId(QString CurrentDate);
    void get_client_changeavailability(int *boot, QString ConnectorId);

    void update_RemoteStart(int start,QString ConnectorId);
    void get_client_remotestartstatus(int *boot, QString ConnectorId);
signals:

public slots:
};

#endif // DATABASE_H
