#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG 1
#define DEBUG_OUT SerialUSB

#if DEBUG
  #define PRINT_DEBUG(x) DEBUG_OUT.print(x)
  #define PRINT_DEBUGLN(x) DEBUG_OUT.println(x)
#else
  #define PRINT_DEBUG(x)    //nop
  #define PRINT_DEBUGLN(x)  //nop
#endif

#endif //DEBUG_H