#ifndef __CAN200_API_H__
#define __CAN200_API_H__

#include "can_codes.h"

/**
 * \file can200_api.h
 * \brief Файл содержит описание функций для работы
 * с платами CAN200.
 */

/**
 * \fn int CAN200_HardReset(int fd)
 * \brief Аппаратный сброс платы
 *
 * Функция выполняет аппаратный сброс плат CAN-200PC, CAN-200MP, CAN-200PC104
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_HardReset(int fd);

/**
 * \fn int CAN200_SetDriverMode(int fd)
 * \brief Настройка выходных преобразователей платы
 *
 * Функция выполняет выходных преобразователей платы
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 * \warning Данную функцию необходимо вызвать до начала приема/выдачи данных
 */
//-----------------------------------------------------------------------------
int CAN200_SetDriverMode(int fd);

/**
 * \fn int CAN200_GetConfig(int fd, struct CAN_info_t *info)
 * \brief Получение описания платы
 *
 * Функция возвращает описание платы (тип, серийный номер и т.д.)
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param info (OUT) Указатель на структуру CAN_info_t, где будет сохранена информация о плате
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_GetConfig(int fd, struct CAN_info_t *info);

/**
 * \fn int CAN200_GetStatus(int fd, unsigned char *status)
 * \brief Получение статуса работы платы CAN200
 *
 * Функция возвращает статус платы. Расшифровка статуса приозводится в соответствии с объединением CAN_status_t
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param status (OUT) Указатель переменную, где будет сохранен статус
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_GetStatus(int fd, unsigned char *status);

/**
 * \fn int CAN200_GetOverCounter(int fd, int *counter)
 * \brief Получение количества отброшенных кадров
 *
 * Фунцкия возвращает количество кадров отброшенных из-за нехватки места в буфере приема драйвера
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param counter (OUT) Указатель переменную, где будет сохранено кол-во отброшенных кадров
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_GetOverCounter(int fd, int *counter);

/**
 * \fn int CAN200_GetWorkMode(int fd, unsigned char *mode)
 * \brief Получение текущего режима работы платы
 *
 * Функция возвращает текущий режи работы платы. Возможные варианты режима работы: #BasicCAN и #PeliCAN
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param mode (OUT) Указатель переменную, где будет сохранен текущий режим работы
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_GetWorkMode(int fd, unsigned char *mode);

/**
 * \fn int CAN200_SetWorkMode(int fd, unsigned char mode)
 * \brief Установка режима работы платы
 *
 * Функция устанавливает режим работы платы. Возможные варианты режима работы: #BasicCAN и #PeliCAN
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param mode (IN) Задаваемый режим работы платы
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_SetWorkMode(int fd, unsigned char mode);

/**
 * \fn int CAN200_GetCANSpeed(int fd, unsigned int *speed)
 * \brief Получение текущей настройки скорости обмена по шине CAN
 *
 * Функция возвращает текущую скорость обмена по шине CAN. Возможные варианты скорости:
 * #CAN_SPEED_1000, #CAN_SPEED_800, #CAN_SPEED_500, #CAN_SPEED_250, #CAN_SPEED_125,
 * #CAN_SPEED_20, #CAN_SPEED_10, #CAN_SPEED_USER_DEFINED
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param speed (OUT) Указатель переменную, где будет сохранена текущая скорость
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_GetCANSpeed(int fd, unsigned int *speed);

/**
 * \fn int CAN200_SetCANSpeed(int fd, unsigned int speed)
 * \brief Получение текущей настройки скорости обмена по шине CAN
 *
 * Функция Установка скорость обмена по шине CAN. Возможные варианты скорости:
 * #CAN_SPEED_1000, #CAN_SPEED_800, #CAN_SPEED_500, #CAN_SPEED_250, #CAN_SPEED_125,
 * #CAN_SPEED_20, #CAN_SPEED_10, #CAN_SPEED_USER_DEFINED
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param speed (IN) Задаваемая скорость
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_SetCANSpeed(int fd, unsigned int speed);

/** \addtogroup BasicCAN
 * \{
 * \fn int CAN200_B_GetFilter(int fd, unsigned char *mask, unsigned char *filter)
 * \brief Получение фильтра входных пакетов (режим #BasicCAN)
 *
 * Функция возвращает текущее значение фильтра входных пакетов
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param mask (OUT) Указатель переменную, где будет сохранено значение маски фильтра
 * \param filter (OUT) Указатель переменную, где будет сохранена значение кода фильтра
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_B_GetFilter(int fd, unsigned char *mask, unsigned char *filter);
/** \} */

/** \addtogroup BasicCAN
 * \{
 * \fn int CAN200_B_SetFilter(int fd, unsigned char mask, unsigned char filter)
 * \brief Установка фильтра входных пакетов (режим #BasicCAN)
 *
 * Функция устанавливает значение фильтра входных пакетов
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param mask (OUT) Значение маски фильтра
 * \param filter (OUT) Значение кода фильтра
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_B_SetFilter(int fd, unsigned char mask, unsigned char filter);
/** \} */

/** \addtogroup PeliCAN
 * \{
 * \fn int CAN200_P_GetFilter(int fd, unsigned char *mode, unsigned char *amr0, unsigned char *amr1,
    unsigned char *amr2, unsigned char *amr3, unsigned char *acr0, unsigned char *acr1,
    unsigned char *acr2, unsigned char *acr3)
 * \brief Получение фильтра входных пакетов (режим #PeliCAN)
 *
 * Функция возвращает текущее значение фильтра входных пакетов
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param mode (OUT) Режим работы фильтра
 * \param amr0, amr1, amr2, amr3 (OUT) Указатель переменную, где будет сохранено значение маски фильтра
 * \param acr0, acr1, acr2, acr3 (OUT) Указатель переменную, где будет сохранена значение кода фильтра
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_P_GetFilter(int fd, unsigned char *mode, unsigned char *amr0, unsigned char *amr1,
    unsigned char *amr2, unsigned char *amr3, unsigned char *acr0, unsigned char *acr1,
    unsigned char *acr2, unsigned char *acr3);
/** \} */

/** \addtogroup PeliCAN
 * \{
 * \fn int CAN200_P_SetFilter(int fd, unsigned char mode, unsigned char amr0, unsigned char amr1,
    unsigned char amr2, unsigned char amr3, unsigned char acr0, unsigned char acr1,
    unsigned char acr2, unsigned char acr3)
 * \brief Установка фильтра входных пакетов (режим #PeliCAN)
 *
 * Функция устанавливает значение фильтра входных пакетов
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param mode (IN) Режим работы фильтра
 * \param amr0, amr1, amr2, amr3 (IN) Значение маски фильтра
 * \param acr0, acr1, acr2, acr3 (IN) Значение кода фильтра
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_P_SetFilter(int fd, unsigned char mode, unsigned char amr0, unsigned char amr1,
    unsigned char amr2, unsigned char amr3, unsigned char acr0, unsigned char acr1,
    unsigned char acr2, unsigned char acr3);
/** \} */

/** \addtogroup PeliCAN
 * \{
 * \fn int CAN200_P_GetRxErrorCounter(int fd, unsigned char *counter)
 * \brief Получение счетчика ошибок приема (режим #PeliCAN)
 *
 * Фунцкия возвращает текущее значение счетчика ошибок приема
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param counter (OUT) Указатель переменную, где будет сохранено значение счетчика
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_P_GetRxErrorCounter(int fd, unsigned char *counter);
/** \} */

 /** \addtogroup PeliCAN
 * \{
 * \fn int CAN200_P_SetRxErrorCounter(int fd, unsigned char counter)
 * \brief Установка счетчика ошибок приема (режим #PeliCAN)
 *
 * Фунцкия устанавливает значение счетчика ошибок приема
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param counter (IN) Значение счетчика ошибок приема
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_P_SetRxErrorCounter(int fd, unsigned char counter);
/** \} */

/** \addtogroup PeliCAN
 * \{
 * \fn int CAN200_P_GetTxErrorCounter(int fd, unsigned char *counter)
 * \brief Получение счетчика ошибок выдачи (режим #PeliCAN)
 *
 * Фунцкия возвращает текущее значение счетчика ошибок выдачи
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param counter (OUT) Указатель переменную, где будет сохранено значение счетчика
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_P_GetTxErrorCounter(int fd, unsigned char *counter);
/** \} */

/** \addtogroup PeliCAN
 * \{
 * \fn int CAN200_P_SetTxErrorCounter(int fd, unsigned char counter)
 * \brief Установка счетчика ошибок выдачи (режим #PeliCAN)
 *
 * Фунцкия устанавливает значение счетчика ошибок выдачи
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param counter (IN) Значение счетчика ошибок выдачи
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_P_SetTxErrorCounter(int fd, unsigned char counter);
/** \} */

/** \addtogroup PeliCAN
 * \{
 * \fn int CAN200_P_GetErrorWarningLimit(int fd, unsigned char *counter)
 * \brief Получение верхнего предела счетчиков ошибок (режим #PeliCAN)
 *
 * Фунцкия возвращает текущее значение верхнего предела счетчиков ошибок
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param counter (OUT) Указатель переменную, где будет сохранено значение счетчика
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_P_GetErrorWarningLimit(int fd, unsigned char *counter);
/** \} */

/** \addtogroup PeliCAN
 * \{
 * \fn int CAN200_P_SetErrorWarningLimit(int fd, unsigned char counter)
 * \brief Установка верхнего предела счетчиков ошибок (режим #PeliCAN)
 *
 * Фунцкия устанавливает значение верхнего предела счетчиков ошибок
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param counter (IN) Значение счетчика ошибок выдачи
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_P_SetErrorWarningLimit(int fd, unsigned char counter);
/** \} */

/**
 * \fn int CAN200_SetBusOn(int fd)
 * \brief Перевод платы в режим bus on
 *
 * Функция позволяет переключить плату из режима bus off в режим bus on
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_SetBusOn(int fd);

/**
 * \fn int CAN200_GetTransmitStatus(int fd, int *status, int *len)
 * \brief Получение статуса выдачи пакетов
 *
 * Функция возвращает текущее состояние выдачи (завершена/не завершена/ошибка) и кол-во
 * оставщихся данных для выдачи. Возможные значение статуса выдачи: #CAN_TRANSMIT_WAIT,
 * #CAN_TRANSMIT, #CAN_TRANSMIT_COMPLETE, #CAN_TRANSMIT_ERROR
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param status (IN) Указатель переменную, где будет сохранен статус выдачи
 * \param len (IN) Указатель переменную, где будет сохранена длина оставшихся данных
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_GetTransmitStatus(int fd, int *status, int *len);

/** \addtogroup PeliCAN
 * \{
 * \fn int CAN200_P_GetErrorCode(int fd, unsigned char *code)
 * \brief Получение кода ошибки (режим #PeliCAN)
 *
 * Фунцкия возврашает код произошедшей ошибки.
 * Расшифровка кода ошибки производится в соответствии с объединением CAN_error_t
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param code (OUT) Значение кода ошибки
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_P_GetErrorCode(int fd, unsigned char *code);
/** \} */

/** \addtogroup PeliCAN
 * \{
 * \fn int CAN200_P_GetArbitrationLostCapture(int fd, unsigned char *data)
 * \brief Получение точку потери арбитража при выдаче кадра (режим #PeliCAN)
 *
 * Фунцкия возврышает код точки потери арбитража при выдаче кадра.
 * Расшифровка кода производится в соответствии с объединением CAN_arbitration_lost_capture_t
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param data (OUT) Значение кода точки потери арбитража
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_P_GetArbitrationLostCapture(int fd, unsigned char *data);
/** \} */

/**
 * \fn int CAN200_DropTransmit(int fd, int command)
 * \brief Отмена выдачи пакетов
 *
 * Функция позволяет отменить операцию выдачи.
 * Вызов с флагом #CAN_DROP_CURRENT останавливает выдачу пакетов, удаляет из очереди все пакеты,
 * поставленные в нее (функцийе write) текущим потоком и, в случае необходимости, вновь запускает выдачу
 * Вызов с флагом #CAN_DROP_ALL останавливает выдачу пакетов и полностью очищает очередь выдачи
 * \param fd (IN) Дискриптор платы, возврашенный функцией open
 * \param command (IN) Флаг определяющий отбрасываемые пакеты (#CAN_DROP_CURRENT или #CAN_DROP_ALL)
 * \return 0 - успешное завершение
 * \return EBADF - Неправильный дескриптор платы
 * \return -1 - системная ошибка
 */
//-----------------------------------------------------------------------------
int CAN200_DropTransmit(int fd, int command);

#endif /* __CAN200_API_H__ */
