/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef FREEFLEETCLIENT__SRC__CLIENT_HPP
#define FREEFLEETCLIENT__SRC__CLIENT_HPP

#include <deque>
#include <mutex>
#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <limits>
#include <vector>

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/BatteryState.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>

#include <move_base_msgs/MoveBaseGoal.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

#include <dds/dds.h>

#include "ClientConfig.hpp"
#include "free_fleet/FreeFleet.h"
#include "dds_utils/DDSPublishHandler.hpp"
#include "dds_utils/DDSSubscribeHandler.hpp"


namespace free_fleet
{

class Client
{
public:

  using SharedPtr = std::shared_ptr<Client>;
  using Duration = std::chrono::steady_clock::duration;

  using ReadLock = std::unique_lock<std::mutex>;
  using WriteLock = std::unique_lock<std::mutex>;

  /// Factory function that creates an instance of the Free Fleet DDS Client
  ///
  static SharedPtr make(const ClientConfig& config);

  /// Desctructor
  ~Client();

  /// Checks that the Client is ready to start
  ///
  bool is_ready();

  /// Starts the subscriptions to all the different topics and starts 
  /// publishing the state over DDS to the server
  void start();

  void print_config();

private:

  ClientConfig client_config;

  std::atomic<bool> ready;

  dds_return_t return_code;
  dds_entity_t participant;

  std::unique_ptr<ros::NodeHandle> node;
  std::unique_ptr<ros::Rate> update_rate;
  std::unique_ptr<ros::Rate> publish_rate;

  // --------------------------------------------------------------------------
  // Everything needed for sending out robot states

  dds::DDSPublishHandler<FreeFleetData_RobotState>::SharedPtr
      state_pub;

  tf2_ros::Buffer tf2_buffer;
  tf2_ros::TransformListener tf2_listener;
  std::mutex robot_transform_mutex;
  geometry_msgs::TransformStamped current_robot_transform;
  geometry_msgs::TransformStamped previous_robot_transform;

  // create other subscribers here for updates
  ros::Subscriber battery_percent_sub;
  ros::Subscriber level_name_sub;

  std::mutex battery_state_mutex;
  sensor_msgs::BatteryState current_battery_state;

  std::mutex level_name_mutex;
  std_msgs::String current_level_name;

  std::mutex task_id_mutex;
  std::string current_task_id;

  void battery_state_callback_fn(const sensor_msgs::BatteryState& msg);

  void level_name_callback_fn(const std_msgs::String& msg);

  bool get_robot_transform();

  /// TODO: figure out the conditions of waiting
  ///
  uint32_t get_robot_mode();

  void publish_robot_state();

  // --------------------------------------------------------------------------
  // Receiving and handling requests in the form of location, mode and path.

  dds::DDSSubscribeHandler<FreeFleetData_ModeRequest>::SharedPtr 
      mode_request_sub;

  dds::DDSSubscribeHandler<FreeFleetData_PathRequest>::SharedPtr 
      path_request_sub;

  dds::DDSSubscribeHandler<FreeFleetData_DestinationRequest>::SharedPtr
      destination_request_sub;

  move_base_msgs::MoveBaseGoal location_to_goal(
      std::shared_ptr<const FreeFleetData_Location> location) const;

  move_base_msgs::MoveBaseGoal location_to_goal(
      const FreeFleetData_Location& location) const;

  void pause_robot();

  void resume_robot();

  /// Checks that the incoming request is valid, for this robot, or if the task is valid.
  ///
  bool is_valid_request(
    const std::string& request_fleet_name,
    const std::string& request_robot_name,
    const std::string& request_task_id);

  /// In the event that within one single cycle, the client receives requests
  /// from all 3 sources, the priority is mode > path > location.
  ///
  void read_requests();

  /// Handling of requests will have a similar priority, with mode > goal
  ///
  void handle_requests();

  // --------------------------------------------------------------------------

  struct Goal
  {
    std::string level_name;
    move_base_msgs::MoveBaseGoal goal;
    bool sent = false;
    ros::Time wait_at_goal_time;
  };

  std::atomic<bool> emergency;

  std::atomic<bool> paused;

  std::mutex goal_path_mutex;
  std::deque<Goal> goal_path;

  using MoveBaseClient = 
      actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>;
  using GoalState = actionlib::SimpleClientGoalState;

  MoveBaseClient move_base_client;

  std::thread update_thread;

  std::thread publish_thread;

  void update_thread_fn();

  void publish_thread_fn();

  Client(const ClientConfig& config);

};

} // namespace free_fleet

#endif // FREEFLEETCLIENT__SRC__CLIENT_HPP
