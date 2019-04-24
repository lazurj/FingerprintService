#ifndef ENUMS_H
#define ENUMS_H

enum class MessageType
  {
     Info,
     Warning,
     Error
  };

enum class WorkerStatus
  {
     INITIALIZING,
     RUNNING,
     WAITING,
     STOPED,
     READY
  };

#endif // ENUMS_H
