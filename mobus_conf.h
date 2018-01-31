#ifndef MODBUS_CONF_H
#define MODBUS_CONF_H

#define MASTER_ID (201)
#define SLAVE_ID  (202)
#define MB_PDU_SIZE (253)
#define MB_TCP_ADU_SIZE (260)
#define MB_MBAP_HEADER_SIZE (7)

#define TCP_READ_TIMEOUT (500)


#define SLAVE_FREE (false)
#define SLAVE_STARTED (true)

#define TCP_SEND_DATA_ERROR (-1)
#define TCP_DISCONNECT_ERROR (-2)
#define TCP_CONNECT_ERROR (-3)
#define MB_TRANSACTION_RECV_ERROR (-4)
#define MB_WRONG_START_ADDR (-5)
#define MB_WRONG_NUMOFREG (-6)
#define MB_WRONG_FUNCTION_RECV (-7)


#define HOLDINGS_AMOUNT (3000)

class Reg_t
{
public:
    QByteArray reg;
};



#endif // MODBUS_CONF_H
