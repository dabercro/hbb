#ifndef DAN_TF_H
#define DAN_TF_H

#include <memory>

#include "tensorflow/core/public/session.h"


namespace {

  void _check_tf (const tensorflow::Status& status) {
    if (not status.ok()) {
      std::cerr << status.error_message() << std::endl;
      throw;
    }
  }

  std::unique_ptr<tensorflow::Session> session {tensorflow::NewSession({})};
  tensorflow::GraphDef graph_def {};

  std::vector<std::pair<std::string, tensorflow::Tensor>> inputs = {
    {"jet1_pt", {tensorflow::DT_FLOAT, {1}}},
    {"jet1_efrac", {tensorflow::DT_FLOAT, {1}}},
    {"jet1_chf", {tensorflow::DT_FLOAT, {1}}},
    {"jet1_nhf", {tensorflow::DT_FLOAT, {1}}},
    {"jet1_leadlep_ptfrac", {tensorflow::DT_FLOAT, {1}}}
  };

  std::vector<tensorflow::Tensor> outputs {};

  float _eval (const unsigned index, float value) {
    inputs[index].second.vec<float>()(0) = value;
    _check_tf(session->Run(inputs, {"linear/linear_model/linear_model/linear_model/weighted_sum"}, {}, &outputs));
    std::cout << "Ran" << std::endl;
    return outputs[0].matrix<float>()(0, 0);
  }

  template <typename ... A>
    float _eval (const unsigned index, float value, A ... args) {
    inputs[index].second.vec<float>()(0) = value;
    return _eval(index + 1, args ...);
  }

}

namespace dantf {

  void init () {
    _check_tf(tensorflow::ReadBinaryProto(tensorflow::Env::Default(),
                                         "/home/dabercro/hbb/analysis/test.pb",
                                         &graph_def));
    _check_tf(session->Create(graph_def));
  }

  template <typename ... A>
    float eval (A ... args) {
    std::cout << "Starting eval" << std::endl;
    return _eval (0, args ...);
  }

}


#endif
