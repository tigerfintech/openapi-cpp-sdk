﻿// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TickData.proto

#ifndef PROTOBUF_TickData_2eproto__INCLUDED
#define PROTOBUF_TickData_2eproto__INCLUDED

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
void protobuf_AddDesc_TickData_2eproto();
void protobuf_InitDefaults_TickData_2eproto();
void protobuf_AssignDesc_TickData_2eproto();
void protobuf_ShutdownFile_TickData_2eproto();

class TickData;
class TickData_Tick;

// ===================================================================

class TickData_Tick : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tigeropen.push.pb.TickData.Tick) */ {
 public:
  TickData_Tick();
  virtual ~TickData_Tick();

  TickData_Tick(const TickData_Tick& from);

  inline TickData_Tick& operator=(const TickData_Tick& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TickData_Tick& default_instance();

  static const TickData_Tick* internal_default_instance();

  void Swap(TickData_Tick* other);

  // implements Message ----------------------------------------------

  inline TickData_Tick* New() const { return New(NULL); }

  TickData_Tick* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TickData_Tick& from);
  void MergeFrom(const TickData_Tick& from);
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
  void InternalSwap(TickData_Tick* other);
  void UnsafeMergeFrom(const TickData_Tick& from);
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

  // optional int64 sn = 1;
  void clear_sn();
  static const int kSnFieldNumber = 1;
  ::google::protobuf::int64 sn() const;
  void set_sn(::google::protobuf::int64 value);

  // optional int64 time = 2;
  void clear_time();
  static const int kTimeFieldNumber = 2;
  ::google::protobuf::int64 time() const;
  void set_time(::google::protobuf::int64 value);

  // optional float price = 3;
  void clear_price();
  static const int kPriceFieldNumber = 3;
  float price() const;
  void set_price(float value);

  // optional int32 volume = 4;
  void clear_volume();
  static const int kVolumeFieldNumber = 4;
  ::google::protobuf::int32 volume() const;
  void set_volume(::google::protobuf::int32 value);

  // optional string type = 5;
  void clear_type();
  static const int kTypeFieldNumber = 5;
  const ::std::string& type() const;
  void set_type(const ::std::string& value);
  void set_type(const char* value);
  void set_type(const char* value, size_t size);
  ::std::string* mutable_type();
  ::std::string* release_type();
  void set_allocated_type(::std::string* type);

  // optional string cond = 6;
  void clear_cond();
  static const int kCondFieldNumber = 6;
  const ::std::string& cond() const;
  void set_cond(const ::std::string& value);
  void set_cond(const char* value);
  void set_cond(const char* value, size_t size);
  ::std::string* mutable_cond();
  ::std::string* release_cond();
  void set_allocated_cond(::std::string* cond);

  // optional string partCode = 7;
  void clear_partcode();
  static const int kPartCodeFieldNumber = 7;
  const ::std::string& partcode() const;
  void set_partcode(const ::std::string& value);
  void set_partcode(const char* value);
  void set_partcode(const char* value, size_t size);
  ::std::string* mutable_partcode();
  ::std::string* release_partcode();
  void set_allocated_partcode(::std::string* partcode);

  // @@protoc_insertion_point(class_scope:tigeropen.push.pb.TickData.Tick)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr type_;
  ::google::protobuf::internal::ArenaStringPtr cond_;
  ::google::protobuf::internal::ArenaStringPtr partcode_;
  ::google::protobuf::int64 sn_;
  ::google::protobuf::int64 time_;
  float price_;
  ::google::protobuf::int32 volume_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_TickData_2eproto_impl();
  friend void  protobuf_AddDesc_TickData_2eproto_impl();
  friend void protobuf_AssignDesc_TickData_2eproto();
  friend void protobuf_ShutdownFile_TickData_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<TickData_Tick> TickData_Tick_default_instance_;

// -------------------------------------------------------------------

class TickData : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tigeropen.push.pb.TickData) */ {
 public:
  TickData();
  virtual ~TickData();

  TickData(const TickData& from);

  inline TickData& operator=(const TickData& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TickData& default_instance();

  static const TickData* internal_default_instance();

  void Swap(TickData* other);

  // implements Message ----------------------------------------------

  inline TickData* New() const { return New(NULL); }

  TickData* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TickData& from);
  void MergeFrom(const TickData& from);
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
  void InternalSwap(TickData* other);
  void UnsafeMergeFrom(const TickData& from);
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

  typedef TickData_Tick Tick;

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

  // repeated .tigeropen.push.pb.TickData.Tick ticks = 2;
  int ticks_size() const;
  void clear_ticks();
  static const int kTicksFieldNumber = 2;
  const ::tigeropen::push::pb::TickData_Tick& ticks(int index) const;
  ::tigeropen::push::pb::TickData_Tick* mutable_ticks(int index);
  ::tigeropen::push::pb::TickData_Tick* add_ticks();
  ::google::protobuf::RepeatedPtrField< ::tigeropen::push::pb::TickData_Tick >*
      mutable_ticks();
  const ::google::protobuf::RepeatedPtrField< ::tigeropen::push::pb::TickData_Tick >&
      ticks() const;

  // optional int64 timestamp = 3;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 3;
  ::google::protobuf::int64 timestamp() const;
  void set_timestamp(::google::protobuf::int64 value);

  // optional string source = 4;
  void clear_source();
  static const int kSourceFieldNumber = 4;
  const ::std::string& source() const;
  void set_source(const ::std::string& value);
  void set_source(const char* value);
  void set_source(const char* value, size_t size);
  ::std::string* mutable_source();
  ::std::string* release_source();
  void set_allocated_source(::std::string* source);

  // @@protoc_insertion_point(class_scope:tigeropen.push.pb.TickData)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField< ::tigeropen::push::pb::TickData_Tick > ticks_;
  ::google::protobuf::internal::ArenaStringPtr symbol_;
  ::google::protobuf::internal::ArenaStringPtr source_;
  ::google::protobuf::int64 timestamp_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_TickData_2eproto_impl();
  friend void  protobuf_AddDesc_TickData_2eproto_impl();
  friend void protobuf_AssignDesc_TickData_2eproto();
  friend void protobuf_ShutdownFile_TickData_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<TickData> TickData_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// TickData_Tick

// optional int64 sn = 1;
inline void TickData_Tick::clear_sn() {
  sn_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 TickData_Tick::sn() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TickData.Tick.sn)
  return sn_;
}
inline void TickData_Tick::set_sn(::google::protobuf::int64 value) {
  
  sn_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TickData.Tick.sn)
}

// optional int64 time = 2;
inline void TickData_Tick::clear_time() {
  time_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 TickData_Tick::time() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TickData.Tick.time)
  return time_;
}
inline void TickData_Tick::set_time(::google::protobuf::int64 value) {
  
  time_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TickData.Tick.time)
}

// optional float price = 3;
inline void TickData_Tick::clear_price() {
  price_ = 0;
}
inline float TickData_Tick::price() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TickData.Tick.price)
  return price_;
}
inline void TickData_Tick::set_price(float value) {
  
  price_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TickData.Tick.price)
}

// optional int32 volume = 4;
inline void TickData_Tick::clear_volume() {
  volume_ = 0;
}
inline ::google::protobuf::int32 TickData_Tick::volume() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TickData.Tick.volume)
  return volume_;
}
inline void TickData_Tick::set_volume(::google::protobuf::int32 value) {
  
  volume_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TickData.Tick.volume)
}

// optional string type = 5;
inline void TickData_Tick::clear_type() {
  type_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TickData_Tick::type() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TickData.Tick.type)
  return type_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TickData_Tick::set_type(const ::std::string& value) {
  
  type_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TickData.Tick.type)
}
inline void TickData_Tick::set_type(const char* value) {
  
  type_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.TickData.Tick.type)
}
inline void TickData_Tick::set_type(const char* value, size_t size) {
  
  type_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.TickData.Tick.type)
}
inline ::std::string* TickData_Tick::mutable_type() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TickData.Tick.type)
  return type_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TickData_Tick::release_type() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.TickData.Tick.type)
  
  return type_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TickData_Tick::set_allocated_type(::std::string* type) {
  if (type != NULL) {
    
  } else {
    
  }
  type_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), type);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.TickData.Tick.type)
}

// optional string cond = 6;
inline void TickData_Tick::clear_cond() {
  cond_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TickData_Tick::cond() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TickData.Tick.cond)
  return cond_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TickData_Tick::set_cond(const ::std::string& value) {
  
  cond_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TickData.Tick.cond)
}
inline void TickData_Tick::set_cond(const char* value) {
  
  cond_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.TickData.Tick.cond)
}
inline void TickData_Tick::set_cond(const char* value, size_t size) {
  
  cond_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.TickData.Tick.cond)
}
inline ::std::string* TickData_Tick::mutable_cond() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TickData.Tick.cond)
  return cond_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TickData_Tick::release_cond() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.TickData.Tick.cond)
  
  return cond_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TickData_Tick::set_allocated_cond(::std::string* cond) {
  if (cond != NULL) {
    
  } else {
    
  }
  cond_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), cond);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.TickData.Tick.cond)
}

// optional string partCode = 7;
inline void TickData_Tick::clear_partcode() {
  partcode_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TickData_Tick::partcode() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TickData.Tick.partCode)
  return partcode_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TickData_Tick::set_partcode(const ::std::string& value) {
  
  partcode_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TickData.Tick.partCode)
}
inline void TickData_Tick::set_partcode(const char* value) {
  
  partcode_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.TickData.Tick.partCode)
}
inline void TickData_Tick::set_partcode(const char* value, size_t size) {
  
  partcode_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.TickData.Tick.partCode)
}
inline ::std::string* TickData_Tick::mutable_partcode() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TickData.Tick.partCode)
  return partcode_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TickData_Tick::release_partcode() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.TickData.Tick.partCode)
  
  return partcode_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TickData_Tick::set_allocated_partcode(::std::string* partcode) {
  if (partcode != NULL) {
    
  } else {
    
  }
  partcode_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), partcode);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.TickData.Tick.partCode)
}

inline const TickData_Tick* TickData_Tick::internal_default_instance() {
  return &TickData_Tick_default_instance_.get();
}
// -------------------------------------------------------------------

// TickData

// optional string symbol = 1;
inline void TickData::clear_symbol() {
  symbol_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TickData::symbol() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TickData.symbol)
  return symbol_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TickData::set_symbol(const ::std::string& value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TickData.symbol)
}
inline void TickData::set_symbol(const char* value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.TickData.symbol)
}
inline void TickData::set_symbol(const char* value, size_t size) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.TickData.symbol)
}
inline ::std::string* TickData::mutable_symbol() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TickData.symbol)
  return symbol_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TickData::release_symbol() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.TickData.symbol)
  
  return symbol_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TickData::set_allocated_symbol(::std::string* symbol) {
  if (symbol != NULL) {
    
  } else {
    
  }
  symbol_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), symbol);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.TickData.symbol)
}

// repeated .tigeropen.push.pb.TickData.Tick ticks = 2;
inline int TickData::ticks_size() const {
  return ticks_.size();
}
inline void TickData::clear_ticks() {
  ticks_.Clear();
}
inline const ::tigeropen::push::pb::TickData_Tick& TickData::ticks(int index) const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TickData.ticks)
  return ticks_.Get(index);
}
inline ::tigeropen::push::pb::TickData_Tick* TickData::mutable_ticks(int index) {
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TickData.ticks)
  return ticks_.Mutable(index);
}
inline ::tigeropen::push::pb::TickData_Tick* TickData::add_ticks() {
  // @@protoc_insertion_point(field_add:tigeropen.push.pb.TickData.ticks)
  return ticks_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::tigeropen::push::pb::TickData_Tick >*
TickData::mutable_ticks() {
  // @@protoc_insertion_point(field_mutable_list:tigeropen.push.pb.TickData.ticks)
  return &ticks_;
}
inline const ::google::protobuf::RepeatedPtrField< ::tigeropen::push::pb::TickData_Tick >&
TickData::ticks() const {
  // @@protoc_insertion_point(field_list:tigeropen.push.pb.TickData.ticks)
  return ticks_;
}

// optional int64 timestamp = 3;
inline void TickData::clear_timestamp() {
  timestamp_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 TickData::timestamp() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TickData.timestamp)
  return timestamp_;
}
inline void TickData::set_timestamp(::google::protobuf::int64 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TickData.timestamp)
}

// optional string source = 4;
inline void TickData::clear_source() {
  source_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TickData::source() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TickData.source)
  return source_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TickData::set_source(const ::std::string& value) {
  
  source_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TickData.source)
}
inline void TickData::set_source(const char* value) {
  
  source_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.TickData.source)
}
inline void TickData::set_source(const char* value, size_t size) {
  
  source_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.TickData.source)
}
inline ::std::string* TickData::mutable_source() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TickData.source)
  return source_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TickData::release_source() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.TickData.source)
  
  return source_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TickData::set_allocated_source(::std::string* source) {
  if (source != NULL) {
    
  } else {
    
  }
  source_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), source);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.TickData.source)
}

inline const TickData* TickData::internal_default_instance() {
  return &TickData_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace push
}  // namespace tigeropen

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_TickData_2eproto__INCLUDED