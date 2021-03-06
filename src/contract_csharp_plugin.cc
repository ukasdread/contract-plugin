/*
 *
 * Copyright 2015 gRPC authors. Modified by AElfProject.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// Generates C# gRPC service interface out of Protobuf IDL.

#include <memory>

#include "config.h"
#include "contract_csharp_generator.h"
#include "contract_csharp_generator_helpers.h"

class ContractCSharpGrpcGenerator : public grpc::protobuf::compiler::CodeGenerator {
public:
    ContractCSharpGrpcGenerator() {}
    ~ContractCSharpGrpcGenerator() {}

    bool Generate(const grpc::protobuf::FileDescriptor* file,
                  const grpc::string& parameter,
                  grpc::protobuf::compiler::GeneratorContext* context,
                  grpc::string* error) const {
      std::vector<std::pair<grpc::string, grpc::string> > options;
      grpc::protobuf::compiler::ParseGeneratorParameter(parameter, &options);

      // default generate contract with event
      char flags = grpc_contract_csharp_generator::GENERATE_CONTRACT_WITH_EVENT;

      for (size_t i = 0; i < options.size(); i++) {
        if (options[i].first == "stub") {
          flags |= grpc_contract_csharp_generator::GENERATE_STUB_WITH_EVENT;
          flags &= ~grpc_contract_csharp_generator::GENERATE_CONTRACT;
        } else if (options[i].first == "reference") {
          // reference doesn't require event
          flags |= grpc_contract_csharp_generator::GENERATE_REFERENCE;
          flags &= ~grpc_contract_csharp_generator::GENERATE_CONTRACT;
        } else if (options[i].first == "nocontract") {
          flags &= ~grpc_contract_csharp_generator::GENERATE_CONTRACT;
        } else if (options[i].first == "noevent") {
          flags &= ~grpc_contract_csharp_generator::GENERATE_EVENT;
        } else if (options[i].first == "internal_access") {
          flags |= grpc_contract_csharp_generator::INTERNAL_ACCESS;
        } else {
          *error = "Unknown generator option: " + options[i].first;
          return false;
        }
      }

      grpc::string code = grpc_contract_csharp_generator::GetServices(file, flags);
      if (code.size() == 0) {
        return true;  // don't generate a file if there are no services
      }

      // Get output file name.
      grpc::string file_name;
      if (!grpc_contract_csharp_generator::ServicesFilename(file, &file_name)) {
        return false;
      }
      std::unique_ptr<grpc::protobuf::io::ZeroCopyOutputStream> output(
              context->Open(file_name));
      grpc::protobuf::io::CodedOutputStream coded_out(output.get());
      coded_out.WriteRaw(code.data(), code.size());
      return true;
    }
};

int main(int argc, char* argv[]) {
  ContractCSharpGrpcGenerator generator;
  return grpc::protobuf::compiler::PluginMain(argc, argv, &generator);
}
