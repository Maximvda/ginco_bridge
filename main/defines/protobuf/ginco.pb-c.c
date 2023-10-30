/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: ginco.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "ginco.pb-c.h"
void   ginco__upgrade__init
                     (Ginco__Upgrade         *message)
{
  static Ginco__Upgrade init_value = GINCO__UPGRADE__INIT;
  *message = init_value;
}
size_t ginco__upgrade__get_packed_size
                     (const Ginco__Upgrade *message)
{
  assert(message->base.descriptor == &ginco__upgrade__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t ginco__upgrade__pack
                     (const Ginco__Upgrade *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &ginco__upgrade__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t ginco__upgrade__pack_to_buffer
                     (const Ginco__Upgrade *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &ginco__upgrade__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Ginco__Upgrade *
       ginco__upgrade__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Ginco__Upgrade *)
     protobuf_c_message_unpack (&ginco__upgrade__descriptor,
                                allocator, len, data);
}
void   ginco__upgrade__free_unpacked
                     (Ginco__Upgrade *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &ginco__upgrade__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   ginco__can_message__init
                     (Ginco__CanMessage         *message)
{
  static Ginco__CanMessage init_value = GINCO__CAN_MESSAGE__INIT;
  *message = init_value;
}
size_t ginco__can_message__get_packed_size
                     (const Ginco__CanMessage *message)
{
  assert(message->base.descriptor == &ginco__can_message__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t ginco__can_message__pack
                     (const Ginco__CanMessage *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &ginco__can_message__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t ginco__can_message__pack_to_buffer
                     (const Ginco__CanMessage *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &ginco__can_message__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Ginco__CanMessage *
       ginco__can_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Ginco__CanMessage *)
     protobuf_c_message_unpack (&ginco__can_message__descriptor,
                                allocator, len, data);
}
void   ginco__can_message__free_unpacked
                     (Ginco__CanMessage *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &ginco__can_message__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   ginco__command__init
                     (Ginco__Command         *message)
{
  static Ginco__Command init_value = GINCO__COMMAND__INIT;
  *message = init_value;
}
size_t ginco__command__get_packed_size
                     (const Ginco__Command *message)
{
  assert(message->base.descriptor == &ginco__command__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t ginco__command__pack
                     (const Ginco__Command *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &ginco__command__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t ginco__command__pack_to_buffer
                     (const Ginco__Command *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &ginco__command__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Ginco__Command *
       ginco__command__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Ginco__Command *)
     protobuf_c_message_unpack (&ginco__command__descriptor,
                                allocator, len, data);
}
void   ginco__command__free_unpacked
                     (Ginco__Command *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &ginco__command__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   ginco__commands__init
                     (Ginco__Commands         *message)
{
  static Ginco__Commands init_value = GINCO__COMMANDS__INIT;
  *message = init_value;
}
size_t ginco__commands__get_packed_size
                     (const Ginco__Commands *message)
{
  assert(message->base.descriptor == &ginco__commands__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t ginco__commands__pack
                     (const Ginco__Commands *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &ginco__commands__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t ginco__commands__pack_to_buffer
                     (const Ginco__Commands *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &ginco__commands__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Ginco__Commands *
       ginco__commands__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Ginco__Commands *)
     protobuf_c_message_unpack (&ginco__commands__descriptor,
                                allocator, len, data);
}
void   ginco__commands__free_unpacked
                     (Ginco__Commands *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &ginco__commands__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor ginco__upgrade__field_descriptors[2] =
{
  {
    "device_id",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_UINT32,
    offsetof(Ginco__Upgrade, has_device_id),
    offsetof(Ginco__Upgrade, device_id),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "image_size",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_UINT32,
    offsetof(Ginco__Upgrade, has_image_size),
    offsetof(Ginco__Upgrade, image_size),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned ginco__upgrade__field_indices_by_name[] = {
  0,   /* field[0] = device_id */
  1,   /* field[1] = image_size */
};
static const ProtobufCIntRange ginco__upgrade__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor ginco__upgrade__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "ginco.Upgrade",
  "Upgrade",
  "Ginco__Upgrade",
  "ginco",
  sizeof(Ginco__Upgrade),
  2,
  ginco__upgrade__field_descriptors,
  ginco__upgrade__field_indices_by_name,
  1,  ginco__upgrade__number_ranges,
  (ProtobufCMessageInit) ginco__upgrade__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor ginco__can_message__field_descriptors[9] =
{
  {
    "source_id",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_UINT32,
    offsetof(Ginco__CanMessage, has_source_id),
    offsetof(Ginco__CanMessage, source_id),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "event",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BOOL,
    offsetof(Ginco__CanMessage, has_event),
    offsetof(Ginco__CanMessage, event),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "linked",
    3,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BOOL,
    offsetof(Ginco__CanMessage, has_linked),
    offsetof(Ginco__CanMessage, linked),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "ack",
    4,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BOOL,
    offsetof(Ginco__CanMessage, has_ack),
    offsetof(Ginco__CanMessage, ack),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "feature_type",
    5,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_UINT32,
    offsetof(Ginco__CanMessage, has_feature_type),
    offsetof(Ginco__CanMessage, feature_type),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "index",
    6,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_UINT32,
    offsetof(Ginco__CanMessage, has_index),
    offsetof(Ginco__CanMessage, index),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "function_address",
    7,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_UINT32,
    offsetof(Ginco__CanMessage, has_function_address),
    offsetof(Ginco__CanMessage, function_address),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "buffer_size",
    8,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_UINT32,
    offsetof(Ginco__CanMessage, has_buffer_size),
    offsetof(Ginco__CanMessage, buffer_size),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "data",
    9,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_UINT64,
    offsetof(Ginco__CanMessage, has_data),
    offsetof(Ginco__CanMessage, data),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned ginco__can_message__field_indices_by_name[] = {
  3,   /* field[3] = ack */
  7,   /* field[7] = buffer_size */
  8,   /* field[8] = data */
  1,   /* field[1] = event */
  4,   /* field[4] = feature_type */
  6,   /* field[6] = function_address */
  5,   /* field[5] = index */
  2,   /* field[2] = linked */
  0,   /* field[0] = source_id */
};
static const ProtobufCIntRange ginco__can_message__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 9 }
};
const ProtobufCMessageDescriptor ginco__can_message__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "ginco.CanMessage",
  "CanMessage",
  "Ginco__CanMessage",
  "ginco",
  sizeof(Ginco__CanMessage),
  9,
  ginco__can_message__field_descriptors,
  ginco__can_message__field_indices_by_name,
  1,  ginco__can_message__number_ranges,
  (ProtobufCMessageInit) ginco__can_message__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor ginco__command__field_descriptors[2] =
{
  {
    "upgrade",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Ginco__Command, command_case),
    offsetof(Ginco__Command, upgrade),
    &ginco__upgrade__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "can_message",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Ginco__Command, command_case),
    offsetof(Ginco__Command, can_message),
    &ginco__can_message__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned ginco__command__field_indices_by_name[] = {
  1,   /* field[1] = can_message */
  0,   /* field[0] = upgrade */
};
static const ProtobufCIntRange ginco__command__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor ginco__command__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "ginco.Command",
  "Command",
  "Ginco__Command",
  "ginco",
  sizeof(Ginco__Command),
  2,
  ginco__command__field_descriptors,
  ginco__command__field_indices_by_name,
  1,  ginco__command__number_ranges,
  (ProtobufCMessageInit) ginco__command__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor ginco__commands__field_descriptors[1] =
{
  {
    "commands",
    1,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Ginco__Commands, n_commands),
    offsetof(Ginco__Commands, commands),
    &ginco__command__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned ginco__commands__field_indices_by_name[] = {
  0,   /* field[0] = commands */
};
static const ProtobufCIntRange ginco__commands__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor ginco__commands__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "ginco.Commands",
  "Commands",
  "Ginco__Commands",
  "ginco",
  sizeof(Ginco__Commands),
  1,
  ginco__commands__field_descriptors,
  ginco__commands__field_indices_by_name,
  1,  ginco__commands__number_ranges,
  (ProtobufCMessageInit) ginco__commands__init,
  NULL,NULL,NULL    /* reserved[123] */
};