/** @file
 *
 * C Wrapper Layer of Protocol Buffers Language library.
 * Copyright 2019, Huang Qiangxiong <qiangxiong.huang@qq.com>
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

/* The packet-protobuf dissector needs get information from *.proto files for dissecting
 * protobuf packet correctly. The information includes:
 *   - The names of MESSAGE, ENUM, FIELD, ENUM_VALUE;
 *   - The data type of FIELD which assuring the value of protobuf field of packet can be dissected correctly.
 *
 * At present, we use C Protocol Buffers Language Parser which generated by protobuf_lang_parser.lemon and protobuf_lang_scanner.l.
 * Because wireshark is mainly implemented in plain ANSI C but the official protobuf library is implemented in C++ language.
 *
 * One day, if C++ library is allowed, we can create a protobuf-helper.cpp file, that invoking official protobuf C++ library directly,
 * to replace protobuf-helper.c. The packet-protobuf.c can keep unchanged.
 */

#ifndef __PROTOBUF_HELPER_H__
#define __PROTOBUF_HELPER_H__

#include <epan/value_string.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Protobuf field type. Must be kept in sync with FieldType of protobuf wire_format_lite.h */
#define protobuf_field_type_VALUE_STRING_LIST(XXX)    \
    XXX(PROTOBUF_TYPE_NONE, 0, "") \
    XXX(PROTOBUF_TYPE_DOUBLE, 1, "double")  \
    XXX(PROTOBUF_TYPE_FLOAT, 2, "float")   \
    XXX(PROTOBUF_TYPE_INT64, 3, "int64") \
    XXX(PROTOBUF_TYPE_UINT64, 4, "uint64") \
    XXX(PROTOBUF_TYPE_INT32, 5, "int32") \
    XXX(PROTOBUF_TYPE_FIXED64, 6, "fixed64") \
    XXX(PROTOBUF_TYPE_FIXED32, 7, "fixed32")  \
    XXX(PROTOBUF_TYPE_BOOL, 8, "bool")  \
    XXX(PROTOBUF_TYPE_STRING, 9, "string")  \
    XXX(PROTOBUF_TYPE_GROUP, 10, "group")  \
    XXX(PROTOBUF_TYPE_MESSAGE, 11, "message")  \
    XXX(PROTOBUF_TYPE_BYTES, 12, "bytes")  \
    XXX(PROTOBUF_TYPE_UINT32, 13, "uint32")  \
    XXX(PROTOBUF_TYPE_ENUM, 14, "enum")  \
    XXX(PROTOBUF_TYPE_SFIXED32, 15, "sfixed32")  \
    XXX(PROTOBUF_TYPE_SFIXED64, 16, "sfixed64")  \
    XXX(PROTOBUF_TYPE_SINT32, 17, "sint32")  \
    XXX(PROTOBUF_TYPE_SINT64, 18, "sint64")

#define PROTOBUF_MAX_FIELD_TYPE 18

VALUE_STRING_ENUM(protobuf_field_type);
VALUE_STRING_ARRAY_GLOBAL_DCL(protobuf_field_type);

/* like google::protobuf::DescriptorPool of protobuf cpp library */
typedef struct PbwDescriptorPool PbwDescriptorPool;
/* like google::protobuf::MethodDescriptor of protobuf cpp library */
typedef struct PbwMethodDescriptor PbwMethodDescriptor;
/* like google::protobuf::Descriptor of protobuf cpp library */
typedef struct PbwDescriptor PbwDescriptor;
/* like google::protobuf::FieldDescriptor of protobuf cpp library */
typedef struct PbwFieldDescriptor PbwFieldDescriptor;
/* like google::protobuf::EnumDescriptor of protobuf cpp library */
typedef struct PbwEnumDescriptor PbwEnumDescriptor;
/* like google::protobuf::EnumValueDescriptor of protobuf cpp library */
typedef struct PbwEnumValueDescriptor PbwEnumValueDescriptor;

typedef void(*pbw_report_error_cb_t)(const char *msg_format, ...);

/**
 Reinitialize PbwDescriptorPool according to proto files directories.
 @param pool  The output DescriptorPool will be created. If *pool is not NULL, it will free it first.
 @param directories  The root directories containing proto files. Must end with NULL element.
 @param error_cb The error reporter callback function. */
void
pbw_reinit_DescriptorPool(PbwDescriptorPool** pool, const char** directories, pbw_report_error_cb_t error_cb);

/* load a proto file, return 0 if successed */
int
pbw_load_proto_file(PbwDescriptorPool* pool, const char* filename);

/* like DescriptorPool::FindMethodByName */
const PbwMethodDescriptor*
pbw_DescriptorPool_FindMethodByName(const PbwDescriptorPool* pool, const char* name);

/* like MethodDescriptor::name() */
const char*
pbw_MethodDescriptor_name(const PbwMethodDescriptor* method);

/* like MethodDescriptor::full_name() */
const char*
pbw_MethodDescriptor_full_name(const PbwMethodDescriptor* method);

/* like MethodDescriptor::input_type() */
const PbwDescriptor*
pbw_MethodDescriptor_input_type(const PbwMethodDescriptor* method);

/* like MethodDescriptor::output_type() */
const PbwDescriptor*
pbw_MethodDescriptor_output_type(const PbwMethodDescriptor* method);

/* like DescriptorPool::FindMessageTypeByName() */
const PbwDescriptor*
pbw_DescriptorPool_FindMessageTypeByName(const PbwDescriptorPool* pool, const char* name);

/* like Descriptor::name() */
const char*
pbw_Descriptor_name(const PbwDescriptor* message);

/* like Descriptor::full_name() */
const char*
pbw_Descriptor_full_name(const PbwDescriptor* message);

/* like Descriptor::field_count() */
int
pbw_Descriptor_field_count(const PbwDescriptor* message);

/* like Descriptor::field() */
const PbwFieldDescriptor*
pbw_Descriptor_field(const PbwDescriptor* message, int field_index);

/* like Descriptor::FindFieldByNumber() */
const PbwFieldDescriptor*
pbw_Descriptor_FindFieldByNumber(const PbwDescriptor* message, int number);

/* like Descriptor::FindFieldByName() */
const PbwFieldDescriptor*
pbw_Descriptor_FindFieldByName(const PbwDescriptor* message, const char* name);

/* like FieldDescriptor::full_name() */
const char*
pbw_FieldDescriptor_full_name(const PbwFieldDescriptor* field);

/* like FieldDescriptor::name() */
const char*
pbw_FieldDescriptor_name(const PbwFieldDescriptor* field);

/* like FieldDescriptor::number() */
int
pbw_FieldDescriptor_number(const PbwFieldDescriptor* field);

/* like FieldDescriptor::type() */
int
pbw_FieldDescriptor_type(const PbwFieldDescriptor* field);

/* like FieldDescriptor::is_repeated() */
int
pbw_FieldDescriptor_is_repeated(const PbwFieldDescriptor* field);

/* like FieldDescriptor::is_packed() */
int
pbw_FieldDescriptor_is_packed(const PbwFieldDescriptor* field);

/* like FieldDescriptor::typeName() */
const char*
pbw_FieldDescriptor_typeName(int field_type);

/* like FieldDescriptor::message_type() */
const PbwDescriptor*
pbw_FieldDescriptor_message_type(const PbwFieldDescriptor* field);

/* like FieldDescriptor::enum_type() */
const PbwEnumDescriptor*
pbw_FieldDescriptor_enum_type(const PbwFieldDescriptor* field);

/* like FieldDescriptor::is_required() */
gboolean
pbw_FieldDescriptor_is_required(const PbwFieldDescriptor* field);

/* like FieldDescriptor::has_default_value().
 * Does this field have an explicitly-declared default value? */
gboolean
pbw_FieldDescriptor_has_default_value(const PbwFieldDescriptor* field);

/* like FieldDescriptor::default_value_int32() */
gint32
pbw_FieldDescriptor_default_value_int32(const PbwFieldDescriptor* field);

/* like FieldDescriptor::default_value_int64() */
gint64
pbw_FieldDescriptor_default_value_int64(const PbwFieldDescriptor* field);

/* like FieldDescriptor::default_value_uint32() */
guint32
pbw_FieldDescriptor_default_value_uint32(const PbwFieldDescriptor* field);

/* like FieldDescriptor::default_value_uint64() */
guint64
pbw_FieldDescriptor_default_value_uint64(const PbwFieldDescriptor* field);

/* like FieldDescriptor::default_value_float() */
gfloat
pbw_FieldDescriptor_default_value_float(const PbwFieldDescriptor* field);

/* like FieldDescriptor::default_value_double() */
gdouble
pbw_FieldDescriptor_default_value_double(const PbwFieldDescriptor* field);

/* like FieldDescriptor::default_value_bool() */
gboolean
pbw_FieldDescriptor_default_value_bool(const PbwFieldDescriptor* field);

/* like FieldDescriptor::default_value_string() */
const gchar*
pbw_FieldDescriptor_default_value_string(const PbwFieldDescriptor* field, int* size);

/* like FieldDescriptor::default_value_enum() */
const PbwEnumValueDescriptor*
pbw_FieldDescriptor_default_value_enum(const PbwFieldDescriptor* field);

/* like EnumDescriptor::name() */
const char*
pbw_EnumDescriptor_name(const PbwEnumDescriptor* anEnum);

/* like EnumDescriptor::full_name() */
const char*
pbw_EnumDescriptor_full_name(const PbwEnumDescriptor* anEnum);

/* like EnumDescriptor::value_count() */
int
pbw_EnumDescriptor_value_count(const PbwEnumDescriptor* anEnum);

/* like EnumDescriptor::value() */
const PbwEnumValueDescriptor*
pbw_EnumDescriptor_value(const PbwEnumDescriptor* anEnum, int value_index);

/* like EnumDescriptor::FindValueByNumber() */
const PbwEnumValueDescriptor*
pbw_EnumDescriptor_FindValueByNumber(const PbwEnumDescriptor* anEnum, int number);

/* like EnumDescriptor::FindValueByName() */
const PbwEnumValueDescriptor*
pbw_EnumDescriptor_FindValueByName(const PbwEnumDescriptor* anEnum, const gchar* name);

/* like EnumValueDescriptor::name() */
const char*
pbw_EnumValueDescriptor_name(const PbwEnumValueDescriptor* enumValue);

/* like EnumValueDescriptor::full_name() */
const char*
pbw_EnumValueDescriptor_full_name(const PbwEnumValueDescriptor* enumValue);

/* like EnumValueDescriptor::number() */
int
pbw_EnumValueDescriptor_number(const PbwEnumValueDescriptor* enumValue);

/* visit all messages of this pool */
void
pbw_foreach_message(const PbwDescriptorPool* pool, void (*cb)(const PbwDescriptor* message, void* userdata), void* userdata);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PROTOBUF_HELPER_H__ */

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 4
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=4 tabstop=8 expandtab:
 * :indentSize=4:tabSize=8:noTabs=true:
 */
