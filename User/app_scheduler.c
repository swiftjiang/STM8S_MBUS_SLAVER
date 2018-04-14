#include "app_scheduler.h"



//STATIC_ASSERT(sizeof(event_header_t) <= APP_SCHED_EVENT_HEADER_SIZE);

static event_header_t * m_queue_event_headers;              /**< Array for holding the queue event headers. */
static uint8_t *        m_queue_event_data;                 /**< Array for holding the queue event data. */
static volatile uint8_t m_queue_start_index;                /**< Index of queue entry at the start of the queue. */
static volatile uint8_t m_queue_end_index;                  /**< Index of queue entry at the end of the queue. */
static uint16_t         m_queue_event_size;                 /**< Maximum event size in queue. */
static uint16_t         m_queue_size;                       /**< Number of queue entries. */

/**@brief Macro for checking if a queue is full. */
#define APP_SCHED_QUEUE_FULL()    (next_index(m_queue_end_index) == m_queue_start_index)

/**@brief Macro for checking if a queue is empty. */
#define APP_SCHED_QUEUE_EMPTY()   (m_queue_end_index == m_queue_start_index)


/**@brief Function for incrementing a queue index, and handle wrap-around.
 *
 * @param[in]   index   Old index.
 *
 * @return      New (incremented) index.
 */
static  uint8_t next_index(uint8_t index)
{
    return (index < m_queue_size) ? (index + 1) : 0;
}

static uint8_t is_word_aligned(void * p)
{
    return (((uint32_t)p & 0x00000003) == 0);
}


uint32_t app_sched_init(uint16_t event_size, uint16_t queue_size, void * p_event_buffer)
{
    uint16_t data_start_index = (queue_size + 1) * sizeof(event_header_t);
    
    // Check that buffer is correctly aligned
    if (!is_word_aligned(p_event_buffer))
    {
        return ERR_SCHEDULER_PARAM;
    }
    
    // Initialize event scheduler
    m_queue_event_headers = p_event_buffer;
    m_queue_event_data    = &((uint8_t *)p_event_buffer)[data_start_index];
    m_queue_end_index     = 0;
    m_queue_start_index   = 0;
    m_queue_event_size    = event_size;
    m_queue_size          = queue_size;

    return ERR_SCHEDULER_SUCCESS;
}

/**@brief Function for reading the next event from specified event queue.
 *
 * @param[out]  pp_event_data       Pointer to pointer to event data.
 * @param[out]  p_event_data_size   Pointer to size of event data.
 * @param[out]  p_event_handler     Pointer to event handler function pointer.
 *
 * @return      NRF_SUCCESS if new event, NRF_ERROR_NOT_FOUND if event queue is empty.
 */
static uint32_t app_sched_event_get(void **                     pp_event_data,
                                    uint16_t *                  p_event_data_size,
                                    app_sched_event_handler_t * p_event_handler)
{
    uint32_t err_code =ERR_SCHEDULER_NOT_FOUND;
    
    uint8_t temp1 = m_queue_end_index;
    uint8_t temp2 = m_queue_start_index;
    //(m_queue_end_index == m_queue_start_index)
    if (!(temp1 == temp2))
    {
        uint16_t event_index;
        
        // NOTE: There is no need for a critical region here, as this function will only be called
        //       from app_sched_execute() from inside the main loop, so it will never interrupt
        //       app_sched_event_put(). Also, updating of (i.e. writing to) the start index will be
        //       an atomic operation.
        event_index         = m_queue_start_index;
        m_queue_start_index = next_index(m_queue_start_index);
        
        *pp_event_data     = &m_queue_event_data[event_index * m_queue_event_size];
        *p_event_data_size = m_queue_event_headers[event_index].event_data_size;
        *p_event_handler   = m_queue_event_headers[event_index].handler;	

        err_code = ERR_SCHEDULER_SUCCESS;
#if DEBUG_APP_SCHEDULER
			printf("APP_SCHED_QUEUE GET DATA\r\n");
			printf("m_queue_start_index = %d\r\n",m_queue_start_index);
			printf("m_queue_end_index   = %d\r\n",m_queue_end_index);
#endif
    }
//		else
//		{
//			printf("APP_SCHED_QUEUE_EMPTY\r\n");
//		}

    return err_code;
}

uint32_t app_sched_event_put(void *                    p_event_data,
                             uint16_t                  event_data_size,
                             app_sched_event_handler_t handler)
{
    uint32_t err_code;
//		uint8_t i;
	
		void *                    p_event_data_get;
    uint16_t                  event_data_size_get;
    app_sched_event_handler_t event_handler_get;
		
    if (event_data_size <= m_queue_event_size )
    {
        uint16_t event_index = 0xFFFF;
            
        //CRITICAL_REGION_ENTER();
        uint8_t temp1 = next_index(m_queue_end_index);
        uint8_t temp2 = m_queue_start_index;
        //(next_index(m_queue_end_index) == m_queue_start_index)
        if (!(temp1 == temp2))
        {
            event_index       = m_queue_end_index;
            m_queue_end_index = next_index(m_queue_end_index);
        }
				else
				{
#if DEBUG_APP_SCHEDULER
					printf("******APP_SCHED_QUEUE_FULL********\r\n");
					printf("m_queue_start_index = %d\r\n",m_queue_start_index);
					printf("m_queue_end_index   = %d\r\n",m_queue_end_index);
#endif
					while((app_sched_event_get(&p_event_data_get, &event_data_size_get, &event_handler_get) == ERR_SCHEDULER_SUCCESS))
					{
							event_handler_get(p_event_data, event_data_size);
					}
				}
//				if (!APP_SCHED_QUEUE_FULL())
//        {
//            event_index       = m_queue_end_index;
//            m_queue_end_index = next_index(m_queue_end_index);
//        }
					
            
        //CRITICAL_REGION_EXIT();
        
        if (event_index != 0xFFFF)
        {
            // NOTE: This can be done outside the critical region since the event consumer will
            //       always be called from the main loop, and will thus never interrupt this code.
            m_queue_event_headers[event_index].handler = handler;
            if ((p_event_data != NULL) && (event_data_size > 0))
            {
                memcpy(&m_queue_event_data[event_index * m_queue_event_size],
                       p_event_data,
                       event_data_size);
                m_queue_event_headers[event_index].event_data_size = event_data_size;
            }
            else
            {
                m_queue_event_headers[event_index].event_data_size = 0;
            } 
#if DEBUG_APP_SCHEDULER
						printf("-----------PUT DATA\r\n");
						printf("m_queue_start_index = %d\r\n",m_queue_start_index);
						printf("m_queue_end_index   = %d\r\n",m_queue_end_index);
#endif
            err_code = ERR_SCHEDULER_SUCCESS;
        }
        else
        {
            err_code = ERR_SCHEDULER_NO_MEM;
        }
    }
    else
    {
#if DEBUG_APP_SCHEDULER
      printf("event_data_size = %d\r\n",event_data_size);
      printf("m_queue_event_size = %d\r\n",m_queue_event_size);
      printf("event_data_size > m_queue_event_size\r\n");
#endif
      err_code = ERR_SCHEDULER_INVALID_LENGTH;
    }

    return err_code;
}





void app_sched_execute(void)
{
    void *                    p_event_data;
    uint16_t                  event_data_size;
    app_sched_event_handler_t event_handler;
	
		//uint32_t ret;

    // Get next event (if any), and execute handler

    while ((app_sched_event_get(&p_event_data, &event_data_size, &event_handler) == ERR_SCHEDULER_SUCCESS))
    {
        event_handler(p_event_data, event_data_size);
    }
		//if(ret != 0 )
		//printf("app_sched_event_get() ret is %d\r\n",ret);
		//printf("app_scheduler.c line 165 err ");
}
