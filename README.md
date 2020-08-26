# Out-Of-Line
A memory pattern for high performance C++, preserving RAII 
and keeping only often used class members in cache, with zero memory overhead.

You may want to use it when in your code there is an array with many
objects, but you don't need all fields in the object for the whole time.

# Description
The idea if very simple, if you can split the class into two classes: hot and cold.
- Hot: often used members.
- Cold: not often used members.

You can use OOL pattern. OOL object looks like a hot class object, but is connected
to a cold value object. Therefore access to different members has different time.
- Hot: instant, as to a variable in a class.
- Cold: depends on implementation. O-time may be logarithmic. Generaly big.

To get cold data, call a cold member function.

In that implementation I decided that:
- Every pair *<HotType, ColdType>* has different map, even that it wold be enough to
    have different map for every different ColdType (or even on map for all types, with some pointer magic).
- I use std::map, so access time to cold data is logarithmical.
- OOL class inherits from HotType.
- There is support for a structured binding declaration [hot, cold].

# Examples

If you have two classes, you can simply use OOL
without creating a new class, just create a new instance of template.
```cpp
using example_t = out_of_line<std::string, std::string>;
// ...
example_t a("a_hot", "a_cold");
```

For examples how to create a new class using OOL patter, look to a file with example 2.

You can also use structured binding declaration.
```cpp
    using example_t = out_of_line<std::string, std::string>;
    // ...
    std::vector<example_t> objs;
    // ...
    for(auto &[hot, cold] : objs) {
        // ...
    }
```


*The idea is based on a [blog post](https://blog.headlandstech.com/2018/08/15/outofline-a-memory-locality-pattern-for-high-performance-c/), describing OOL pattern.*
