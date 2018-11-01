// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/type/latlng.proto

#ifndef PROTOBUF_INCLUDED_google_2ftype_2flatlng_2eproto
#define PROTOBUF_INCLUDED_google_2ftype_2flatlng_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_google_2ftype_2flatlng_2eproto 

namespace protobuf_google_2ftype_2flatlng_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_google_2ftype_2flatlng_2eproto
namespace google {
namespace type {
class LatLng;
class LatLngDefaultTypeInternal;
extern LatLngDefaultTypeInternal _LatLng_default_instance_;
}  // namespace type
}  // namespace google
namespace google {
namespace protobuf {
template<> ::google::type::LatLng* Arena::CreateMaybeMessage<::google::type::LatLng>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace google {
namespace type {

// ===================================================================

class LatLng : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:google.type.LatLng) */ {
 public:
  LatLng();
  virtual ~LatLng();

  LatLng(const LatLng& from);

  inline LatLng& operator=(const LatLng& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  LatLng(LatLng&& from) noexcept
    : LatLng() {
    *this = ::std::move(from);
  }

  inline LatLng& operator=(LatLng&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const LatLng& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const LatLng* internal_default_instance() {
    return reinterpret_cast<const LatLng*>(
               &_LatLng_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(LatLng* other);
  friend void swap(LatLng& a, LatLng& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline LatLng* New() const final {
    return CreateMaybeMessage<LatLng>(NULL);
  }

  LatLng* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<LatLng>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const LatLng& from);
  void MergeFrom(const LatLng& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(LatLng* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // double latitude = 1;
  void clear_latitude();
  static const int kLatitudeFieldNumber = 1;
  double latitude() const;
  void set_latitude(double value);

  // double longitude = 2;
  void clear_longitude();
  static const int kLongitudeFieldNumber = 2;
  double longitude() const;
  void set_longitude(double value);

  // @@protoc_insertion_point(class_scope:google.type.LatLng)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  double latitude_;
  double longitude_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_google_2ftype_2flatlng_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// LatLng

// double latitude = 1;
inline void LatLng::clear_latitude() {
  latitude_ = 0;
}
inline double LatLng::latitude() const {
  // @@protoc_insertion_point(field_get:google.type.LatLng.latitude)
  return latitude_;
}
inline void LatLng::set_latitude(double value) {
  
  latitude_ = value;
  // @@protoc_insertion_point(field_set:google.type.LatLng.latitude)
}

// double longitude = 2;
inline void LatLng::clear_longitude() {
  longitude_ = 0;
}
inline double LatLng::longitude() const {
  // @@protoc_insertion_point(field_get:google.type.LatLng.longitude)
  return longitude_;
}
inline void LatLng::set_longitude(double value) {
  
  longitude_ = value;
  // @@protoc_insertion_point(field_set:google.type.LatLng.longitude)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace type
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_google_2ftype_2flatlng_2eproto