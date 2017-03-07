#ifndef SHUTTER_CONTROLLER_TASK_TASK_HPP
#define SHUTTER_CONTROLLER_TASK_TASK_HPP

#include "shutter_controller/TaskBase.hpp"
#include <rtt/OperationCaller.hpp> // to call pancam operations
#include <rtt/Operation.hpp>
#include "camera_base/TaskBase.hpp" // for CamAttrib
#include "camera_base/Task.hpp"

namespace shutter_controller {
    class ShutterController;

    class Task : public TaskBase
    {
    friend class TaskBase;
    protected:
        //int shutter_time; // initialized by configure hook with shutter time from yml value for left camera
        ShutterController* shutterCtrl;
        RTT::extras::ReadOnlyPointer<base::samples::frame::Frame> frame;

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
    };
}

#endif
