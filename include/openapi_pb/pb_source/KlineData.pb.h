﻿// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: KlineData.proto

#ifndef PROTOBUF_KlineData_2eproto__INCLUDED
#define PROTOBUF_KlineData_2eproto__INCLUDED

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
// @@protoc_insertion_point(includes)

namespace tigeropen {
namespace push {
namespace pb {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_KlineData_2eproto();
void protobuf_InitDefaults_KlineData_2eproto();
void protobuf_AssignDesc_KlineData_2eproto();
void protobuf_ShutdownFile_KlineData_2eproto();

class KlineData;

// ===================================================================

class KlineData : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tigeropen.push.pb.KlineData) */ {
 public:
  KlineData();
  virtual ~KlineData();

  KlineData(const KlineData& from);

  inline KlineData& operator=(const KlineData& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const KlineData& default_instance();

  static const KlineData* internal_default_instance();

  void Swap(KlineData* other);

  // implements Message ----------------------------------------------

  inline KlineData* New() const { return New(NULL); }

  KlineData* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const KlineData& from);
  void MergeFrom(const KlineData& from);
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
  void InternalSwap(KlineData* other);
  void UnsafeMergeFrom(const KlineData& from);
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

  // optional int64 time = 1;
  void clear_time();
  static const int kTimeFieldNumber = 1;
  ::google::protobuf::int64 time() const;
  void set_time(::google::protobuf::int64 value);

  // optional float open = 2;
  void clear_open();
  static const int kOpenFieldNumber = 2;
  float open() const;
  void set_open(float value);

  // optional float high = 3;
  void clear_high();
  static const int kHighFieldNumber = 3;
  float high() const;
  void set_high(float value);

  // optional float low = 4;
  void clear_low();
  static const int kLowFieldNumber = 4;
  float low() const;
  void set_low(float value);

  // optional float close = 5;
  void clear_close();
  static const int kCloseFieldNumber = 5;
  float close() const;
  void set_close(float value);

  // optional float avg = 6;
  void clear_avg();
  static const int kAvgFieldNumber = 6;
  float avg() const;
  void set_avg(float value);

  // optional int64 volume = 7;
  void clear_volume();
  static const int kVolumeFieldNumber = 7;
  ::google::protobuf::int64 volume() const;
  void set_volume(::google::protobuf::int64 value);

  // optional int32 count = 8;
  void clear_count();
  static const int kCountFieldNumber = 8;
  ::google::protobuf::int32 count() const;
  void set_count(::google::protobuf::int32 value);

  // optional string symbol = 9;
  void clear_symbol();
  static const int kSymbolFieldNumber = 9;
  const ::std::string& symbol() const;
  void set_symbol(const ::std::string& value);
  void set_symbol(const char* value);
  void set_symbol(const char* value, size_t size);
  ::std::string* mutable_symbol();
  ::std::string* release_symbol();
  void set_allocated_symbol(::std::string* symbol);

  // optional double amount = 10;
  void clear_amount();
  static const int kAmountFieldNumber = 10;
  double amount() const;
  void set_amount(double value);

  // optional uint64 serverTimestamp = 11;
  void clear_servertimestamp();
  static const int kServerTimestampFieldNumber = 11;
  ::google::protobuf::uint64 servertimestamp() const;
  void set_servertimestamp(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:tigeropen.push.pb.KlineData)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr symbol_;
  ::google::protobuf::int64 time_;
  float open_;
  float high_;
  float low_;
  float close_;
  ::google::protobuf::int64 volume_;
  float avg_;
  ::google::protobuf::int32 count_;
  double amount_;
  ::google::protobuf::uint64 servertimestamp_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_KlineData_2eproto_impl();
  friend void  protobuf_AddDesc_KlineData_2eproto_impl();
  friend void protobuf_AssignDesc_KlineData_2eproto();
  friend void protobuf_ShutdownFile_KlineData_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<KlineData> KlineData_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// KlineData

// optional int64 time = 1;
inline void KlineData::clear_time() {
  time_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 KlineData::time() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.time)
  return time_;
}
inline void KlineData::set_time(::google::protobuf::int64 value) {
  
  time_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.time)
}

// optional float open = 2;
inline void KlineData::clear_open() {
  open_ = 0;
}
inline float KlineData::open() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.open)
  return open_;
}
inline void KlineData::set_open(float value) {
  
  open_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.open)
}

// optional float high = 3;
inline void KlineData::clear_high() {
  high_ = 0;
}
inline float KlineData::high() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.high)
  return high_;
}
inline void KlineData::set_high(float value) {
  
  high_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.high)
}

// optional float low = 4;
inline void KlineData::clear_low() {
  low_ = 0;
}
inline float KlineData::low() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.low)
  return low_;
}
inline void KlineData::set_low(float value) {
  
  low_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.low)
}

// optional float close = 5;
inline void KlineData::clear_close() {
  close_ = 0;
}
inline float KlineData::close() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.close)
  return close_;
}
inline void KlineData::set_close(float value) {
  
  close_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.close)
}

// optional float avg = 6;
inline void KlineData::clear_avg() {
  avg_ = 0;
}
inline float KlineData::avg() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.avg)
  return avg_;
}
inline void KlineData::set_avg(float value) {
  
  avg_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.avg)
}

// optional int64 volume = 7;
inline void KlineData::clear_volume() {
  volume_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 KlineData::volume() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.volume)
  return volume_;
}
inline void KlineData::set_volume(::google::protobuf::int64 value) {
  
  volume_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.volume)
}

// optional int32 count = 8;
inline void KlineData::clear_count() {
  count_ = 0;
}
inline ::google::protobuf::int32 KlineData::count() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.count)
  return count_;
}
inline void KlineData::set_count(::google::protobuf::int32 value) {
  
  count_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.count)
}

// optional string symbol = 9;
inline void KlineData::clear_symbol() {
  symbol_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& KlineData::symbol() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.symbol)
  return symbol_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void KlineData::set_symbol(const ::std::string& value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.symbol)
}
inline void KlineData::set_symbol(const char* value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.KlineData.symbol)
}
inline void KlineData::set_symbol(const char* value, size_t size) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.KlineData.symbol)
}
inline ::std::string* KlineData::mutable_symbol() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.KlineData.symbol)
  return symbol_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* KlineData::release_symbol() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.KlineData.symbol)
  
  return symbol_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void KlineData::set_allocated_symbol(::std::string* symbol) {
  if (symbol != NULL) {
    
  } else {
    
  }
  symbol_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), symbol);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.KlineData.symbol)
}

// optional double amount = 10;
inline void KlineData::clear_amount() {
  amount_ = 0;
}
inline double KlineData::amount() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.amount)
  return amount_;
}
inline void KlineData::set_amount(double value) {
  
  amount_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.amount)
}

// optional uint64 serverTimestamp = 11;
inline void KlineData::clear_servertimestamp() {
  servertimestamp_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 KlineData::servertimestamp() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.KlineData.serverTimestamp)
  return servertimestamp_;
}
inline void KlineData::set_servertimestamp(::google::protobuf::uint64 value) {
  
  servertimestamp_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.KlineData.serverTimestamp)
}

inline const KlineData* KlineData::internal_default_instance() {
  return &KlineData_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace push
}  // namespace tigeropen

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_KlineData_2eproto__INCLUDED
