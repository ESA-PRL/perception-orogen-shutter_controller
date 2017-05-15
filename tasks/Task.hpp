#ifndef SHUTTER_CONTROLLER_TASK_TASK_HPP
#define SHUTTER_CONTROLLER_TASK_TASK_HPP

#include "shutter_controller/TaskBase.hpp"
#include <rtt/OperationCaller.hpp> // to call pancam operations
//#include <rtt/internal/DataSources.hpp>
#include "camera_firewire/CameraTask.hpp" // for CamAttrib
#include "camera_firewire/CamFireWire.h" // set shutter time directly, without rock/orocos operations

namespace shutter_controller {
    class ShutterController;

    class Task : public TaskBase
    {
    friend class TaskBase;
    protected:
        //int shutter_time; // initialized by configure hook with shutter time from yml value for left camera
        ShutterController* shutterCtrl;
        RTT::extras::ReadOnlyPointer<base::samples::frame::Frame> frame;
        bool stop;

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

    private:
        int periodCounter;
        int shutterCompPeriod;
    };
}

#endif
