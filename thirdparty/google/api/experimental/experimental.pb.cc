// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/api/experimental/experimental.proto

#include "google/api/experimental/experimental.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

namespace protobuf_google_2fapi_2fexperimental_2fauthorization_5fconfig_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_google_2fapi_2fexperimental_2fauthorization_5fconfig_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_AuthorizationConfig;
}  // namespace protobuf_google_2fapi_2fexperimental_2fauthorization_5fconfig_2eproto
namespace google {
namespace api {
class ExperimentalDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<Experimental>
      _instance;
} _Experimental_default_instance_;
}  // namespace api
}  // namespace google
namespace protobuf_google_2fapi_2fexperimental_2fexperimental_2eproto {
static void InitDefaultsExperimental() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::google::api::_Experimental_default_instance_;
    new (ptr) ::google::api::Experimental();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::google::api::Experimental::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<1> scc_info_Experimental =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 1, InitDefaultsExperimental}, {
      &protobuf_google_2fapi_2fexperimental_2fauthorization_5fconfig_2eproto::scc_info_AuthorizationConfig.base,}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_Experimental.base);
}

::google::protobuf::Metadata file_level_metadata[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::google::api::Experimental, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::google::api::Experimental, authorization_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::google::api::Experimental)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::google::api::_Experimental_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  AssignDescriptors(
      "google/api/experimental/experimental.proto", schemas, file_default_instances, TableStruct::offsets,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n*google/api/experimental/experimental.p"
      "roto\022\ngoogle.api\032\034google/api/annotations"
      ".proto\0322google/api/experimental/authoriz"
      "ation_config.proto\"F\n\014Experimental\0226\n\rau"
      "thorization\030\010 \001(\0132\037.google.api.Authoriza"
      "tionConfigB[\n\016com.google.apiB\021Experiment"
      "alProtoP\001Z-google.golang.org/genproto/go"
      "ogleapis/api;api\242\002\004GAPIb\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 311);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "google/api/experimental/experimental.proto", &protobuf_RegisterTypes);
  ::protobuf_google_2fapi_2fannotations_2eproto::AddDescriptors();
  ::protobuf_google_2fapi_2fexperimental_2fauthorization_5fconfig_2eproto::AddDescriptors();
}

void AddDescriptors() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_google_2fapi_2fexperimental_2fexperimental_2eproto
namespace google {
namespace api {

// ===================================================================

void Experimental::InitAsDefaultInstance() {
  ::google::api::_Experimental_default_instance_._instance.get_mutable()->authorization_ = const_cast< ::google::api::AuthorizationConfig*>(
      ::google::api::AuthorizationConfig::internal_default_instance());
}
void Experimental::clear_authorization() {
  if (GetArenaNoVirtual() == NULL && authorization_ != NULL) {
    delete authorization_;
  }
  authorization_ = NULL;
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Experimental::kAuthorizationFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Experimental::Experimental()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_google_2fapi_2fexperimental_2fexperimental_2eproto::scc_info_Experimental.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:google.api.Experimental)
}
Experimental::Experimental(const Experimental& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  if (from.has_authorization()) {
    authorization_ = new ::google::api::AuthorizationConfig(*from.authorization_);
  } else {
    authorization_ = NULL;
  }
  // @@protoc_insertion_point(copy_constructor:google.api.Experimental)
}

void Experimental::SharedCtor() {
  authorization_ = NULL;
}

Experimental::~Experimental() {
  // @@protoc_insertion_point(destructor:google.api.Experimental)
  SharedDtor();
}

void Experimental::SharedDtor() {
  if (this != internal_default_instance()) delete authorization_;
}

void Experimental::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ::google::protobuf::Descriptor* Experimental::descriptor() {
  ::protobuf_google_2fapi_2fexperimental_2fexperimental_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_google_2fapi_2fexperimental_2fexperimental_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Experimental& Experimental::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_google_2fapi_2fexperimental_2fexperimental_2eproto::scc_info_Experimental.base);
  return *internal_default_instance();
}


void Experimental::Clear() {
// @@protoc_insertion_point(message_clear_start:google.api.Experimental)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaNoVirtual() == NULL && authorization_ != NULL) {
    delete authorization_;
  }
  authorization_ = NULL;
  _internal_metadata_.Clear();
}

bool Experimental::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:google.api.Experimental)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // .google.api.AuthorizationConfig authorization = 8;
      case 8: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(66u /* 66 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
               input, mutable_authorization()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:google.api.Experimental)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:google.api.Experimental)
  return false;
#undef DO_
}

void Experimental::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:google.api.Experimental)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .google.api.AuthorizationConfig authorization = 8;
  if (this->has_authorization()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      8, this->_internal_authorization(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:google.api.Experimental)
}

::google::protobuf::uint8* Experimental::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:google.api.Experimental)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .google.api.AuthorizationConfig authorization = 8;
  if (this->has_authorization()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageToArray(
        8, this->_internal_authorization(), deterministic, target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:google.api.Experimental)
  return target;
}

size_t Experimental::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:google.api.Experimental)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // .google.api.AuthorizationConfig authorization = 8;
  if (this->has_authorization()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSize(
        *authorization_);
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Experimental::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:google.api.Experimental)
  GOOGLE_DCHECK_NE(&from, this);
  const Experimental* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Experimental>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:google.api.Experimental)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:google.api.Experimental)
    MergeFrom(*source);
  }
}

void Experimental::MergeFrom(const Experimental& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:google.api.Experimental)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.has_authorization()) {
    mutable_authorization()->::google::api::AuthorizationConfig::MergeFrom(from.authorization());
  }
}

void Experimental::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:google.api.Experimental)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Experimental::CopyFrom(const Experimental& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:google.api.Experimental)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Experimental::IsInitialized() const {
  return true;
}

void Experimental::Swap(Experimental* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Experimental::InternalSwap(Experimental* other) {
  using std::swap;
  swap(authorization_, other->authorization_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::google::protobuf::Metadata Experimental::GetMetadata() const {
  protobuf_google_2fapi_2fexperimental_2fexperimental_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_google_2fapi_2fexperimental_2fexperimental_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace api
}  // namespace google
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::google::api::Experimental* Arena::CreateMaybeMessage< ::google::api::Experimental >(Arena* arena) {
  return Arena::CreateInternal< ::google::api::Experimental >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
