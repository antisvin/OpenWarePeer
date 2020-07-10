#ifndef CHMEMPOOL_HPP
#define CHMEMPOOL_HPP

#include "ch.hpp"

namespace chibios_rt {

#if ((CH_CFG_USE_MEMPOOLS == TRUE) && (CH_CFG_USE_SEMAPHORES == TRUE)) ||      \
    defined(__DOXYGEN__)
/*------------------------------------------------------------------------*
 * chibios_rt::GuardedMemoryPool                                                 *
 *------------------------------------------------------------------------*/
/**
 * @brief   Class encapsulating a memory pool.
 */
class GuardedMemoryPool {
    /**
     * @brief   Embedded @p guarded_memory_pool_t structure.
     */
    guarded_memory_pool_t pool;

public:
    /**
     * @brief   GuardedMemoryPool constructor.
     *
     * @param[in] size      the size of the objects contained in this memory
     *                      pool, the minimum accepted size is the size of
     *                      a pointer to void.
     *
     * @init
     */
    GuardedMemoryPool(size_t size)
        : pool() {

        chGuardedPoolObjectInit(&pool, size);
    }

    /**
     * @brief   MemoryPool constructor.
     *
     * @param[in] size      the size of the objects contained in this memory
     *                      pool, the minimum accepted size is the size of
     *                      a pointer to void.
     * @param[in] p         pointer to the array first element
     * @param[in] n         number of elements in the array
     *
     * @init
     */
    GuardedMemoryPool(size_t size, void* p, size_t n)
        : pool() {

        chGuardedPoolObjectInit(&pool, size);
        chGuardedPoolLoadArray(&pool, p, n);
    }

    /* Prohibit copy construction and assignment, but allow move.*/
    GuardedMemoryPool(const GuardedMemoryPool&) = delete;
    GuardedMemoryPool& operator=(const GuardedMemoryPool&) = delete;
    GuardedMemoryPool(GuardedMemoryPool&&) = default;
    GuardedMemoryPool& operator=(GuardedMemoryPool&&) = default;

    /**
     * @brief   Loads a memory pool with an array of static objects.
     * @pre     The memory pool must be already been initialized.
     * @pre     The array elements must be of the right size for the specified
     *          memory pool.
     * @post    The memory pool contains the elements of the input array.
     *
     * @param[in] p         pointer to the array first element
     * @param[in] n         number of elements in the array
     *
     * @api
     */
    void loadArray(void* p, size_t n) {
        chGuardedPoolLoadArray(&pool, p, n);
    }

    /**
     * @brief   Allocates an object from a memory pool.
     * @pre     The memory pool must be already been initialized.
     *
     * @param[in] timeout      timeout for allocation
     *
     * @return              The pointer to the allocated object.
     * @retval nullptr      if pool is empty.
     *
     * @iclass
     */
    void* allocTimeout(sysinterval_t timeout) {

        return chGuardedPoolAllocTimeout(&pool, timeout);
    }

    /**
     * @brief   Allocates an object from a memory pool.
     * @pre     The memory pool must be already been initialized.
     *
     * @param[in] timeout      timeout for allocation
     *
     * @return              The pointer to the allocated object.
     * @retval nullptr      if pool is empty.
     *
     * @api
     */
    void* allocTimeoutS(sysinterval_t timeout) {

        return chGuardedPoolAllocTimeoutS(&pool, timeout);
    }

    /**
     * @brief   Releases an object into a memory pool.
     * @pre     The memory pool must be already been initialized.
     * @pre     The freed object must be of the right size for the specified
     *          memory pool.
     * @pre     The object must be properly aligned to contain a pointer to
     *          void.
     *
     * @param[in] objp      the pointer to the object to be released
     *
     * @iclassb
     */
    void free(void* objp) {
        chGuardedPoolFree(&pool, objp);
    }

    /**
     * @brief   Adds an object to a memory pool.
     * @pre     The memory pool must be already been initialized.
     * @pre     The added object must be of the right size for the specified
     *          memory pool.
     * @pre     The added object must be memory aligned to the size of
     *          @p stkalign_t type.
     * @note    This function is just an alias for @p chPoolFree() and has been
     *          added for clarity.
     *
     * @param[in] objp      the pointer to the object to be added
     *
     * @iclass
     */
    void freeI(void* objp) {
        chGuardedPoolFreeI(&pool, objp);
    }
};

/*------------------------------------------------------------------------*
 * chibios_rt::GuardedObjectsPool                                                *
 *------------------------------------------------------------------------*/
/**
 * @brief   Template class encapsulating a memory pool and its elements.
 */
template <class T, size_t N>
class GuardedObjectsPool : public GuardedMemoryPool {
    /* The buffer is declared as an array of pointers to void for two
       reasons:
       1) The objects must be properly aligned to hold a pointer as
          first field.
       2) Objects are dirtied when loaded in the pool.*/
    void* pool_buf[(N * sizeof(T)) / sizeof(void*)];

public:
    /**
     * @brief   GuardedObjectsPool constructor.
     *
     * @init
     */
    GuardedObjectsPool(void)
        : GuardedMemoryPool(sizeof(T)) {

        loadArray(pool_buf, N);
    }
};

/*------------------------------------------------------------------------*
 * chibios_rt::ThreadsPool                                                *
 *------------------------------------------------------------------------*/
/**
 * @brief   Template class encapsulating a pool of threads.
 */
template <size_t S, size_t N, const char* C>
class GuardedThreadsPool : public BaseDynamicThread {
    THD_WORKING_AREA(working_areas, S)[N];
    GuardedMemoryPool threads_pool;

public:
    /**
     * @brief   ThreadsPool constructor.
     *
     * @init
     */
    GuardedThreadsPool(void)
        : threads_pool(THD_WORKING_AREA_SIZE(S)) {

        threads_pool.loadArray(working_areas, N);
    }

    /**
     * @brief   Starts a dynamic thread from the pool.
     *
     * @param[in] prio          thread priority
     * @return                  A reference to the created thread with
     *                          reference counter set to one.
     *
     * @api
     */
    ThreadReference start(tprio_t prio) override {
        void _thd_start(void* arg);

        return ThreadReference(chThdCreateFromMemoryPool(
            &threads_pool.pool, C, prio, _thd_start, this));
    }
};

}
#endif /* CH_CFG_USE_MEMPOOLS == TRUE */
#endif /* CHMEMPOOL_HPP */