# Sketchable-Interaction

**Reference implementation of a novel and generic computational environment for defining user interfaces and workflows**

##  Overview
* Based on earlier reference implementation: https://github.com/PDA-UR/MTT
* Previous version turned into a messy hack of features which somewhat function as desired

## Planned changes:

### Architecture 2.0

* design the software archtitecture to actually support the concepts implementation
* include exceptions for physical world and potential other occurrences
* define the concept in a way that these exceptions are no exceptions

### Technology 2.0

* Backend
  * Integrate Basic Tracking into Python Application while keeping TUIO2 support
    * Both applications form the minimum core to actually run an instance of SI, hence start it with one click
    * Every other possible sending daemon of input is bonus
    * Interesting to see whether latency improvements are observable when main SI component is not UDP dependent
  * Remove currently unnecessary tracking steps or artifacts (e.g. hands)
  * Reiterate the dynamic tangible approach
* Frontend
  * Implement the actual concept
  * Interactive Regions are:
    * sketchable
    * *directional* (uni-, or bi-) --> emit and receive effects
    * bound to *roles*
    * *linkable* to other regions (sketched regions, finger, tangible, etc.)
    * *context dependent on when they actually do things*
