//
// Created by Zhen Peng on 7/21/20.
//

#ifndef PANNS_RUNTIME_H
#define PANNS_RUNTIME_H

#include <cstdint>

namespace PANNS {

struct Runtime {
    double add_to_worklist_ = 0.0;
    double add_to_result_ = 0.0;
    double pick_tops_ = 0.0;
    double expand_neighbors_ = 0.0;
    double exp_worklist_check_ = 0.0;
    double exp_result_check_ = 0.0;

//    void clear()
//    {
//        add_to_worklist_ = 0.0;
//        add_to_result_ = 0.0;
//        pick_tops_ = 0.0;
//        expand_neighbors_ = 0.0;
//        exp_worklist_check_ = 0.0;
//        exp_result_check_ = 0.0;
//    }
};

} // namespace PANNS

#endif //PANNS_RUNTIME_H
