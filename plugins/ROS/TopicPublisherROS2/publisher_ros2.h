#ifndef STATE_PUBLISHER_ROS2TOPIC_H
#define STATE_PUBLISHER_ROS2TOPIC_H

#include <QObject>
#include <QtPlugin>
#include <map>

#include <rclcpp/rclcpp.hpp>
#include <rosbag2_storage/serialized_bag_message.hpp>
#include "PlotJuggler/statepublisher_base.h"

using SerializedMessage = rosbag2_storage::SerializedBagMessage;

class TopicPublisherROS2 : public StatePublisher
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "com.icarustechnology.PlotJuggler.StatePublisher"
                        "../statepublisher.json")
  Q_INTERFACES(StatePublisher)

public:
  TopicPublisherROS2();
  virtual ~TopicPublisherROS2() override;

  virtual void updateState(double current_time) override;

  virtual const char* name() const override
  {
    return "ROS2 Topic Re-Publisher";
  }

  virtual bool enabled() const override
  {
    return _enabled;
  }

  void setParentMenu(QMenu* menu, QAction* action) override;

  virtual void play(double interval) override;

public slots:
  virtual void setEnabled(bool enabled) override;

  void filterDialog(bool autoconfirm);

private:

  std::shared_ptr<rclcpp::Context> _context;
  std::unique_ptr<rclcpp::executors::MultiThreadedExecutor> _executor;
  std::shared_ptr<rclcpp::Node> _node;

  bool _enabled;

  bool _publish_clock;
 // std::unique_ptr<tf::TransformBroadcaster> _tf_publisher;

  std::unordered_map<std::string, ros::Publisher> _publishers;
  ros::Publisher _tf_static_pub;
  ros::Publisher _clock_publisher;

  QAction* _enable_self_action;
  QAction* _select_topics_to_publish;

  std::unordered_map<std::string, bool> _topics_to_publish;

  bool toPublish(const std::string& topic_name);

  double previous_time;

  int _previous_play_index;

  void broadcastTF(double current_time);

  void publishAnyMsg(const std::shared_ptr<SerializedMessage> &msg);
};

#endif  // STATE_PUBLISHER_ROS2TOPIC_H
