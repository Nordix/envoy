#include "extensions/formatter/regex_substitute/regex_substitute.h"

#include <string>

#include "common/protobuf/utility.h"

namespace Envoy {
namespace Extensions {
namespace Formatter {

void stripQueryString(std::string& path) {
  const size_t query_pos = path.find('?');
  path = std::string(path.data(), query_pos != path.npos ? query_pos : path.size());
}

void truncate(std::string& str, absl::optional<uint32_t> max_length) {
  if (!max_length) {
    return;
  }

  str = str.substr(0, max_length.value());
}

RegexSubstitute::RegexSubstitute(const std::string& main_header,
                                 const std::string& alternative_header,
                                 absl::optional<size_t> max_length,
                                 const RegexSubstituteConfig& config)
    : main_header_(main_header), alternative_header_(alternative_header), max_length_(max_length),
      matcher_(Regex::Utility::parseRegex(config.regex_value_rewrite().pattern())),
      substitution_(config.regex_value_rewrite().substitution()) {}

absl::optional<std::string> RegexSubstitute::format(const Http::RequestHeaderMap& request,
                                                    const Http::ResponseHeaderMap&,
                                                    const Http::ResponseTrailerMap&,
                                                    const StreamInfo::StreamInfo&,
                                                    absl::string_view) const {
  const Http::HeaderEntry* header = findHeader(request);
  if (!header) {
    return absl::nullopt;
  }

  std::string val = std::string(header->value().getStringView());
  val = matcher_->replaceAll(val, substitution_);
  truncate(val, max_length_);

  return val;
}

ProtobufWkt::Value RegexSubstitute::formatValue(const Http::RequestHeaderMap& request,
                                                const Http::ResponseHeaderMap&,
                                                const Http::ResponseTrailerMap&,
                                                const StreamInfo::StreamInfo&,
                                                absl::string_view) const {
  const Http::HeaderEntry* header = findHeader(request);
  if (!header) {
    return ValueUtil::nullValue();
  }

  std::string val = std::string(header->value().getStringView());
  stripQueryString(val);
  truncate(val, max_length_);
  return ValueUtil::stringValue(val);
}

const Http::HeaderEntry* RegexSubstitute::findHeader(const Http::HeaderMap& headers) const {
  const auto header = headers.get(main_header_);

  if (header.empty() && !alternative_header_.get().empty()) {
    const auto alternate_header = headers.get(alternative_header_);
    // TODO(https://github.com/envoyproxy/envoy/issues/13454): Potentially log all header values.
    return alternate_header.empty() ? nullptr : alternate_header[0];
  }

  return header.empty() ? nullptr : header[0];
}

RegexSubstituteCommandParser::RegexSubstituteCommandParser(const RegexSubstituteConfig& config)
    : config_(config) { }

::Envoy::Formatter::FormatterProviderPtr
RegexSubstituteCommandParser::parse(const std::string& token, size_t, size_t) const {
  std::string command_name(config_.command_name() + '(');
  if (absl::StartsWith(token, command_name)) {
    std::string main_header, alternative_header;
    absl::optional<size_t> max_length;

    Envoy::Formatter::SubstitutionFormatParser::parseCommandHeader(
        token, command_name.length(), main_header, alternative_header, max_length);

    return std::make_unique<RegexSubstitute>(main_header, alternative_header, max_length, config_);
  }

  return nullptr;
}

} // namespace Formatter
} // namespace Extensions
} // namespace Envoy
