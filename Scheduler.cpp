#include "Scheduler.h"

extern Logger *plog;

Scheduler::Scheduler(DialogRobot* _robot)
//  : QObject(nullptr)
  : robot(_robot)
  , processValue(0)
  , shouldProcess(false)
  , currentLayer(0)
  , currentDetection(-1)
  , continueBTN(false)
//  , robot(nullptr)
  , isInitialized(false)
{
    pushpullState = AS_LAYER_IDLE;
    yoloState = AS_YOLO_IDLE;
    noDetectionState = AS_DEPTH_IDLE;
    ROBOT_OCCUPY = RO_FREE;

    strlist_first_input
            << "INPUT IDLE"
            << "TAKING PROCESS..."
            << "MAKING KIMBAP..."
            << "RETURNING..."
            << "KIMBAP IS MADE";

    strlist_button
            << "LAYER IDLE"
            << "LAYER WAITING FOR BUTTON CLICK ..."
            << "LAYER RETURNING..."
            << "LAYER DONE";

    counter = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->start(100);  // 100ms interval
    qDebug() << "Scheduler initialized";
}

//Scheduler::~Scheduler()
//{
//    timer->stop();
//    delete timer;
//}

void Scheduler::handleButtonClick()
{
    continueBTN = true;
}

void Scheduler::startProcessing(int value)
{
    if (value > 0)
    {  // Only set new value if it's positive
        processValue = value;
        qDebug() << "Scheduler starting new process with value:" << value;
    }
    shouldProcess = true;
    counter = 0;
    pushpullState = AS_LAYER_IDLE;  // Reset state
    yoloState = AS_YOLO_IDLE;
    noDetectionState = AS_DEPTH_IDLE;
    ROBOT_OCCUPY = RO_FREE;
    qDebug() << "Scheduler continuing with current value:" << processValue;
    // Log current state for debugging
    qDebug() << "Scheduler state at start - Value:" << processValue
             << "Layer:" << currentLayer
             << "Detection:" << currentDetection;
    emit currentValueChanged(processValue);
}

void Scheduler::setLayerAndDetection(int layer, int detection)
{
    if(ROBOT_OCCUPY != RO_FREE)
    {
        return;
    }
    currentLayer = layer;
    currentDetection = detection;
    qDebug() << "Set layer:" << layer << "detection:" << detection;
}

//void Scheduler::initializeRobot(DialogRobot* _robot)
//{
//    if (!isInitialized && _robot != nullptr)
//    {
//        robot = _robot;
//        isInitialized = true;
//        qDebug() << "Robot initialized in Scheduler";
//    }
//}

void Scheduler::onTimer()
{
//    if (!isInitialized || !robot)
//    {
//        return;
//    }

    // YOLO processing state machine
    switch(yoloState)
    {
        case AS_YOLO_IDLE:
        {
            if(ROBOT_OCCUPY != RO_FREE)
            {
                break;
            }

            if(robot->RobotMoving == true)
                break;

            if (processValue >= 1 && shouldProcess &&
                currentLayer != 0 && currentDetection != -1)
            {
                qDebug() << "YOLO Conditions: Value=" << processValue
                                 << "Layer=" << currentLayer
                                 << "Detection=" << currentDetection;
                qDebug() << "AS_IDLE_YOLO: YOLO conditions met, moving to PROCESS";

                ROBOT_OCCUPY = RO_INPUT;
                yoloState = AS_YOLO_PROCESS;
            }
            break;
        }

        case AS_YOLO_PROCESS:
        {
            if(robot->RobotMoving == true)
                break;

            qDebug() << "AS_PROCESS_YOLO";
            if(currentLayer != -1 && currentDetection != -1)
            {
                keymotion = QString("LAYER_%1_CELL_%2").arg(currentLayer).arg(currentDetection);
            }
            else
            {
                qDebug() << "Error (AS_YOLO_PROCESS) wrong this: " << currentLayer;
                ROBOT_OCCUPY = RO_FREE;
                yoloState = AS_YOLO_IDLE;
                break;
            }
            qDebug()<<keymotion;
//            processValue--;
//            emit currentValueChanged(processValue);
//            robot->RobotMoving = true;
//            robot->MotionServerCommand(keymotion);

//            yoloState = AS_YOLO_RETURN;
//            break;

            //---------------
            counter++;
            qDebug() << "YOLO Counting:" << counter;

            if(counter < MAX_COUNT)
            {
                break;
            }

            qDebug() << "YOLO counting complete!";
            counter = 0;
            processValue--;
            emit currentValueChanged(processValue);
            qDebug() << "YOLO processing value decreased to:" << processValue;
            yoloState = AS_YOLO_MAKING;
            break;
            //-------------------
        }

        case AS_YOLO_MAKING:
        {
            if(robot->RobotMoving == true)
                break;

            keymotion = QString("MAKING_KIMBAP");
            qDebug() << keymotion;
//            robot->RobotMoving = true;
//            robot->MotionServerCommand(keymotion);
            yoloState = AS_YOLO_RETURN;
            break;
        }
        case AS_YOLO_RETURN:
        {
            if(robot->RobotMoving == true)
                break;

            qDebug() << "AS_RETURN_YOLO";
            yoloState = AS_YOLO_DONE;
            break;
        }

        case AS_YOLO_DONE:
        {
            if(robot->RobotMoving == true)
                break;

            qDebug() << "AS_DONE_YOLO";
            shouldProcess = false;
            ROBOT_OCCUPY = RO_FREE;
            yoloState = AS_YOLO_IDLE;
            emit processingComplete();
            break;
        }
    }

    switch(noDetectionState)
    {
        case AS_DEPTH_IDLE:
        {
            if(ROBOT_OCCUPY != RO_FREE)
                break;
            if(robot->RobotMoving == true)
                break;

            if (processValue >= 1 && shouldProcess && currentLayer >= 0 && currentDetection == -1)
            {
                qDebug() << "AS_DEPTH_IDLE: No conditions met";
                ROBOT_OCCUPY = RO_LAYER;
                noDetectionState = AS_DEPTH_CHECK;
                continueBTN = false;  // Reset button state
            }
            break;
        }
        case AS_DEPTH_CHECK:
        {
            if(robot->RobotMoving == true)
                break;

            qDebug() << "AS_DEPTH_CHECK: Waiting for button click";
            if (continueBTN && currentLayer >= 0 && currentDetection == -1)
            {
                qDebug() << "Continue to new tray!!";
                emit currentValueChanged(processValue);
                noDetectionState = AS_DEPTH_RETURN;
            }
//            robot->RobotMoving = true;
//            robot->MotionServerCommand(keymotion);
            break;
        }
        case AS_DEPTH_RETURN:
        {
            if(robot->RobotMoving == true)
                break;

            qDebug() << "AS_DEPTH_RETURN";
            noDetectionState = AS_DEPTH_DONE;
            break;
        }

        case AS_DEPTH_DONE:
        {
            if(robot->RobotMoving == true)
                break;
            qDebug() << "AS_DEPTH_DONE";
            shouldProcess = false;
            ROBOT_OCCUPY = RO_FREE;
            noDetectionState = AS_DEPTH_IDLE;
            emit processingComplete();
            break;
        }
    }
}

QString Scheduler::GetActionStrInput()
{
    return strlist_first_input[yoloState];
}

QString Scheduler::GetWaitingButton()
{
    return strlist_button[noDetectionState];
}
