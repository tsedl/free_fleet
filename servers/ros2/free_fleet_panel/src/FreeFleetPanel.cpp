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

#include "FreeFleetPanel.hpp"

#include <QVBoxLayout>

namespace free_fleet
{
namespace viz
{

FreeFleetPanel::FreeFleetPanel(QWidget* parent) :
  rviz_common::Panel(parent),
  Node("free_fleet_panel_node")
{
  robot_mode_pub = create_publisher<RobotMode>(
      "robot_mode", rclcpp::SystemDefaultsQoS());
  
  fleet_state_sub = create_subscription<FleetState>(
      "fleet_state", rclcpp::SystemDefaultsQoS(), 
      [&](FleetState::UniquePtr msg)
      {
        fleet_state_cb_fn(std::move(msg));
      });

  test_push_button = new QPushButton("Publish Robot Mode", this);
  test_push_button->setSizePolicy(
      QSizePolicy::Expanding, QSizePolicy::Expanding);

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(test_push_button);
  setLayout(layout);

  connect(
      test_push_button, &QPushButton::clicked, this, 
      &FreeFleetPanel::publish_robot_mode);
}

void FreeFleetPanel::fleet_state_cb_fn(FleetState::UniquePtr _msg)
{
  RCLCPP_INFO(get_logger(), "got a fleet_state");
}

void FreeFleetPanel::publish_robot_mode()
{
  RobotMode msg;
  msg.mode = msg.MODE_IDLE;
  robot_mode_pub->publish(msg);
}

} // namespace viz
} // namespace free_fleet

#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(free_fleet::viz::FreeFleetPanel, rviz_common::Panel)
