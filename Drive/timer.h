#ifndef _TIMER_H
#define _TIMER_H

#include "stm8s.h"

typedef void   (*tmr_fnct_ptr) (void* arg);
typedef uint8_t TMR_STATE;

typedef struct{
  uint32_t		expire;				
  uint32_t              period;
  tmr_fnct_ptr	        callback;	
  TMR_STATE		state;	
  uint32_t		opt;
  void* arg;
}timer_t;

#define   TIMER_LIST_MAX		5U		


/* timer state */
#define  TMR_STATE_UNUSED                    (TMR_STATE)(0u)
#define  TMR_STATE_STOPPED                   (TMR_STATE)(1u)
#define  TMR_STATE_RUNNING                   (TMR_STATE)(2u)
#define  TMR_STATE_COMPLETED                 (TMR_STATE)(3u)

#define  OPT_TMR_NONE                        (0u)  /* No option selected                                 */
#define  OPT_TMR_ONE_SHOT                    (1u)  /* Timer will not auto restart when it expires        */
#define  OPT_TMR_PERIODIC                    (2u)  /* Timer will     auto restart when it expires        */

//��ʼ��timer������Ӳ����TIM2����ʱ�ӵδ�
void timer_init(void);

//����һ����ʱ��
//����ptimer����ʱ���ṹ��ָ��
//����period����ʱ����
//����opt��ѡ������޲������ر��Զ������������Զ�����
//����pcallback����ʱ�ص�����ָ��
//����arg���ɴ��ݸ����ǻص������Ĳ���
uint8_t timer_create(timer_t   		*ptimer,	/* point to timer_t */
                     uint32_t  		period,		/* period			*/
                     uint32_t  		opt,
                     tmr_fnct_ptr	pcallback,
                     void* arg );

//ɾ����ʱ��
uint8_t timer_del(timer_t *ptimer);
//�򿪶�ʱ��
uint8_t timer_start(timer_t *ptimer);
//�رն�ʱ��
uint8_t timer_stop(timer_t *ptimer);
//��ȡ��ǰ��ʱ��ms�δ����
uint32_t get_timer_ms(void);

#endif

