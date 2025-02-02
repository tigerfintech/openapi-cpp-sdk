﻿// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: PositionData.proto

#ifndef PROTOBUF_PositionData_2eproto__INCLUDED
#define PROTOBUF_PositionData_2eproto__INCLUDED

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
void protobuf_AddDesc_PositionData_2eproto();
void protobuf_InitDefaults_PositionData_2eproto();
void protobuf_AssignDesc_PositionData_2eproto();
void protobuf_ShutdownFile_PositionData_2eproto();

class PositionData;

// ===================================================================

class PositionData : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tigeropen.push.pb.PositionData) */ {
 public:
  PositionData();
  virtual ~PositionData();

  PositionData(const PositionData& from);

  inline PositionData& operator=(const PositionData& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PositionData& default_instance();

  static const PositionData* internal_default_instance();

  void Swap(PositionData* other);

  // implements Message ----------------------------------------------

  inline PositionData* New() const { return New(NULL); }

  PositionData* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PositionData& from);
  void MergeFrom(const PositionData& from);
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
  void InternalSwap(PositionData* other);
  void UnsafeMergeFrom(const PositionData& from);
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

  // optional string account = 1;
  void clear_account();
  static const int kAccountFieldNumber = 1;
  const ::std::string& account() const;
  void set_account(const ::std::string& value);
  void set_account(const char* value);
  void set_account(const char* value, size_t size);
  ::std::string* mutable_account();
  ::std::string* release_account();
  void set_allocated_account(::std::string* account);

  // optional string symbol = 2;
  void clear_symbol();
  static const int kSymbolFieldNumber = 2;
  const ::std::string& symbol() const;
  void set_symbol(const ::std::string& value);
  void set_symbol(const char* value);
  void set_symbol(const char* value, size_t size);
  ::std::string* mutable_symbol();
  ::std::string* release_symbol();
  void set_allocated_symbol(::std::string* symbol);

  // optional string expiry = 3;
  void clear_expiry();
  static const int kExpiryFieldNumber = 3;
  const ::std::string& expiry() const;
  void set_expiry(const ::std::string& value);
  void set_expiry(const char* value);
  void set_expiry(const char* value, size_t size);
  ::std::string* mutable_expiry();
  ::std::string* release_expiry();
  void set_allocated_expiry(::std::string* expiry);

  // optional string strike = 4;
  void clear_strike();
  static const int kStrikeFieldNumber = 4;
  const ::std::string& strike() const;
  void set_strike(const ::std::string& value);
  void set_strike(const char* value);
  void set_strike(const char* value, size_t size);
  ::std::string* mutable_strike();
  ::std::string* release_strike();
  void set_allocated_strike(::std::string* strike);

  // optional string right = 5;
  void clear_right();
  static const int kRightFieldNumber = 5;
  const ::std::string& right() const;
  void set_right(const ::std::string& value);
  void set_right(const char* value);
  void set_right(const char* value, size_t size);
  ::std::string* mutable_right();
  ::std::string* release_right();
  void set_allocated_right(::std::string* right);

  // optional string identifier = 6;
  void clear_identifier();
  static const int kIdentifierFieldNumber = 6;
  const ::std::string& identifier() const;
  void set_identifier(const ::std::string& value);
  void set_identifier(const char* value);
  void set_identifier(const char* value, size_t size);
  ::std::string* mutable_identifier();
  ::std::string* release_identifier();
  void set_allocated_identifier(::std::string* identifier);

  // optional uint32 multiplier = 7;
  void clear_multiplier();
  static const int kMultiplierFieldNumber = 7;
  ::google::protobuf::uint32 multiplier() const;
  void set_multiplier(::google::protobuf::uint32 value);

  // optional string market = 8;
  void clear_market();
  static const int kMarketFieldNumber = 8;
  const ::std::string& market() const;
  void set_market(const ::std::string& value);
  void set_market(const char* value);
  void set_market(const char* value, size_t size);
  ::std::string* mutable_market();
  ::std::string* release_market();
  void set_allocated_market(::std::string* market);

  // optional string currency = 9;
  void clear_currency();
  static const int kCurrencyFieldNumber = 9;
  const ::std::string& currency() const;
  void set_currency(const ::std::string& value);
  void set_currency(const char* value);
  void set_currency(const char* value, size_t size);
  ::std::string* mutable_currency();
  ::std::string* release_currency();
  void set_allocated_currency(::std::string* currency);

  // optional string segType = 10;
  void clear_segtype();
  static const int kSegTypeFieldNumber = 10;
  const ::std::string& segtype() const;
  void set_segtype(const ::std::string& value);
  void set_segtype(const char* value);
  void set_segtype(const char* value, size_t size);
  ::std::string* mutable_segtype();
  ::std::string* release_segtype();
  void set_allocated_segtype(::std::string* segtype);

  // optional string secType = 11;
  void clear_sectype();
  static const int kSecTypeFieldNumber = 11;
  const ::std::string& sectype() const;
  void set_sectype(const ::std::string& value);
  void set_sectype(const char* value);
  void set_sectype(const char* value, size_t size);
  ::std::string* mutable_sectype();
  ::std::string* release_sectype();
  void set_allocated_sectype(::std::string* sectype);

  // optional sint64 position = 12;
  void clear_position();
  static const int kPositionFieldNumber = 12;
  ::google::protobuf::int64 position() const;
  void set_position(::google::protobuf::int64 value);

  // optional sint32 positionScale = 13;
  void clear_positionscale();
  static const int kPositionScaleFieldNumber = 13;
  ::google::protobuf::int32 positionscale() const;
  void set_positionscale(::google::protobuf::int32 value);

  // optional double averageCost = 14;
  void clear_averagecost();
  static const int kAverageCostFieldNumber = 14;
  double averagecost() const;
  void set_averagecost(double value);

  // optional double latestPrice = 15;
  void clear_latestprice();
  static const int kLatestPriceFieldNumber = 15;
  double latestprice() const;
  void set_latestprice(double value);

  // optional double marketValue = 16;
  void clear_marketvalue();
  static const int kMarketValueFieldNumber = 16;
  double marketvalue() const;
  void set_marketvalue(double value);

  // optional double unrealizedPnl = 17;
  void clear_unrealizedpnl();
  static const int kUnrealizedPnlFieldNumber = 17;
  double unrealizedpnl() const;
  void set_unrealizedpnl(double value);

  // optional string name = 18;
  void clear_name();
  static const int kNameFieldNumber = 18;
  const ::std::string& name() const;
  void set_name(const ::std::string& value);
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  ::std::string* mutable_name();
  ::std::string* release_name();
  void set_allocated_name(::std::string* name);

  // optional uint64 timestamp = 19;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 19;
  ::google::protobuf::uint64 timestamp() const;
  void set_timestamp(::google::protobuf::uint64 value);

  // optional sint64 saleable = 20;
  void clear_saleable();
  static const int kSaleableFieldNumber = 20;
  ::google::protobuf::int64 saleable() const;
  void set_saleable(::google::protobuf::int64 value);

  // optional double positionQty = 21;
  void clear_positionqty();
  static const int kPositionQtyFieldNumber = 21;
  double positionqty() const;
  void set_positionqty(double value);

  // optional double salableQty = 22;
  void clear_salableqty();
  static const int kSalableQtyFieldNumber = 22;
  double salableqty() const;
  void set_salableqty(double value);

  // @@protoc_insertion_point(class_scope:tigeropen.push.pb.PositionData)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr account_;
  ::google::protobuf::internal::ArenaStringPtr symbol_;
  ::google::protobuf::internal::ArenaStringPtr expiry_;
  ::google::protobuf::internal::ArenaStringPtr strike_;
  ::google::protobuf::internal::ArenaStringPtr right_;
  ::google::protobuf::internal::ArenaStringPtr identifier_;
  ::google::protobuf::internal::ArenaStringPtr market_;
  ::google::protobuf::internal::ArenaStringPtr currency_;
  ::google::protobuf::internal::ArenaStringPtr segtype_;
  ::google::protobuf::internal::ArenaStringPtr sectype_;
  ::google::protobuf::internal::ArenaStringPtr name_;
  ::google::protobuf::uint32 multiplier_;
  ::google::protobuf::int32 positionscale_;
  ::google::protobuf::int64 position_;
  double averagecost_;
  double latestprice_;
  double marketvalue_;
  double unrealizedpnl_;
  ::google::protobuf::uint64 timestamp_;
  ::google::protobuf::int64 saleable_;
  double positionqty_;
  double salableqty_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_PositionData_2eproto_impl();
  friend void  protobuf_AddDesc_PositionData_2eproto_impl();
  friend void protobuf_AssignDesc_PositionData_2eproto();
  friend void protobuf_ShutdownFile_PositionData_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<PositionData> PositionData_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// PositionData

// optional string account = 1;
inline void PositionData::clear_account() {
  account_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PositionData::account() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.account)
  return account_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_account(const ::std::string& value) {
  
  account_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.account)
}
inline void PositionData::set_account(const char* value) {
  
  account_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.PositionData.account)
}
inline void PositionData::set_account(const char* value, size_t size) {
  
  account_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.PositionData.account)
}
inline ::std::string* PositionData::mutable_account() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.PositionData.account)
  return account_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PositionData::release_account() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.PositionData.account)
  
  return account_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_allocated_account(::std::string* account) {
  if (account != NULL) {
    
  } else {
    
  }
  account_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), account);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.PositionData.account)
}

// optional string symbol = 2;
inline void PositionData::clear_symbol() {
  symbol_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PositionData::symbol() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.symbol)
  return symbol_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_symbol(const ::std::string& value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.symbol)
}
inline void PositionData::set_symbol(const char* value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.PositionData.symbol)
}
inline void PositionData::set_symbol(const char* value, size_t size) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.PositionData.symbol)
}
inline ::std::string* PositionData::mutable_symbol() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.PositionData.symbol)
  return symbol_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PositionData::release_symbol() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.PositionData.symbol)
  
  return symbol_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_allocated_symbol(::std::string* symbol) {
  if (symbol != NULL) {
    
  } else {
    
  }
  symbol_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), symbol);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.PositionData.symbol)
}

// optional string expiry = 3;
inline void PositionData::clear_expiry() {
  expiry_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PositionData::expiry() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.expiry)
  return expiry_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_expiry(const ::std::string& value) {
  
  expiry_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.expiry)
}
inline void PositionData::set_expiry(const char* value) {
  
  expiry_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.PositionData.expiry)
}
inline void PositionData::set_expiry(const char* value, size_t size) {
  
  expiry_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.PositionData.expiry)
}
inline ::std::string* PositionData::mutable_expiry() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.PositionData.expiry)
  return expiry_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PositionData::release_expiry() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.PositionData.expiry)
  
  return expiry_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_allocated_expiry(::std::string* expiry) {
  if (expiry != NULL) {
    
  } else {
    
  }
  expiry_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), expiry);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.PositionData.expiry)
}

// optional string strike = 4;
inline void PositionData::clear_strike() {
  strike_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PositionData::strike() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.strike)
  return strike_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_strike(const ::std::string& value) {
  
  strike_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.strike)
}
inline void PositionData::set_strike(const char* value) {
  
  strike_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.PositionData.strike)
}
inline void PositionData::set_strike(const char* value, size_t size) {
  
  strike_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.PositionData.strike)
}
inline ::std::string* PositionData::mutable_strike() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.PositionData.strike)
  return strike_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PositionData::release_strike() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.PositionData.strike)
  
  return strike_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_allocated_strike(::std::string* strike) {
  if (strike != NULL) {
    
  } else {
    
  }
  strike_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), strike);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.PositionData.strike)
}

// optional string right = 5;
inline void PositionData::clear_right() {
  right_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PositionData::right() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.right)
  return right_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_right(const ::std::string& value) {
  
  right_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.right)
}
inline void PositionData::set_right(const char* value) {
  
  right_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.PositionData.right)
}
inline void PositionData::set_right(const char* value, size_t size) {
  
  right_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.PositionData.right)
}
inline ::std::string* PositionData::mutable_right() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.PositionData.right)
  return right_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PositionData::release_right() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.PositionData.right)
  
  return right_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_allocated_right(::std::string* right) {
  if (right != NULL) {
    
  } else {
    
  }
  right_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), right);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.PositionData.right)
}

// optional string identifier = 6;
inline void PositionData::clear_identifier() {
  identifier_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PositionData::identifier() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.identifier)
  return identifier_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_identifier(const ::std::string& value) {
  
  identifier_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.identifier)
}
inline void PositionData::set_identifier(const char* value) {
  
  identifier_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.PositionData.identifier)
}
inline void PositionData::set_identifier(const char* value, size_t size) {
  
  identifier_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.PositionData.identifier)
}
inline ::std::string* PositionData::mutable_identifier() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.PositionData.identifier)
  return identifier_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PositionData::release_identifier() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.PositionData.identifier)
  
  return identifier_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_allocated_identifier(::std::string* identifier) {
  if (identifier != NULL) {
    
  } else {
    
  }
  identifier_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), identifier);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.PositionData.identifier)
}

// optional uint32 multiplier = 7;
inline void PositionData::clear_multiplier() {
  multiplier_ = 0u;
}
inline ::google::protobuf::uint32 PositionData::multiplier() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.multiplier)
  return multiplier_;
}
inline void PositionData::set_multiplier(::google::protobuf::uint32 value) {
  
  multiplier_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.multiplier)
}

// optional string market = 8;
inline void PositionData::clear_market() {
  market_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PositionData::market() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.market)
  return market_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_market(const ::std::string& value) {
  
  market_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.market)
}
inline void PositionData::set_market(const char* value) {
  
  market_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.PositionData.market)
}
inline void PositionData::set_market(const char* value, size_t size) {
  
  market_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.PositionData.market)
}
inline ::std::string* PositionData::mutable_market() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.PositionData.market)
  return market_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PositionData::release_market() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.PositionData.market)
  
  return market_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_allocated_market(::std::string* market) {
  if (market != NULL) {
    
  } else {
    
  }
  market_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), market);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.PositionData.market)
}

// optional string currency = 9;
inline void PositionData::clear_currency() {
  currency_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PositionData::currency() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.currency)
  return currency_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_currency(const ::std::string& value) {
  
  currency_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.currency)
}
inline void PositionData::set_currency(const char* value) {
  
  currency_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.PositionData.currency)
}
inline void PositionData::set_currency(const char* value, size_t size) {
  
  currency_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.PositionData.currency)
}
inline ::std::string* PositionData::mutable_currency() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.PositionData.currency)
  return currency_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PositionData::release_currency() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.PositionData.currency)
  
  return currency_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_allocated_currency(::std::string* currency) {
  if (currency != NULL) {
    
  } else {
    
  }
  currency_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), currency);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.PositionData.currency)
}

// optional string segType = 10;
inline void PositionData::clear_segtype() {
  segtype_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PositionData::segtype() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.segType)
  return segtype_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_segtype(const ::std::string& value) {
  
  segtype_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.segType)
}
inline void PositionData::set_segtype(const char* value) {
  
  segtype_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.PositionData.segType)
}
inline void PositionData::set_segtype(const char* value, size_t size) {
  
  segtype_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.PositionData.segType)
}
inline ::std::string* PositionData::mutable_segtype() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.PositionData.segType)
  return segtype_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PositionData::release_segtype() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.PositionData.segType)
  
  return segtype_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_allocated_segtype(::std::string* segtype) {
  if (segtype != NULL) {
    
  } else {
    
  }
  segtype_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), segtype);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.PositionData.segType)
}

// optional string secType = 11;
inline void PositionData::clear_sectype() {
  sectype_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PositionData::sectype() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.secType)
  return sectype_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_sectype(const ::std::string& value) {
  
  sectype_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.secType)
}
inline void PositionData::set_sectype(const char* value) {
  
  sectype_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.PositionData.secType)
}
inline void PositionData::set_sectype(const char* value, size_t size) {
  
  sectype_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.PositionData.secType)
}
inline ::std::string* PositionData::mutable_sectype() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.PositionData.secType)
  return sectype_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PositionData::release_sectype() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.PositionData.secType)
  
  return sectype_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_allocated_sectype(::std::string* sectype) {
  if (sectype != NULL) {
    
  } else {
    
  }
  sectype_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), sectype);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.PositionData.secType)
}

// optional sint64 position = 12;
inline void PositionData::clear_position() {
  position_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 PositionData::position() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.position)
  return position_;
}
inline void PositionData::set_position(::google::protobuf::int64 value) {
  
  position_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.position)
}

// optional sint32 positionScale = 13;
inline void PositionData::clear_positionscale() {
  positionscale_ = 0;
}
inline ::google::protobuf::int32 PositionData::positionscale() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.positionScale)
  return positionscale_;
}
inline void PositionData::set_positionscale(::google::protobuf::int32 value) {
  
  positionscale_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.positionScale)
}

// optional double averageCost = 14;
inline void PositionData::clear_averagecost() {
  averagecost_ = 0;
}
inline double PositionData::averagecost() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.averageCost)
  return averagecost_;
}
inline void PositionData::set_averagecost(double value) {
  
  averagecost_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.averageCost)
}

// optional double latestPrice = 15;
inline void PositionData::clear_latestprice() {
  latestprice_ = 0;
}
inline double PositionData::latestprice() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.latestPrice)
  return latestprice_;
}
inline void PositionData::set_latestprice(double value) {
  
  latestprice_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.latestPrice)
}

// optional double marketValue = 16;
inline void PositionData::clear_marketvalue() {
  marketvalue_ = 0;
}
inline double PositionData::marketvalue() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.marketValue)
  return marketvalue_;
}
inline void PositionData::set_marketvalue(double value) {
  
  marketvalue_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.marketValue)
}

// optional double unrealizedPnl = 17;
inline void PositionData::clear_unrealizedpnl() {
  unrealizedpnl_ = 0;
}
inline double PositionData::unrealizedpnl() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.unrealizedPnl)
  return unrealizedpnl_;
}
inline void PositionData::set_unrealizedpnl(double value) {
  
  unrealizedpnl_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.unrealizedPnl)
}

// optional string name = 18;
inline void PositionData::clear_name() {
  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PositionData::name() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.name)
  return name_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_name(const ::std::string& value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.name)
}
inline void PositionData::set_name(const char* value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.PositionData.name)
}
inline void PositionData::set_name(const char* value, size_t size) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.PositionData.name)
}
inline ::std::string* PositionData::mutable_name() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.PositionData.name)
  return name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PositionData::release_name() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.PositionData.name)
  
  return name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PositionData::set_allocated_name(::std::string* name) {
  if (name != NULL) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.PositionData.name)
}

// optional uint64 timestamp = 19;
inline void PositionData::clear_timestamp() {
  timestamp_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 PositionData::timestamp() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.timestamp)
  return timestamp_;
}
inline void PositionData::set_timestamp(::google::protobuf::uint64 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.timestamp)
}

// optional sint64 saleable = 20;
inline void PositionData::clear_saleable() {
  saleable_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 PositionData::saleable() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.saleable)
  return saleable_;
}
inline void PositionData::set_saleable(::google::protobuf::int64 value) {
  
  saleable_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.saleable)
}

// optional double positionQty = 21;
inline void PositionData::clear_positionqty() {
  positionqty_ = 0;
}
inline double PositionData::positionqty() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.positionQty)
  return positionqty_;
}
inline void PositionData::set_positionqty(double value) {
  
  positionqty_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.positionQty)
}

// optional double salableQty = 22;
inline void PositionData::clear_salableqty() {
  salableqty_ = 0;
}
inline double PositionData::salableqty() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.PositionData.salableQty)
  return salableqty_;
}
inline void PositionData::set_salableqty(double value) {
  
  salableqty_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.PositionData.salableQty)
}

inline const PositionData* PositionData::internal_default_instance() {
  return &PositionData_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace push
}  // namespace tigeropen

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_PositionData_2eproto__INCLUDED
