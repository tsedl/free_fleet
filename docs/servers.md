# Free Fleet Server notes

* all server configuration is received from the ROS 2 parameter server

# Building Instructions

Clone the repository somewhere,

```bash
cd
git clone https://github.com/osrf/free_fleet
```

Start a new ROS 2 workspace while adding a symbolic link to the server, as well as cloning `rmf_core`,

```bash
mkdir -p ~/server_ws/src
cd ~/server_ws/src
ln -s ~/free_fleet/servers/ros2 free_fleet_server

git clone https://github.com/osrf/rmf_core
```

Source ROS 2 and build,

```bash
cd ~/server_ws
source /opt/ros/eloquent/setup.bash
colcon build
```
