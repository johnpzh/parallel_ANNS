//
// Copyright (c) 2017 ZJULearning. All rights reserved.
//
// This source code is licensed under the MIT license.
//
#include "index.h"
namespace efanna2e {
Index::Index(const size_t dimension, const size_t n, Metric metric = L2)
  : dimension_ (dimension), nd_(n), has_built(false) {
    switch (metric) {
      case L2:distance_ = new DistanceL2();
        break;
      default:distance_ = new DistanceL2();
        break;
    }
}
Index::~Index()
{
    // Added by Johnpzh
    delete distance_;
    // Ended by Johnpzh
}
}
