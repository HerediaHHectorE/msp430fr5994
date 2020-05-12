#include "general.h"
#define TM_PER_B_ACTIVO    0x01
#define TM_PER_B_TC        0x02

#define Period_Flag(TCP, NUM)    ((TCP)->t_period[(NUM)].flags & TM_PER_B_TC)
#define Timeout_Flag(TCP, NUM)   (!(TCP)->t_timeouts[(NUM)])
#define Period_Clear(TCP, NUM)   ((TCP)->t_period[(NUM)].flags &= ~TM_PER_B_TC)


typedef unsigned int Tm_Timeout;                       /* Timeout */
typedef unsigned char Tm_Qty;                          /* Cantidad  de Timers  o Timeouts*/

typedef struct Tm_Period Tm_Period;                    /* Timers */
struct Tm_Period{
   unsigned char flags;
   unsigned int count,
                period;
 };

typedef struct Tm_Control Tm_Control;                   /* Arreglo de Timers y Timeouts */
struct Tm_Control{
   /* Tabla y número de períodos */
   Tm_Period *t_period;
   Tm_Qty nts;

   /* Tabla y número de timeouts */
   Tm_Timeout *t_timeouts;
   Tm_Qty nto;
};

void TimerInit();                                       /* Inicialización en HW (Depende del Micro) */
unsigned char AttendTimer();                            /* Consulta de bandera de timer (Depende del Micro) */
void TimerProcess (Tm_Control *tcp);                    /* Valida los Timers */
void TimerStart(Tm_Control *tcp, unsigned int ntick, unsigned char tnum);   /* Iniciar un timer de la tabla*/
void TimeOutStart(Tm_Control *tcp, unsigned int ntick, unsigned char tnum); /* Iniciar un Timeout de la tabla */
void ArrayTimersInit(Tm_Control *tcp, Tm_Period *t_period,
                     Tm_Qty nts, Tm_Timeout *t_timeouts,
                     Tm_Qty nto);                       /* Inicializa la tabla de timers y timeouts */


