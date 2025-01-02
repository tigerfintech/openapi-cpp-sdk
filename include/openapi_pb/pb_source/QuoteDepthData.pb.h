// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: QuoteDepthData.proto

#ifndef PROTOBUF_QuoteDepthData_2eproto__INCLUDED
#define PROTOBUF_QuoteDepthData_2eproto__INCLUDED

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
void protobuf_AddDesc_QuoteDepthData_2eproto();
void protobuf_InitDefaults_QuoteDepthData_2eproto();
void protobuf_AssignDesc_QuoteDepthData_2eproto();
void protobuf_ShutdownFile_QuoteDepthData_2eproto();

class QuoteDepthData;
class QuoteDepthData_OrderBook;

// ===================================================================

class QuoteDepthData_OrderBook : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tigeropen.push.pb.QuoteDepthData.OrderBook) */ {
 public:
  QuoteDepthData_OrderBook();
  virtual ~QuoteDepthData_OrderBook();

  QuoteDepthData_OrderBook(const QuoteDepthData_OrderBook& from);

  inline QuoteDepthData_OrderBook& operator=(const QuoteDepthData_OrderBook& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const QuoteDepthData_OrderBook& default_instance();

  static const QuoteDepthData_OrderBook* internal_default_instance();

  void Swap(QuoteDepthData_OrderBook* other);

  // implements Message ----------------------------------------------

  inline QuoteDepthData_OrderBook* New() const { return New(NULL); }

  QuoteDepthData_OrderBook* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const QuoteDepthData_OrderBook& from);
  void MergeFrom(const QuoteDepthData_OrderBook& from);
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
  void InternalSwap(QuoteDepthData_OrderBook* other);
  void UnsafeMergeFrom(const QuoteDepthData_OrderBook& from);
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

  // repeated double price = 1;
  int price_size() const;
  void clear_price();
  static const int kPriceFieldNumber = 1;
  double price(int index) const;
  void set_price(int index, double value);
  void add_price(double value);
  const ::google::protobuf::RepeatedField< double >&
      price() const;
  ::google::protobuf::RepeatedField< double >*
      mutable_price();

  // repeated sint64 volume = 2;
  int volume_size() const;
  void clear_volume();
  static const int kVolumeFieldNumber = 2;
  ::google::protobuf::int64 volume(int index) const;
  void set_volume(int index, ::google::protobuf::int64 value);
  void add_volume(::google::protobuf::int64 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
      volume() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
      mutable_volume();

  // repeated uint32 orderCount = 3;
  int ordercount_size() const;
  void clear_ordercount();
  static const int kOrderCountFieldNumber = 3;
  ::google::protobuf::uint32 ordercount(int index) const;
  void set_ordercount(int index, ::google::protobuf::uint32 value);
  void add_ordercount(::google::protobuf::uint32 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
      ordercount() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
      mutable_ordercount();

  // repeated string exchange = 4;
  int exchange_size() const;
  void clear_exchange();
  static const int kExchangeFieldNumber = 4;
  const ::std::string& exchange(int index) const;
  ::std::string* mutable_exchange(int index);
  void set_exchange(int index, const ::std::string& value);
  void set_exchange(int index, const char* value);
  void set_exchange(int index, const char* value, size_t size);
  ::std::string* add_exchange();
  void add_exchange(const ::std::string& value);
  void add_exchange(const char* value);
  void add_exchange(const char* value, size_t size);
  const ::google::protobuf::RepeatedPtrField< ::std::string>& exchange() const;
  ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_exchange();

  // repeated sint64 time = 5;
  int time_size() const;
  void clear_time();
  static const int kTimeFieldNumber = 5;
  ::google::protobuf::int64 time(int index) const;
  void set_time(int index, ::google::protobuf::int64 value);
  void add_time(::google::protobuf::int64 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
      time() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
      mutable_time();

  // @@protoc_insertion_point(class_scope:tigeropen.push.pb.QuoteDepthData.OrderBook)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedField< double > price_;
  mutable int _price_cached_byte_size_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 > volume_;
  mutable int _volume_cached_byte_size_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > ordercount_;
  mutable int _ordercount_cached_byte_size_;
  ::google::protobuf::RepeatedPtrField< ::std::string> exchange_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 > time_;
  mutable int _time_cached_byte_size_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_QuoteDepthData_2eproto_impl();
  friend void  protobuf_AddDesc_QuoteDepthData_2eproto_impl();
  friend void protobuf_AssignDesc_QuoteDepthData_2eproto();
  friend void protobuf_ShutdownFile_QuoteDepthData_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<QuoteDepthData_OrderBook> QuoteDepthData_OrderBook_default_instance_;

// -------------------------------------------------------------------

class QuoteDepthData : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:tigeropen.push.pb.QuoteDepthData) */ {
 public:
  QuoteDepthData();
  virtual ~QuoteDepthData();

  QuoteDepthData(const QuoteDepthData& from);

  inline QuoteDepthData& operator=(const QuoteDepthData& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const QuoteDepthData& default_instance();

  static const QuoteDepthData* internal_default_instance();

  void Swap(QuoteDepthData* other);

  // implements Message ----------------------------------------------

  inline QuoteDepthData* New() const { return New(NULL); }

  QuoteDepthData* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const QuoteDepthData& from);
  void MergeFrom(const QuoteDepthData& from);
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
  void InternalSwap(QuoteDepthData* other);
  void UnsafeMergeFrom(const QuoteDepthData& from);
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

  typedef QuoteDepthData_OrderBook OrderBook;

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

  // optional uint64 timestamp = 2;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 2;
  ::google::protobuf::uint64 timestamp() const;
  void set_timestamp(::google::protobuf::uint64 value);

  // optional .tigeropen.push.pb.QuoteDepthData.OrderBook ask = 3;
  bool has_ask() const;
  void clear_ask();
  static const int kAskFieldNumber = 3;
  const ::tigeropen::push::pb::QuoteDepthData_OrderBook& ask() const;
  ::tigeropen::push::pb::QuoteDepthData_OrderBook* mutable_ask();
  ::tigeropen::push::pb::QuoteDepthData_OrderBook* release_ask();
  void set_allocated_ask(::tigeropen::push::pb::QuoteDepthData_OrderBook* ask);

  // optional .tigeropen.push.pb.QuoteDepthData.OrderBook bid = 4;
  bool has_bid() const;
  void clear_bid();
  static const int kBidFieldNumber = 4;
  const ::tigeropen::push::pb::QuoteDepthData_OrderBook& bid() const;
  ::tigeropen::push::pb::QuoteDepthData_OrderBook* mutable_bid();
  ::tigeropen::push::pb::QuoteDepthData_OrderBook* release_bid();
  void set_allocated_bid(::tigeropen::push::pb::QuoteDepthData_OrderBook* bid);

  // @@protoc_insertion_point(class_scope:tigeropen.push.pb.QuoteDepthData)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr symbol_;
  ::tigeropen::push::pb::QuoteDepthData_OrderBook* ask_;
  ::tigeropen::push::pb::QuoteDepthData_OrderBook* bid_;
  ::google::protobuf::uint64 timestamp_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_QuoteDepthData_2eproto_impl();
  friend void  protobuf_AddDesc_QuoteDepthData_2eproto_impl();
  friend void protobuf_AssignDesc_QuoteDepthData_2eproto();
  friend void protobuf_ShutdownFile_QuoteDepthData_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<QuoteDepthData> QuoteDepthData_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// QuoteDepthData_OrderBook

// repeated double price = 1;
inline int QuoteDepthData_OrderBook::price_size() const {
  return price_.size();
}
inline void QuoteDepthData_OrderBook::clear_price() {
  price_.Clear();
}
inline double QuoteDepthData_OrderBook::price(int index) const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteDepthData.OrderBook.price)
  return price_.Get(index);
}
inline void QuoteDepthData_OrderBook::set_price(int index, double value) {
  price_.Set(index, value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteDepthData.OrderBook.price)
}
inline void QuoteDepthData_OrderBook::add_price(double value) {
  price_.Add(value);
  // @@protoc_insertion_point(field_add:tigeropen.push.pb.QuoteDepthData.OrderBook.price)
}
inline const ::google::protobuf::RepeatedField< double >&
QuoteDepthData_OrderBook::price() const {
  // @@protoc_insertion_point(field_list:tigeropen.push.pb.QuoteDepthData.OrderBook.price)
  return price_;
}
inline ::google::protobuf::RepeatedField< double >*
QuoteDepthData_OrderBook::mutable_price() {
  // @@protoc_insertion_point(field_mutable_list:tigeropen.push.pb.QuoteDepthData.OrderBook.price)
  return &price_;
}

// repeated sint64 volume = 2;
inline int QuoteDepthData_OrderBook::volume_size() const {
  return volume_.size();
}
inline void QuoteDepthData_OrderBook::clear_volume() {
  volume_.Clear();
}
inline ::google::protobuf::int64 QuoteDepthData_OrderBook::volume(int index) const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteDepthData.OrderBook.volume)
  return volume_.Get(index);
}
inline void QuoteDepthData_OrderBook::set_volume(int index, ::google::protobuf::int64 value) {
  volume_.Set(index, value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteDepthData.OrderBook.volume)
}
inline void QuoteDepthData_OrderBook::add_volume(::google::protobuf::int64 value) {
  volume_.Add(value);
  // @@protoc_insertion_point(field_add:tigeropen.push.pb.QuoteDepthData.OrderBook.volume)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
QuoteDepthData_OrderBook::volume() const {
  // @@protoc_insertion_point(field_list:tigeropen.push.pb.QuoteDepthData.OrderBook.volume)
  return volume_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
QuoteDepthData_OrderBook::mutable_volume() {
  // @@protoc_insertion_point(field_mutable_list:tigeropen.push.pb.QuoteDepthData.OrderBook.volume)
  return &volume_;
}

// repeated uint32 orderCount = 3;
inline int QuoteDepthData_OrderBook::ordercount_size() const {
  return ordercount_.size();
}
inline void QuoteDepthData_OrderBook::clear_ordercount() {
  ordercount_.Clear();
}
inline ::google::protobuf::uint32 QuoteDepthData_OrderBook::ordercount(int index) const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteDepthData.OrderBook.orderCount)
  return ordercount_.Get(index);
}
inline void QuoteDepthData_OrderBook::set_ordercount(int index, ::google::protobuf::uint32 value) {
  ordercount_.Set(index, value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteDepthData.OrderBook.orderCount)
}
inline void QuoteDepthData_OrderBook::add_ordercount(::google::protobuf::uint32 value) {
  ordercount_.Add(value);
  // @@protoc_insertion_point(field_add:tigeropen.push.pb.QuoteDepthData.OrderBook.orderCount)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
QuoteDepthData_OrderBook::ordercount() const {
  // @@protoc_insertion_point(field_list:tigeropen.push.pb.QuoteDepthData.OrderBook.orderCount)
  return ordercount_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
QuoteDepthData_OrderBook::mutable_ordercount() {
  // @@protoc_insertion_point(field_mutable_list:tigeropen.push.pb.QuoteDepthData.OrderBook.orderCount)
  return &ordercount_;
}

// repeated string exchange = 4;
inline int QuoteDepthData_OrderBook::exchange_size() const {
  return exchange_.size();
}
inline void QuoteDepthData_OrderBook::clear_exchange() {
  exchange_.Clear();
}
inline const ::std::string& QuoteDepthData_OrderBook::exchange(int index) const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteDepthData.OrderBook.exchange)
  return exchange_.Get(index);
}
inline ::std::string* QuoteDepthData_OrderBook::mutable_exchange(int index) {
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.QuoteDepthData.OrderBook.exchange)
  return exchange_.Mutable(index);
}
inline void QuoteDepthData_OrderBook::set_exchange(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteDepthData.OrderBook.exchange)
  exchange_.Mutable(index)->assign(value);
}
inline void QuoteDepthData_OrderBook::set_exchange(int index, const char* value) {
  exchange_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.QuoteDepthData.OrderBook.exchange)
}
inline void QuoteDepthData_OrderBook::set_exchange(int index, const char* value, size_t size) {
  exchange_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.QuoteDepthData.OrderBook.exchange)
}
inline ::std::string* QuoteDepthData_OrderBook::add_exchange() {
  // @@protoc_insertion_point(field_add_mutable:tigeropen.push.pb.QuoteDepthData.OrderBook.exchange)
  return exchange_.Add();
}
inline void QuoteDepthData_OrderBook::add_exchange(const ::std::string& value) {
  exchange_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:tigeropen.push.pb.QuoteDepthData.OrderBook.exchange)
}
inline void QuoteDepthData_OrderBook::add_exchange(const char* value) {
  exchange_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:tigeropen.push.pb.QuoteDepthData.OrderBook.exchange)
}
inline void QuoteDepthData_OrderBook::add_exchange(const char* value, size_t size) {
  exchange_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:tigeropen.push.pb.QuoteDepthData.OrderBook.exchange)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
QuoteDepthData_OrderBook::exchange() const {
  // @@protoc_insertion_point(field_list:tigeropen.push.pb.QuoteDepthData.OrderBook.exchange)
  return exchange_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
QuoteDepthData_OrderBook::mutable_exchange() {
  // @@protoc_insertion_point(field_mutable_list:tigeropen.push.pb.QuoteDepthData.OrderBook.exchange)
  return &exchange_;
}

// repeated sint64 time = 5;
inline int QuoteDepthData_OrderBook::time_size() const {
  return time_.size();
}
inline void QuoteDepthData_OrderBook::clear_time() {
  time_.Clear();
}
inline ::google::protobuf::int64 QuoteDepthData_OrderBook::time(int index) const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteDepthData.OrderBook.time)
  return time_.Get(index);
}
inline void QuoteDepthData_OrderBook::set_time(int index, ::google::protobuf::int64 value) {
  time_.Set(index, value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteDepthData.OrderBook.time)
}
inline void QuoteDepthData_OrderBook::add_time(::google::protobuf::int64 value) {
  time_.Add(value);
  // @@protoc_insertion_point(field_add:tigeropen.push.pb.QuoteDepthData.OrderBook.time)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
QuoteDepthData_OrderBook::time() const {
  // @@protoc_insertion_point(field_list:tigeropen.push.pb.QuoteDepthData.OrderBook.time)
  return time_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
QuoteDepthData_OrderBook::mutable_time() {
  // @@protoc_insertion_point(field_mutable_list:tigeropen.push.pb.QuoteDepthData.OrderBook.time)
  return &time_;
}

inline const QuoteDepthData_OrderBook* QuoteDepthData_OrderBook::internal_default_instance() {
  return &QuoteDepthData_OrderBook_default_instance_.get();
}
// -------------------------------------------------------------------

// QuoteDepthData

// optional string symbol = 1;
inline void QuoteDepthData::clear_symbol() {
  symbol_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& QuoteDepthData::symbol() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteDepthData.symbol)
  return symbol_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void QuoteDepthData::set_symbol(const ::std::string& value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteDepthData.symbol)
}
inline void QuoteDepthData::set_symbol(const char* value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:tigeropen.push.pb.QuoteDepthData.symbol)
}
inline void QuoteDepthData::set_symbol(const char* value, size_t size) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:tigeropen.push.pb.QuoteDepthData.symbol)
}
inline ::std::string* QuoteDepthData::mutable_symbol() {
  
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.QuoteDepthData.symbol)
  return symbol_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* QuoteDepthData::release_symbol() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.QuoteDepthData.symbol)
  
  return symbol_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void QuoteDepthData::set_allocated_symbol(::std::string* symbol) {
  if (symbol != NULL) {
    
  } else {
    
  }
  symbol_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), symbol);
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.QuoteDepthData.symbol)
}

// optional uint64 timestamp = 2;
inline void QuoteDepthData::clear_timestamp() {
  timestamp_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 QuoteDepthData::timestamp() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteDepthData.timestamp)
  return timestamp_;
}
inline void QuoteDepthData::set_timestamp(::google::protobuf::uint64 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:tigeropen.push.pb.QuoteDepthData.timestamp)
}

// optional .tigeropen.push.pb.QuoteDepthData.OrderBook ask = 3;
inline bool QuoteDepthData::has_ask() const {
  return this != internal_default_instance() && ask_ != NULL;
}
inline void QuoteDepthData::clear_ask() {
  if (GetArenaNoVirtual() == NULL && ask_ != NULL) delete ask_;
  ask_ = NULL;
}
inline const ::tigeropen::push::pb::QuoteDepthData_OrderBook& QuoteDepthData::ask() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteDepthData.ask)
  return ask_ != NULL ? *ask_
                         : *::tigeropen::push::pb::QuoteDepthData_OrderBook::internal_default_instance();
}
inline ::tigeropen::push::pb::QuoteDepthData_OrderBook* QuoteDepthData::mutable_ask() {
  
  if (ask_ == NULL) {
    ask_ = new ::tigeropen::push::pb::QuoteDepthData_OrderBook;
  }
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.QuoteDepthData.ask)
  return ask_;
}
inline ::tigeropen::push::pb::QuoteDepthData_OrderBook* QuoteDepthData::release_ask() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.QuoteDepthData.ask)
  
  ::tigeropen::push::pb::QuoteDepthData_OrderBook* temp = ask_;
  ask_ = NULL;
  return temp;
}
inline void QuoteDepthData::set_allocated_ask(::tigeropen::push::pb::QuoteDepthData_OrderBook* ask) {
  delete ask_;
  ask_ = ask;
  if (ask) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.QuoteDepthData.ask)
}

// optional .tigeropen.push.pb.QuoteDepthData.OrderBook bid = 4;
inline bool QuoteDepthData::has_bid() const {
  return this != internal_default_instance() && bid_ != NULL;
}
inline void QuoteDepthData::clear_bid() {
  if (GetArenaNoVirtual() == NULL && bid_ != NULL) delete bid_;
  bid_ = NULL;
}
inline const ::tigeropen::push::pb::QuoteDepthData_OrderBook& QuoteDepthData::bid() const {
  // @@protoc_insertion_point(field_get:tigeropen.push.pb.QuoteDepthData.bid)
  return bid_ != NULL ? *bid_
                         : *::tigeropen::push::pb::QuoteDepthData_OrderBook::internal_default_instance();
}
inline ::tigeropen::push::pb::QuoteDepthData_OrderBook* QuoteDepthData::mutable_bid() {
  
  if (bid_ == NULL) {
    bid_ = new ::tigeropen::push::pb::QuoteDepthData_OrderBook;
  }
  // @@protoc_insertion_point(field_mutable:tigeropen.push.pb.QuoteDepthData.bid)
  return bid_;
}
inline ::tigeropen::push::pb::QuoteDepthData_OrderBook* QuoteDepthData::release_bid() {
  // @@protoc_insertion_point(field_release:tigeropen.push.pb.QuoteDepthData.bid)
  
  ::tigeropen::push::pb::QuoteDepthData_OrderBook* temp = bid_;
  bid_ = NULL;
  return temp;
}
inline void QuoteDepthData::set_allocated_bid(::tigeropen::push::pb::QuoteDepthData_OrderBook* bid) {
  delete bid_;
  bid_ = bid;
  if (bid) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:tigeropen.push.pb.QuoteDepthData.bid)
}

inline const QuoteDepthData* QuoteDepthData::internal_default_instance() {
  return &QuoteDepthData_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace push
}  // namespace tigeropen

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_QuoteDepthData_2eproto__INCLUDED
