#include <string>
#include "rclcpp/rclcpp.hpp"
#include "my_robot_interfaces/srv/set_led.hpp"

using namespace std::chrono_literals;
using namespace std::placeholders;


class BatteryNode : public rclcpp::Node{
public:
    BatteryNode() : Node("battery_node"), batteryState_("full")
    {
        client_ = this->create_client<my_robot_interfaces::srv::SetLed>("set_led");
    }

    void callBatteryLifecycle(){
        while(true){
            auto request = std::make_shared<my_robot_interfaces::srv::SetLed::Request>();

            if(batteryState_ == "full"){
                rclcpp::sleep_for(4s);
                request->led_number = 3;
                request->state = "on";
                client_->async_send_request(
                    request,
                    std::bind(&BatteryNode::callbackEmpty, this, _1)
                );
            } else if(batteryState_ == "empty"){
                rclcpp::sleep_for(6s);
                request->led_number = 3;
                request->state = "off";
                client_->async_send_request(
                    request,
                    std::bind(&BatteryNode::callbackFull, this, _1)
                );
            }
        }
    }

private:

    void callbackEmpty(
        rclcpp::Client<my_robot_interfaces::srv::SetLed>::SharedFuture future){

        auto response = future.get();

        std::cout << "hello" << std::endl;
        std::cout << response << std::endl;

        if(response->success){
            RCLCPP_INFO(this->get_logger(), "Succeeded!");
            batteryState_ = "empty";
        } else {
            RCLCPP_INFO(this->get_logger(), "Failed.");
        }
    }

    void callbackFull(
        rclcpp::Client<my_robot_interfaces::srv::SetLed>::SharedFuture future){

        auto response = future.get();
        if(response->success){
            RCLCPP_INFO(this->get_logger(), "Succeeded!");
            batteryState_ = "full";
        } else {
            RCLCPP_INFO(this->get_logger(), "Failed.");
        }
    }

    rclcpp::Client<my_robot_interfaces::srv::SetLed>::SharedPtr client_;
    std::string batteryState_;
};


int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<BatteryNode>();
    node->callBatteryLifecycle();
    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}
