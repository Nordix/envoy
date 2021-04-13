#include "extensions/formatter/regex_substitute/config.h"

#include "common/protobuf/message_validator_impl.h"
#include "envoy/protobuf/message_validator.h"
#include "envoy/formatter/substitution_formatter.h"

#include "extensions/formatter/regex_substitute/regex_substitute.h"

namespace Envoy {
namespace Extensions {
namespace Formatter {

::Envoy::Formatter::CommandParserPtr
RegexSubstituteFactory::createCommandParserFromProto(const Protobuf::Message& message) {
  const auto config = dynamic_cast<const envoy::extensions::formatter::regex_substitute::v3::RegexSubstituteConfig&>(message);
  return std::make_unique<RegexSubstituteCommandParser>(config);
}

ProtobufTypes::MessagePtr RegexSubstituteFactory::createEmptyConfigProto() {
  return std::make_unique<envoy::extensions::formatter::regex_substitute::v3::RegexSubstituteConfig>();
}

std::string RegexSubstituteFactory::name() const { return "envoy.formatter.regex_substitute"; }

REGISTER_FACTORY(RegexSubstituteFactory, Envoy::Formatter::CommandParserFactory);

} // namespace Formatter
} // namespace Extensions
} // namespace Envoy
