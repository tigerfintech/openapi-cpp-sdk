// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Response.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "Response.pb.h"

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

const ::google::protobuf::Descriptor* Response_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Response_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_Response_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_Response_2eproto() {
  protobuf_AddDesc_Response_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "Response.proto");
  GOOGLE_CHECK(file != NULL);
  Response_descriptor_ = file->message_type(0);
  static const int Response_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Response, command_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Response, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Response, code_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Response, msg_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Response, body_),
  };
  Response_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      Response_descriptor_,
      Response::internal_default_instance(),
      Response_offsets_,
      -1,
      -1,
      -1,
      sizeof(Response),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Response, _internal_metadata_));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_Response_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      Response_descriptor_, Response::internal_default_instance());
}

}  // namespace

void protobuf_ShutdownFile_Response_2eproto() {
  Response_default_instance_.Shutdown();
  delete Response_reflection_;
}

void protobuf_InitDefaults_Response_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::tigeropen::push::pb::protobuf_InitDefaults_SocketCommon_2eproto();
  ::tigeropen::push::pb::protobuf_InitDefaults_PushData_2eproto();
  ::google::protobuf::internal::GetEmptyString();
  Response_default_instance_.DefaultConstruct();
  Response_default_instance_.get_mutable()->InitAsDefaultInstance();
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_InitDefaults_Response_2eproto_once_);
void protobuf_InitDefaults_Response_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_InitDefaults_Response_2eproto_once_,
                 &protobuf_InitDefaults_Response_2eproto_impl);
}
void protobuf_AddDesc_Response_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  protobuf_InitDefaults_Response_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\016Response.proto\022\021tigeropen.push.pb\032\022Soc"
    "ketCommon.proto\032\016PushData.proto\"\226\001\n\010Resp"
    "onse\0228\n\007command\030\001 \001(\0162\'.tigeropen.push.p"
    "b.SocketCommon.Command\022\n\n\002id\030\002 \001(\r\022\014\n\004co"
    "de\030\003 \001(\005\022\013\n\003msg\030\004 \001(\t\022)\n\004body\030\005 \001(\0132\033.ti"
    "geropen.push.pb.PushDataP\000P\001b\006proto3", 236);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "Response.proto", &protobuf_RegisterTypes);
  ::tigeropen::push::pb::protobuf_AddDesc_SocketCommon_2eproto();
  ::tigeropen::push::pb::protobuf_AddDesc_PushData_2eproto();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_Response_2eproto);
}

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_Response_2eproto_once_);
void protobuf_AddDesc_Response_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_Response_2eproto_once_,
                 &protobuf_AddDesc_Response_2eproto_impl);
}
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_Response_2eproto {
  StaticDescriptorInitializer_Response_2eproto() {
    protobuf_AddDesc_Response_2eproto();
  }
} static_descriptor_initializer_Response_2eproto_;

namespace {

static void MergeFromFail(int line) GOOGLE_ATTRIBUTE_COLD GOOGLE_ATTRIBUTE_NORETURN;
static void MergeFromFail(int line) {
  ::google::protobuf::internal::MergeFromFail(__FILE__, line);
}

}  // namespace


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Response::kCommandFieldNumber;
const int Response::kIdFieldNumber;
const int Response::kCodeFieldNumber;
const int Response::kMsgFieldNumber;
const int Response::kBodyFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Response::Response()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (this != internal_default_instance()) protobuf_InitDefaults_Response_2eproto();
  SharedCtor();
  // @@protoc_insertion_point(constructor:tigeropen.push.pb.Response)
}

void Response::InitAsDefaultInstance() {
  body_ = const_cast< ::tigeropen::push::pb::PushData*>(
      ::tigeropen::push::pb::PushData::internal_default_instance());
}

Response::Response(const Response& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  UnsafeMergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:tigeropen.push.pb.Response)
}

void Response::SharedCtor() {
  msg_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  body_ = NULL;
  ::memset(&command_, 0, reinterpret_cast<char*>(&code_) -
    reinterpret_cast<char*>(&command_) + sizeof(code_));
  _cached_size_ = 0;
}

Response::~Response() {
  // @@protoc_insertion_point(destructor:tigeropen.push.pb.Response)
  SharedDtor();
}

void Response::SharedDtor() {
  msg_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (this != &Response_default_instance_.get()) {
    delete body_;
  }
}

void Response::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Response::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Response_descriptor_;
}

const Response& Response::default_instance() {
  protobuf_InitDefaults_Response_2eproto();
  return *internal_default_instance();
}

::google::protobuf::internal::ExplicitlyConstructed<Response> Response_default_instance_;

Response* Response::New(::google::protobuf::Arena* arena) const {
  Response* n = new Response;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Response::Clear() {
// @@protoc_insertion_point(message_clear_start:tigeropen.push.pb.Response)
#if defined(__clang__)
#define ZR_HELPER_(f) \
  _Pragma("clang diagnostic push") \
  _Pragma("clang diagnostic ignored \"-Winvalid-offsetof\"") \
  __builtin_offsetof(Response, f) \
  _Pragma("clang diagnostic pop")
#else
#define ZR_HELPER_(f) reinterpret_cast<char*>(\
  &reinterpret_cast<Response*>(16)->f)
#endif

#define ZR_(first, last) do {\
  ::memset(&(first), 0,\
           ZR_HELPER_(last) - ZR_HELPER_(first) + sizeof(last));\
} while (0)

  ZR_(command_, code_);
  msg_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (GetArenaNoVirtual() == NULL && body_ != NULL) delete body_;
  body_ = NULL;

#undef ZR_HELPER_
#undef ZR_

}

bool Response::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:tigeropen.push.pb.Response)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .tigeropen.push.pb.SocketCommon.Command command = 1;
      case 1: {
        if (tag == 8) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          set_command(static_cast< ::tigeropen::push::pb::SocketCommon_Command >(value));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_id;
        break;
      }

      // optional uint32 id = 2;
      case 2: {
        if (tag == 16) {
         parse_id:

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &id_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(24)) goto parse_code;
        break;
      }

      // optional int32 code = 3;
      case 3: {
        if (tag == 24) {
         parse_code:

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &code_)));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(34)) goto parse_msg;
        break;
      }

      // optional string msg = 4;
      case 4: {
        if (tag == 34) {
         parse_msg:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_msg()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->msg().data(), this->msg().length(),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "tigeropen.push.pb.Response.msg"));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(42)) goto parse_body;
        break;
      }

      // optional .tigeropen.push.pb.PushData body = 5;
      case 5: {
        if (tag == 42) {
         parse_body:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_body()));
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
  // @@protoc_insertion_point(parse_success:tigeropen.push.pb.Response)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:tigeropen.push.pb.Response)
  return false;
#undef DO_
}

void Response::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:tigeropen.push.pb.Response)
  // optional .tigeropen.push.pb.SocketCommon.Command command = 1;
  if (this->command() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->command(), output);
  }

  // optional uint32 id = 2;
  if (this->id() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->id(), output);
  }

  // optional int32 code = 3;
  if (this->code() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->code(), output);
  }

  // optional string msg = 4;
  if (this->msg().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->msg().data(), this->msg().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "tigeropen.push.pb.Response.msg");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      4, this->msg(), output);
  }

  // optional .tigeropen.push.pb.PushData body = 5;
  if (this->has_body()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      5, *this->body_, output);
  }

  // @@protoc_insertion_point(serialize_end:tigeropen.push.pb.Response)
}

::google::protobuf::uint8* Response::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:tigeropen.push.pb.Response)
  // optional .tigeropen.push.pb.SocketCommon.Command command = 1;
  if (this->command() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->command(), target);
  }

  // optional uint32 id = 2;
  if (this->id() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->id(), target);
  }

  // optional int32 code = 3;
  if (this->code() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->code(), target);
  }

  // optional string msg = 4;
  if (this->msg().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->msg().data(), this->msg().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "tigeropen.push.pb.Response.msg");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        4, this->msg(), target);
  }

  // optional .tigeropen.push.pb.PushData body = 5;
  if (this->has_body()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        5, *this->body_, false, target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:tigeropen.push.pb.Response)
  return target;
}

size_t Response::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:tigeropen.push.pb.Response)
  size_t total_size = 0;

  // optional .tigeropen.push.pb.SocketCommon.Command command = 1;
  if (this->command() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->command());
  }

  // optional uint32 id = 2;
  if (this->id() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->id());
  }

  // optional int32 code = 3;
  if (this->code() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->code());
  }

  // optional string msg = 4;
  if (this->msg().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->msg());
  }

  // optional .tigeropen.push.pb.PushData body = 5;
  if (this->has_body()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        *this->body_);
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Response::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:tigeropen.push.pb.Response)
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const Response* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Response>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:tigeropen.push.pb.Response)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:tigeropen.push.pb.Response)
    UnsafeMergeFrom(*source);
  }
}

void Response::MergeFrom(const Response& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:tigeropen.push.pb.Response)
  if (GOOGLE_PREDICT_TRUE(&from != this)) {
    UnsafeMergeFrom(from);
  } else {
    MergeFromFail(__LINE__);
  }
}

void Response::UnsafeMergeFrom(const Response& from) {
  GOOGLE_DCHECK(&from != this);
  if (from.command() != 0) {
    set_command(from.command());
  }
  if (from.id() != 0) {
    set_id(from.id());
  }
  if (from.code() != 0) {
    set_code(from.code());
  }
  if (from.msg().size() > 0) {

    msg_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.msg_);
  }
  if (from.has_body()) {
    mutable_body()->::tigeropen::push::pb::PushData::MergeFrom(from.body());
  }
}

void Response::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:tigeropen.push.pb.Response)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Response::CopyFrom(const Response& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:tigeropen.push.pb.Response)
  if (&from == this) return;
  Clear();
  UnsafeMergeFrom(from);
}

bool Response::IsInitialized() const {

  return true;
}

void Response::Swap(Response* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Response::InternalSwap(Response* other) {
  std::swap(command_, other->command_);
  std::swap(id_, other->id_);
  std::swap(code_, other->code_);
  msg_.Swap(&other->msg_);
  std::swap(body_, other->body_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Response::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Response_descriptor_;
  metadata.reflection = Response_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// Response

// optional .tigeropen.push.pb.SocketCommon.Command command = 1;
void Response::clear_command() {
  command_ = 0;
}
::tigeropen::push::pb::SocketCommon_Command Response::command() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.Response.command)
  return static_cast< ::tigeropen::push::pb::SocketCommon_Command >(command_);
}
void Response::set_command(::tigeropen::push::pb::SocketCommon_Command value) {
  
  command_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.Response.command)
}

// optional uint32 id = 2;
void Response::clear_id() {
  id_ = 0u;
}
::google::protobuf::uint32 Response::id() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.Response.id)
  return id_;
}
void Response::set_id(::google::protobuf::uint32 value) {
  
  id_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.Response.id)
}

// optional int32 code = 3;
void Response::clear_code() {
  code_ = 0;
}
::google::protobuf::int32 Response::code() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.Response.code)
  return code_;
}
void Response::set_code(::google::protobuf::int32 value) {
  
  code_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.Response.code)
}

// optional string msg = 4;
void Response::clear_msg() {
  msg_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
const ::std::string& Response::msg() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.Response.msg)
  return msg_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void Response::set_msg(const ::std::string& value) {
  
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.Response.msg)
}
void Response::set_msg(const char* value) {
  
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.Response.msg)
}
void Response::set_msg(const char* value, size_t size) {
  
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.Response.msg)
}
::std::string* Response::mutable_msg() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.Response.msg)
  return msg_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
::std::string* Response::release_msg() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.Response.msg)
  
  return msg_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void Response::set_allocated_msg(::std::string* msg) {
  if (msg != NULL) {
    
  } else {
    
  }
  msg_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), msg);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.Response.msg)
}

// optional .tigeropen.push.pb.PushData body = 5;
bool Response::has_body() const {
  return this != internal_default_instance() && body_ != NULL;
}
void Response::clear_body() {
  if (GetArenaNoVirtual() == NULL && body_ != NULL) delete body_;
  body_ = NULL;
}
const ::tigeropen::push::pb::PushData& Response::body() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.Response.body)
  return body_ != NULL ? *body_
                         : *::tigeropen::push::pb::PushData::internal_default_instance();
}
::tigeropen::push::pb::PushData* Response::mutable_body() {
  
  if (body_ == NULL) {
    body_ = new ::tigeropen::push::pb::PushData;
  }
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.Response.body)
  return body_;
}
::tigeropen::push::pb::PushData* Response::release_body() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.Response.body)
  
  ::tigeropen::push::pb::PushData* temp = body_;
  body_ = NULL;
  return temp;
}
void Response::set_allocated_body(::tigeropen::push::pb::PushData* body) {
  delete body_;
  body_ = body;
  if (body) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.Response.body)
}

inline const Response* Response::internal_default_instance() {
  return &Response_default_instance_.get();
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace push
}  // namespace tigeropen

// @@protoc_insertion_point(global_scope)
