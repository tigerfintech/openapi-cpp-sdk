// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: SocketCommon.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "SocketCommon.pb.h"

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

const ::google::protobuf::Descriptor* SocketCommon_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  SocketCommon_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* SocketCommon_Command_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* SocketCommon_DataType_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* SocketCommon_QuoteType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_SocketCommon_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_SocketCommon_2eproto() {
  protobuf_AddDesc_SocketCommon_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "SocketCommon.proto");
  GOOGLE_CHECK(file != NULL);
  SocketCommon_descriptor_ = file->message_type(0);
  static const int SocketCommon_offsets_[1] = {
  };
  SocketCommon_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      SocketCommon_descriptor_,
      SocketCommon::internal_default_instance(),
      SocketCommon_offsets_,
      -1,
      -1,
      -1,
      sizeof(SocketCommon),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SocketCommon, _internal_metadata_));
  SocketCommon_Command_descriptor_ = SocketCommon_descriptor_->enum_type(0);
  SocketCommon_DataType_descriptor_ = SocketCommon_descriptor_->enum_type(1);
  SocketCommon_QuoteType_descriptor_ = SocketCommon_descriptor_->enum_type(2);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_SocketCommon_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      SocketCommon_descriptor_, SocketCommon::internal_default_instance());
}

}  // namespace

void protobuf_ShutdownFile_SocketCommon_2eproto() {
  SocketCommon_default_instance_.Shutdown();
  delete SocketCommon_reflection_;
}

void protobuf_InitDefaults_SocketCommon_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  SocketCommon_default_instance_.DefaultConstruct();
  SocketCommon_default_instance_.get_mutable()->InitAsDefaultInstance();
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_InitDefaults_SocketCommon_2eproto_once_);
void protobuf_InitDefaults_SocketCommon_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_InitDefaults_SocketCommon_2eproto_once_,
                 &protobuf_InitDefaults_SocketCommon_2eproto_impl);
}
void protobuf_AddDesc_SocketCommon_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  protobuf_InitDefaults_SocketCommon_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022SocketCommon.proto\022\021tigeropen.push.pb\""
    "\240\003\n\014SocketCommon\"\227\001\n\007Command\022\013\n\007UNKNOWN\020"
    "\000\022\013\n\007CONNECT\020\001\022\r\n\tCONNECTED\020\002\022\010\n\004SEND\020\003\022"
    "\r\n\tSUBSCRIBE\020\004\022\017\n\013UNSUBSCRIBE\020\005\022\016\n\nDISCO"
    "NNECT\020\006\022\013\n\007MESSAGE\020\007\022\r\n\tHEARTBEAT\020\010\022\r\n\tE"
    "RRORINFO\020\t\"\301\001\n\010DataType\022\013\n\007Unknown\020\000\022\t\n\005"
    "Quote\020\001\022\n\n\006Option\020\002\022\n\n\006Future\020\003\022\016\n\nQuote"
    "Depth\020\004\022\r\n\tTradeTick\020\005\022\t\n\005Asset\020\006\022\014\n\010Pos"
    "ition\020\007\022\017\n\013OrderStatus\020\010\022\024\n\020OrderTransac"
    "tion\020\t\022\014\n\010StockTop\020\n\022\r\n\tOptionTop\020\013\022\t\n\005K"
    "line\020\014\"2\n\tQuoteType\022\010\n\004None\020\000\022\t\n\005BASIC\020\001"
    "\022\007\n\003BBO\020\002\022\007\n\003ALL\020\003b\006proto3", 466);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "SocketCommon.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_SocketCommon_2eproto);
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_SocketCommon_2eproto_once_);
void protobuf_AddDesc_SocketCommon_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_SocketCommon_2eproto_once_,
                 &protobuf_AddDesc_SocketCommon_2eproto_impl);
}
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_SocketCommon_2eproto {
  StaticDescriptorInitializer_SocketCommon_2eproto() {
    protobuf_AddDesc_SocketCommon_2eproto();
  }
} static_descriptor_initializer_SocketCommon_2eproto_;

namespace {

static void MergeFromFail(int line) GOOGLE_ATTRIBUTE_COLD GOOGLE_ATTRIBUTE_NORETURN;
static void MergeFromFail(int line) {
  ::google::protobuf::internal::MergeFromFail(__FILE__, line);
}

}  // namespace


// ===================================================================

const ::google::protobuf::EnumDescriptor* SocketCommon_Command_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SocketCommon_Command_descriptor_;
}
bool SocketCommon_Command_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      return true;
    default:
      return false;
  }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const SocketCommon_Command SocketCommon::UNKNOWN;
const SocketCommon_Command SocketCommon::CONNECT;
const SocketCommon_Command SocketCommon::CONNECTED;
const SocketCommon_Command SocketCommon::SEND;
const SocketCommon_Command SocketCommon::SUBSCRIBE;
const SocketCommon_Command SocketCommon::UNSUBSCRIBE;
const SocketCommon_Command SocketCommon::DISCONNECT;
const SocketCommon_Command SocketCommon::MESSAGE;
const SocketCommon_Command SocketCommon::HEARTBEAT;
const SocketCommon_Command SocketCommon::ERRORINFO;
const SocketCommon_Command SocketCommon::Command_MIN;
const SocketCommon_Command SocketCommon::Command_MAX;
const int SocketCommon::Command_ARRAYSIZE;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900
const ::google::protobuf::EnumDescriptor* SocketCommon_DataType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SocketCommon_DataType_descriptor_;
}
bool SocketCommon_DataType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
      return true;
    default:
      return false;
  }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const SocketCommon_DataType SocketCommon::Unknown;
const SocketCommon_DataType SocketCommon::Quote;
const SocketCommon_DataType SocketCommon::Option;
const SocketCommon_DataType SocketCommon::Future;
const SocketCommon_DataType SocketCommon::QuoteDepth;
const SocketCommon_DataType SocketCommon::TradeTick;
const SocketCommon_DataType SocketCommon::Asset;
const SocketCommon_DataType SocketCommon::Position;
const SocketCommon_DataType SocketCommon::OrderStatus;
const SocketCommon_DataType SocketCommon::OrderTransaction;
const SocketCommon_DataType SocketCommon::StockTop;
const SocketCommon_DataType SocketCommon::OptionTop;
const SocketCommon_DataType SocketCommon::Kline;
const SocketCommon_DataType SocketCommon::DataType_MIN;
const SocketCommon_DataType SocketCommon::DataType_MAX;
const int SocketCommon::DataType_ARRAYSIZE;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900
const ::google::protobuf::EnumDescriptor* SocketCommon_QuoteType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SocketCommon_QuoteType_descriptor_;
}
bool SocketCommon_QuoteType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const SocketCommon_QuoteType SocketCommon::None;
const SocketCommon_QuoteType SocketCommon::BASIC;
const SocketCommon_QuoteType SocketCommon::BBO;
const SocketCommon_QuoteType SocketCommon::ALL;
const SocketCommon_QuoteType SocketCommon::QuoteType_MIN;
const SocketCommon_QuoteType SocketCommon::QuoteType_MAX;
const int SocketCommon::QuoteType_ARRAYSIZE;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900
#if !defined(_MSC_VER) || _MSC_VER >= 1900
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

SocketCommon::SocketCommon()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_SocketCommon_2eproto();
  SharedCtor();
  // @@protoc_insertion_point(constructor:tigeropen.push.pb.SocketCommon)
}

void SocketCommon::InitAsDefaultInstance() {
}

SocketCommon::SocketCommon(const SocketCommon& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:tigeropen.push.pb.SocketCommon)
}

void SocketCommon::SharedCtor() {
  _cached_size_ = 0;
}

SocketCommon::~SocketCommon() {
  // @@protoc_insertion_point(destructor:tigeropen.push.pb.SocketCommon)
  SharedDtor();
}

void SocketCommon::SharedDtor() {
}

void SocketCommon::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* SocketCommon::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SocketCommon_descriptor_;
}

const SocketCommon& SocketCommon::default_instance() {
  protobuf_InitDefaults_SocketCommon_2eproto();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<SocketCommon> SocketCommon_default_instance_;

SocketCommon* SocketCommon::New(::google::protobuf::Arena* arena) const {
  SocketCommon* n = new SocketCommon;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void SocketCommon::Clear() {
// @@protoc_insertion_point(message_clear_start:tigeropen.push.pb.SocketCommon)
}

bool SocketCommon::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:tigeropen.push.pb.SocketCommon)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
  handle_unusual:
    if (tag == 0 ||
        ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
        ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
      goto success;
    }
    DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
  }
success:
  // @@protoc_insertion_point(parse_success:tigeropen.push.pb.SocketCommon)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:tigeropen.push.pb.SocketCommon)
  return false;
#undef DO_
}

void SocketCommon::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:tigeropen.push.pb.SocketCommon)
  // @@protoc_insertion_point(serialize_end:tigeropen.push.pb.SocketCommon)
}

::google::protobuf::uint8* SocketCommon::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:tigeropen.push.pb.SocketCommon)
  // @@protoc_insertion_point(serialize_to_array_end:tigeropen.push.pb.SocketCommon)
  return target;
}

size_t SocketCommon::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:tigeropen.push.pb.SocketCommon)
  size_t total_size = 0;

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void SocketCommon::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:tigeropen.push.pb.SocketCommon)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const SocketCommon* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const SocketCommon>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:tigeropen.push.pb.SocketCommon)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:tigeropen.push.pb.SocketCommon)
    UnsafeMergeFrom(*source);
  }
}

void SocketCommon::MergeFrom(const SocketCommon& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:tigeropen.push.pb.SocketCommon)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void SocketCommon::UnsafeMergeFrom(const SocketCommon& from) {
  GOOGLE_DCHECK(&from != this);
}

void SocketCommon::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:tigeropen.push.pb.SocketCommon)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SocketCommon::CopyFrom(const SocketCommon& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:tigeropen.push.pb.SocketCommon)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool SocketCommon::IsInitialized() const {

  return true;
}

void SocketCommon::Swap(SocketCommon* other) {
  if (other == this) return;
  InternalSwap(other);
}
void SocketCommon::InternalSwap(SocketCommon* other) {
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata SocketCommon::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = SocketCommon_descriptor_;
  metadata.reflection = SocketCommon_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// SocketCommon

inline const SocketCommon* SocketCommon::internal_default_instance() {
  return &SocketCommon_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace push
}  // namespace tigeropen

// @@protoc_insertion_point(global_scope)
