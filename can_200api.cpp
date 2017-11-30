#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

#include "can_codes.h"

/**
 * \file can200_api.c
 * \brief Файл содержит реализацию функций для работы с платами CAN200.
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
int CAN200_HardReset(int fd)
{
    /* Контроль входных параметров */
    assert(fd > 0);

    if (fd < 0)
    {
        return EBADF;
    }
    return ioctl(fd, CAN200_HARDRESET);
}

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
int CAN200_SetDriverMode(int fd)
{
    /* Контроль входных параметров */
    assert(fd > 0);

    if (fd < 0)
    {
        return EBADF;
    }
    return ioctl(fd, CAN200_SETDRIVERMODE);
}

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
int CAN200_GetConfig(int fd, struct CAN_info_t *info)
{
    int               res;
    struct CAN_info_t tmp;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != info);

    if (fd < 0)
    {
        return EBADF;
    }
    if (NULL == info)
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_GETCONFIG, &tmp);
    if (-1 != res)
    {
        memcpy(info, &tmp, sizeof(struct CAN_info_t));
    }

    return res;
}

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
int CAN200_GetStatus(int fd, unsigned char *status)
{
    int           res;
    unsigned char tmp;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != status);

    if (fd < 0)
    {
        return EBADF;
    }
    if (NULL == status)
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_GETSTATUS, &tmp);
    if (-1 != res)
    {
        *status = tmp;
    }

    return res;
}

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
int CAN200_GetOverCounter(int fd, int *counter)
{
    int res;
    int tmp;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != counter);

    if (fd < 0)
    {
        return EBADF;
    }
    if (NULL == counter)
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_GETOVERCOUNTER, &tmp);
    if (-1 != res)
    {
        *counter = tmp;
    }

    return res;
}

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
int CAN200_GetWorkMode(int fd, unsigned char *mode)
{
    int           res;
    unsigned char tmp;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != mode);

    if (fd < 0)
    {
        return EBADF;
    }
    if (NULL == mode)
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_GETWORKMODE, &tmp);
    if (-1 != res)
    {
        *mode = tmp;
    }

    return res;
}

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
int CAN200_SetWorkMode(int fd, unsigned char mode)
{
    /* Контроль входных параметров */
    assert(fd > 0);
    assert((BasicCAN == mode) || (PeliCAN == mode));

    if (fd < 0)
    {
        return EBADF;
    }
    if ((BasicCAN != mode) && (PeliCAN != mode))
    {
        return EINVAL;
    }

    return ioctl(fd, CAN200_SETWORKMODE, &mode);
}


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
int CAN200_GetCANSpeed(int fd, unsigned int *speed)
{
    int            res;
    unsigned int   tmp;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != speed);

    if (fd < 0)
    {
        return EBADF;
    }
    if (NULL == speed)
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_GETCANSPEED, &tmp);
    if (-1 != res)
    {
        *speed = tmp;
    }

    return res;
}

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
int CAN200_SetCANSpeed(int fd, unsigned int speed)
{
    /* Контроль входных параметров */
    assert(fd > 0);
    assert( (CAN_SPEED_1000 == speed) || (CAN_SPEED_800 == speed) ||
            (CAN_SPEED_500  == speed) || (CAN_SPEED_250 == speed) ||
            (CAN_SPEED_125  == speed) || (CAN_SPEED_50  == speed) ||
            (CAN_SPEED_20   == speed) || (CAN_SPEED_10  == speed) ||
            (IS_CAN_SPEED_USER_DEFINED(speed)));

    if (fd < 0)
    {
        return EBADF;
    }
    if ((CAN_SPEED_1000 != speed) && (CAN_SPEED_800 != speed) &&
        (CAN_SPEED_500  != speed) && (CAN_SPEED_250 != speed) &&
        (CAN_SPEED_125  != speed) && (CAN_SPEED_50  != speed) &&
        (CAN_SPEED_20   != speed) && (CAN_SPEED_10  != speed) &&
        (!IS_CAN_SPEED_USER_DEFINED(speed)))
    {
        return EINVAL;
    }

    return ioctl(fd, CAN200_SETCANSPEED, &speed);
}

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
int CAN200_B_GetFilter(int fd, unsigned char *mask, unsigned char *filter)
{
    int               res;
    struct b_filter_t b_filter;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != mask);
    assert(NULL != filter);

    if (fd < 0)
    {
        return EBADF;
    }
    if ((NULL == mask) || (NULL == filter))
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_B_GETFILTER, &b_filter);
    if (-1 != res)
    {
        *mask = b_filter.mask;
        *filter = b_filter.filter;
    }

    return res;
}
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
int CAN200_B_SetFilter(int fd, unsigned char mask, unsigned char filter)
{
    struct b_filter_t b_filter;

    /* Контроль входных параметров */
    assert(fd > 0);

    if (fd < 0)
    {
        return EBADF;
    }

    b_filter.mask = mask;
    b_filter.filter = filter;

    return ioctl(fd, CAN200_B_SETFILTER, &b_filter);
}
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
    unsigned char *acr2, unsigned char *acr3)
{
    int               res;
    struct p_filter_t p_filter;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != mode);
    assert(NULL != amr0);
    assert(NULL != amr1);
    assert(NULL != amr2);
    assert(NULL != amr3);
    assert(NULL != acr0);
    assert(NULL != acr1);
    assert(NULL != acr2);
    assert(NULL != acr3);

    if (fd < 0)
    {
        return EBADF;
    }
    if (NULL == mode)
    {
        return EINVAL;
    }
    if ((NULL == amr0) || (NULL == amr1) || (NULL == amr2) || (NULL == amr3))
    {
        return EINVAL;
    }
    if ((NULL == acr0) || (NULL == acr1) || (NULL == acr2) || (NULL == acr3))
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_P_GETFILTER, &p_filter);
    if (-1 != res)
    {
        *amr0 = p_filter.mask[0];
        *amr1 = p_filter.mask[1];
        *amr2 = p_filter.mask[2];
        *amr3 = p_filter.mask[3];
        *acr0 = p_filter.filter[0];
        *acr1 = p_filter.filter[1];
        *acr2 = p_filter.filter[2];
        *acr3 = p_filter.filter[3];
        *mode = p_filter.mode;
    }

    return res;
}
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
    unsigned char acr2, unsigned char acr3)
{
    struct p_filter_t p_filter;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert((0 == mode) || (1 == mode));

    if (fd < 0)
    {
        return EBADF;
    }
    if ((0 != mode) && (1 != mode))
    {
        return EINVAL;
    }

    p_filter.mask[0] = amr0;
    p_filter.mask[1] = amr1;
    p_filter.mask[2] = amr2;
    p_filter.mask[3] = amr3;
    p_filter.filter[0] = acr0;
    p_filter.filter[1] = acr1;
    p_filter.filter[2] = acr2;
    p_filter.filter[3] = acr3;
    p_filter.mode = mode;

    return ioctl(fd, CAN200_P_SETFILTER, &p_filter);
}
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
int CAN200_P_GetRxErrorCounter(int fd, unsigned char *counter)
{
    int           res;
    unsigned char tmp;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != counter);

    if (fd < 0)
    {
        return EBADF;
    }
    if (NULL == counter)
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_P_GETRXERRORCOUNTER, &tmp);
    if (-1 != res)
    {
        *counter = tmp;
    }

    return res;
}
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
int CAN200_P_SetRxErrorCounter(int fd, unsigned char counter)
{
    /* Контроль входных параметров */
    assert(fd > 0);

    if (fd < 0)
    {
        return EBADF;
    }

    return ioctl(fd, CAN200_P_SETRXERRORCOUNTER, &counter);
}
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
int CAN200_P_GetTxErrorCounter(int fd, unsigned char *counter)
{
    int           res;
    unsigned char tmp;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != counter);

    if (fd < 0)
    {
        return EBADF;
    }
    if (NULL == counter)
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_P_GETTXERRORCOUNTER, &tmp);
    if (-1 != res)
    {
        *counter = tmp;
    }

    return res;
}
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
int CAN200_P_SetTxErrorCounter(int fd, unsigned char counter)
{
    /* Контроль входных параметров */
    assert(fd > 0);

    if (fd < 0)
    {
        return EBADF;
    }

    return ioctl(fd, CAN200_P_SETTXERRORCOUNTER, &counter);
}
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
int CAN200_P_GetErrorWarningLimit(int fd, unsigned char *counter)
{
    int           res;
    unsigned char tmp;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != counter);

    if (fd < 0)
    {
        return EBADF;
    }
    if (NULL == counter)
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_P_GETERRORWARNINGLIMIT, &tmp);
    if (-1 != res)
    {
        *counter = tmp;
    }

    return res;
}
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
int CAN200_P_SetErrorWarningLimit(int fd, unsigned char counter)
{
    /* Контроль входных параметров */
    assert(fd > 0);

    if (fd < 0)
    {
        return EBADF;
    }

    return ioctl(fd, CAN200_P_SETERRORWARNINGLIMIT, &counter);
}
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
int CAN200_SetBusOn(int fd)
{
    /* Контроль входных параметров */
    assert(fd > 0);

    if (fd < 0)
    {
        return EBADF;
    }

    return ioctl(fd, CAN200_SETBUSON);
}

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
int CAN200_GetTransmitStatus(int fd, int *status, int *len)
{
    int                      res;
    struct transmit_status_t sts;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != status);
    assert(NULL != len);

    if (fd < 0)
    {
        return EBADF;
    }
    if ((NULL == status) || (NULL == len))
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_GETTRANSMITSTATUS, &sts);
    if (-1 != res)
    {
        *status = sts.status;
        *len = sts.len;
    }

    return res;
}

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
int CAN200_P_GetErrorCode(int fd, unsigned char *code)
{
    int           res;
    unsigned char tmp;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != code);

    if (fd < 0)
    {
        return EBADF;
    }
    if (NULL == code)
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_P_GETERRORCODE, &tmp);
    if (-1 != res)
    {
        *code = tmp;
    }
    return res;
}
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
int CAN200_P_GetArbitrationLostCapture(int fd, unsigned char *data)
{
    int           res;
    unsigned char tmp;

    /* Контроль входных параметров */
    assert(fd > 0);
    assert(NULL != data);

    if (fd < 0)
    {
        return EBADF;
    }
    if (NULL == data)
    {
        return EINVAL;
    }

    res = ioctl(fd, CAN200_P_GETEARBITRATIONLOSTCAPTURE, &tmp);
    if (-1 != res)
    {
        *data = tmp;
    }
    return res;
}
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
int CAN200_DropTransmit(int fd, int command)
{
    /* Контроль входных параметров */
    assert(fd > 0);
    assert((CAN_DROP_CURRENT == command) || (CAN_DROP_ALL == command));

    if (fd < 0)
    {
        return EBADF;
    }
    if ((CAN_DROP_CURRENT != command) && (CAN_DROP_ALL != command))
    {
        return EINVAL;
    }

    return ioctl(fd, CAN200_DROPTRANSMIT, &command);
}

