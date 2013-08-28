// Copyright (c) 2012, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#include "vm/bootstrap_natives.h"

#include "vm/bigint_operations.h"
#include "vm/dart_entry.h"
#include "vm/exceptions.h"
#include "vm/native_entry.h"
#include "vm/object.h"
#include "vm/object_store.h"
#include "vm/symbols.h"

namespace dart {

DEFINE_FLAG(bool, trace_intrinsified_natives, false,
    "Report if any of the intrinsified natives are called");

// Smi natives.

// Returns false if integer is in wrong representation, e.g., as is a Bigint
// when it could have been a Smi.
static bool CheckInteger(const Integer& i) {
  if (i.IsBigint()) {
    const Bigint& bigint = Bigint::Cast(i);
    return !BigintOperations::FitsIntoSmi(bigint) &&
        !BigintOperations::FitsIntoInt64(bigint);
  }
  if (i.IsMint()) {
    const Mint& mint = Mint::Cast(i);
    return !Smi::IsValid64(mint.value());
  }
  return true;
}


DEFINE_NATIVE_ENTRY(Integer_bitAndFromInteger, 2) {
  const Integer& right = Integer::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, left, arguments->NativeArgAt(1));
  ASSERT(CheckInteger(right));
  ASSERT(CheckInteger(left));
  if (FLAG_trace_intrinsified_natives) {
    OS::Print("Integer_bitAndFromInteger %s & %s\n",
        right.ToCString(), left.ToCString());
  }
  const Integer& result =
      Integer::Handle(left.BitOp(Token::kBIT_AND, right));
  return result.AsValidInteger();
}


DEFINE_NATIVE_ENTRY(Integer_bitOrFromInteger, 2) {
  const Integer& right = Integer::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, left, arguments->NativeArgAt(1));
  ASSERT(CheckInteger(right));
  ASSERT(CheckInteger(left));
  if (FLAG_trace_intrinsified_natives) {
    OS::Print("Integer_bitOrFromInteger %s | %s\n",
        left.ToCString(), right.ToCString());
  }
  const Integer& result =
      Integer::Handle(left.BitOp(Token::kBIT_OR, right));
  return result.AsValidInteger();
}


DEFINE_NATIVE_ENTRY(Integer_bitXorFromInteger, 2) {
  const Integer& right = Integer::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, left, arguments->NativeArgAt(1));
  ASSERT(CheckInteger(right));
  ASSERT(CheckInteger(left));
  if (FLAG_trace_intrinsified_natives) {
    OS::Print("Integer_bitXorFromInteger %s ^ %s\n",
        left.ToCString(), right.ToCString());
  }
  const Integer& result =
      Integer::Handle(left.BitOp(Token::kBIT_XOR, right));
  return result.AsValidInteger();
}


DEFINE_NATIVE_ENTRY(Integer_addFromInteger, 2) {
  const Integer& right_int = Integer::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, left_int, arguments->NativeArgAt(1));
  ASSERT(CheckInteger(right_int));
  ASSERT(CheckInteger(left_int));
  if (FLAG_trace_intrinsified_natives) {
    OS::Print("Integer_addFromInteger %s + %s\n",
        left_int.ToCString(), right_int.ToCString());
  }
  const Integer& result =
      Integer::Handle(left_int.ArithmeticOp(Token::kADD, right_int));
  return result.AsValidInteger();
}


DEFINE_NATIVE_ENTRY(Integer_subFromInteger, 2) {
  const Integer& right_int = Integer::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, left_int, arguments->NativeArgAt(1));
  ASSERT(CheckInteger(right_int));
  ASSERT(CheckInteger(left_int));
  if (FLAG_trace_intrinsified_natives) {
    OS::Print("Integer_subFromInteger %s - %s\n",
        left_int.ToCString(), right_int.ToCString());
  }
  const Integer& result =
      Integer::Handle(left_int.ArithmeticOp(Token::kSUB, right_int));
  return result.AsValidInteger();
}


DEFINE_NATIVE_ENTRY(Integer_mulFromInteger, 2) {
  const Integer& right_int = Integer::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, left_int, arguments->NativeArgAt(1));
  ASSERT(CheckInteger(right_int));
  ASSERT(CheckInteger(left_int));
  if (FLAG_trace_intrinsified_natives) {
    OS::Print("Integer_mulFromInteger %s * %s\n",
        left_int.ToCString(), right_int.ToCString());
  }
  const Integer& result =
      Integer::Handle(left_int.ArithmeticOp(Token::kMUL, right_int));
  return result.AsValidInteger();
}


DEFINE_NATIVE_ENTRY(Integer_truncDivFromInteger, 2) {
  const Integer& right_int = Integer::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, left_int, arguments->NativeArgAt(1));
  ASSERT(CheckInteger(right_int));
  ASSERT(CheckInteger(left_int));
  ASSERT(!right_int.IsZero());
  const Integer& result =
      Integer::Handle(left_int.ArithmeticOp(Token::kTRUNCDIV, right_int));
  return result.AsValidInteger();
}


DEFINE_NATIVE_ENTRY(Integer_moduloFromInteger, 2) {
  const Integer& right_int = Integer::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, left_int, arguments->NativeArgAt(1));
  ASSERT(CheckInteger(right_int));
  ASSERT(CheckInteger(right_int));
  if (FLAG_trace_intrinsified_natives) {
    OS::Print("Integer_moduloFromInteger %s mod %s\n",
        left_int.ToCString(), right_int.ToCString());
  }
  if (right_int.IsZero()) {
    // Should have been caught before calling into runtime.
    UNIMPLEMENTED();
  }
  const Integer& result =
      Integer::Handle(left_int.ArithmeticOp(Token::kMOD, right_int));
  return result.AsValidInteger();
}


DEFINE_NATIVE_ENTRY(Integer_greaterThanFromInteger, 2) {
  const Integer& right = Integer::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, left, arguments->NativeArgAt(1));
  ASSERT(CheckInteger(right));
  ASSERT(CheckInteger(left));
  if (FLAG_trace_intrinsified_natives) {
    OS::Print("Integer_greaterThanFromInteger %s > %s\n",
        left.ToCString(), right.ToCString());
  }
  return Bool::Get(left.CompareWith(right) == 1).raw();
}


DEFINE_NATIVE_ENTRY(Integer_equalToInteger, 2) {
  const Integer& left = Integer::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, right, arguments->NativeArgAt(1));
  ASSERT(CheckInteger(left));
  ASSERT(CheckInteger(right));
  if (FLAG_trace_intrinsified_natives) {
    OS::Print("Integer_equalToInteger %s == %s\n",
        left.ToCString(), right.ToCString());
  }
  return Bool::Get(left.CompareWith(right) == 0).raw();
}


DEFINE_NATIVE_ENTRY(Integer_parse, 1) {
  GET_NON_NULL_NATIVE_ARGUMENT(String, value, arguments->NativeArgAt(0));
  if (value.IsOneByteString()) {
    // Quick conversion for unpadded integers in strings.
    const intptr_t len = value.Length();
    if (len > 0) {
      const char* cstr = value.ToCString();
      ASSERT(cstr != NULL);
      char* p_end = NULL;
      const int64_t int_value = strtoll(cstr, &p_end, 10);
      if (p_end == (cstr + len)) {
        if ((int_value != LLONG_MIN) && (int_value != LLONG_MAX)) {
          return Integer::New(int_value);
        }
      }
    }
  }

  Scanner scanner(value, Symbols::Empty());
  const Scanner::GrowableTokenStream& tokens = scanner.GetStream();
  String* int_string;
  bool is_positive;
  if (Scanner::IsValidLiteral(tokens,
                             Token::kINTEGER,
                             &is_positive,
                             &int_string)) {
    if (is_positive) {
      return Integer::New(*int_string);
    }
    String& temp = String::Handle();
    temp = String::Concat(Symbols::Dash(), *int_string);
    return Integer::New(temp);
  }

  const Array& args = Array::Handle(Array::New(1));
  args.SetAt(0, value);
  Exceptions::ThrowByType(Exceptions::kFormat, args);
  return Object::null();
}


// Passing true for 'silent' prevents throwing JavascriptIntegerOverflow.
static RawInteger* ShiftOperationHelper(Token::Kind kind,
                                        const Integer& value,
                                        const Smi& amount,
                                        const bool silent = false) {
  if (amount.Value() < 0) {
    const Array& args = Array::Handle(Array::New(1));
    args.SetAt(0, amount);
    Exceptions::ThrowByType(Exceptions::kArgument, args);
  }
  if (value.IsSmi()) {
    const Smi& smi_value = Smi::Cast(value);
    return smi_value.ShiftOp(kind, amount, silent);
  }
  Bigint& big_value = Bigint::Handle();
  if (value.IsMint()) {
    const int64_t mint_value = value.AsInt64Value();
    const int count = Utils::HighestBit(mint_value);
    if ((count + amount.Value()) < Mint::kBits) {
      switch (kind) {
        case Token::kSHL:
          return Integer::New(mint_value << amount.Value(), Heap::kNew, silent);
        case Token::kSHR:
          return Integer::New(mint_value >> amount.Value(), Heap::kNew, silent);
        default:
          UNIMPLEMENTED();
      }
    } else {
      // Overflow in shift, use Bigints
      big_value = BigintOperations::NewFromInt64(mint_value);
    }
  } else {
    ASSERT(value.IsBigint());
    big_value = Bigint::Cast(value).raw();
  }
  switch (kind) {
    case Token::kSHL:
      return BigintOperations::ShiftLeft(big_value, amount.Value());
    case Token::kSHR:
      return BigintOperations::ShiftRight(big_value, amount.Value());
    default:
      UNIMPLEMENTED();
  }
  return Integer::null();
}


DEFINE_NATIVE_ENTRY(Integer_leftShiftWithMask32, 3) {
  const Integer& value = Integer::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, shift_count, arguments->NativeArgAt(1));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, mask, arguments->NativeArgAt(2));
  ASSERT(CheckInteger(value));
  ASSERT(CheckInteger(shift_count));
  ASSERT(CheckInteger(mask));
  if (!shift_count.IsSmi()) {
    // Shift count is too large..
    const Instance& exception =
        Instance::Handle(isolate->object_store()->out_of_memory());
    Exceptions::Throw(exception);
  }
  const Smi& smi_shift_count = Smi::Cast(shift_count);
  const Integer& shift_result = Integer::Handle(
      ShiftOperationHelper(Token::kSHL, value, smi_shift_count, true));
  const Integer& result =
      Integer::Handle(shift_result.BitOp(Token::kBIT_AND, mask));
  return result.AsValidInteger();
}


DEFINE_NATIVE_ENTRY(Smi_shrFromInt, 2) {
  const Smi& amount = Smi::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, value, arguments->NativeArgAt(1));
  ASSERT(CheckInteger(amount));
  ASSERT(CheckInteger(value));
  const Integer& result = Integer::Handle(
      ShiftOperationHelper(Token::kSHR, value, amount));
  return result.AsValidInteger();
}



DEFINE_NATIVE_ENTRY(Smi_shlFromInt, 2) {
  const Smi& amount = Smi::CheckedHandle(arguments->NativeArgAt(0));
  GET_NON_NULL_NATIVE_ARGUMENT(Integer, value, arguments->NativeArgAt(1));
  ASSERT(CheckInteger(amount));
  ASSERT(CheckInteger(value));
  if (FLAG_trace_intrinsified_natives) {
    OS::Print("Smi_shlFromInt: %s << %s\n",
        value.ToCString(), amount.ToCString());
  }
  const Integer& result = Integer::Handle(
      ShiftOperationHelper(Token::kSHL, value, amount));
  return result.AsValidInteger();
}


DEFINE_NATIVE_ENTRY(Smi_bitNegate, 1) {
  const Smi& operand = Smi::CheckedHandle(arguments->NativeArgAt(0));
  if (FLAG_trace_intrinsified_natives) {
    OS::Print("Smi_bitNegate: %s\n", operand.ToCString());
  }
  intptr_t result = ~operand.Value();
  ASSERT(Smi::IsValid(result));
  return Smi::New(result);
}


// Mint natives.

DEFINE_NATIVE_ENTRY(Mint_bitNegate, 1) {
  const Mint& operand = Mint::CheckedHandle(arguments->NativeArgAt(0));
  ASSERT(CheckInteger(operand));
  if (FLAG_trace_intrinsified_natives) {
    OS::Print("Mint_bitNegate: %s\n", operand.ToCString());
  }
  int64_t result = ~operand.value();
  return Integer::New(result);
}


DEFINE_NATIVE_ENTRY(Mint_shlFromInt, 2) {
  // Use the preallocated out of memory exception to avoid calling
  // into dart code or allocating any code.
  const Instance& exception =
      Instance::Handle(isolate->object_store()->out_of_memory());
  Exceptions::Throw(exception);
  UNREACHABLE();
  return 0;
}


// Bigint natives.

DEFINE_NATIVE_ENTRY(Bigint_bitNegate, 1) {
  const Bigint& value = Bigint::CheckedHandle(arguments->NativeArgAt(0));
  const Bigint& result = Bigint::Handle(BigintOperations::BitNot(value));
  ASSERT(CheckInteger(value));
  ASSERT(CheckInteger(result));
  return result.AsValidInteger();
}


DEFINE_NATIVE_ENTRY(Bigint_shlFromInt, 2) {
  // Use the preallocated out of memory exception to avoid calling
  // into dart code or allocating any code.
  const Instance& exception =
      Instance::Handle(isolate->object_store()->out_of_memory());
  Exceptions::Throw(exception);
  UNREACHABLE();
  return 0;
}

}  // namespace dart
