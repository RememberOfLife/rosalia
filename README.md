# rosalia

General purpose convencience APIs for C.

|library|version|description|
|---|---|---|
|[`alloc.h`](./includes/rosalia/alloc.h)||**Composable allocators**, general and specific purpose, or create your own. Ready for use with allocator agnostic (rosalia) datastructures.|
|[`argparse.h`](./includes/rosalia/argparse.h)|`0.1.4`|**CLI argument parsing** (//TODO: with different flavors).|
|[`base64.h`](./includes/rosalia/base64.h)|`0.1.6`|**Base64** encode and decode capabilities.|
|[`heapstackify.h`](./includes/rosalia/heapstackify.h)||**Heap based arbitrary depth recursion** across different functions using natural feeling macros to easily simulate co-routines on a heap based stack. Bring the allocator of your choice.|
|[`jobs.h`](./includes/rosalia/jobs.h)||**Job queue** (or task queue or thread pool) with the ability to request and honor cancellation. *Beware this has a **C++** implementation currently.*|
|[`json.h`](./includes/rosalia/json.h)|`0.1.1`|**JSON** representation and (de-)serialization. *There are many other, more stable, json libraries out there.*|
|[`math.h`](./includes/rosalia/math.h)|`0.3.2`|**Math utils.**|
|[`noise.h`](./includes/rosalia/noise.h)|`0.1.5`|**Stateless pseudorandom INT->INT mapping** using prime numbers and a little bit of magic.|
|[`rand.h`](./includes/rosalia/rand.h)|`0.1.4`|**PCG32 PRNG** with acceptable speed.|
|[`semver.h`](./includes/rosalia/semver.h)|`0.1.5`|**Very basic semver wrapper.** Just Major-Minor-Patch and comparisons, nothing else.|
|[`serialization.h`](./includes/rosalia/serialization.h)|`0.5.0`|**Composable layout based serialization** of arbitrary datatypes using user supplied runtime information. Crash resistant deserialization and fully extensible by custom types. As well as a *low overhead serialization for primitive datatypes* without safeties or guarantees.|
|[`timestamp.h`](./includes/rosalia/timestamp.h)|`0.1.4`|**Monotonic timestamps (with fractional seconds).**|
|[`vector.h`](./includes/rosalia/vector.h)|`0.4.6`|**Typed dynamic vectors.** Slight adaptation of the stb classic.|

Or just include [`rosalia.h`](./includes/rosalia/rosalia.h) to get everything.

## Notes and Todos
* problem with undefined functions when importing: json and then noise, because noise has static impl, but not non static then
* decide if it is ok for libs here to depend on another, e.g. json uses vector and noise
* Bump versions to 1.0.0 once all libs are header only
* decorate structs/enums/functypes in single headers
  * can name decorators be split from how the impl has to be named?
  * drop decoration? since it doesnt work with macros anyway?
* decorate macro calls are split on 2 lines if not assigned! fix clang format file is possible
* add tests for the things that can be tested:
  * jobs (mostly about resizing, cancellation and queue wakeup)
* possibly work through and adapt some things from the [stb_howto](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt)
* feature todos:
  * include original opensimplex noise
  * make macros available for ptradd ptrdiff ptrsub
  * skiplist
  * linked lists (also pointer stable segmented array)
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
  * string
  * heapstackify yield and continue execution, also dont crash on oom
  * in-place version for hm set etc.. (so a struct can be both in a linked list and a search tree and more without being just a pointer in each)
  * mini readline alike thing just for history and tab completion
  * locks and atomics also with windows specifics
