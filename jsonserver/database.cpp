#include "database.h"
#include<QMessageBox>
#include<QDebug>
database::database(QObject *parent) : QObject(parent)
{
   createConnection();
}
database::~database()
{

}
bool database::createConnection()
{
   QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
   db.setHostName("103.83.81.25");
   db.setDatabaseName("fca");
   db.setUserName("ocpp");
   db.setPassword("4ZEcka&JI");
//   db.setHostName("103.83.81.25");
//   db.setDatabaseName("fca");
//   db.setUserName("ocppuser");
//   db.setPassword("IafZ0$jER");
//   db.setHostName("192.168.1.32");
//   db.setDatabaseName("fca");
//   db.setUserName("root");
//   db.setPassword("");
   if (!db.open())
   {
       std::cerr<<"error while opening database.."<<std::endl;

       QMessageBox::critical(0,QObject::tr("Database Error"),db.lastError().text());
       return false;
   }else
   {std::cerr<<" opening database.."<<std::endl;
   return true;
   }
}
void database::get_php_bitset(QString *Operation,QString *para_1,QString *para_2,QString *para_3,QString *para_4,QString *para_5,QString *para_6,QString *para_7,QString *para_8,QString *para_9)
{
    int instruction_number;
   QSqlQuery query;
query.exec("SELECT `instruction_number` FROM `php_bitset` WHERE `s.no.`=1");
while(query.next())
{
     instruction_number =query.value(0).toInt();
}
if(instruction_number!=0)
{
    QSqlQuery query;
    query.exec("SELECT `operation`, `para_1`, `para_2`, `para_3`, `para_4`, `para_5`, `para_6`, `para_7`, `para_8`, `para_9` FROM `php_qt` WHERE `s.no.`="+instruction_number);
    while(query.next())
    {
        *Operation=query.value(0).toString();
        *para_1   =query.value(1).toString();
        *para_2   =query.value(2).toString();
        *para_3   =query.value(3).toString();
        *para_4   =query.value(4).toString();
        *para_5   =query.value(5).toString();
        *para_6   =query.value(6).toString();
        *para_7   =query.value(7).toString();
        *para_8   =query.value(8).toString();
        *para_9   =query.value(10).toString();
    }
}
else
{
    *Operation="";
}
}
void database::update_BootNotification(QString SerialNumber,QString Model,QString ChargePointSerialNumber,QString Vendor,QString FirmwareVersion,QString ICCID,QString IMSI,QString MeterSerialNumber,QString MeterType,QString ip)
{
    QSqlQuery query;
    query.prepare("UPDATE  `client_table` SET `serial_no`=:SerialNumber,`model`=:Model,`chargepoint_serial_no`=:ChargePointSerialNumber,`vendor_id`=:Vendor,`firmware_version`=:FirmwareVersion,`ICCID`=:ICCID,`IMSI`=:IMSI,`meter_serial_no`=:MeterSerialNumber,`meter_type`=:MeterType where `client_ip`=:ip");
    query.bindValue(":SerialNumber",SerialNumber);
    query.bindValue(":Model",Model);
    query.bindValue(":ChargePointSerialNumber",ChargePointSerialNumber);
    query.bindValue(":Vendor",Vendor);
    query.bindValue(":FirmwareVersion",FirmwareVersion);
    query.bindValue(":ICCID",ICCID);
    query.bindValue(":IMSI",IMSI);
    query.bindValue(":MeterSerialNumber",MeterSerialNumber);
    query.bindValue(":MeterType",MeterType);
    query.bindValue(":ip",ip);
    query.exec();
}
void database::insert_MeterValues(QString TimeStamp,QString TransactionId,QString ConnectorId,QString Value,QString Context,QString Format,QString Measured,QString Phase,QString Location,QString Unit)
{
    QSqlQuery query;
    query.prepare("INSERT INTO `transaction_table` (`meter_timestamp`,`transaction_id`,`connector_id`,`value`,`context`,`format`,`measured`,`phase`,`location`,`unit`) VALUES (:TimeStamp,:TransactionId,:ConnectorId,:Value,:Context,:Format,:Measured,:Phase,:Location,:Unit)");
    query.bindValue(":TimeStamp",TimeStamp);
    query.bindValue(":TransactionId",TransactionId);
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":Value",Value);
    query.bindValue(":Context",Context);
    query.bindValue(":Format",Format);
    query.bindValue(":Measured",Measured);
    query.bindValue(":Phase",Phase);
    query.bindValue(":Location",Location);
    query.bindValue(":Unit",Unit);
    query.exec();
}
void database::update_MeterValues(QString TimeStamp,QString TransactionId,QString ConnectorId,QString Value,QString Context,QString Format,QString Measured,QString Phase,QString Location,QString Unit)
{
    QSqlQuery query;
    query.prepare("UPDATE  `transaction_table` SET `meter_timestamp`=:TimeStamp,`transaction_id`=:TransactionId,`value`=:Value,`context`=:Context,`format`=:Format,`measured`=:Measured,`phase`=:Phase,`location`=:Location,`unit`=:Unit where `connector_id`=:ConnectorId ORDER BY `id` DESC LIMIT 1");
    query.bindValue(":TimeStamp",TimeStamp);
    query.bindValue(":TransactionId",TransactionId);
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":Value",Value);
    query.bindValue(":Context",Context);
    query.bindValue(":Format",Format);
    query.bindValue(":Measured",Measured);
    query.bindValue(":Phase",Phase);
    query.bindValue(":Location",Location);
    query.bindValue(":Unit",Unit);
    query.exec();
}
void database::update_meterstatus(int boot, QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `connectorid_details` SET `meter_status`=:boot where `connector_id`=:ConnectorId");
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":boot",boot);
    query.exec();
}
void database::check_StartTransaction(int *mON,QString *timestamp,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("SELECT `meter_start`,`timestamp` FROM `transaction_table` WHERE `connector_id`=:ConnectorId ORDER BY `id` DESC LIMIT 1");
    query.bindValue(":ConnectorId",ConnectorId);
    query.exec();
    while(query.next())
    {
         *mON  =query.value(0).toInt();
         *timestamp=query.value(1).toString();
    }
}
void database::check_TransactionId(QString *TransactionId,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("SELECT `transaction_id` FROM `transaction_table` WHERE `connector_id`=:ConnectorId ORDER BY `id` DESC LIMIT 1");
    query.bindValue(":ConnectorId",ConnectorId);
    query.exec();
    while(query.next())
    {
         *TransactionId=query.value(0).toString();
    }
}
void database::dropReservationId(QString CurrentDate)
{
    QSqlQuery query;
    query.prepare("UPDATE `connectorid_details` SET `reserve_status`='1' where `connector_id`=(SELECT `connector_id` FROM `reservation_backup` WHERE `reserved_time`=:CurrentDate)");
    query.bindValue(":CurrentDate",CurrentDate);
    query.exec();
    query.prepare("UPDATE `connectorid_details` SET `reserve_status`='0' where `connector_id`=(SELECT `connector_id` FROM `reservation_backup` WHERE `expiry_date`=:CurrentDate)");
    query.bindValue(":CurrentDate",CurrentDate);
    query.exec();
    query.prepare("UPDATE `reservation_backup` SET `cancel_bit`='1' WHERE `expiry_date`=:CurrentDate");
    query.bindValue(":CurrentDate",CurrentDate);
    query.exec();
}
void database::insert_StartTransaction(QString ConnectedId, QString IdTag, QString MeterStart, QString ReservationId, QString TimeStamp)

{
    QSqlQuery query;
    query.prepare("INSERT INTO `transaction_table` (`connector_id`,`idtag`,`meter_start`,`reservation_id`,`timestamp`) VALUES (:ConnectedId,:IdTag,:MeterStart,:ReservationId,:TimeStamp)");
    query.bindValue(":ConnectedId",ConnectedId);
    query.bindValue(":IdTag",IdTag);
    query.bindValue(":MeterStart",MeterStart);
    query.bindValue(":ReservationId",ReservationId);
    query.bindValue(":TimeStamp",TimeStamp);
    query.exec();
}
void database::update_StartTransaction(QString TimeStamp,QString MeterStart,QString ReservationId,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `transaction_table` SET `timestamp`=:TimeStamp,`reservation_id`=:ReservationId,`meter_start`=:MeterStart where `connector_id`=:ConnectorId ORDER BY `id` DESC LIMIT 1");
    query.bindValue(":TimeStamp",TimeStamp);
    query.bindValue(":ReseravationId",ReservationId);
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":MeterStart",MeterStart);
    query.exec();
}

void database::insert_StopTransaction(QString IdTag,QString Reason, QString MeterStop, QString TimeStamp, QString TransactionId, QString ConnectorId, QString Value, QString Context, QString Format, QString Measured, QString Phase, QString Location,QString Unit)
{
    QSqlQuery query;
    query.prepare("INSERT INTO transaction_table (`idtag`,`stop_reason`,`connector_id`,`meter_stop`,`transaction_id`,`timestamp`,`value`,`context`,`format`,`measured`,`phase`,`location`,`unit`) VALUES(:IdTag,:Reason,:ConnectorId,:MeterStop,:TransactionId,:TimeStamp,:Value,:Context,:Format,:Measured,:Phase,:Location,:Unit)");
    query.bindValue(":IdTag",IdTag);
    query.bindValue(":Reason",Reason);
    query.bindValue(":MeterStop",MeterStop);
    query.bindValue(":TimeStamp",TimeStamp);
    query.bindValue(":TransactionId",TransactionId);
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":Value",Value);
    query.bindValue(":Context",Context);
    query.bindValue(":Format",Format);
    query.bindValue(":Measured",Measured);
    query.bindValue(":Phase",Phase);
    query.bindValue(":Location",Location);
    query.bindValue(":Unit",Unit);
    query.exec();
}

void database::Get_Authorize(QString *ExpiryDate,QString *ParentIdTag, QString IdTag)
{
   QSqlQuery query;
   query.prepare("SELECT   `expiry_date`,`parent_idtag` FROM `user_details` WHERE `idtag` =:IdTag");
   query.bindValue(":IdTag",IdTag);
   query.exec();
   while(query.next())
   {
       *ExpiryDate  =query.value(0).toString();
       *ParentIdTag  =query.value(1).toString();
   }

}
void database::Get_Interval(QString *Interval, QString ClientIp)
{
   QSqlQuery query;
   query.prepare("SELECT   `interval` FROM `client_table` WHERE `client_ip` =:ClientIp");
   query.bindValue(":ClientIp",ClientIp);
   query.exec();
   while(query.next())
   {
       *Interval  =query.value(0).toByteArray();
   }

}
void database::get_client_changeavailability(int *boot,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("SELECT `change_availability` FROM `connectorid_details` WHERE `connector_id`=:ConnectorId");
    query.bindValue(":ConnectorId",ConnectorId);
    query.exec();
    while(query.next())
    {
        *boot=query.value(0).toInt();
    }
}

void database::update_RemoteStart(int start,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("UPDATE `connectorid_details` SET `Start`=:Start where `connector_id`=:ConnectorId");
    query.bindValue(":Start",start);
    query.bindValue(":ConnectorId",ConnectorId);
    query.exec();
}
void database::get_client_remotestartstatus(int *boot, QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("SELECT   `Start` FROM `connectorid_details` WHERE `connector_id` =:ConnectorId ");
      query.bindValue(":ConnectorId",ConnectorId);
      query.exec();
      while(query.next())
      {
           *boot  =query.value(0).toInt();
      }
}
