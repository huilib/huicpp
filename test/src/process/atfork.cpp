

#include "atfork.h"

#include <list>

#include <pthread.h>

namespace HUICPP {


struct AtForkTask {
    HCSTRR strName;
    std::function<void()> prepare;
    std::function<void()> parent;
    std::function<void()> child;
};


class AtForkList {
public:
    void Prepare() {
        for (std::list<AtForkTask>::iterator it = m_task.begin(); 
            it != m_task.end(); ++it) {
            if (it->prepare) {
                it->prepare();
            }
        }
    }

    void Parent() {
        for (std::list<AtForkTask>::iterator it = m_task.begin(); 
            it != m_task.end(); ++it) {
            if (it->parent) {
                it->parent();
            }
        }
    }

    void Child() {
        for (std::list<AtForkTask>::iterator it = m_task.begin(); 
            it != m_task.end(); ++it) {
            if (it->child) {
                it->child();
            }
        }
    }

    void Append(HCSTRR strName, std::function<void()> prepare, std::function<void()> parent, 
        std::function<void()> son) {
        m_task.push_back({strName, std::move(prepare), std::move(parent), std::move(son)});
    }

    void Remove(HCSTRR strName) {
        for (std::list<AtForkTask>::iterator it = m_task.begin(); it != m_task.end(); ++it) {
            if (it->strName == strName) {
                m_task.erase(it);
                return;
            }
        }
    }

private:
    std::list<AtForkTask> m_task;
};


struct AtForkListSingleton {
    static AtForkList& Make() {
        auto& instance = *new AtForkList;
        SYS_RET_T ret = pthread_atfork(Prepare, Parent, Child);
        if (ret != 0) {
            printf("pthread_atfork failed with %d", ret);
            exit(EXIT_FAILURE);
        }
        return instance;
    }

    static AtForkList& Get() {
        static AtForkList& instance = Make();
        return instance;
    }

    static void Prepare() {
        Get().Prepare();        
    }

    static void Parent() {
        Get().Parent();    
    }

    static void Child() {
        Get().Child();
    }
};


void HAtFork::RegisterHandler (HCSTRR strName, std::function<void()> prepare, std::function<void()> parent, 
        std::function<void()> son) {

    AtForkList& af_list = AtForkListSingleton::Get();

    af_list.Append(strName, std::move(prepare), std::move(parent), std::move(son));

}


void HAtFork::UnregisterHandler(HCSTRR strName) {

    AtForkList& af_list = AtForkListSingleton::Get();

    af_list.Remove(strName);

}


}