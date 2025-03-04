﻿// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TradeTickData.proto

#ifndef PROTOBUF_TradeTickData_2eproto__INCLUDED
#define PROTOBUF_TradeTickData_2eproto__INCLUDED

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
void protobuf_AddDesc_TradeTickData_2eproto();
void protobuf_InitDefaults_TradeTickData_2eproto();
void protobuf_AssignDesc_TradeTickData_2eproto();
void protobuf_ShutdownFile_TradeTickData_2eproto();

class TradeTickData;
class TradeTickData_MergedVol;

// ===================================================================

class TradeTickData_MergedVol : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tigeropen.push.pb.TradeTickData.MergedVol) */ {
 public:
  TradeTickData_MergedVol();
  virtual ~TradeTickData_MergedVol();

  TradeTickData_MergedVol(const TradeTickData_MergedVol& from);

  inline TradeTickData_MergedVol& operator=(const TradeTickData_MergedVol& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TradeTickData_MergedVol& default_instance();

  static const TradeTickData_MergedVol* internal_default_instance();

  void Swap(TradeTickData_MergedVol* other);

  // implements Message ----------------------------------------------

  inline TradeTickData_MergedVol* New() const { return New(NULL); }

  TradeTickData_MergedVol* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TradeTickData_MergedVol& from);
  void MergeFrom(const TradeTickData_MergedVol& from);
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
  void InternalSwap(TradeTickData_MergedVol* other);
  void UnsafeMergeFrom(const TradeTickData_MergedVol& from);
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

  // optional int32 mergeTimes = 1;
  void clear_mergetimes();
  static const int kMergeTimesFieldNumber = 1;
  ::google::protobuf::int32 mergetimes() const;
  void set_mergetimes(::google::protobuf::int32 value);

  // repeated int64 vol = 2;
  int vol_size() const;
  void clear_vol();
  static const int kVolFieldNumber = 2;
  ::google::protobuf::int64 vol(int index) const;
  void set_vol(int index, ::google::protobuf::int64 value);
  void add_vol(::google::protobuf::int64 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
      vol() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
      mutable_vol();

  // @@protoc_insertion_point(class_scope:tigeropen.push.pb.TradeTickData.MergedVol)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 > vol_;
  mutable int _vol_cached_byte_size_;
  ::google::protobuf::int32 mergetimes_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_TradeTickData_2eproto_impl();
  friend void  protobuf_AddDesc_TradeTickData_2eproto_impl();
  friend void protobuf_AssignDesc_TradeTickData_2eproto();
  friend void protobuf_ShutdownFile_TradeTickData_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<TradeTickData_MergedVol> TradeTickData_MergedVol_default_instance_;

// -------------------------------------------------------------------

class TradeTickData : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tigeropen.push.pb.TradeTickData) */ {
 public:
  TradeTickData();
  virtual ~TradeTickData();

  TradeTickData(const TradeTickData& from);

  inline TradeTickData& operator=(const TradeTickData& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TradeTickData& default_instance();

  static const TradeTickData* internal_default_instance();

  void Swap(TradeTickData* other);

  // implements Message ----------------------------------------------

  inline TradeTickData* New() const { return New(NULL); }

  TradeTickData* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TradeTickData& from);
  void MergeFrom(const TradeTickData& from);
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
  void InternalSwap(TradeTickData* other);
  void UnsafeMergeFrom(const TradeTickData& from);
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

  typedef TradeTickData_MergedVol MergedVol;

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

  // optional string type = 2;
  void clear_type();
  static const int kTypeFieldNumber = 2;
  const ::std::string& type() const;
  void set_type(const ::std::string& value);
  void set_type(const char* value);
  void set_type(const char* value, size_t size);
  ::std::string* mutable_type();
  ::std::string* release_type();
  void set_allocated_type(::std::string* type);

  // optional string cond = 3;
  void clear_cond();
  static const int kCondFieldNumber = 3;
  const ::std::string& cond() const;
  void set_cond(const ::std::string& value);
  void set_cond(const char* value);
  void set_cond(const char* value, size_t size);
  ::std::string* mutable_cond();
  ::std::string* release_cond();
  void set_allocated_cond(::std::string* cond);

  // optional int64 sn = 4;
  void clear_sn();
  static const int kSnFieldNumber = 4;
  ::google::protobuf::int64 sn() const;
  void set_sn(::google::protobuf::int64 value);

  // optional int64 priceBase = 5;
  void clear_pricebase();
  static const int kPriceBaseFieldNumber = 5;
  ::google::protobuf::int64 pricebase() const;
  void set_pricebase(::google::protobuf::int64 value);

  // optional int32 priceOffset = 6;
  void clear_priceoffset();
  static const int kPriceOffsetFieldNumber = 6;
  ::google::protobuf::int32 priceoffset() const;
  void set_priceoffset(::google::protobuf::int32 value);

  // repeated int64 time = 7;
  int time_size() const;
  void clear_time();
  static const int kTimeFieldNumber = 7;
  ::google::protobuf::int64 time(int index) const;
  void set_time(int index, ::google::protobuf::int64 value);
  void add_time(::google::protobuf::int64 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
      time() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
      mutable_time();

  // repeated int64 price = 8;
  int price_size() const;
  void clear_price();
  static const int kPriceFieldNumber = 8;
  ::google::protobuf::int64 price(int index) const;
  void set_price(int index, ::google::protobuf::int64 value);
  void add_price(::google::protobuf::int64 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
      price() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
      mutable_price();

  // repeated int64 volume = 9;
  int volume_size() const;
  void clear_volume();
  static const int kVolumeFieldNumber = 9;
  ::google::protobuf::int64 volume(int index) const;
  void set_volume(int index, ::google::protobuf::int64 value);
  void add_volume(::google::protobuf::int64 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
      volume() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
      mutable_volume();

  // repeated string partCode = 10;
  int partcode_size() const;
  void clear_partcode();
  static const int kPartCodeFieldNumber = 10;
  const ::std::string& partcode(int index) const;
  ::std::string* mutable_partcode(int index);
  void set_partcode(int index, const ::std::string& value);
  void set_partcode(int index, const char* value);
  void set_partcode(int index, const char* value, size_t size);
  ::std::string* add_partcode();
  void add_partcode(const ::std::string& value);
  void add_partcode(const char* value);
  void add_partcode(const char* value, size_t size);
  const ::google::protobuf::RepeatedPtrField< ::std::string>& partcode() const;
  ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_partcode();

  // optional string quoteLevel = 11;
  void clear_quotelevel();
  static const int kQuoteLevelFieldNumber = 11;
  const ::std::string& quotelevel() const;
  void set_quotelevel(const ::std::string& value);
  void set_quotelevel(const char* value);
  void set_quotelevel(const char* value, size_t size);
  ::std::string* mutable_quotelevel();
  ::std::string* release_quotelevel();
  void set_allocated_quotelevel(::std::string* quotelevel);

  // optional uint64 timestamp = 12;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 12;
  ::google::protobuf::uint64 timestamp() const;
  void set_timestamp(::google::protobuf::uint64 value);

  // optional string secType = 13;
  void clear_sectype();
  static const int kSecTypeFieldNumber = 13;
  const ::std::string& sectype() const;
  void set_sectype(const ::std::string& value);
  void set_sectype(const char* value);
  void set_sectype(const char* value, size_t size);
  ::std::string* mutable_sectype();
  ::std::string* release_sectype();
  void set_allocated_sectype(::std::string* sectype);

  // repeated .tigeropen.push.pb.TradeTickData.MergedVol mergedVols = 14;
  int mergedvols_size() const;
  void clear_mergedvols();
  static const int kMergedVolsFieldNumber = 14;
  const ::tigeropen::push::pb::TradeTickData_MergedVol& mergedvols(int index) const;
  ::tigeropen::push::pb::TradeTickData_MergedVol* mutable_mergedvols(int index);
  ::tigeropen::push::pb::TradeTickData_MergedVol* add_mergedvols();
  ::google::protobuf::RepeatedPtrField< ::tigeropen::push::pb::TradeTickData_MergedVol >*
      mutable_mergedvols();
  const ::google::protobuf::RepeatedPtrField< ::tigeropen::push::pb::TradeTickData_MergedVol >&
      mergedvols() const;

  // @@protoc_insertion_point(class_scope:tigeropen.push.pb.TradeTickData)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 > time_;
  mutable int _time_cached_byte_size_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 > price_;
  mutable int _price_cached_byte_size_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 > volume_;
  mutable int _volume_cached_byte_size_;
  ::google::protobuf::RepeatedPtrField< ::std::string> partcode_;
  ::google::protobuf::RepeatedPtrField< ::tigeropen::push::pb::TradeTickData_MergedVol > mergedvols_;
  ::google::protobuf::internal::ArenaStringPtr symbol_;
  ::google::protobuf::internal::ArenaStringPtr type_;
  ::google::protobuf::internal::ArenaStringPtr cond_;
  ::google::protobuf::internal::ArenaStringPtr quotelevel_;
  ::google::protobuf::internal::ArenaStringPtr sectype_;
  ::google::protobuf::int64 sn_;
  ::google::protobuf::int64 pricebase_;
  ::google::protobuf::uint64 timestamp_;
  ::google::protobuf::int32 priceoffset_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_TradeTickData_2eproto_impl();
  friend void  protobuf_AddDesc_TradeTickData_2eproto_impl();
  friend void protobuf_AssignDesc_TradeTickData_2eproto();
  friend void protobuf_ShutdownFile_TradeTickData_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<TradeTickData> TradeTickData_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// TradeTickData_MergedVol

// optional int32 mergeTimes = 1;
inline void TradeTickData_MergedVol::clear_mergetimes() {
  mergetimes_ = 0;
}
inline ::google::protobuf::int32 TradeTickData_MergedVol::mergetimes() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.MergedVol.mergeTimes)
  return mergetimes_;
}
inline void TradeTickData_MergedVol::set_mergetimes(::google::protobuf::int32 value) {
  
  mergetimes_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.MergedVol.mergeTimes)
}

// repeated int64 vol = 2;
inline int TradeTickData_MergedVol::vol_size() const {
  return vol_.size();
}
inline void TradeTickData_MergedVol::clear_vol() {
  vol_.Clear();
}
inline ::google::protobuf::int64 TradeTickData_MergedVol::vol(int index) const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.MergedVol.vol)
  return vol_.Get(index);
}
inline void TradeTickData_MergedVol::set_vol(int index, ::google::protobuf::int64 value) {
  vol_.Set(index, value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.MergedVol.vol)
}
inline void TradeTickData_MergedVol::add_vol(::google::protobuf::int64 value) {
  vol_.Add(value);
  // @@protoc_insertion_point(field_add:tigeropen.push.pb.TradeTickData.MergedVol.vol)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
TradeTickData_MergedVol::vol() const {
  // @@protoc_insertion_point(field_list:tigeropen.push.pb.TradeTickData.MergedVol.vol)
  return vol_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
TradeTickData_MergedVol::mutable_vol() {
  // @@protoc_insertion_point(field_mutable_list:tigeropen.push.pb.TradeTickData.MergedVol.vol)
  return &vol_;
}

inline const TradeTickData_MergedVol* TradeTickData_MergedVol::internal_default_instance() {
  return &TradeTickData_MergedVol_default_instance_.get();
}
// -------------------------------------------------------------------

// TradeTickData

// optional string symbol = 1;
inline void TradeTickData::clear_symbol() {
  symbol_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TradeTickData::symbol() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.symbol)
  return symbol_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TradeTickData::set_symbol(const ::std::string& value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.symbol)
}
inline void TradeTickData::set_symbol(const char* value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.TradeTickData.symbol)
}
inline void TradeTickData::set_symbol(const char* value, size_t size) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.TradeTickData.symbol)
}
inline ::std::string* TradeTickData::mutable_symbol() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TradeTickData.symbol)
  return symbol_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TradeTickData::release_symbol() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.TradeTickData.symbol)
  
  return symbol_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TradeTickData::set_allocated_symbol(::std::string* symbol) {
  if (symbol != NULL) {
    
  } else {
    
  }
  symbol_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), symbol);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.TradeTickData.symbol)
}

// optional string type = 2;
inline void TradeTickData::clear_type() {
  type_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TradeTickData::type() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.type)
  return type_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TradeTickData::set_type(const ::std::string& value) {
  
  type_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.type)
}
inline void TradeTickData::set_type(const char* value) {
  
  type_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.TradeTickData.type)
}
inline void TradeTickData::set_type(const char* value, size_t size) {
  
  type_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.TradeTickData.type)
}
inline ::std::string* TradeTickData::mutable_type() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TradeTickData.type)
  return type_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TradeTickData::release_type() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.TradeTickData.type)
  
  return type_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TradeTickData::set_allocated_type(::std::string* type) {
  if (type != NULL) {
    
  } else {
    
  }
  type_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), type);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.TradeTickData.type)
}

// optional string cond = 3;
inline void TradeTickData::clear_cond() {
  cond_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TradeTickData::cond() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.cond)
  return cond_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TradeTickData::set_cond(const ::std::string& value) {
  
  cond_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.cond)
}
inline void TradeTickData::set_cond(const char* value) {
  
  cond_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.TradeTickData.cond)
}
inline void TradeTickData::set_cond(const char* value, size_t size) {
  
  cond_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.TradeTickData.cond)
}
inline ::std::string* TradeTickData::mutable_cond() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TradeTickData.cond)
  return cond_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TradeTickData::release_cond() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.TradeTickData.cond)
  
  return cond_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TradeTickData::set_allocated_cond(::std::string* cond) {
  if (cond != NULL) {
    
  } else {
    
  }
  cond_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), cond);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.TradeTickData.cond)
}

// optional int64 sn = 4;
inline void TradeTickData::clear_sn() {
  sn_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 TradeTickData::sn() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.sn)
  return sn_;
}
inline void TradeTickData::set_sn(::google::protobuf::int64 value) {
  
  sn_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.sn)
}

// optional int64 priceBase = 5;
inline void TradeTickData::clear_pricebase() {
  pricebase_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 TradeTickData::pricebase() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.priceBase)
  return pricebase_;
}
inline void TradeTickData::set_pricebase(::google::protobuf::int64 value) {
  
  pricebase_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.priceBase)
}

// optional int32 priceOffset = 6;
inline void TradeTickData::clear_priceoffset() {
  priceoffset_ = 0;
}
inline ::google::protobuf::int32 TradeTickData::priceoffset() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.priceOffset)
  return priceoffset_;
}
inline void TradeTickData::set_priceoffset(::google::protobuf::int32 value) {
  
  priceoffset_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.priceOffset)
}

// repeated int64 time = 7;
inline int TradeTickData::time_size() const {
  return time_.size();
}
inline void TradeTickData::clear_time() {
  time_.Clear();
}
inline ::google::protobuf::int64 TradeTickData::time(int index) const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.time)
  return time_.Get(index);
}
inline void TradeTickData::set_time(int index, ::google::protobuf::int64 value) {
  time_.Set(index, value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.time)
}
inline void TradeTickData::add_time(::google::protobuf::int64 value) {
  time_.Add(value);
  // @@protoc_insertion_point(field_add:tigeropen.push.pb.TradeTickData.time)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
TradeTickData::time() const {
  // @@protoc_insertion_point(field_list:tigeropen.push.pb.TradeTickData.time)
  return time_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
TradeTickData::mutable_time() {
  // @@protoc_insertion_point(field_mutable_list:tigeropen.push.pb.TradeTickData.time)
  return &time_;
}

// repeated int64 price = 8;
inline int TradeTickData::price_size() const {
  return price_.size();
}
inline void TradeTickData::clear_price() {
  price_.Clear();
}
inline ::google::protobuf::int64 TradeTickData::price(int index) const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.price)
  return price_.Get(index);
}
inline void TradeTickData::set_price(int index, ::google::protobuf::int64 value) {
  price_.Set(index, value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.price)
}
inline void TradeTickData::add_price(::google::protobuf::int64 value) {
  price_.Add(value);
  // @@protoc_insertion_point(field_add:tigeropen.push.pb.TradeTickData.price)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
TradeTickData::price() const {
  // @@protoc_insertion_point(field_list:tigeropen.push.pb.TradeTickData.price)
  return price_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
TradeTickData::mutable_price() {
  // @@protoc_insertion_point(field_mutable_list:tigeropen.push.pb.TradeTickData.price)
  return &price_;
}

// repeated int64 volume = 9;
inline int TradeTickData::volume_size() const {
  return volume_.size();
}
inline void TradeTickData::clear_volume() {
  volume_.Clear();
}
inline ::google::protobuf::int64 TradeTickData::volume(int index) const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.volume)
  return volume_.Get(index);
}
inline void TradeTickData::set_volume(int index, ::google::protobuf::int64 value) {
  volume_.Set(index, value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.volume)
}
inline void TradeTickData::add_volume(::google::protobuf::int64 value) {
  volume_.Add(value);
  // @@protoc_insertion_point(field_add:tigeropen.push.pb.TradeTickData.volume)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
TradeTickData::volume() const {
  // @@protoc_insertion_point(field_list:tigeropen.push.pb.TradeTickData.volume)
  return volume_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
TradeTickData::mutable_volume() {
  // @@protoc_insertion_point(field_mutable_list:tigeropen.push.pb.TradeTickData.volume)
  return &volume_;
}

// repeated string partCode = 10;
inline int TradeTickData::partcode_size() const {
  return partcode_.size();
}
inline void TradeTickData::clear_partcode() {
  partcode_.Clear();
}
inline const ::std::string& TradeTickData::partcode(int index) const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.partCode)
  return partcode_.Get(index);
}
inline ::std::string* TradeTickData::mutable_partcode(int index) {
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TradeTickData.partCode)
  return partcode_.Mutable(index);
}
inline void TradeTickData::set_partcode(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.partCode)
  partcode_.Mutable(index)->assign(value);
}
inline void TradeTickData::set_partcode(int index, const char* value) {
  partcode_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.TradeTickData.partCode)
}
inline void TradeTickData::set_partcode(int index, const char* value, size_t size) {
  partcode_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.TradeTickData.partCode)
}
inline ::std::string* TradeTickData::add_partcode() {
  // @@protoc_insertion_point(field_add_mutable:tigeropen.push.pb.TradeTickData.partCode)
  return partcode_.Add();
}
inline void TradeTickData::add_partcode(const ::std::string& value) {
  partcode_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:tigeropen.push.pb.TradeTickData.partCode)
}
inline void TradeTickData::add_partcode(const char* value) {
  partcode_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:tigeropen.push.pb.TradeTickData.partCode)
}
inline void TradeTickData::add_partcode(const char* value, size_t size) {
  partcode_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:tigeropen.push.pb.TradeTickData.partCode)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
TradeTickData::partcode() const {
  // @@protoc_insertion_point(field_list:tigeropen.push.pb.TradeTickData.partCode)
  return partcode_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
TradeTickData::mutable_partcode() {
  // @@protoc_insertion_point(field_mutable_list:tigeropen.push.pb.TradeTickData.partCode)
  return &partcode_;
}

// optional string quoteLevel = 11;
inline void TradeTickData::clear_quotelevel() {
  quotelevel_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TradeTickData::quotelevel() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.quoteLevel)
  return quotelevel_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TradeTickData::set_quotelevel(const ::std::string& value) {
  
  quotelevel_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.quoteLevel)
}
inline void TradeTickData::set_quotelevel(const char* value) {
  
  quotelevel_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.TradeTickData.quoteLevel)
}
inline void TradeTickData::set_quotelevel(const char* value, size_t size) {
  
  quotelevel_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.TradeTickData.quoteLevel)
}
inline ::std::string* TradeTickData::mutable_quotelevel() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TradeTickData.quoteLevel)
  return quotelevel_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TradeTickData::release_quotelevel() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.TradeTickData.quoteLevel)
  
  return quotelevel_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TradeTickData::set_allocated_quotelevel(::std::string* quotelevel) {
  if (quotelevel != NULL) {
    
  } else {
    
  }
  quotelevel_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), quotelevel);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.TradeTickData.quoteLevel)
}

// optional uint64 timestamp = 12;
inline void TradeTickData::clear_timestamp() {
  timestamp_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 TradeTickData::timestamp() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.timestamp)
  return timestamp_;
}
inline void TradeTickData::set_timestamp(::google::protobuf::uint64 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.timestamp)
}

// optional string secType = 13;
inline void TradeTickData::clear_sectype() {
  sectype_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TradeTickData::sectype() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.secType)
  return sectype_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TradeTickData::set_sectype(const ::std::string& value) {
  
  sectype_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.TradeTickData.secType)
}
inline void TradeTickData::set_sectype(const char* value) {
  
  sectype_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.TradeTickData.secType)
}
inline void TradeTickData::set_sectype(const char* value, size_t size) {
  
  sectype_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.TradeTickData.secType)
}
inline ::std::string* TradeTickData::mutable_sectype() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TradeTickData.secType)
  return sectype_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TradeTickData::release_sectype() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.TradeTickData.secType)
  
  return sectype_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TradeTickData::set_allocated_sectype(::std::string* sectype) {
  if (sectype != NULL) {
    
  } else {
    
  }
  sectype_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), sectype);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.TradeTickData.secType)
}

// repeated .tigeropen.push.pb.TradeTickData.MergedVol mergedVols = 14;
inline int TradeTickData::mergedvols_size() const {
  return mergedvols_.size();
}
inline void TradeTickData::clear_mergedvols() {
  mergedvols_.Clear();
}
inline const ::tigeropen::push::pb::TradeTickData_MergedVol& TradeTickData::mergedvols(int index) const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.TradeTickData.mergedVols)
  return mergedvols_.Get(index);
}
inline ::tigeropen::push::pb::TradeTickData_MergedVol* TradeTickData::mutable_mergedvols(int index) {
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.TradeTickData.mergedVols)
  return mergedvols_.Mutable(index);
}
inline ::tigeropen::push::pb::TradeTickData_MergedVol* TradeTickData::add_mergedvols() {
  // @@protoc_insertion_point(field_add:tigeropen.push.pb.TradeTickData.mergedVols)
  return mergedvols_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::tigeropen::push::pb::TradeTickData_MergedVol >*
TradeTickData::mutable_mergedvols() {
  // @@protoc_insertion_point(field_mutable_list:tigeropen.push.pb.TradeTickData.mergedVols)
  return &mergedvols_;
}
inline const ::google::protobuf::RepeatedPtrField< ::tigeropen::push::pb::TradeTickData_MergedVol >&
TradeTickData::mergedvols() const {
  // @@protoc_insertion_point(field_list:tigeropen.push.pb.TradeTickData.mergedVols)
  return mergedvols_;
}

inline const TradeTickData* TradeTickData::internal_default_instance() {
  return &TradeTickData_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace push
}  // namespace tigeropen

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_TradeTickData_2eproto__INCLUDED
