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

#include <iostream>

#include <Eigen/Geometry>

int main(int argc, char** argv)
{
  Eigen::Matrix3d fleet_to_rmf_transform;
  fleet_to_rmf_transform << 1.077, -0.014, 4.834, 
      0.014, 1.077, -29.30, 
      0.0, 0.0, 1.0;

  std::cout << "Transform: " << std::endl;
  std::cout << fleet_to_rmf_transform << std::endl << std::endl;

  Eigen::Vector3d o1(0.0, 0.0, 1.0);
  Eigen::Vector3d p1(1.0, 0.0, 1.0);

  Eigen::Vector3d o2 = fleet_to_rmf_transform * o1;
  o2 /= o2[2];
  Eigen::Vector3d p2 = fleet_to_rmf_transform * p1;
  p2 /= p2[2];

  double fleet_to_rmf_yaw = std::atan((p2[1] - o2[1]) / (p2[0] - o2[0]));
  
  std::cout << "Yaw: " << fleet_to_rmf_yaw << std::endl << std::endl;

  // manual tuning for correctness
  double scale_x = 1.078;
  double scale_y = 1.078;
  double x = 4.834;
  double y = -29.3;

  Eigen::Matrix3d scale_mat;
  scale_mat << scale_x, 0.0, 0.0, 0.0, scale_y, 0.0, 0.0, 0.0, 1.0;

  double cos = std::cos(fleet_to_rmf_yaw);
  double sin = std::sin(fleet_to_rmf_yaw);
  Eigen::Matrix3d hom_rot;
  hom_rot << cos, -sin, 0.0, sin, cos, 0.0, 0.0, 0.0, 1.0;
  std::cout << "Hom_rot: " << std::endl << hom_rot << std::endl << std::endl;

  Eigen::Matrix3d hom_trans;
  hom_trans << 1.0, 0.0, x, 0.0, 1.0, y, 0.0, 0.0, 1.0;
  std::cout << "Hom_trans: " << std::endl << hom_trans << std::endl << std::endl;

  Eigen::Matrix3d hom = hom_trans * (hom_rot * scale_mat);
  std::cout << "hom: " << std::endl << hom;
  std::cout << std::endl << std::endl;

  // checking with some random point
  double rand_x = 687.53284;
  double rand_y = 82.48;
  double rand_yaw = 1.234;

  Eigen::Vector3d pt(rand_x, rand_y, 1.0);
  Eigen::Vector3d trans_pt = fleet_to_rmf_transform * pt;
  trans_pt /= trans_pt[2];  
  
  // checking those values using the newest commit's calculations
  const auto scaled = Eigen::Vector2d(rand_x * scale_x, rand_y * scale_y);
  const auto rotated = Eigen::Rotation2D<double>(fleet_to_rmf_yaw) * scaled;
  const auto translated = rotated + Eigen::Vector2d(x, y);
  
  std::cout << "Using transformation matrix: " << trans_pt.transpose() << std::endl;
  std::cout << "Using step-by-step:          " << translated.transpose() 
      << std::endl;

  return 0;
}
