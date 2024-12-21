# =============================================================================
# CMake utilities
# =============================================================================
# File Name:      LogMessage.cmake
# Functions:
#   LogMessage    Write a log message
#   setLogsOn     Activate logs
#   setLogsOff    Deactivate logs
# =============================================================================

function(LogMessage message)
  if(LOG_MESSAGE_STATUS)
    set(LOG_MESSAGE_PREFIX "[${PROJECT_NAME}]:")
    message(STATUS "${LOG_MESSAGE_PREFIX} ${message}")
  endif()
endfunction(LogMessage)

function(LogWarning message)
  if(LOG_MESSAGE_STATUS)
    set(LOG_MESSAGE_PREFIX "[${PROJECT_NAME}]:")
    message(WARNING "${LOG_MESSAGE_PREFIX} ${message}")
  endif()
endfunction(LogWarning)

function(setLogsOn)
  set(LOG_MESSAGE_STATUS "ON" CACHE INTERNAL "Status for log messages")
endfunction(setLogsOn)

function(setLogsOff)
  set(LOG_MESSAGE_STATUS "OFF" CACHE INTERNAL "Status for log messages")
endfunction(setLogsOff)
