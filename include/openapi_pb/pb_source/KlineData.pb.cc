// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: KlineData.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "KlineData.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace tigeropen {
namespace push {
namespace pb {

namespace {

const ::google::protobuf::Descriptor* KlineData_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  KlineData_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_KlineData_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_KlineData_2eproto() {
  protobuf_AddDesc_KlineData_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "KlineData.proto");
  GOOGLE_CHECK(file != NULL);
  KlineData_descriptor_ = file->message_type(0);
  static const int KlineData_offsets_[11] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(KlineData, time_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(KlineData, open_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(KlineData, high_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(KlineData, low_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(KlineData, close_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(KlineData, avg_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(KlineData, volume_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(KlineData, count_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(KlineData, symbol_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(KlineData, amount_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(KlineData, servertimestamp_),
  };
  KlineData_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      KlineData_descriptor_,
      KlineData::internal_default_instance(),
      KlineData_offsets_,
      -1,
      -1,
      -1,
      sizeof(KlineData),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(KlineData, _internal_metadata_));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_KlineData_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      KlineData_descriptor_, KlineData::internal_default_instance());
}

}  // namespace

void protobuf_ShutdownFile_KlineData_2eproto() {
  KlineData_default_instance_.Shutdown();
  delete KlineData_reflection_;
}

void protobuf_InitDefaults_KlineData_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::internal::GetEmptyString();
  KlineData_default_instance_.DefaultConstruct();
  KlineData_default_instance_.get_mutable()->InitAsDefaultInstance();
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_InitDefaults_KlineData_2eproto_once_);
void protobuf_InitDefaults_KlineData_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_InitDefaults_KlineData_2eproto_once_,
                 &protobuf_InitDefaults_KlineData_2eproto_impl);
}
void protobuf_AddDesc_KlineData_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  protobuf_InitDefaults_KlineData_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\017KlineData.proto\022\021tigeropen.push.pb\"\266\001\n"
    "\tKlineData\022\014\n\004time\030\001 \001(\003\022\014\n\004open\030\002 \001(\002\022\014"
    "\n\004high\030\003 \001(\002\022\013\n\003low\030\004 \001(\002\022\r\n\005close\030\005 \001(\002"
    "\022\013\n\003avg\030\006 \001(\002\022\016\n\006volume\030\007 \001(\003\022\r\n\005count\030\010"
    " \001(\005\022\016\n\006symbol\030\t \001(\t\022\016\n\006amount\030\n \001(\001\022\027\n\017"
    "serverTimestamp\030\013 \001(\004b\006proto3", 229);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "KlineData.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_KlineData_2eproto);
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_KlineData_2eproto_once_);
void protobuf_AddDesc_KlineData_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_KlineData_2eproto_once_,
                 &protobuf_AddDesc_KlineData_2eproto_impl);
}
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_KlineData_2eproto {
  StaticDescriptorInitializer_KlineData_2eproto() {
    protobuf_AddDesc_KlineData_2eproto();
  }
} static_descriptor_initializer_KlineData_2eproto_;

namespace {

static void MergeFromFail(int line) GOOGLE_ATTRIBUTE_COLD GOOGLE_ATTRIBUTE_NORETURN;
static void MergeFromFail(int line) {
  ::google::protobuf::internal::MergeFromFail(__FILE__, line);
}

}  // namespace


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int KlineData::kTimeFieldNumber;
const int KlineData::kOpenFieldNumber;
const int KlineData::kHighFieldNumber;
const int KlineData::kLowFieldNumber;
const int KlineData::kCloseFieldNumber;
const int KlineData::kAvgFieldNumber;
const int KlineData::kVolumeFieldNumber;
const int KlineData::kCountFieldNumber;
const int KlineData::kSymbolFieldNumber;
const int KlineData::kAmountFieldNumber;
const int KlineData::kServerTimestampFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

KlineData::KlineData()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_KlineData_2eproto();
  SharedCtor();
  // @@protoc_insertion_point(constructor:tigeropen.push.pb.KlineData)
}

void KlineData::InitAsDefaultInstance() {
}

KlineData::KlineData(const KlineData& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:tigeropen.push.pb.KlineData)
}

void KlineData::SharedCtor() {
  symbol_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&time_, 0, reinterpret_cast<char*>(&servertimestamp_) -
    reinterpret_cast<char*>(&time_) + sizeof(servertimestamp_));
  _cached_size_ = 0;
}

KlineData::~KlineData() {
  // @@protoc_insertion_point(destructor:tigeropen.push.pb.KlineData)
  SharedDtor();
}

void KlineData::SharedDtor() {
  symbol_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void KlineData::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* KlineData::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return KlineData_descriptor_;
}

const KlineData& KlineData::default_instance() {
  protobuf_InitDefaults_KlineData_2eproto();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<KlineData> KlineData_default_instance_;

KlineData* KlineData::New(::google::protobuf::Arena* arena) const {
  KlineData* n = new KlineData;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void KlineData::Clear() {
// @@protoc_insertion_point(message_clear_start:tigeropen.push.pb.KlineData)
#if defined(__clang__)
#define ZR_HELPER_(f) \
  _Pragma("clang diagnostic push") \
  _Pragma("clang diagnostic ignored \"-Winvalid-offsetof\"") \
  __builtin_offsetof(KlineData, f) \
  _Pragma("clang diagnostic pop")
#else
#define ZR_HELPER_(f) reinterpret_cast<char*>(\
  &reinterpret_cast<KlineData*>(16)->f)
#endif

#define ZR_(first, last) do {\
  ::memset(&(first), 0,\
           ZR_HELPER_(last) - ZR_HELPER_(first) + sizeof(last));\
} while (0)

  ZR_(time_, count_);
  ZR_(amount_, servertimestamp_);
  symbol_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());

#undef ZR_HELPER_
#undef ZR_

}

bool KlineData::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:tigeropen.push.pb.KlineData)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int64 time = 1;
      case 1: {
        if (tag == 8) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &time_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(21)) goto parse_open;
        break;
      }

      // optional float open = 2;
      case 2: {
        if (tag == 21) {
         parse_open:

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &open_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(29)) goto parse_high;
        break;
      }

      // optional float high = 3;
      case 3: {
        if (tag == 29) {
         parse_high:

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &high_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(37)) goto parse_low;
        break;
      }

      // optional float low = 4;
      case 4: {
        if (tag == 37) {
         parse_low:

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &low_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(45)) goto parse_close;
        break;
      }

      // optional float close = 5;
      case 5: {
        if (tag == 45) {
         parse_close:

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &close_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(53)) goto parse_avg;
        break;
      }

      // optional float avg = 6;
      case 6: {
        if (tag == 53) {
         parse_avg:

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &avg_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(56)) goto parse_volume;
        break;
      }

      // optional int64 volume = 7;
      case 7: {
        if (tag == 56) {
         parse_volume:

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &volume_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(64)) goto parse_count;
        break;
      }

      // optional int32 count = 8;
      case 8: {
        if (tag == 64) {
         parse_count:

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &count_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(74)) goto parse_symbol;
        break;
      }

      // optional string symbol = 9;
      case 9: {
        if (tag == 74) {
         parse_symbol:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_symbol()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->symbol().data(), this->symbol().length(),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "tigeropen.push.pb.KlineData.symbol"));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(81)) goto parse_amount;
        break;
      }

      // optional double amount = 10;
      case 10: {
        if (tag == 81) {
         parse_amount:

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &amount_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(88)) goto parse_serverTimestamp;
        break;
      }

      // optional uint64 serverTimestamp = 11;
      case 11: {
        if (tag == 88) {
         parse_serverTimestamp:

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &servertimestamp_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:tigeropen.push.pb.KlineData)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:tigeropen.push.pb.KlineData)
  return false;
#undef DO_
}

void KlineData::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:tigeropen.push.pb.KlineData)
  // optional int64 time = 1;
  if (this->time() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->time(), output);
  }

  // optional float open = 2;
  if (this->open() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(2, this->open(), output);
  }

  // optional float high = 3;
  if (this->high() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(3, this->high(), output);
  }

  // optional float low = 4;
  if (this->low() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(4, this->low(), output);
  }

  // optional float close = 5;
  if (this->close() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(5, this->close(), output);
  }

  // optional float avg = 6;
  if (this->avg() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(6, this->avg(), output);
  }

  // optional int64 volume = 7;
  if (this->volume() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(7, this->volume(), output);
  }

  // optional int32 count = 8;
  if (this->count() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(8, this->count(), output);
  }

  // optional string symbol = 9;
  if (this->symbol().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->symbol().data(), this->symbol().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "tigeropen.push.pb.KlineData.symbol");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      9, this->symbol(), output);
  }

  // optional double amount = 10;
  if (this->amount() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(10, this->amount(), output);
  }

  // optional uint64 serverTimestamp = 11;
  if (this->servertimestamp() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(11, this->servertimestamp(), output);
  }

  // @@protoc_insertion_point(serialize_end:tigeropen.push.pb.KlineData)
}

::google::protobuf::uint8* KlineData::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:tigeropen.push.pb.KlineData)
  // optional int64 time = 1;
  if (this->time() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->time(), target);
  }

  // optional float open = 2;
  if (this->open() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(2, this->open(), target);
  }

  // optional float high = 3;
  if (this->high() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(3, this->high(), target);
  }

  // optional float low = 4;
  if (this->low() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(4, this->low(), target);
  }

  // optional float close = 5;
  if (this->close() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(5, this->close(), target);
  }

  // optional float avg = 6;
  if (this->avg() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(6, this->avg(), target);
  }

  // optional int64 volume = 7;
  if (this->volume() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(7, this->volume(), target);
  }

  // optional int32 count = 8;
  if (this->count() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(8, this->count(), target);
  }

  // optional string symbol = 9;
  if (this->symbol().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->symbol().data(), this->symbol().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "tigeropen.push.pb.KlineData.symbol");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        9, this->symbol(), target);
  }

  // optional double amount = 10;
  if (this->amount() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(10, this->amount(), target);
  }

  // optional uint64 serverTimestamp = 11;
  if (this->servertimestamp() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(11, this->servertimestamp(), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:tigeropen.push.pb.KlineData)
  return target;
}

size_t KlineData::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:tigeropen.push.pb.KlineData)
  size_t total_size = 0;

  // optional int64 time = 1;
  if (this->time() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int64Size(
        this->time());
  }

  // optional float open = 2;
  if (this->open() != 0) {
    total_size += 1 + 4;
  }

  // optional float high = 3;
  if (this->high() != 0) {
    total_size += 1 + 4;
  }

  // optional float low = 4;
  if (this->low() != 0) {
    total_size += 1 + 4;
  }

  // optional float close = 5;
  if (this->close() != 0) {
    total_size += 1 + 4;
  }

  // optional float avg = 6;
  if (this->avg() != 0) {
    total_size += 1 + 4;
  }

  // optional int64 volume = 7;
  if (this->volume() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int64Size(
        this->volume());
  }

  // optional int32 count = 8;
  if (this->count() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->count());
  }

  // optional string symbol = 9;
  if (this->symbol().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->symbol());
  }

  // optional double amount = 10;
  if (this->amount() != 0) {
    total_size += 1 + 8;
  }

  // optional uint64 serverTimestamp = 11;
  if (this->servertimestamp() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt64Size(
        this->servertimestamp());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void KlineData::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:tigeropen.push.pb.KlineData)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const KlineData* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const KlineData>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:tigeropen.push.pb.KlineData)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:tigeropen.push.pb.KlineData)
    UnsafeMergeFrom(*source);
  }
}

void KlineData::MergeFrom(const KlineData& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:tigeropen.push.pb.KlineData)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void KlineData::UnsafeMergeFrom(const KlineData& from) {
  GOOGLE_DCHECK(&from != this);
  if (from.time() != 0) {
    set_time(from.time());
  }
  if (from.open() != 0) {
    set_open(from.open());
  }
  if (from.high() != 0) {
    set_high(from.high());
  }
  if (from.low() != 0) {
    set_low(from.low());
  }
  if (from.close() != 0) {
    set_close(from.close());
  }
  if (from.avg() != 0) {
    set_avg(from.avg());
  }
  if (from.volume() != 0) {
    set_volume(from.volume());
  }
  if (from.count() != 0) {
    set_count(from.count());
  }
  if (from.symbol().size() > 0) {

    symbol_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.symbol_);
  }
  if (from.amount() != 0) {
    set_amount(from.amount());
  }
  if (from.servertimestamp() != 0) {
    set_servertimestamp(from.servertimestamp());
  }
}

void KlineData::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:tigeropen.push.pb.KlineData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void KlineData::CopyFrom(const KlineData& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:tigeropen.push.pb.KlineData)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool KlineData::IsInitialized() const {

  return true;
}

void KlineData::Swap(KlineData* other) {
  if (other == this) return;
  InternalSwap(other);
}
void KlineData::InternalSwap(KlineData* other) {
  std::swap(time_, other->time_);
  std::swap(open_, other->open_);
  std::swap(high_, other->high_);
  std::swap(low_, other->low_);
  std::swap(close_, other->close_);
  std::swap(avg_, other->avg_);
  std::swap(volume_, other->volume_);
  std::swap(count_, other->count_);
  symbol_.Swap(&other->symbol_);
  std::swap(amount_, other->amount_);
  std::swap(servertimestamp_, other->servertimestamp_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata KlineData::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = KlineData_descriptor_;
  metadata.reflection = KlineData_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// KlineData

// optional int64 time = 1;
void KlineData::clear_time() {
  time_ = GOOGLE_LONGLONG(0);
}
::google::protobuf::int64 KlineData::time() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.time)
  return time_;
}
void KlineData::set_time(::google::protobuf::int64 value) {
  
  time_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.time)
}

// optional float open = 2;
void KlineData::clear_open() {
  open_ = 0;
}
float KlineData::open() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.open)
  return open_;
}
void KlineData::set_open(float value) {
  
  open_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.open)
}

// optional float high = 3;
void KlineData::clear_high() {
  high_ = 0;
}
float KlineData::high() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.high)
  return high_;
}
void KlineData::set_high(float value) {
  
  high_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.high)
}

// optional float low = 4;
void KlineData::clear_low() {
  low_ = 0;
}
float KlineData::low() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.low)
  return low_;
}
void KlineData::set_low(float value) {
  
  low_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.low)
}

// optional float close = 5;
void KlineData::clear_close() {
  close_ = 0;
}
float KlineData::close() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.close)
  return close_;
}
void KlineData::set_close(float value) {
  
  close_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.close)
}

// optional float avg = 6;
void KlineData::clear_avg() {
  avg_ = 0;
}
float KlineData::avg() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.avg)
  return avg_;
}
void KlineData::set_avg(float value) {
  
  avg_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.avg)
}

// optional int64 volume = 7;
void KlineData::clear_volume() {
  volume_ = GOOGLE_LONGLONG(0);
}
::google::protobuf::int64 KlineData::volume() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.volume)
  return volume_;
}
void KlineData::set_volume(::google::protobuf::int64 value) {
  
  volume_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.volume)
}

// optional int32 count = 8;
void KlineData::clear_count() {
  count_ = 0;
}
::google::protobuf::int32 KlineData::count() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.count)
  return count_;
}
void KlineData::set_count(::google::protobuf::int32 value) {
  
  count_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.count)
}

// optional string symbol = 9;
void KlineData::clear_symbol() {
  symbol_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
const ::std::string& KlineData::symbol() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.symbol)
  return symbol_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void KlineData::set_symbol(const ::std::string& value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.symbol)
}
void KlineData::set_symbol(const char* value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.KlineData.symbol)
}
void KlineData::set_symbol(const char* value, size_t size) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.KlineData.symbol)
}
::std::string* KlineData::mutable_symbol() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.KlineData.symbol)
  return symbol_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
::std::string* KlineData::release_symbol() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.KlineData.symbol)
  
  return symbol_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void KlineData::set_allocated_symbol(::std::string* symbol) {
  if (symbol != NULL) {
    
  } else {
    
  }
  symbol_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), symbol);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.KlineData.symbol)
}

// optional double amount = 10;
void KlineData::clear_amount() {
  amount_ = 0;
}
double KlineData::amount() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.amount)
  return amount_;
}
void KlineData::set_amount(double value) {
  
  amount_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.amount)
}

// optional uint64 serverTimestamp = 11;
void KlineData::clear_servertimestamp() {
  servertimestamp_ = GOOGLE_ULONGLONG(0);
}
::google::protobuf::uint64 KlineData::servertimestamp() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.serverTimestamp)
  return servertimestamp_;
}
void KlineData::set_servertimestamp(::google::protobuf::uint64 value) {
  
  servertimestamp_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.serverTimestamp)
}

inline const KlineData* KlineData::internal_default_instance() {
  return &KlineData_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace push
}  // namespace tigeropen

// @@protoc_insertion_point(global_scope)