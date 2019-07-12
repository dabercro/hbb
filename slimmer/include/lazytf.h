#ifndef LAZYTF_H
#define LAZYTF_H

#include <list>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include "tensorflow/core/public/session.h"

#include "debugevent.h"

namespace {

  void _check_tf (const tensorflow::Status& status) {
    if (not status.ok()) {
      std::cerr << status.error_message() << std::endl;
      throw;
    }
  }

}


namespace lazytf {

  class TFReader {
  public:

    TFReader(const std::string& graph_file,
             const std::string& input_names_file,
             const std::vector<std::string>& output_nodes,
             unsigned number_targets) :
    output_nodes{output_nodes},
    outputs(number_targets * output_nodes.size()),
    targets_per_node{number_targets}
    {

      std::cout << "Initializing session to read " << graph_file << std::endl;

      _check_tf(tensorflow::ReadBinaryProto(tensorflow::Env::Default(),
                                            graph_file, &graph_def));
      _check_tf(session->Create(graph_def));

      std::list<std::string> keys {};

      std::ifstream infile {input_names_file};
      for (std::string line; std::getline(infile, line); ) {
        auto pos = line.find(' ');
        keys.emplace_back(pos != std::string::npos ? line.substr(0, pos) : line);
      }

      inputs.reserve(keys.size());

      for (auto& key : keys)
        inputs.emplace_back(std::make_pair(key, tensorflow::Tensor(tensorflow::DT_FLOAT, {1})));

      std::cout << "Saving places for " << inputs.size() << " variables" << std::endl;

    }


    template <typename ... A>
      const std::vector<float>& eval(A ... args) {

      return _eval(0, args ...);
      
    }

  private:
    
    const std::vector<float>& _eval (unsigned index, float value) {
      inputs[index].second.vec<float>()(0) = value;
      if (debugevent::debug)
        std::cout << "Running session" << std::endl;
      _check_tf(session->Run(inputs, output_nodes, {}, &output_tensor));
      if (debugevent::debug)
        std::cout << "Done" << std::endl;
      for (unsigned i_node = 0; i_node < output_nodes.size(); i_node++) {
        for (unsigned i = 0; i < targets_per_node; i++)
          outputs[i_node * targets_per_node + i] = output_tensor[i_node].matrix<float>()(0, i);
      }
      return outputs;
    }


    template <typename ... A>
      const std::vector<float>& _eval (const unsigned index, float value, A ... args) {
      inputs[index].second.vec<float>()(0) = value;
      return _eval(index + 1, args ...);
    }


    std::unique_ptr<tensorflow::Session> session {tensorflow::NewSession({})};
    tensorflow::GraphDef graph_def {};

    std::vector<std::pair<std::string, tensorflow::Tensor>> inputs {};
    std::vector<tensorflow::Tensor> output_tensor {};

    std::vector<std::string> output_nodes;
    std::vector<float> outputs;
    unsigned targets_per_node;

  };


  template <typename ... A>
    const std::vector<float>& eval (const std::string& graph_file,
                                    const std::string& input_names_file,
                                    const std::vector<std::string>& output_nodes,
                                    unsigned number_targets,
                                    A ... args) {

    static std::map<std::string, TFReader> readers {};

    auto iter = readers.find(graph_file);
    if (iter == readers.end())
      iter = readers.emplace(std::make_pair(graph_file,
                                            TFReader(graph_file, input_names_file, output_nodes, number_targets)
                                            )).first;

    if (debugevent::debug)
      std::cout << "Call evaluator" << std::endl;

    auto& out = iter->second.eval(args ...);

    if (debugevent::debug)
      std::cout << "Done" << std::endl;

    return out;

  }

}


#endif
