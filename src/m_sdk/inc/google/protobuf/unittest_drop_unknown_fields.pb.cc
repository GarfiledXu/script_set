// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/unittest_drop_unknown_fields.proto

#include "google/protobuf/unittest_drop_unknown_fields.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace unittest_drop_unknown_fields {
class FooDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Foo> _instance;
} _Foo_default_instance_;
class FooWithExtraFieldsDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<FooWithExtraFields> _instance;
} _FooWithExtraFields_default_instance_;
}  // namespace unittest_drop_unknown_fields
static void InitDefaultsscc_info_Foo_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::unittest_drop_unknown_fields::_Foo_default_instance_;
    new (ptr) ::unittest_drop_unknown_fields::Foo();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::unittest_drop_unknown_fields::Foo::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Foo_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_Foo_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto}, {}};

static void InitDefaultsscc_info_FooWithExtraFields_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::unittest_drop_unknown_fields::_FooWithExtraFields_default_instance_;
    new (ptr) ::unittest_drop_unknown_fields::FooWithExtraFields();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::unittest_drop_unknown_fields::FooWithExtraFields::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_FooWithExtraFields_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_FooWithExtraFields_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto[2];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::unittest_drop_unknown_fields::Foo, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::unittest_drop_unknown_fields::Foo, int32_value_),
  PROTOBUF_FIELD_OFFSET(::unittest_drop_unknown_fields::Foo, enum_value_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::unittest_drop_unknown_fields::FooWithExtraFields, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::unittest_drop_unknown_fields::FooWithExtraFields, int32_value_),
  PROTOBUF_FIELD_OFFSET(::unittest_drop_unknown_fields::FooWithExtraFields, enum_value_),
  PROTOBUF_FIELD_OFFSET(::unittest_drop_unknown_fields::FooWithExtraFields, extra_int32_value_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::unittest_drop_unknown_fields::Foo)},
  { 7, -1, sizeof(::unittest_drop_unknown_fields::FooWithExtraFields)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::unittest_drop_unknown_fields::_Foo_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::unittest_drop_unknown_fields::_FooWithExtraFields_default_instance_),
};

const char descriptor_table_protodef_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n2google/protobuf/unittest_drop_unknown_"
  "fields.proto\022\034unittest_drop_unknown_fiel"
  "ds\"\205\001\n\003Foo\022\023\n\013int32_value\030\001 \001(\005\022@\n\nenum_"
  "value\030\002 \001(\0162,.unittest_drop_unknown_fiel"
  "ds.Foo.NestedEnum\"\'\n\nNestedEnum\022\007\n\003FOO\020\000"
  "\022\007\n\003BAR\020\001\022\007\n\003BAZ\020\002\"\307\001\n\022FooWithExtraField"
  "s\022\023\n\013int32_value\030\001 \001(\005\022O\n\nenum_value\030\002 \001"
  "(\0162;.unittest_drop_unknown_fields.FooWit"
  "hExtraFields.NestedEnum\022\031\n\021extra_int32_v"
  "alue\030\003 \001(\005\"0\n\nNestedEnum\022\007\n\003FOO\020\000\022\007\n\003BAR"
  "\020\001\022\007\n\003BAZ\020\002\022\007\n\003QUX\020\003B,\242\002\014DropUnknowns\252\002\032"
  "Google.Protobuf.TestProtosb\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto_sccs[2] = {
  &scc_info_Foo_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto.base,
  &scc_info_FooWithExtraFields_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto = {
  false, false, descriptor_table_protodef_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto, "google/protobuf/unittest_drop_unknown_fields.proto", 474,
  &descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto_once, descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto_sccs, descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto_deps, 2, 0,
  schemas, file_default_instances, TableStruct_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto::offsets,
  file_level_metadata_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto, 2, file_level_enum_descriptors_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto, file_level_service_descriptors_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto)), true);
namespace unittest_drop_unknown_fields {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Foo_NestedEnum_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto);
  return file_level_enum_descriptors_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto[0];
}
bool Foo_NestedEnum_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr Foo_NestedEnum Foo::FOO;
constexpr Foo_NestedEnum Foo::BAR;
constexpr Foo_NestedEnum Foo::BAZ;
constexpr Foo_NestedEnum Foo::NestedEnum_MIN;
constexpr Foo_NestedEnum Foo::NestedEnum_MAX;
constexpr int Foo::NestedEnum_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* FooWithExtraFields_NestedEnum_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto);
  return file_level_enum_descriptors_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto[1];
}
bool FooWithExtraFields_NestedEnum_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr FooWithExtraFields_NestedEnum FooWithExtraFields::FOO;
constexpr FooWithExtraFields_NestedEnum FooWithExtraFields::BAR;
constexpr FooWithExtraFields_NestedEnum FooWithExtraFields::BAZ;
constexpr FooWithExtraFields_NestedEnum FooWithExtraFields::QUX;
constexpr FooWithExtraFields_NestedEnum FooWithExtraFields::NestedEnum_MIN;
constexpr FooWithExtraFields_NestedEnum FooWithExtraFields::NestedEnum_MAX;
constexpr int FooWithExtraFields::NestedEnum_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

void Foo::InitAsDefaultInstance() {
}
class Foo::_Internal {
 public:
};

Foo::Foo(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:unittest_drop_unknown_fields.Foo)
}
Foo::Foo(const Foo& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&int32_value_, &from.int32_value_,
    static_cast<size_t>(reinterpret_cast<char*>(&enum_value_) -
    reinterpret_cast<char*>(&int32_value_)) + sizeof(enum_value_));
  // @@protoc_insertion_point(copy_constructor:unittest_drop_unknown_fields.Foo)
}

void Foo::SharedCtor() {
  ::memset(&int32_value_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&enum_value_) -
      reinterpret_cast<char*>(&int32_value_)) + sizeof(enum_value_));
}

Foo::~Foo() {
  // @@protoc_insertion_point(destructor:unittest_drop_unknown_fields.Foo)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void Foo::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void Foo::ArenaDtor(void* object) {
  Foo* _this = reinterpret_cast< Foo* >(object);
  (void)_this;
}
void Foo::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Foo::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Foo& Foo::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Foo_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto.base);
  return *internal_default_instance();
}


void Foo::Clear() {
// @@protoc_insertion_point(message_clear_start:unittest_drop_unknown_fields.Foo)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&int32_value_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&enum_value_) -
      reinterpret_cast<char*>(&int32_value_)) + sizeof(enum_value_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Foo::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int32 int32_value = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          int32_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .unittest_drop_unknown_fields.Foo.NestedEnum enum_value = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_enum_value(static_cast<::unittest_drop_unknown_fields::Foo_NestedEnum>(val));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Foo::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:unittest_drop_unknown_fields.Foo)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 int32_value = 1;
  if (this->int32_value() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_int32_value(), target);
  }

  // .unittest_drop_unknown_fields.Foo.NestedEnum enum_value = 2;
  if (this->enum_value() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      2, this->_internal_enum_value(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:unittest_drop_unknown_fields.Foo)
  return target;
}

size_t Foo::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:unittest_drop_unknown_fields.Foo)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 int32_value = 1;
  if (this->int32_value() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_int32_value());
  }

  // .unittest_drop_unknown_fields.Foo.NestedEnum enum_value = 2;
  if (this->enum_value() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_enum_value());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Foo::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:unittest_drop_unknown_fields.Foo)
  GOOGLE_DCHECK_NE(&from, this);
  const Foo* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Foo>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:unittest_drop_unknown_fields.Foo)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:unittest_drop_unknown_fields.Foo)
    MergeFrom(*source);
  }
}

void Foo::MergeFrom(const Foo& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:unittest_drop_unknown_fields.Foo)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.int32_value() != 0) {
    _internal_set_int32_value(from._internal_int32_value());
  }
  if (from.enum_value() != 0) {
    _internal_set_enum_value(from._internal_enum_value());
  }
}

void Foo::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:unittest_drop_unknown_fields.Foo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Foo::CopyFrom(const Foo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:unittest_drop_unknown_fields.Foo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Foo::IsInitialized() const {
  return true;
}

void Foo::InternalSwap(Foo* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Foo, enum_value_)
      + sizeof(Foo::enum_value_)
      - PROTOBUF_FIELD_OFFSET(Foo, int32_value_)>(
          reinterpret_cast<char*>(&int32_value_),
          reinterpret_cast<char*>(&other->int32_value_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Foo::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void FooWithExtraFields::InitAsDefaultInstance() {
}
class FooWithExtraFields::_Internal {
 public:
};

FooWithExtraFields::FooWithExtraFields(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:unittest_drop_unknown_fields.FooWithExtraFields)
}
FooWithExtraFields::FooWithExtraFields(const FooWithExtraFields& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&int32_value_, &from.int32_value_,
    static_cast<size_t>(reinterpret_cast<char*>(&extra_int32_value_) -
    reinterpret_cast<char*>(&int32_value_)) + sizeof(extra_int32_value_));
  // @@protoc_insertion_point(copy_constructor:unittest_drop_unknown_fields.FooWithExtraFields)
}

void FooWithExtraFields::SharedCtor() {
  ::memset(&int32_value_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&extra_int32_value_) -
      reinterpret_cast<char*>(&int32_value_)) + sizeof(extra_int32_value_));
}

FooWithExtraFields::~FooWithExtraFields() {
  // @@protoc_insertion_point(destructor:unittest_drop_unknown_fields.FooWithExtraFields)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void FooWithExtraFields::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void FooWithExtraFields::ArenaDtor(void* object) {
  FooWithExtraFields* _this = reinterpret_cast< FooWithExtraFields* >(object);
  (void)_this;
}
void FooWithExtraFields::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void FooWithExtraFields::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const FooWithExtraFields& FooWithExtraFields::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_FooWithExtraFields_google_2fprotobuf_2funittest_5fdrop_5funknown_5ffields_2eproto.base);
  return *internal_default_instance();
}


void FooWithExtraFields::Clear() {
// @@protoc_insertion_point(message_clear_start:unittest_drop_unknown_fields.FooWithExtraFields)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&int32_value_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&extra_int32_value_) -
      reinterpret_cast<char*>(&int32_value_)) + sizeof(extra_int32_value_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* FooWithExtraFields::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int32 int32_value = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          int32_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .unittest_drop_unknown_fields.FooWithExtraFields.NestedEnum enum_value = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_enum_value(static_cast<::unittest_drop_unknown_fields::FooWithExtraFields_NestedEnum>(val));
        } else goto handle_unusual;
        continue;
      // int32 extra_int32_value = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          extra_int32_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* FooWithExtraFields::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:unittest_drop_unknown_fields.FooWithExtraFields)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 int32_value = 1;
  if (this->int32_value() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_int32_value(), target);
  }

  // .unittest_drop_unknown_fields.FooWithExtraFields.NestedEnum enum_value = 2;
  if (this->enum_value() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      2, this->_internal_enum_value(), target);
  }

  // int32 extra_int32_value = 3;
  if (this->extra_int32_value() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(3, this->_internal_extra_int32_value(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:unittest_drop_unknown_fields.FooWithExtraFields)
  return target;
}

size_t FooWithExtraFields::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:unittest_drop_unknown_fields.FooWithExtraFields)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 int32_value = 1;
  if (this->int32_value() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_int32_value());
  }

  // .unittest_drop_unknown_fields.FooWithExtraFields.NestedEnum enum_value = 2;
  if (this->enum_value() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_enum_value());
  }

  // int32 extra_int32_value = 3;
  if (this->extra_int32_value() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_extra_int32_value());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void FooWithExtraFields::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:unittest_drop_unknown_fields.FooWithExtraFields)
  GOOGLE_DCHECK_NE(&from, this);
  const FooWithExtraFields* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<FooWithExtraFields>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:unittest_drop_unknown_fields.FooWithExtraFields)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:unittest_drop_unknown_fields.FooWithExtraFields)
    MergeFrom(*source);
  }
}

void FooWithExtraFields::MergeFrom(const FooWithExtraFields& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:unittest_drop_unknown_fields.FooWithExtraFields)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.int32_value() != 0) {
    _internal_set_int32_value(from._internal_int32_value());
  }
  if (from.enum_value() != 0) {
    _internal_set_enum_value(from._internal_enum_value());
  }
  if (from.extra_int32_value() != 0) {
    _internal_set_extra_int32_value(from._internal_extra_int32_value());
  }
}

void FooWithExtraFields::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:unittest_drop_unknown_fields.FooWithExtraFields)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void FooWithExtraFields::CopyFrom(const FooWithExtraFields& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:unittest_drop_unknown_fields.FooWithExtraFields)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FooWithExtraFields::IsInitialized() const {
  return true;
}

void FooWithExtraFields::InternalSwap(FooWithExtraFields* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(FooWithExtraFields, extra_int32_value_)
      + sizeof(FooWithExtraFields::extra_int32_value_)
      - PROTOBUF_FIELD_OFFSET(FooWithExtraFields, int32_value_)>(
          reinterpret_cast<char*>(&int32_value_),
          reinterpret_cast<char*>(&other->int32_value_));
}

::PROTOBUF_NAMESPACE_ID::Metadata FooWithExtraFields::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace unittest_drop_unknown_fields
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::unittest_drop_unknown_fields::Foo* Arena::CreateMaybeMessage< ::unittest_drop_unknown_fields::Foo >(Arena* arena) {
  return Arena::CreateMessageInternal< ::unittest_drop_unknown_fields::Foo >(arena);
}
template<> PROTOBUF_NOINLINE ::unittest_drop_unknown_fields::FooWithExtraFields* Arena::CreateMaybeMessage< ::unittest_drop_unknown_fields::FooWithExtraFields >(Arena* arena) {
  return Arena::CreateMessageInternal< ::unittest_drop_unknown_fields::FooWithExtraFields >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
