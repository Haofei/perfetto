/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <limits>
#include <memory>
#include <vector>

#include "perfetto/protozero/message_handle.h"
#include "perfetto/protozero/packed_repeated_fields.h"
#include "test/gtest_and_gmock.h"

// Autogenerated headers in out/*/gen/
#include "src/protozero/test/example_proto/library.gen.h"
#include "src/protozero/test/example_proto/other_package/test_messages.gen.h"
#include "src/protozero/test/example_proto/subpackage/test_messages.gen.h"
#include "src/protozero/test/example_proto/test_messages.gen.h"
#include "src/protozero/test/example_proto/test_messages.pb.h"

// Generated by the cppgen compiler.
namespace pbtest = protozero::test::protos::gen;
namespace pbtest_subpackage = protozero::test::protos::subpackage::gen;
namespace pbtest_otherpackage = other_package::gen;

// Generated by the official protobuf compiler.
namespace pbgold = protozero::test::protos;
namespace pbgold_subpackage = protozero::test::protos::subpackage;
namespace pbgold_other_package = other_package;

namespace protozero {
namespace {

using testing::ElementsAreArray;

// The two functions below are templated because they are reused both on the
// libprotobuf and protozero versions to check both libproto -> zero and
// zero -> libproto compatibility.
template <typename T>
void SetTestingFields(T* msg) {
  msg->set_field_int32(-1);
  msg->set_field_int64(-333123456789ll);
  msg->set_field_uint32(600);
  msg->set_field_uint64(333123456789ll);
  msg->set_field_sint32(-5);
  msg->set_field_sint64(-9000);
  msg->set_field_fixed32(12345);
  msg->set_field_fixed64(444123450000ll);
  msg->set_field_sfixed32(-69999);
  msg->set_field_sfixed64(-200);
  msg->set_field_float(3.14f);
  msg->set_field_double(0.5555);
  msg->set_field_bool(true);

  msg->set_small_enum(decltype(msg->small_enum())::TO_BE);
  msg->set_signed_enum(decltype(msg->signed_enum())::NEGATIVE);
  msg->set_big_enum(decltype(msg->big_enum())::BEGIN);
  msg->set_very_negative_enum(decltype(msg->very_negative_enum())::VAL);

  msg->set_field_string("FizzBuzz");
  msg->set_field_bytes(reinterpret_cast<const uint8_t*>("\x11\x00\xBE\xEF"),
                       4u);
  msg->add_repeated_int32(1);
  msg->add_repeated_int32(-1);
  msg->add_repeated_int32(100);
  msg->add_repeated_int32(2000000);
}

template <typename T>
void CheckTestingFields(const T& msg) {
  EXPECT_EQ(-1, msg.field_int32());
  EXPECT_EQ(-333123456789ll, msg.field_int64());
  EXPECT_EQ(600u, msg.field_uint32());
  EXPECT_EQ(333123456789ull, msg.field_uint64());
  EXPECT_EQ(-5, msg.field_sint32());
  EXPECT_EQ(-9000, msg.field_sint64());
  EXPECT_EQ(12345u, msg.field_fixed32());
  EXPECT_EQ(444123450000ull, msg.field_fixed64());
  EXPECT_EQ(-69999, msg.field_sfixed32());
  EXPECT_EQ(-200, msg.field_sfixed64());
  EXPECT_FLOAT_EQ(3.14f, msg.field_float());
  EXPECT_DOUBLE_EQ(0.5555, msg.field_double());
  EXPECT_EQ(true, msg.field_bool());
  EXPECT_EQ(decltype(msg.small_enum())::TO_BE, msg.small_enum());
  EXPECT_EQ(decltype(msg.signed_enum())::NEGATIVE, msg.signed_enum());
  EXPECT_EQ(decltype(msg.big_enum())::BEGIN, msg.big_enum());
  EXPECT_EQ(decltype(msg.very_negative_enum())::VAL, msg.very_negative_enum());
  EXPECT_EQ("FizzBuzz", msg.field_string());
  EXPECT_EQ(std::string("\x11\x00\xBE\xEF", 4), msg.field_bytes());
  ASSERT_THAT(msg.repeated_int32(), ElementsAreArray({1, -1, 100, 2000000}));
}

TEST(ProtoCppConformanceTest, GenEncode_GoldDecode) {
  pbtest::EveryField msg;
  SetTestingFields(&msg);
  std::string serialized = msg.SerializeAsString();

  pbgold::EveryField gold_msg;
  gold_msg.ParseFromString(serialized);
  CheckTestingFields(gold_msg);
  EXPECT_EQ(serialized.size(), static_cast<size_t>(gold_msg.ByteSizeLong()));
}

TEST(ProtoCppConformanceTest, GoldEncode_GenDecode) {
  pbgold::EveryField gold_msg;
  SetTestingFields(&gold_msg);
  std::string serialized = gold_msg.SerializeAsString();

  pbtest::EveryField msg;
  EXPECT_TRUE(msg.ParseFromString(serialized));
  CheckTestingFields(msg);
}

TEST(ProtoCppConformanceTest, GenEncode_GenDecode) {
  pbtest::EveryField msg;
  SetTestingFields(&msg);
  std::string serialized = msg.SerializeAsString();

  pbtest::EveryField dec_msg;
  dec_msg.ParseFromString(serialized);
  CheckTestingFields(dec_msg);
  EXPECT_EQ(serialized.size(), dec_msg.SerializeAsString().size());
}

TEST(ProtoCppConformanceTest, NestedMessages) {
  pbtest::NestedA msg_a;
  pbtest::NestedA::NestedB* msg_b = msg_a.add_repeated_a();
  pbtest::NestedA::NestedB::NestedC* msg_c = msg_b->mutable_value_b();
  msg_c->set_value_c(321);
  msg_b = msg_a.add_repeated_a();
  msg_a.mutable_super_nested()->set_value_c(1000);
  std::string serialized = msg_a.SerializeAsString();

  pbgold::NestedA gold_msg_a;
  gold_msg_a.ParseFromString(serialized);
  EXPECT_EQ(2, gold_msg_a.repeated_a_size());
  EXPECT_EQ(321, gold_msg_a.repeated_a(0).value_b().value_c());
  EXPECT_FALSE(gold_msg_a.repeated_a(1).has_value_b());
  EXPECT_EQ(gold_msg_a.repeated_a(1).value_b().value_c(), 0);
  EXPECT_EQ(1000, gold_msg_a.super_nested().value_c());
}

// Tests that unknown fields are preserved when re-serializing. This test uses
// the messages NestedA and NestedA_V2, where NestedA_V2 mimics a future
// extension of NestedA. It starts filling the V2 version, than decodes it with
// V1, then re-encodes V1 and decodes with V2 and finally check that all the V2
// original fields have been preserved.
TEST(ProtoCppConformanceTest, PreserveUnknownFields) {
  std::string serialized;
  {
    pbtest::TestVersioning_V2 msg_v2;
    msg_v2.set_root_int(10);
    msg_v2.set_root_int_v2(11);
    msg_v2.add_enumz(pbtest::TestVersioning_V2::ONE);
    msg_v2.add_enumz(pbtest::TestVersioning_V2::TWO);
    msg_v2.add_enumz(pbtest::TestVersioning_V2::THREE_V2);
    msg_v2.set_root_string("root-string");
    msg_v2.add_rep_string("root-rep-string-1");
    msg_v2.add_rep_string("root-rep-string-2");
    for (int i = 0; i < 3; i++) {
      auto* sub1 = i == 0 ? msg_v2.mutable_sub1() : msg_v2.add_sub1_rep();
      sub1->set_sub1_int(12);
      sub1->set_sub1_string("sub1-string");
      sub1->set_sub1_int_v2(13);
      sub1->set_sub1_string_v2("sub1-string-v2");

      auto* sub2 = i == 0 ? msg_v2.mutable_sub2() : msg_v2.add_sub2_rep();
      sub2->set_sub2_int(14);
      sub2->set_sub2_string("sub2-string");
    }
    {
      pbtest::TestVersioning_V2::Sub1_V2 lazy;
      lazy.set_sub1_int(15);
      lazy.set_sub1_string("sub1-lazy-string");
      lazy.set_sub1_int_v2(16);
      lazy.set_sub1_string_v2("sub1-lazy-string-v2");
      msg_v2.set_sub1_lazy_raw(lazy.SerializeAsString());
    }
    {
      pbtest::TestVersioning_V2::Sub2_V2 lazy;
      lazy.set_sub2_int(17);
      lazy.set_sub2_string("sub2-v2-lazy-string");
      msg_v2.set_sub2_lazy_raw(lazy.SerializeAsString());
    }

    serialized = msg_v2.SerializeAsString();
  }

  // Now decode with an earlier version that has less fields. Then re-serialize
  // the v1 message.
  {
    pbtest::TestVersioning_V1 msg_v1;
    EXPECT_TRUE(msg_v1.ParseFromString(serialized));
    EXPECT_EQ(msg_v1.root_int(), 10);
    EXPECT_EQ(msg_v1.enumz_size(), 3);
    // These are to workaround "ODR-usage" rules that would require that the
    // constexpr in the .gen.h file have a matching symbol definition in the .cc
    // file.
    const auto ONE = pbtest::TestVersioning_V1::ONE;
    const auto TWO = pbtest::TestVersioning_V1::TWO;
    EXPECT_EQ(msg_v1.enumz()[0], ONE);
    EXPECT_EQ(msg_v1.enumz()[1], TWO);
    EXPECT_EQ(static_cast<int>(msg_v1.enumz()[2]), 3);
    EXPECT_EQ(msg_v1.root_string(), "root-string");
    EXPECT_EQ(msg_v1.rep_string_size(), 2);
    EXPECT_EQ(msg_v1.rep_string()[0], "root-rep-string-1");
    EXPECT_EQ(msg_v1.rep_string()[1], "root-rep-string-2");
    EXPECT_EQ(msg_v1.sub1_rep_size(), 2);
    for (size_t i = 0; i < 3; i++) {
      auto* sub1 = i == 0 ? &msg_v1.sub1() : &msg_v1.sub1_rep()[i - 1];
      EXPECT_EQ(sub1->sub1_int(), 12);
      EXPECT_EQ(sub1->sub1_string(), "sub1-string");
    }

    // Append an extra field and change the valaue of an existing one before
    // re-serializing.
    msg_v1.set_root_int(101);
    msg_v1.add_rep_string("extra-string");
    serialized = msg_v1.SerializeAsString();
  }

  // Decode the re-serialized v1 message with with v2. Check that the _v2
  // fields have been preserved in the double de/serializataion across versions.
  {
    pbtest::TestVersioning_V2 msg_v2;
    EXPECT_TRUE(msg_v2.ParseFromString(serialized));
    EXPECT_EQ(msg_v2.root_int(), 101);
    EXPECT_EQ(msg_v2.root_int_v2(), 11);
    EXPECT_EQ(msg_v2.enumz_size(), 3);
    const auto ONE = pbtest::TestVersioning_V2::ONE;
    const auto TWO = pbtest::TestVersioning_V2::TWO;
    const auto THREE_V2 = pbtest::TestVersioning_V2::THREE_V2;
    EXPECT_EQ(msg_v2.enumz()[0], ONE);
    EXPECT_EQ(msg_v2.enumz()[1], TWO);
    EXPECT_EQ(msg_v2.enumz()[2], THREE_V2);
    EXPECT_EQ(msg_v2.root_string(), "root-string");
    EXPECT_EQ(msg_v2.rep_string_size(), 3);
    EXPECT_EQ(msg_v2.rep_string()[0], "root-rep-string-1");
    EXPECT_EQ(msg_v2.rep_string()[1], "root-rep-string-2");
    EXPECT_EQ(msg_v2.rep_string()[2], "extra-string");
    EXPECT_EQ(msg_v2.sub1_rep_size(), 2);
    for (size_t i = 0; i < 3; i++) {
      auto* sub1 = i == 0 ? &msg_v2.sub1() : &msg_v2.sub1_rep()[i - 1];
      EXPECT_EQ(sub1->sub1_int(), 12);
      EXPECT_EQ(sub1->sub1_string(), "sub1-string");
      EXPECT_EQ(sub1->sub1_int_v2(), 13);
      EXPECT_EQ(sub1->sub1_string_v2(), "sub1-string-v2");
      auto* sub2 = i == 0 ? &msg_v2.sub2() : &msg_v2.sub2_rep()[i - 1];
      EXPECT_EQ(sub2->sub2_int(), 14);
      EXPECT_EQ(sub2->sub2_string(), "sub2-string");
    }
    {
      pbtest::TestVersioning_V2::Sub1_V2 lazy;
      EXPECT_TRUE(lazy.ParseFromString(msg_v2.sub1_lazy_raw()));
      EXPECT_EQ(lazy.sub1_int(), 15);
      EXPECT_EQ(lazy.sub1_string(), "sub1-lazy-string");
      EXPECT_EQ(lazy.sub1_int_v2(), 16);
      EXPECT_EQ(lazy.sub1_string_v2(), "sub1-lazy-string-v2");
    }
    {
      pbtest::TestVersioning_V2::Sub2_V2 lazy;
      EXPECT_TRUE(lazy.ParseFromString(msg_v2.sub2_lazy_raw()));
      EXPECT_EQ(lazy.sub2_int(), 17);
      EXPECT_EQ(lazy.sub2_string(), "sub2-v2-lazy-string");
    }
  }
}

// Checks that unset fields aren't zero-initialized in the decode -> re-encode
// process.
TEST(ProtoCppConformanceTest, DontDefaultInitialize) {
  pbtest::EveryField msg;
  EXPECT_FALSE(msg.has_field_int32());
  msg.set_field_int32(0);
  EXPECT_TRUE(msg.has_field_int32());

  EXPECT_FALSE(msg.has_field_float());
  msg.set_field_float(.0f);
  EXPECT_TRUE(msg.has_field_float());

  EXPECT_FALSE(msg.has_field_string());
  msg.set_field_string("");
  EXPECT_TRUE(msg.has_field_string());

  pbtest::EveryField dec;
  dec.ParseFromString(msg.SerializeAsString());
  std::string reserialized = dec.SerializeAsString();

  pbgold::EveryField gold_msg;
  gold_msg.ParseFromString(reserialized);
  EXPECT_TRUE(gold_msg.has_field_int32());
  EXPECT_EQ(gold_msg.field_int32(), 0);
  EXPECT_TRUE(gold_msg.has_field_float());
  EXPECT_FLOAT_EQ(gold_msg.field_float(), .0f);
  EXPECT_TRUE(gold_msg.has_field_string());
  EXPECT_EQ(gold_msg.field_string(), "");
  EXPECT_FALSE(gold_msg.has_field_int64());
  EXPECT_FALSE(gold_msg.has_field_uint64());
  EXPECT_FALSE(gold_msg.has_field_bytes());
}

// Tests serialization and deserialization of packed encoding fields.
TEST(ProtoCppConformanceTest, PackedRepeatedFields) {
  std::string serialized;
  {
    pbtest::PackedRepeatedFields msg;

    for (int i = -100; i < 100; i++)
      msg.add_field_int32(i);

    for (uint32_t i = 100; i < 200; i++)
      msg.add_field_fixed32(i);

    for (int64_t i = 5000000000; i < 5000001000; i++)
      msg.add_field_sfixed64(i);

    serialized = msg.SerializeAsString();
  }
  {
    pbtest::PackedRepeatedFields msg;
    EXPECT_TRUE(msg.ParseFromString(serialized));

    std::vector<int> exp_int32;
    for (int i = -100; i < 100; i++)
      exp_int32.emplace_back(i);
    ASSERT_THAT(msg.field_int32(), ElementsAreArray(exp_int32));

    std::vector<uint32_t> exp_fixed32;
    for (uint32_t i = 100; i < 200; i++)
      exp_fixed32.emplace_back(i);
    ASSERT_THAT(msg.field_fixed32(), ElementsAreArray(exp_fixed32));

    std::vector<int64_t> exp_sfixed64;
    for (int64_t i = 5000000000; i < 5000001000; i++)
      exp_sfixed64.emplace_back(i);
    ASSERT_THAT(msg.field_sfixed64(), ElementsAreArray(exp_sfixed64));
  }
}

TEST(ProtoCppConformanceTest, DifferentPackages) {
  pbtest::DifferentPackages msg;

  // Populate fields defined in "protozero.test.protos.subpackage"
  pbtest_subpackage::Message* msgSubpackage = msg.mutable_subpackage_message();
  msgSubpackage->set_field_int32(1);
  msgSubpackage->set_field_enum(pbtest_subpackage::Enum::A);
  msgSubpackage->set_field_nested_enum(pbtest_subpackage::Message_NestedEnum_C);
  msg.mutable_subpackage_nested_message()->set_field_int32(2);
  msg.set_subpackage_enum(pbtest_subpackage::Enum::B);
  msg.set_subpackage_nested_enum(pbtest_subpackage::Message_NestedEnum_D);

  // Populate fields defined in "other_package"
  pbtest_otherpackage::Message* msgOtherPackage =
      msg.mutable_otherpackage_message();
  msgOtherPackage->set_field_int32(11);
  msgOtherPackage->set_field_enum(pbtest_otherpackage::Enum::A);
  msgOtherPackage->set_field_nested_enum(
      pbtest_otherpackage::Message_NestedEnum_C);
  msg.mutable_otherpackage_nested_message()->set_field_int32(12);
  msg.set_otherpackage_enum(pbtest_otherpackage::Enum::B);
  msg.set_otherpackage_nested_enum(pbtest_otherpackage::Message_NestedEnum_D);

  // Deserialize into golden proto
  std::string serialized = msg.SerializeAsString();
  pbgold::DifferentPackages gold_msg;
  gold_msg.ParseFromString(serialized);
  EXPECT_EQ(serialized.size(), static_cast<size_t>(gold_msg.ByteSizeLong()));

  // Check fields defined in "protozero.test.protos.subpackage"
  EXPECT_EQ(1, gold_msg.subpackage_message().field_int32());
  EXPECT_EQ(pbgold_subpackage::Enum::A,
            gold_msg.subpackage_message().field_enum());
  EXPECT_EQ(pbgold_subpackage::Message_NestedEnum_C,
            gold_msg.subpackage_message().field_nested_enum());
  EXPECT_EQ(2, gold_msg.subpackage_nested_message().field_int32());
  EXPECT_EQ(pbgold_subpackage::Enum::B, gold_msg.subpackage_enum());
  EXPECT_EQ(pbgold_subpackage::Message_NestedEnum_D,
            gold_msg.subpackage_nested_enum());

  // Check fields defined in "other_package"
  EXPECT_EQ(11, gold_msg.otherpackage_message().field_int32());
  EXPECT_EQ(pbgold_other_package::Enum::A,
            gold_msg.otherpackage_message().field_enum());
  EXPECT_EQ(pbgold_other_package::Message_NestedEnum_C,
            gold_msg.otherpackage_message().field_nested_enum());
  EXPECT_EQ(12, gold_msg.otherpackage_nested_message().field_int32());
  EXPECT_EQ(pbgold_other_package::Enum::B, gold_msg.otherpackage_enum());
  EXPECT_EQ(pbgold_other_package::Message_NestedEnum_D,
            gold_msg.otherpackage_nested_enum());
}
}  // namespace
}  // namespace protozero
