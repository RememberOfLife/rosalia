# rosalia

General purpose convencience APIs for C.

Almost everything is prefixed with `rosa` or `rosalia` so there should be no collisions with common libraries.

|library|version|description|
|---|---|---|
|[`alloc.h`](./include/rosalia/alloc.h)|`0.1.0`|**Composable allocators**, general and specific purpose, or create your own. Ready for use with allocator agnostic (rosalia) datastructures.|
|[`argparse.h`](./include/rosalia/argparse.h)|`0.3.0`|**Key-Value Pair CLI argument parsing**.|
<!-- |[`arraylist.h`](./include/rosalia/arraylist.h)|`0.0.0`|**Typed dynamically sized arrays.** Unsorted.| -->
|[`base64.h`](./include/rosalia/base64.h)|`0.2.1`|**Base64** encode and decode.|
<!-- |[`hashmap.h`](./include/rosalia/hashmap.h)|`0.0.0`|**Typed hashmap and hashset.** Unsorted.| -->
<!-- |[`heapstackify.h`](./include/rosalia/heapstackify.h)||**Heap based arbitrary depth recursion** across different functions using natural feeling macros to easily simulate co-routines on a heap based stack. Bring the allocator of your choice.| -->
<!-- |[`jobs.h`](./include/rosalia/jobs.h)||**Job queue** (or task queue or thread pool) with the ability to request and honor cancellation. *Beware this has a **C++** implementation currently.*| -->
<!-- |[`json.h`](./include/rosalia/json.h)|`0.1.1`|**JSON** representation and (de-)serialization. *There are many other, more stable, json libraries out there.*| -->
<!-- |[`linklist.h`](./include/rosalia/linklist.h)|`0.0.0`|**Typed singly- and doubly-linked lists.** Sorted and unsorted.| -->
<!-- |[`log.h`](./include/rosalia/log.h)|`0.1.0`|**LOG** to your own outputs with customizable layouts.| -->
<!-- |[`math.h`](./include/rosalia/math.h)|`0.3.2`|**Math utils.**| -->
|[`noise.h`](./include/rosalia/noise.h)|`0.2.0`|**Stateless pseudorandom INT->INT mapping** using prime numbers and a little bit of magic.|
|[`rand.h`](./include/rosalia/rand.h)|`0.2.0`|**PCG32 PRNG** with acceptable speed.|
|[`semver.h`](./include/rosalia/semver.h)|`0.2.0`|**Very basic semver wrapper.** Just Major-Minor-Patch and comparisons, nothing else.|
<!-- |[`serialization.h`](./include/rosalia/serialization.h)|`0.8.1`|**Composable layout based serialization** of arbitrary datatypes using user supplied runtime information. Crash resistant deserialization and fully extensible by custom types. As well as a *low overhead serialization for primitive datatypes* without safeties or guarantees.| -->
|[`timestamp.h`](./include/rosalia/timestamp.h)|`0.2.0`|**Monotonic timestamps (with fractional seconds).**|
<!-- |[`treemap.h`](./include/rosalia/treemap.h)|`0.0.0`|**Typed treemap and treeset.** Sorted.| -->
|[`util.h`](./include/rosalia/util.h)|`0.2.1`|**Stuff.**|
<!-- |[`vector.h`](./include/rosalia/vector.h)|`0.4.7`|**Typed dynamic vectors.** Slight adaptation of the stb classic.| -->

Or just include [`rosalia.h`](./include/rosalia/rosalia.h) to get everything.

### Why no Single-Headers?

I tried doing it in single header style once, but I did not like it. I never used this as a single header library and I never would have. For just me, as the only user, that is unmaintainable. And if I don't use it because it feels icky, it's useless to me. So now it's back to header+impl.  
Maybe I will change it again, *one day*.
