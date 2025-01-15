﻿// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: QuoteBBOData.proto

#ifndef PROTOBUF_QuoteBBOData_2eproto__INCLUDED
#define PROTOBUF_QuoteBBOData_2eproto__INCLUDED

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
#include <google/protobuf/unknown_field_set.h>
#include "SocketCommon.pb.h"  // IWYU pragma: export
// @@protoc_insertion_point(includes)

namespace tigeropen {
namespace push {
namespace pb {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_QuoteBBOData_2eproto();
void protobuf_InitDefaults_QuoteBBOData_2eproto();
void protobuf_AssignDesc_QuoteBBOData_2eproto();
void protobuf_ShutdownFile_QuoteBBOData_2eproto();

class QuoteBBOData;

// ===================================================================

class QuoteBBOData : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tigeropen.push.pb.QuoteBBOData) */ {
 public:
  QuoteBBOData();
  virtual ~QuoteBBOData();

  QuoteBBOData(const QuoteBBOData& from);

  inline QuoteBBOData& operator=(const QuoteBBOData& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const QuoteBBOData& default_instance();

  static const QuoteBBOData* internal_default_instance();

  void Swap(QuoteBBOData* other);

  // implements Message ----------------------------------------------

  inline QuoteBBOData* New() const { return New(NULL); }

  QuoteBBOData* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const QuoteBBOData& from);
  void MergeFrom(const QuoteBBOData& from);
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
  void InternalSwap(QuoteBBOData* other);
  void UnsafeMergeFrom(const QuoteBBOData& from);
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

  // accessors -------------------------------------------------------

  // optional string symbol = 1;
  void clear_symbol();
  static const int kSymbolFieldNumber = 1;
  const ::std::string& symbol() const;
  void set_symbol(const ::std::string& value);
  void set_symbol(const char* value);
  void set_symbol(const char* value, size_t size);
  ::std::string* mutable_symbol();
  ::std::string* release_symbol();
  void set_allocated_symbol(::std::string* symbol);

  // optional .tigeropen.push.pb.SocketCommon.QuoteType type = 2;
  void clear_type();
  static const int kTypeFieldNumber = 2;
  ::tigeropen::push::pb::SocketCommon_QuoteType type() const;
  void set_type(::tigeropen::push::pb::SocketCommon_QuoteType value);

  // optional uint64 timestamp = 3;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 3;
  ::google::protobuf::uint64 timestamp() const;
  void set_timestamp(::google::protobuf::uint64 value);

  // optional double askPrice = 17;
  void clear_askprice();
  static const int kAskPriceFieldNumber = 17;
  double askprice() const;
  void set_askprice(double value);

  // optional sint64 askSize = 18;
  void clear_asksize();
  static const int kAskSizeFieldNumber = 18;
  ::google::protobuf::int64 asksize() const;
  void set_asksize(::google::protobuf::int64 value);

  // optional uint64 askTimestamp = 19;
  void clear_asktimestamp();
  static const int kAskTimestampFieldNumber = 19;
  ::google::protobuf::uint64 asktimestamp() const;
  void set_asktimestamp(::google::protobuf::uint64 value);

  // optional double bidPrice = 20;
  void clear_bidprice();
  static const int kBidPriceFieldNumber = 20;
  double bidprice() const;
  void set_bidprice(double value);

  // optional sint64 bidSize = 21;
  void clear_bidsize();
  static const int kBidSizeFieldNumber = 21;
  ::google::protobuf::int64 bidsize() const;
  void set_bidsize(::google::protobuf::int64 value);

  // optional uint64 bidTimestamp = 22;
  void clear_bidtimestamp();
  static const int kBidTimestampFieldNumber = 22;
  ::google::protobuf::uint64 bidtimestamp() const;
  void set_bidtimestamp(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:tigeropen.push.pb.QuoteBBOData)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr symbol_;
  ::google::protobuf::uint64 timestamp_;
  double askprice_;
  ::google::protobuf::int64 asksize_;
  ::google::protobuf::uint64 asktimestamp_;
  double bidprice_;
  ::google::protobuf::int64 bidsize_;
  ::google::protobuf::uint64 bidtimestamp_;
  int type_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_QuoteBBOData_2eproto_impl();
  friend void  protobuf_AddDesc_QuoteBBOData_2eproto_impl();
  friend void protobuf_AssignDesc_QuoteBBOData_2eproto();
  friend void protobuf_ShutdownFile_QuoteBBOData_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<QuoteBBOData> QuoteBBOData_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// QuoteBBOData

// optional string symbol = 1;
inline void QuoteBBOData::clear_symbol() {
  symbol_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& QuoteBBOData::symbol() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteBBOData.symbol)
  return symbol_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void QuoteBBOData::set_symbol(const ::std::string& value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteBBOData.symbol)
}
inline void QuoteBBOData::set_symbol(const char* value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.QuoteBBOData.symbol)
}
inline void QuoteBBOData::set_symbol(const char* value, size_t size) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.QuoteBBOData.symbol)
}
inline ::std::string* QuoteBBOData::mutable_symbol() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.QuoteBBOData.symbol)
  return symbol_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* QuoteBBOData::release_symbol() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.QuoteBBOData.symbol)
  
  return symbol_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void QuoteBBOData::set_allocated_symbol(::std::string* symbol) {
  if (symbol != NULL) {
    
  } else {
    
  }
  symbol_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), symbol);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.QuoteBBOData.symbol)
}

// optional .tigeropen.push.pb.SocketCommon.QuoteType type = 2;
inline void QuoteBBOData::clear_type() {
  type_ = 0;
}
inline ::tigeropen::push::pb::SocketCommon_QuoteType QuoteBBOData::type() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteBBOData.type)
  return static_cast< ::tigeropen::push::pb::SocketCommon_QuoteType >(type_);
}
inline void QuoteBBOData::set_type(::tigeropen::push::pb::SocketCommon_QuoteType value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteBBOData.type)
}

// optional uint64 timestamp = 3;
inline void QuoteBBOData::clear_timestamp() {
  timestamp_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 QuoteBBOData::timestamp() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteBBOData.timestamp)
  return timestamp_;
}
inline void QuoteBBOData::set_timestamp(::google::protobuf::uint64 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteBBOData.timestamp)
}

// optional double askPrice = 17;
inline void QuoteBBOData::clear_askprice() {
  askprice_ = 0;
}
inline double QuoteBBOData::askprice() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteBBOData.askPrice)
  return askprice_;
}
inline void QuoteBBOData::set_askprice(double value) {
  
  askprice_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteBBOData.askPrice)
}

// optional sint64 askSize = 18;
inline void QuoteBBOData::clear_asksize() {
  asksize_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 QuoteBBOData::asksize() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteBBOData.askSize)
  return asksize_;
}
inline void QuoteBBOData::set_asksize(::google::protobuf::int64 value) {
  
  asksize_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteBBOData.askSize)
}

// optional uint64 askTimestamp = 19;
inline void QuoteBBOData::clear_asktimestamp() {
  asktimestamp_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 QuoteBBOData::asktimestamp() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteBBOData.askTimestamp)
  return asktimestamp_;
}
inline void QuoteBBOData::set_asktimestamp(::google::protobuf::uint64 value) {
  
  asktimestamp_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteBBOData.askTimestamp)
}

// optional double bidPrice = 20;
inline void QuoteBBOData::clear_bidprice() {
  bidprice_ = 0;
}
inline double QuoteBBOData::bidprice() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteBBOData.bidPrice)
  return bidprice_;
}
inline void QuoteBBOData::set_bidprice(double value) {
  
  bidprice_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteBBOData.bidPrice)
}

// optional sint64 bidSize = 21;
inline void QuoteBBOData::clear_bidsize() {
  bidsize_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 QuoteBBOData::bidsize() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteBBOData.bidSize)
  return bidsize_;
}
inline void QuoteBBOData::set_bidsize(::google::protobuf::int64 value) {
  
  bidsize_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteBBOData.bidSize)
}

// optional uint64 bidTimestamp = 22;
inline void QuoteBBOData::clear_bidtimestamp() {
  bidtimestamp_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 QuoteBBOData::bidtimestamp() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteBBOData.bidTimestamp)
  return bidtimestamp_;
}
inline void QuoteBBOData::set_bidtimestamp(::google::protobuf::uint64 value) {
  
  bidtimestamp_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteBBOData.bidTimestamp)
}

inline const QuoteBBOData* QuoteBBOData::internal_default_instance() {
  return &QuoteBBOData_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace push
}  // namespace tigeropen

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_QuoteBBOData_2eproto__INCLUDED