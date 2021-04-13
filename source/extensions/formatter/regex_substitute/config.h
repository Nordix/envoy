#pragma once

#include "common/formatter/substitution_formatter.h"

namespace Envoy {
namespace Extensions {
namespace Formatter {

class RegexSubstituteFactory : public ::Envoy::Formatter::CommandParserFactory {
public:
  ::Envoy::Formatter::CommandParserPtr
  createCommandParserFromProto(const Protobuf::Message& message) override;
  ProtobufTypes::MessagePtr createEmptyConfigProto() override;
  std::string name() const override;
};

} // namespace Formatter
} // namespace Extensions
} // namespace Envoy
