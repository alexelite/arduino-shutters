#pragma once

#include "Arduino.h"

#include "Shutters/StoredState.hpp"

class Shutters;

namespace ShuttersInternal {
  const uint16_t SAFETY_DELAY = 1 * 1000;
  const uint8_t LEVELS = 100;

  enum State : uint8_t {
    STATE_IDLE, // not moving
    STATE_RESETTING, // when state not known, goes to 0
    STATE_TARGETING, // when going to target
    STATE_NORMALIZING, // when target changed, goes to next known level
    STATE_CALIBRATING // when 0 or 100, to ensure actually at the end
  };
  enum Direction : bool { DIRECTION_DOWN, DIRECTION_UP };

  typedef void (*OperationFunction)(::Shutters*);
  typedef uint32_t (*GetStateFunction)(::Shutters*);
  typedef void (*SetStateFunction)(::Shutters*, uint32_t);
  typedef void (*LevelReachedCallback)(::Shutters*, uint8_t);
}

class Shutters {
private:
  uint32_t _upCourseTime;
  uint32_t _downCourseTime;
  float _calibrationRatio;
  uint32_t _upStepTime;
  uint32_t _downStepTime;
  uint32_t _upCalibrationTime;
  uint32_t _downCalibrationTime;

  ShuttersInternal::State _state;
  uint32_t _stateTime;
  ShuttersInternal::Direction _direction;

  ShuttersInternal::StoredState _storedState;

  uint8_t _targetLevel;

  bool _safetyDelay;
  uint32_t _safetyDelayTime;

  bool _reset;

  ShuttersInternal::OperationFunction _upFunction;
  ShuttersInternal::OperationFunction _downFunction;
  ShuttersInternal::OperationFunction _haltFunction;

  ShuttersInternal::GetStateFunction _getStateFunction;
  ShuttersInternal::SetStateFunction _setStateFunction;

  ShuttersInternal::LevelReachedCallback _levelReachedCallback;

  void _up();
  void _down();
  void _halt();
  void _setSafetyDelay();
  void _notifyLevel();
public:
  Shutters(ShuttersInternal::OperationFunction up, ShuttersInternal::OperationFunction down, ShuttersInternal::OperationFunction halt, ShuttersInternal::GetStateFunction getState, ShuttersInternal::SetStateFunction setState);
  uint32_t getUpCourseTime();
  uint32_t getDownCourseTime();
  void setCourseTime(uint32_t upCourseTime, uint32_t downCourseTime = 0);
  float getCalibrationRatio();
  void setCalibrationRatio(float calibrationRatio);
  void onLevelReached(ShuttersInternal::LevelReachedCallback callback);
  void begin();
  void setLevel(uint8_t level);
  void stop();
  void loop();
  bool isIdle();
  uint8_t getCurrentLevel();
  void reset();
};
