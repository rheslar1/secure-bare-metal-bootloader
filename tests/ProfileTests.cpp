#include <array>
#include <cassert>
#include <string_view>

class IReadinessRule {
 public:
  virtual ~IReadinessRule() = default;
  virtual bool passes(std::string_view evidenceTarget) const = 0;
};

class RequiredEvidenceRule final : public IReadinessRule {
 public:
  bool passes(std::string_view evidenceTarget) const override {
    return !evidenceTarget.empty();
  }
};

struct ProjectProfile {
  std::string_view title;
  std::string_view summary;
  std::string_view evidenceTarget;
  std::array<std::string_view, 9> tags;
};

constexpr ProjectProfile profile{
  "Secure Bare-Metal Bootloader",
  "Custom bootloader with flash memory partitioning, rollback-safe image slots, cryptographic signature verification, and update diagnostics.",
  "Deep hardware/software integration, secure boot fundamentals, memory maps, and recoverable field updates.",
  {
    "C++17",
    "C++ Design Patterns",
    "SOLID",
    "Bare metal C",
    "Linker scripts",
    "Flash layout",
    "ECDSA",
    "SHA-256",
    "UART/USB DFU"
  }
};

int main() {
  const RequiredEvidenceRule rule;
  assert(!profile.title.empty());
  assert(!profile.summary.empty());
  assert(rule.passes(profile.evidenceTarget));
  assert(profile.tags[0] == "C++17");
  return 0;
}
