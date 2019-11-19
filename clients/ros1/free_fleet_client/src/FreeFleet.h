/****************************************************************

  Generated by Eclipse Cyclone DDS IDL to C Translator
  File name: FreeFleet.h
  Source: FreeFleet.idl
  Cyclone DDS: V0.1.0

*****************************************************************/

#include "dds/ddsc/dds_public_impl.h"

#ifndef _DDSL_FREEFLEET_H_
#define _DDSL_FREEFLEET_H_


#ifdef __cplusplus
extern "C" {
#endif

#define FreeFleetData_RobotMode_Constants_MODE_IDLE 0
#define FreeFleetData_RobotMode_Constants_MODE_CHARGING 1
#define FreeFleetData_RobotMode_Constants_MODE_MOVING 2
#define FreeFleetData_RobotMode_Constants_MODE_PAUSED 3
#define FreeFleetData_RobotMode_Constants_MODE_WAITING 4
#define FreeFleetData_RobotMode_Constants_MODE_EMERGENCY 5


typedef struct FreeFleetData_RobotMode
{
  uint32_t mode;
} FreeFleetData_RobotMode;

extern const dds_topic_descriptor_t FreeFleetData_RobotMode_desc;

#define FreeFleetData_RobotMode__alloc() \
((FreeFleetData_RobotMode*) dds_alloc (sizeof (FreeFleetData_RobotMode)));

#define FreeFleetData_RobotMode_free(d,o) \
dds_sample_free ((d), &FreeFleetData_RobotMode_desc, (o))


typedef struct FreeFleetData_Location
{
  int32_t sec;
  uint32_t nanosec;
  float x;
  float y;
  float yaw;
  char * level_name;
} FreeFleetData_Location;

extern const dds_topic_descriptor_t FreeFleetData_Location_desc;

#define FreeFleetData_Location__alloc() \
((FreeFleetData_Location*) dds_alloc (sizeof (FreeFleetData_Location)));

#define FreeFleetData_Location_free(d,o) \
dds_sample_free ((d), &FreeFleetData_Location_desc, (o))

typedef struct FreeFleetData_RobotState_path_seq
{
  uint32_t _maximum;
  uint32_t _length;
  FreeFleetData_Location *_buffer;
  bool _release;
} FreeFleetData_RobotState_path_seq;

#define FreeFleetData_RobotState_path_seq__alloc() \
((FreeFleetData_RobotState_path_seq*) dds_alloc (sizeof (FreeFleetData_RobotState_path_seq)));

#define FreeFleetData_RobotState_path_seq_allocbuf(l) \
((FreeFleetData_Location *) dds_alloc ((l) * sizeof (FreeFleetData_Location)))


typedef struct FreeFleetData_RobotState
{
  char * name;
  char * model;
  FreeFleetData_RobotMode mode;
  float battery_percent;
  FreeFleetData_Location location;
  FreeFleetData_RobotState_path_seq path;
} FreeFleetData_RobotState;

extern const dds_topic_descriptor_t FreeFleetData_RobotState_desc;

#define FreeFleetData_RobotState__alloc() \
((FreeFleetData_RobotState*) dds_alloc (sizeof (FreeFleetData_RobotState)));

#define FreeFleetData_RobotState_free(d,o) \
dds_sample_free ((d), &FreeFleetData_RobotState_desc, (o))

#ifdef __cplusplus
}
#endif
#endif /* _DDSL_FREEFLEET_H_ */
