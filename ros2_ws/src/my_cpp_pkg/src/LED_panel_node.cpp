#include <string>
#include "rclcpp/rclcpp.hpp"
#include "my_robot_interfaces/msg/led_panel_state.hpp"
#include "my_robot_interfaces/srv/set_led.hpp"

using namespace std::placeholders;
using namespace std::chrono_literals;


class LEDPanelNode: public rclcpp::Node{
public:
    LEDPanelNode() : Node("LED_panel_node"), state_("off"), panelStates_{0, 0, 0}
    {
        server_ = this->create_service<my_robot_interfaces::srv::SetLed>(
            "set_led",
            std::bind(&LEDPanelNode::callbackTurnOnLed, this, _1, _2)
        );
        pub_ = this->create_publisher<my_robot_interfaces::msg::LedPanelState>(
            "led_panel_state",
            10
        );
        timer_ = this->create_wall_timer(
            1s,
            std::bind(&LEDPanelNode::publishLEDState, this)
        );
        RCLCPP_INFO(this->get_logger(), "Set Panel Service and LED Panel State Topic have been started");
    }
private:

    void callbackTurnOnLed(
        const my_robot_interfaces::srv::SetLed::Request::SharedPtr request,
        const my_robot_interfaces::srv::SetLed::Response::SharedPtr response){

        std::cout << "====================" << std::endl;
        std::cout << state_ << std::endl;

        try {
            if(request->state == "on"){
                state_ = "on";
                panelStates_[request->led_number] = 1;
                RCLCPP_INFO(this->get_logger(), "Tunred on %d LED\n", request->led_number);
            } else if(request->state == "off"){
                state_ = "off";
                panelStates_[request->led_number] = 0;
                RCLCPP_INFO(this->get_logger(), "Tunred off %d LED\n", request->led_number);
            }
            response->success = true;
        }
        catch (const std::exception& e) {
            std::cerr << "Caught exception: " << e.what() << std::endl;
            response->success = false;
        }
    }

    void publishLEDState(){
        auto msg = my_robot_interfaces::msg::LedPanelState();
        msg.state = state_;
        msg.debug_message = "Led panel working properly!";
        pub_ -> publish(msg);
    }

    std::string state_;
    int panelStates_[3];

    rclcpp::Service<my_robot_interfaces::srv::SetLed>::SharedPtr server_;
    rclcpp::Publisher<my_robot_interfaces::msg::LedPanelState>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};


int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<LEDPanelNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
