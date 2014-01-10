//  Copyright (c) 2013 Jeremy Kemp
//  Copyright (c) 2013 Bryce Adelstein-Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>
#include <hpx/hpx_fwd.hpp>
#include <hpx/hpx_start.hpp>
#include <hpx/runtime/threads/topology.hpp>
#include <hpx/lcos/local/barrier.hpp>
#include <hpx/util/static.hpp>
#include <hpx/include/lcos.hpp>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/cstdint.hpp>

#include <hpx/util/high_resolution_timer.hpp>
#include <map>

typedef void *frame_pointer_t;
typedef int omp_tid;
typedef void (*omp_micro)(int , frame_pointer_t);

typedef void (*omp_task_func)(void *firstprivates, void *fp);

typedef hpx::lcos::local::spinlock mutex_type;

using hpx::lcos::local::barrier;
using hpx::lcos::unique_future;
using std::cout;
using std::endl;
using std::vector;
using std::map;
using hpx::util::high_resolution_timer;

struct thread_data {
    int thread_num;
    vector<unique_future<void>> task_handles;
};

class hpx_runtime {
    public:
        hpx_runtime(int num_threads);
        void fork(int num_threads, omp_task_func task_func, frame_pointer_t fp);
        int get_thread_num();
        int get_num_threads();
        void barrier_wait();
        void lock(int lock_id);
        bool trylock(int lock_id);
        void unlock(int lock_id);
        int new_mtx();
        void create_task(omp_task_func taskfunc, void *frame_pointer,
                         void *firstprivates, int may_delay,
                         int is_tied, int blocks_parent);
        void task_wait();
        double get_time();
        
    private:
        bool hpx_initialized = false;
//        mutex_type single_mtx;
        mutex_type init_mtx;
        barrier *globalBarrier;
        int num_threads;
        //vector<mutex_type*> lock_list;
        map<int, mutex_type> lock_map;
        high_resolution_timer walltime;
};

