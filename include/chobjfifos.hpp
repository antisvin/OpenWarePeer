#ifndef CHOBJFIFOS_HPP
#define CHOBJFIFOS_HPP

#include "ch.hpp"
#include "chmempool.hpp"

namespace chibios_rt {


#if (CH_CFG_USE_OBJ_FIFOS == TRUE) || defined(__DOXYGEN__)
/*------------------------------------------------------------------------*
 * chibios_rt::Fifo                                                       *
 *------------------------------------------------------------------------*/
/**
 * @brief   Class encapsulating an objects FIFO queue.
 */
class Fifo {
    /**
     * @brief   Embedded @p objects_fifo_t structure.
     */
    objects_fifo_t fifo;

public:
    /**
     * @brief   Initializes a FIFO object.
     * @pre     The messages size must be a multiple of the alignment
     *          requirement.
     *
     * @param[in] size      size of objects
     * @param[in] n         number of objects available
     * @param[in] buf       pointer to the buffer of objects, it must be able
     *                      to hold @p objn objects of @p objsize size
     * @param[in] msgbuf    pointer to the buffer of messages, it must be able
     *                      to hold @p objn messages
     *
     * @init
     */
    
    Fifo(size_t size, size_t n, void* buf, msg_t* msgbuf)
        : fifo() {
        chFifoObjectInit(&fifo, size, n, buf, msgbuf);
    }

    /* Prohibit copy construction and assignment, but allow move.*/
    Fifo(const Fifo&) = delete;
    Fifo& operator=(const Fifo&) = delete;
    Fifo(Fifo&&) = default;
    Fifo& operator=(Fifo&&) = default;    

    /**
     * @brief   Allocates a free object.
     *
     * @return              The pointer to the allocated object.
     * @retval NULL         if an object is not immediately available.
     *
     * @iclass
     */
    void* takeObjectI(){
        return chFifoTakeObjectI(&fifo);        
    }

    /**
     * @brief   Allocates a free object.
     *
     * @param[in] timeout   the number of ticks before the operation timeouts,
     *                      the following special values are allowed:
     *                      - @a TIME_IMMEDIATE immediate timeout.
     *                      - @a TIME_INFINITE no timeout.
     *                      .
     * @return              The pointer to the allocated object.
     * @retval NULL         if an object is not available within the specified
     *                      timeout.
     *
     * @sclass
     */
    void* takeObjectTimeoutS(sysinterval_t timeout) {
        return chFifoTakeObjectTimeoutS(&fifo, timeout);
    }

    /**
     * @brief   Allocates a free object.
     *
     * @param[in] timeout   the number of ticks before the operation timeouts,
     *                      the following special values are allowed:
     *                      - @a TIME_IMMEDIATE immediate timeout.
     *                      - @a TIME_INFINITE no timeout.
     *                      .
     * @return              The pointer to the allocated object.
     * @retval NULL         if an object is not available within the specified
     *                      timeout.
     *
     * @api
     */
    void* takeObjectTimeout(sysinterval_t timeout) {
        return chFifoTakeObjectTimeout(&fifo, timeout);
    }

    /**
     * @brief   Releases a fetched object.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @iclass
     */
    void returnObjectI(void* objp){
        return chFifoReturnObjectI(&fifo, objp);
    }

    /**
     * @brief   Releases a fetched object.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @sclass
     */
    void returnObjectS(void* objp){
        return chFifoReturnObjectS(&fifo, objp);
    }

    /**
     * @brief   Releases a fetched object.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @api
     */
    void returnObject(void* objp){
        return chFifoReturnObject(&fifo, objp);
    }

    /**
     * @brief   Posts an object.
     * @note    By design the object can be always immediately posted.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @iclass
     */
    void sendObjectI(void* objp){
        return chFifoSendObjectI(&fifo, objp);
    }

    /**
     * @brief   Posts an object.
     * @note    By design the object can be always immediately posted.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @sclass
     */
    void sendObjectS(void* objp){
        return chFifoSendObjectS(&fifo, objp);
    }

    /**
     * @brief   Posts an object.
     * @note    By design the object can be always immediately posted.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @api
     */
    void sendObject(void* objp){
        return chFifoSendObject(&fifo, objp);
    }
    
    /**
     * @brief   Posts an high priority object.
     * @note    By design the object can be always immediately posted.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @iclass
     */
    void sendObjectAheadI(void* objp){
        return chFifoSendObjectAheadI(&fifo, objp);
    }                                            

    /**
     * @brief   Posts an high priority object.
     * @note    By design the object can be always immediately posted.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @sclass
     */
    void sendObjectAheadS(void* objp){
        return chFifoSendObjectAheadS(&fifo, objp);
    }

    /**
     * @brief   Posts an high priority object.
     * @note    By design the object can be always immediately posted.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @api
     */
    void sendObjectAhead(void* objp){
        return chFifoSendObjectAhead(&fifo, objp);
    }

    /**
     * @brief   Fetches an object
     *
     * @param[in] objpp     pointer to the fetched object reference
     * @return              The operation status.
     * @retval MSG_OK       if an object has been correctly fetched.
     * @retval MSG_TIMEOUT  if the FIFO is empty and a message cannot be fetched.     *
     * @iclass
     */
    msg_t receiveObjectI(void** objpp){
        return chFifoReceiveObjectI(&fifo, objpp);
    }

    /**
     * @brief   Fetches an object
     *
     * @param[in] objpp     pointer to the fetched object reference
     * @param[in] timeout   the number of ticks before the operation timeouts,
     *                      the following special values are allowed:
     *                      - @a TIME_IMMEDIATE immediate timeout.
     *                      - @a TIME_INFINITE no timeout.
     * @return              The operation status.
     * @retval MSG_OK       if an object has been correctly fetched.
     * @retval MSG_TIMEOUT  if the FIFO is empty and a message cannot be fetched.     *
     * @sclass
     */
    msg_t receiveObjectTimeoutS(void** objpp, sysinterval_t timeout){
        return chFifoReceiveObjectTimeoutS(&fifo, objpp, timeout);
    }

    /**
     * @brief   Fetches an object
     *
     * @param[in] objpp     pointer to the fetched object reference
     * @param[in] timeout   the number of ticks before the operation timeouts,
     *                      the following special values are allowed:
     *                      - @a TIME_IMMEDIATE immediate timeout.
     *                      - @a TIME_INFINITE no timeout.
     * @return              The operation status.
     * @retval MSG_OK       if an object has been correctly fetched.
     * @retval MSG_TIMEOUT  if the FIFO is empty and a message cannot be fetched.     *
     * @api
     */
    msg_t receiveObjectTimeout(void** objpp, sysinterval_t timeout){
        return chFifoReceiveObjectTimeout(&fifo, objpp, timeout);
    }
};

/*------------------------------------------------------------------------*
 * chibios_rt::ObjectsFifo                                                *
 *------------------------------------------------------------------------*/
/**
 * @brief   Template class encapsulating an objects FIFO pool and its elements.
 */
template <class T, size_t N>
class ObjectsFifo : public Fifo {
    void* fifo_buf[(N * sizeof(T)) / sizeof(void*)];
    msg_t mb_buf[N];
    
public:
    /**
     * @brief   ObjectsFifo constructor.
     *
     * @init
     */

    ObjectsFifo(void) : Fifo(sizeof(T), N, fifo_buf, mb_buf) {};

    /**
     * @brief   Allocates a free object.
     *
     * @return              The pointer to the allocated object.
     * @retval NULL         if an object is not immediately available.
     *
     * @iclass
     */
    T* takeObjectI(){
        return reinterpret_cast<T*>(Fifo::takeObjectI());
    }

    /**
     * @brief   Allocates a free object.
     *
     * @param[in] timeout   the number of ticks before the operation timeouts,
     *                      the following special values are allowed:
     *                      - @a TIME_IMMEDIATE immediate timeout.
     *                      - @a TIME_INFINITE no timeout.
     *                      .
     * @return              The pointer to the allocated object.
     * @retval NULL         if an object is not available within the specified
     *                      timeout.
     *
     * @sclass
     */
    T* takeObjectTimeoutS(sysinterval_t timeout) {
        return reinterpret_cast<T*>(Fifo::takeObjectTimeoutS(timeout));
    }

    /**
     * @brief   Allocates a free object using infinite timeout.
     *
     * @return              The pointer to the allocated object.
     *
     * @sclass
     */
    T* takeObjectTimeoutInfiniteS() {
        return reinterpret_cast<T*>(Fifo::takeObjectTimeoutS(TIME_INFINITE));
    }

    /**
     * @brief   Allocates a free object.
     *
     * @param[in] timeout   the number of ticks before the operation timeouts,
     *                      the following special values are allowed:
     *                      - @a TIME_IMMEDIATE immediate timeout.
     *                      - @a TIME_INFINITE no timeout.
     *                      .
     * @return              The pointer to the allocated object.
     * @retval NULL         if an object is not available within the specified
     *                      timeout.
     *
     * @api
     */
    T* takeObjectTimeout(sysinterval_t timeout) {
        return reinterpret_cast<T*>(Fifo::takeObjectTimeout(timeout));
    }
    
    /**
     * @brief   Allocates a free object with infinite timeout.
     *
     * @return              The pointer to the allocated object.
     *
     * @api
     */
    T* takeObjectTimeoutInfinite() {
        return reinterpret_cast<T*>(Fifo::takeObjectTimeout(TIME_INFINITE));
    }

    /**
     * @brief   Releases a fetched object.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @iclass
     */
    void returnObjectI(T* objp){
        return Fifo::returnObjectI(reinterpret_cast<void*>(objp));
    }

    /**
     * @brief   Releases a fetched object.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @sclass
     */
    void returnObjectS(T* objp){
        return Fifo::returnObjectS(reinterpret_cast<void*>(objp));
    }

    /**
     * @brief   Releases a fetched object.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @api
     */
    void returnObject(T* objp){
        return Fifo::returnObject(reinterpret_cast<void*>(objp));
    }

    /**
     * @brief   Posts an object.
     * @note    By design the object can be always immediately posted.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @iclass
     */
    void sendObjectI(T* objp){
        return Fifo::sendObjectI(reinterpret_cast<void*>(objp));
    }

    /**
     * @brief   Posts an object.
     * @note    By design the object can be always immediately posted.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @sclass
     */
    void sendObjectS(T* objp){
        return Fifo::sendObjectS(reinterpret_cast<void*>(objp));
    }

    /**
     * @brief   Posts an object.
     * @note    By design the object can be always immediately posted.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @api
     */
    void sendObject(T* objp){
        return Fifo::sendObject(reinterpret_cast<void*>(objp));
    }
    
    /**
     * @brief   Posts an high priority object.
     * @note    By design the object can be always immediately posted.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @iclass
     */
    void sendObjectAheadI(T* objp){
        return Fifo::sendObjectAheadI(reinterpret_cast<void*>(objp));
    }                                            

    /**
     * @brief   Posts an high priority object.
     * @note    By design the object can be always immediately posted.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @sclass
     */
    void sendObjectAheadS(T* objp){
        return Fifo::sendObjectAheadS(reinterpret_cast<void*>(objp));
    }

    /**
     * @brief   Posts an high priority object.
     * @note    By design the object can be always immediately posted.
     *
     * @param[in] objp      pointer to the object to be released
     *
     * @api
     */
    void sendObjectAhead(T* objp){
        return Fifo::sendObjectAhead(reinterpret_cast<void*>(objp));
    }

    /**
     * @brief   Fetches an object
     *
     * @param[in] objpp     pointer to the fetched object reference
     * @return              The operation status.
     * @retval MSG_OK       if an object has been correctly fetched.
     * @retval MSG_TIMEOUT  if the FIFO is empty and a message cannot be fetched.     *
     * @iclass
     */
    msg_t receiveObjectI(T** objpp){
        return Fifo::receiveObjectI(reinterpret_cast<void**>(objpp));
    }

    /**
     * @brief   Fetches an object
     *
     * @param[in] objpp     pointer to the fetched object reference
     * @param[in] timeout   the number of ticks before the operation timeouts,
     *                      the following special values are allowed:
     *                      - @a TIME_IMMEDIATE immediate timeout.
     *                      - @a TIME_INFINITE no timeout.
     * @return              The operation status.
     * @retval MSG_OK       if an object has been correctly fetched.
     * @retval MSG_TIMEOUT  if the FIFO is empty and a message cannot be fetched.     *
     * @sclass
     */
    msg_t receiveObjectTimeoutS(T** objpp, sysinterval_t timeout){
        return Fifo::receiveObjectTimeoutS(reinterpret_cast<void**>(objpp), timeout);
    }

    /**
     * @brief   Fetches an object
     *
     * @param[in] objpp     pointer to the fetched object reference
     * @param[in] timeout   the number of ticks before the operation timeouts,
     *                      the following special values are allowed:
     *                      - @a TIME_IMMEDIATE immediate timeout.
     *                      - @a TIME_INFINITE no timeout.
     * @return              The operation status.
     * @retval MSG_OK       if an object has been correctly fetched.
     * @retval MSG_TIMEOUT  if the FIFO is empty and a message cannot be fetched.     *
     * @api
     */
    msg_t receiveObjectTimeout(T** objpp, sysinterval_t timeout){
        return Fifo::receiveObjectTimeout(reinterpret_cast<void**>(objpp), timeout);
    }

    /**
     * @brief   Fetches an object with infinite timeout
     *
     * @param[in] objpp     pointer to the fetched object reference
     * @return              The operation status.
     * @retval MSG_OK       if an object has been correctly fetched.
     * @retval MSG_TIMEOUT  if the FIFO is empty and a message cannot be fetched.     *
     * @api
     */
    msg_t receiveObjectTimeoutInfinite(T** objpp){
        return Fifo::receiveObjectTimeout(reinterpret_cast<void**>(objpp), TIME_INFINITE);
    }       
};

#endif

}
#endif