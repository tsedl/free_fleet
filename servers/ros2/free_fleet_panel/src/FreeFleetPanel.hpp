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

#ifndef FREE_FLEET_RVIZ2_PLUGIN__SRC__FREEFLEETPANEL_HPP
#define FREE_FLEET_RVIZ2_PLUGIN__SRC__FREEFLEETPANEL_HPP

#include <rclcpp/rclcpp.hpp>

#include <rmf_fleet_msgs/msg/robot_mode.hpp>
#include <rmf_fleet_msgs/msg/fleet_state.hpp>

#include <rviz_common/panel.hpp>

#include <QPushButton>

namespace free_fleet
{
namespace viz
{

class FreeFleetPanel : public rviz_common::Panel, public rclcpp::Node
{

Q_OBJECT

public:

  using RobotMode = rmf_fleet_msgs::msg::RobotMode;
  using FleetState = rmf_fleet_msgs::msg::FleetState;

  FreeFleetPanel(QWidget* parent = 0);

protected:

  QPushButton* test_push_button;

  rclcpp::Publisher<RobotMode>::SharedPtr robot_mode_pub;

  rclcpp::Subscription<FleetState>::SharedPtr fleet_state_sub;

  void fleet_state_cb_fn(FleetState::UniquePtr msg);

  void publish_robot_mode();

};

} // namespace viz
} // namespace free_fleet

#endif // FREE_FLEET_RVIZ2_PLUGIN__SRC__FREEFLEETPANEL_HPP
