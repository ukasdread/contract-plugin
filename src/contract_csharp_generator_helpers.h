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

#ifndef GRPC_INTERNAL_COMPILER_CONTRACT_CSHARP_GENERATOR_HELPERS_H
#define GRPC_INTERNAL_COMPILER_CONTRACT_CSHARP_GENERATOR_HELPERS_H

#include "config.h"
#include "generator_helpers.h"

namespace grpc_contract_csharp_generator {

inline bool ServicesFilename(const grpc::protobuf::FileDescriptor* file,
                             grpc::string* file_name_or_error) {
  *file_name_or_error =
      grpc_generator::FileNameInUpperCamel(file, false) + ".c.cs";
  return true;
}

// Get leading or trailing comments in a string. Comment lines start with "// ".
// Leading detached comments are put in front of leading comments.
template <typename DescriptorType>
inline grpc::string GetCsharpComments(const DescriptorType* desc,
                                      bool leading) {
  return grpc_generator::GetPrefixedComments(desc, leading, "//");
}

}  // namespace grpc_contract_csharp_generator

#endif  // GRPC_INTERNAL_COMPILER_CSHARP_GENERATOR_HELPERS_H
