#ifndef RFL_VALIDATOR_HPP_
#define RFL_VALIDATOR_HPP_

#include <concepts>
#include <optional>
#include <regex>
#include <string>
#include <type_traits>

#include "internal/HasValidation.hpp"
#include "internal/StringLiteral.hpp"
#include "rfl/Result.hpp"

namespace rfl {

template <class T, class V>
requires internal::HasValidation<V, T>
struct Validator {
 public:
  using ReflectionType = T;

  Validator() : value_(V::validate(T()).value()) {}

  Validator(const T& _value) : value_(V::validate(_value).value()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  Validator(const U& _value) : value_(V::validate(T(_value)).value()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  Validator(U&& _value)
      : value_(V::validate(T(std::forward<U>(_value))).value()) {}

  ~Validator() = default;

  /// Exposes the underlying value.
  T& value() { return value_; }

  /// Exposes the underlying value.
  T value() const { return value_; }

  /// Necessary for the serialization to work.
  T reflection() const { return value_; }

 private:
  /// The underlying value.
  T value_;
};

}  // namespace rfl

#endif
