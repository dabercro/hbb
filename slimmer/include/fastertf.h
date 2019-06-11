#ifndef FASTERTF_H
#define FASTERTF_H

#include <iostream>

#include <unordered_map>
#include <string>
#include <vector>

#include "tensorflow/core/public/session.h"

namespace {

  void _check_tf (const tensorflow::Status& status) {
    if (not status.ok()) {
      std::cerr << status.error_message() << std::endl;
      throw;
    }
  }

  struct tfstuff {
    tfstuff (const std::string& graph_file) {
      std::cout << "Loading " << graph_file << std::endl;
      _check_tf(tensorflow::ReadBinaryProto(tensorflow::Env::Default(),
                                            graph_file, &graph_def));
      _check_tf(session->Create(graph_def));
    }

    std::unique_ptr<tensorflow::Session> session {tensorflow::NewSession({})};
    tensorflow::GraphDef graph_def {};
    std::vector<tensorflow::Tensor> output {};
  }

  std::unordered_map<std::string, tfstuff> init (const std::vector& graph_files) {
    std::unordered_map<std::string, tfstuff> output {};
    for (auto& file : graph_files)
      output.insert({file, tfstuff{file}});
    return output;
  }

}

const std::unordered_map<std::string, tfstuff>& nopf (const std::vector<std::pair<std::string>>& )

#endif
