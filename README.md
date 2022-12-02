# rosalia

General purpose convencience APIs for C.

|library|description|
|---|---|
|[`alloc.h`](./includes/rosalia/alloc.h)|**Composable allocators**, general and specific purpose, or create your own. Ready for use with allocator agnostic (rosalia) datastructures.|
|[`argparse.h`](./includes/rosalia/argparse.h)|**CLI argument parsing** (//TODO: with different flavors).|
|[`base64.h`](./includes/rosalia/base64.h)|**Base64** encode and decode capabilities.|
|[`config.h`](./includes/rosalia/config.h)|**JSON** representation and (de-)serialization.|
|[`heapstackify.h`](./includes/rosalia/heapstackify.h)|**Heap based arbitrary depth recursion** across different functions using natural feeling macros to easily simulate "co-routines" on a heap based stack. Bring the allocator of your choice.|
|[`jobs.h`](./includes/rosalia/jobs.h)|**Job queue** (or task queue or thread pool) with the ability to request and honor cancellation.|
|[`math.h`](./includes/rosalia/math.h)|**Math utils**, mostly integer for now.|
|[`noise.h`](./includes/rosalia/noise.h)|**Stateless pseudorandom INT->INT mapping** using prime numbers and a little bit of magic.|
|[`rand.h`](./includes/rosalia/rand.h)|**PCG32 PRNG** with acceptable speed.|
|[`raw_stream.h`](./includes/rosalia/raw_stream.h)|**Low overhead serialization for primitive datatypes.** Provides no safeties or guarantees, just maps primitives (and strings and blobs) to byte streams and back.|
|[`semver.h`](./includes/rosalia/semver.h)|**Very basic semver wrapper.** Just Major-Minor-Patch and comparisons, nothing else.|
|[`serialization.h`](./includes/rosalia/serialization.h)|**Composable layout based serialization** of arbitrary datatypes using user supplied runtime information. Crash resistant deserialization and fully extensible by custom types.|
|[`timestamp.h`](./includes/rosalia/timestamp.h)|**Monotonic timestamps (with fractional seconds).**|

Or just include [`rosalia.h`](./includes/rosalia/rosalia.h) to get everything.

## Notes and Todos
* possibly rename config to json
* add tests for the things that can be tested:
  * argparse (once it has flavors, show them working)
  * base64 (basic static example for expected input/output)
  * config (basic static example for deserialization)
  * heapstackify (something that properly shows the use)
  * jobs (mostly about resizing, cancellation and queue wakeup)
  * noise (specifically test some string hash collisions and generate a few statistics about bit correlations)
  * (rand)
  * raw_stream (serialize random types and check that deserialize produces expected output)
  * semver
  * timestamp (show monotonicity and fractional use)
* versioning for apis?
* possibly work through and adapt some things from the [stb_howto](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt)
* feature todos:
  * noise needs brownian motion (i.e. layered noise)
  * make macros available for ptradd ptrdiff ptrsub
  * skiplist
  * linked lists
  * markdown parser
  * dynamic str based plugin loading?
  * general purpose events and event queue?
  * networking (with a proper cancellable host resolution, ipv6 and tls out of the box)
  * sound around [miniaudio](https://miniaud.io/)?
  * subprocess control (i.e. fork and launch executable but keep stdin and stdout to control it)
  * terminatable job? i.e. fork process with pid handle to make it stoppable and execute a single function, need some way to share data across
  * coroutines
  * hashmap
  * io (sprintf/sscanf with sane functionality)
  * log (logging)
  * memcheck (quick and dirty for leaks,write-after-free,buffer-overrun)
  * set
    * red-black vs avl? and ofc usual cpp sets
  * stack
  * string
  * vector
