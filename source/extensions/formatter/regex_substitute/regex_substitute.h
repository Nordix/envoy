#pragma once

#include <string>

#include "envoy/config/typed_config.h"
#include "envoy/registry/registry.h"
#include "common/common/regex.h"

#include "envoy/extensions/formatter/regex_substitute/v3/regex_substitute.pb.h"
#include "envoy/extensions/formatter/regex_substitute/v3/regex_substitute.pb.validate.h"

#include "common/formatter/substitution_formatter.h"

namespace Envoy {
namespace Extensions {
namespace Formatter {

using envoy::extensions::formatter::regex_substitute::v3::RegexSubstituteConfig;

class RegexSubstitute : public ::Envoy::Formatter::FormatterProvider {
public:
  RegexSubstitute() = delete;
  RegexSubstitute(const std::string& main_header, const std::string& alternative_header,
                  absl::optional<size_t> max_length, const RegexSubstituteConfig& config);

  absl::optional<std::string> format(const Http::RequestHeaderMap&, const Http::ResponseHeaderMap&,
                                     const Http::ResponseTrailerMap&, const StreamInfo::StreamInfo&,
                                     absl::string_view) const override;
  ProtobufWkt::Value formatValue(const Http::RequestHeaderMap&, const Http::ResponseHeaderMap&,
                                 const Http::ResponseTrailerMap&, const StreamInfo::StreamInfo&,
                                 absl::string_view) const override;

private:
  const Http::HeaderEntry* findHeader(const Http::HeaderMap& headers) const;

  Http::LowerCaseString main_header_;
  Http::LowerCaseString alternative_header_;
  absl::optional<size_t> max_length_;
  const Regex::CompiledMatcherPtr matcher_;
  const std::string substitution_;
};

class RegexSubstituteCommandParser : public ::Envoy::Formatter::CommandParser {
public:
  RegexSubstituteCommandParser(const RegexSubstituteConfig&);

  ::Envoy::Formatter::FormatterProviderPtr parse(const std::string& token, size_t,
                                                 size_t) const override;

private:
  const RegexSubstituteConfig config_;
};

} // namespace Formatter
} // namespace Extensions
} // namespace Envoy
