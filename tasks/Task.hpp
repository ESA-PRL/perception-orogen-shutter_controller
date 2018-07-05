#ifndef SHUTTER_CONTROLLER_TASK_TASK_HPP
#define SHUTTER_CONTROLLER_TASK_TASK_HPP

#include "camera_firewire/CameraTask.hpp" // for CamAttrib
#include "camera_firewire/CamFireWire.h"  // set shutter time directly, without rock/orocos operations

#include "shutter_controller/TaskBase.hpp"
#include <shutter_controller/ShutterController.hpp>

namespace shutter_controller {
    class ShutterController;

    class Task : public TaskBase
    {
    friend class TaskBase;
    public:
        Task(std::string const& name = "shutter_controller::Task");
        Task(std::string const& name, RTT::ExecutionEngine* engine);

        ~Task();

        bool configureHook();
        bool startHook();
        void updateHook();
        void errorHook();
        void stopHook();
        void cleanupHook();

    protected:
        ShutterController* shutter_controller;
        RTT::extras::ReadOnlyPointer<base::samples::frame::Frame> frame;
        bool stop;
        int period_counter;
        int shutter_computation_period;
    };
}

#endif
