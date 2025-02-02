﻿// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: SocketCommon.proto

#ifndef PROTOBUF_SocketCommon_2eproto__INCLUDED
#define PROTOBUF_SocketCommon_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3001000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3001000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace tigeropen {
namespace push {
namespace pb {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_SocketCommon_2eproto();
void protobuf_InitDefaults_SocketCommon_2eproto();
void protobuf_AssignDesc_SocketCommon_2eproto();
void protobuf_ShutdownFile_SocketCommon_2eproto();

class SocketCommon;

enum SocketCommon_Command {
  SocketCommon_Command_UNKNOWN = 0,
  SocketCommon_Command_CONNECT = 1,
  SocketCommon_Command_CONNECTED = 2,
  SocketCommon_Command_SEND = 3,
  SocketCommon_Command_SUBSCRIBE = 4,
  SocketCommon_Command_UNSUBSCRIBE = 5,
  SocketCommon_Command_DISCONNECT = 6,
  SocketCommon_Command_MESSAGE = 7,
  SocketCommon_Command_HEARTBEAT = 8,
  SocketCommon_Command_ERRORINFO = 9,
  SocketCommon_Command_SocketCommon_Command_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  SocketCommon_Command_SocketCommon_Command_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool SocketCommon_Command_IsValid(int value);
const SocketCommon_Command SocketCommon_Command_Command_MIN = SocketCommon_Command_UNKNOWN;
const SocketCommon_Command SocketCommon_Command_Command_MAX = SocketCommon_Command_ERRORINFO;
const int SocketCommon_Command_Command_ARRAYSIZE = SocketCommon_Command_Command_MAX + 1;

const ::google::protobuf::EnumDescriptor* SocketCommon_Command_descriptor();
inline const ::std::string& SocketCommon_Command_Name(SocketCommon_Command value) {
  return ::google::protobuf::internal::NameOfEnum(
    SocketCommon_Command_descriptor(), value);
}
inline bool SocketCommon_Command_Parse(
    const ::std::string& name, SocketCommon_Command* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SocketCommon_Command>(
    SocketCommon_Command_descriptor(), name, value);
}
enum SocketCommon_DataType {
  SocketCommon_DataType_Unknown = 0,
  SocketCommon_DataType_Quote = 1,
  SocketCommon_DataType_Option = 2,
  SocketCommon_DataType_Future = 3,
  SocketCommon_DataType_QuoteDepth = 4,
  SocketCommon_DataType_TradeTick = 5,
  SocketCommon_DataType_Asset = 6,
  SocketCommon_DataType_Position = 7,
  SocketCommon_DataType_OrderStatus = 8,
  SocketCommon_DataType_OrderTransaction = 9,
  SocketCommon_DataType_StockTop = 10,
  SocketCommon_DataType_OptionTop = 11,
  SocketCommon_DataType_Kline = 12,
  SocketCommon_DataType_SocketCommon_DataType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  SocketCommon_DataType_SocketCommon_DataType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool SocketCommon_DataType_IsValid(int value);
const SocketCommon_DataType SocketCommon_DataType_DataType_MIN = SocketCommon_DataType_Unknown;
const SocketCommon_DataType SocketCommon_DataType_DataType_MAX = SocketCommon_DataType_Kline;
const int SocketCommon_DataType_DataType_ARRAYSIZE = SocketCommon_DataType_DataType_MAX + 1;

const ::google::protobuf::EnumDescriptor* SocketCommon_DataType_descriptor();
inline const ::std::string& SocketCommon_DataType_Name(SocketCommon_DataType value) {
  return ::google::protobuf::internal::NameOfEnum(
    SocketCommon_DataType_descriptor(), value);
}
inline bool SocketCommon_DataType_Parse(
    const ::std::string& name, SocketCommon_DataType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SocketCommon_DataType>(
    SocketCommon_DataType_descriptor(), name, value);
}
enum SocketCommon_QuoteType {
  SocketCommon_QuoteType_None = 0,
  SocketCommon_QuoteType_BASIC = 1,
  SocketCommon_QuoteType_BBO = 2,
  SocketCommon_QuoteType_ALL = 3,
  SocketCommon_QuoteType_SocketCommon_QuoteType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  SocketCommon_QuoteType_SocketCommon_QuoteType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool SocketCommon_QuoteType_IsValid(int value);
const SocketCommon_QuoteType SocketCommon_QuoteType_QuoteType_MIN = SocketCommon_QuoteType_None;
const SocketCommon_QuoteType SocketCommon_QuoteType_QuoteType_MAX = SocketCommon_QuoteType_ALL;
const int SocketCommon_QuoteType_QuoteType_ARRAYSIZE = SocketCommon_QuoteType_QuoteType_MAX + 1;

const ::google::protobuf::EnumDescriptor* SocketCommon_QuoteType_descriptor();
inline const ::std::string& SocketCommon_QuoteType_Name(SocketCommon_QuoteType value) {
  return ::google::protobuf::internal::NameOfEnum(
    SocketCommon_QuoteType_descriptor(), value);
}
inline bool SocketCommon_QuoteType_Parse(
    const ::std::string& name, SocketCommon_QuoteType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<SocketCommon_QuoteType>(
    SocketCommon_QuoteType_descriptor(), name, value);
}
// ===================================================================

class SocketCommon : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tigeropen.push.pb.SocketCommon) */ {
 public:
  SocketCommon();
  virtual ~SocketCommon();

  SocketCommon(const SocketCommon& from);

  inline SocketCommon& operator=(const SocketCommon& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const SocketCommon& default_instance();

  static const SocketCommon* internal_default_instance();

  void Swap(SocketCommon* other);

  // implements Message ----------------------------------------------

  inline SocketCommon* New() const { return New(NULL); }

  SocketCommon* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SocketCommon& from);
  void MergeFrom(const SocketCommon& from);
  void Clear();
  bool IsInitialized() const;

  size_t ByteSizeLong() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(SocketCommon* other);
  void UnsafeMergeFrom(const SocketCommon& from);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef SocketCommon_Command Command;
  static const Command UNKNOWN =
    SocketCommon_Command_UNKNOWN;
  static const Command CONNECT =
    SocketCommon_Command_CONNECT;
  static const Command CONNECTED =
    SocketCommon_Command_CONNECTED;
  static const Command SEND =
    SocketCommon_Command_SEND;
  static const Command SUBSCRIBE =
    SocketCommon_Command_SUBSCRIBE;
  static const Command UNSUBSCRIBE =
    SocketCommon_Command_UNSUBSCRIBE;
  static const Command DISCONNECT =
    SocketCommon_Command_DISCONNECT;
  static const Command MESSAGE =
    SocketCommon_Command_MESSAGE;
  static const Command HEARTBEAT =
    SocketCommon_Command_HEARTBEAT;
  static const Command ERRORINFO =
    SocketCommon_Command_ERRORINFO;
  static inline bool Command_IsValid(int value) {
    return SocketCommon_Command_IsValid(value);
  }
  static const Command Command_MIN =
    SocketCommon_Command_Command_MIN;
  static const Command Command_MAX =
    SocketCommon_Command_Command_MAX;
  static const int Command_ARRAYSIZE =
    SocketCommon_Command_Command_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Command_descriptor() {
    return SocketCommon_Command_descriptor();
  }
  static inline const ::std::string& Command_Name(Command value) {
    return SocketCommon_Command_Name(value);
  }
  static inline bool Command_Parse(const ::std::string& name,
      Command* value) {
    return SocketCommon_Command_Parse(name, value);
  }

  typedef SocketCommon_DataType DataType;
  static const DataType Unknown =
    SocketCommon_DataType_Unknown;
  static const DataType Quote =
    SocketCommon_DataType_Quote;
  static const DataType Option =
    SocketCommon_DataType_Option;
  static const DataType Future =
    SocketCommon_DataType_Future;
  static const DataType QuoteDepth =
    SocketCommon_DataType_QuoteDepth;
  static const DataType TradeTick =
    SocketCommon_DataType_TradeTick;
  static const DataType Asset =
    SocketCommon_DataType_Asset;
  static const DataType Position =
    SocketCommon_DataType_Position;
  static const DataType OrderStatus =
    SocketCommon_DataType_OrderStatus;
  static const DataType OrderTransaction =
    SocketCommon_DataType_OrderTransaction;
  static const DataType StockTop =
    SocketCommon_DataType_StockTop;
  static const DataType OptionTop =
    SocketCommon_DataType_OptionTop;
  static const DataType Kline =
    SocketCommon_DataType_Kline;
  static inline bool DataType_IsValid(int value) {
    return SocketCommon_DataType_IsValid(value);
  }
  static const DataType DataType_MIN =
    SocketCommon_DataType_DataType_MIN;
  static const DataType DataType_MAX =
    SocketCommon_DataType_DataType_MAX;
  static const int DataType_ARRAYSIZE =
    SocketCommon_DataType_DataType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  DataType_descriptor() {
    return SocketCommon_DataType_descriptor();
  }
  static inline const ::std::string& DataType_Name(DataType value) {
    return SocketCommon_DataType_Name(value);
  }
  static inline bool DataType_Parse(const ::std::string& name,
      DataType* value) {
    return SocketCommon_DataType_Parse(name, value);
  }

  typedef SocketCommon_QuoteType QuoteType;
  static const QuoteType None =
    SocketCommon_QuoteType_None;
  static const QuoteType BASIC =
    SocketCommon_QuoteType_BASIC;
  static const QuoteType BBO =
    SocketCommon_QuoteType_BBO;
  static const QuoteType ALL =
    SocketCommon_QuoteType_ALL;
  static inline bool QuoteType_IsValid(int value) {
    return SocketCommon_QuoteType_IsValid(value);
  }
  static const QuoteType QuoteType_MIN =
    SocketCommon_QuoteType_QuoteType_MIN;
  static const QuoteType QuoteType_MAX =
    SocketCommon_QuoteType_QuoteType_MAX;
  static const int QuoteType_ARRAYSIZE =
    SocketCommon_QuoteType_QuoteType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  QuoteType_descriptor() {
    return SocketCommon_QuoteType_descriptor();
  }
  static inline const ::std::string& QuoteType_Name(QuoteType value) {
    return SocketCommon_QuoteType_Name(value);
  }
  static inline bool QuoteType_Parse(const ::std::string& name,
      QuoteType* value) {
    return SocketCommon_QuoteType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:tigeropen.push.pb.SocketCommon)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_SocketCommon_2eproto_impl();
  friend void  protobuf_AddDesc_SocketCommon_2eproto_impl();
  friend void protobuf_AssignDesc_SocketCommon_2eproto();
  friend void protobuf_ShutdownFile_SocketCommon_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<SocketCommon> SocketCommon_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// SocketCommon

inline const SocketCommon* SocketCommon::internal_default_instance() {
  return &SocketCommon_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace push
}  // namespace tigeropen

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::tigeropen::push::pb::SocketCommon_Command> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::tigeropen::push::pb::SocketCommon_Command>() {
  return ::tigeropen::push::pb::SocketCommon_Command_descriptor();
}
template <> struct is_proto_enum< ::tigeropen::push::pb::SocketCommon_DataType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::tigeropen::push::pb::SocketCommon_DataType>() {
  return ::tigeropen::push::pb::SocketCommon_DataType_descriptor();
}
template <> struct is_proto_enum< ::tigeropen::push::pb::SocketCommon_QuoteType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::tigeropen::push::pb::SocketCommon_QuoteType>() {
  return ::tigeropen::push::pb::SocketCommon_QuoteType_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_SocketCommon_2eproto__INCLUDED
