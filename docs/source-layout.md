Source Tree Layout
==================

* docs - these docs
* external - externally sourced libraries and dependencies
* include - headers
* src - source of the application
  * common - general logic and reusable code
  * targets - target-specific application code
    * stm32 - stm32-specific stuff
  * tests - unit tests for components
* targets - toolchain files and board support for targets