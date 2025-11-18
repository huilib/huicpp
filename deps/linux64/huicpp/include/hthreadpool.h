/*
 * Created: Joseph Hui Thu Sep 21 2017
 * Description: 
  		Wrapper for huicpp thread pool.
*/


#ifndef __H_HUICPP_THREADPOOL_H__
#define __H_HUICPP_THREADPOOL_H__

#include "internal/pool_in.h"
#include "hthread.h"
#include "hmutex.h"
#include "hthreadkey.h"
#include "hthreadonce.h"

namespace HUICPP {

class HThreadPool  : public hc_internal::Pool {
public:
   using base = hc_internal::Pool;

public:
   typedef struct _child_info {
      ChildStatus m_cs;
      HThread m_thread;
      _child_info () : m_cs(ChildStatus::CS_UNUSED), m_thread() { }
   } child_info;

   typedef struct _thread_arg {
      HThreadPool* m_pool;
      HN m_index;
   } thread_arg;


public:
   explicit HThreadPool(HN uChildCount) noexcept;
   
    virtual ~ HThreadPool ();

public:
    virtual HRET Init () override;

    HN GetThreadIndex() const;

protected:
   virtual void childThreadRun () = 0;   

   /*
      虚函数. 线程退出前, 会调用.
   */
   virtual void beforeThreadExit(HN) { }

   void childThreadExit();

   void JoinAll();

private: 
   
   void create_info();
   void free_info();

   

   virtual void set_child_status(ChildStatus status);

private:
   static void* thread_init_run(void* argv);   

public:
   static HWaitSonLock s_son_lock;
   static HThreadKey s_thread_key;
   static HThreadOnce s_thread_once;
   static HN s_uChildCreated;

   static void ThreadSpecifiOnce ();
   static void ThreadSpecifiDestructor(void* ptr);
   static HSZ CreateThreadSpecific(HUN len);

protected:
   child_info* m_pinfos;
   HMutex m_mutex;
};

}

#endif //__H_HUICPP_THREADPOOL_H__
