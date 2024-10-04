# Rock Bomber: An elegant C++ Godot4 example

<p align="center"><img src="RockBomber.gif?raw=true" alt="Rock Bomber: An elegant C++ Godot4 example" width="550" height="auto"></p>

_**Godot**_ is a very popular and comprehensive open source game engine. It is cross-platform, allowing developers to create 2D and 3D games for multiple platforms, including Windows, macOS, Linux, Android, iOS and web browsers. It features its own Python-inspired language called GDScript, and also officially supports C# and  C++ (the latter being less promoted).

Creating games in Godot involves working with nodes and creating custom node classes by inheriting from built-in ones. Using C++ for this can be less intuitive compared to other languages, and it is not as well documented online.

This C++ integration is called _**GDExtension**_ since the Godot engine itself is implemented in C++. Custom nodes created using C++ will appear alongside official nodes in the Godot editor.

After working with C++ and Godot for some time, I have established a set of personal principles that help me approach development in an elegant, clean and modern C++ style.

These recommendations will likely make the process more comfortable for C++ developers.

**Please note that this page is only applicable to Godot 4, as the use of C++ with Godot 3 (GDNative) is quite different.**

It also assumes that you have already followed the [official GDExtension C++ example](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html), which includes installing the SCons and compiling a small project.

>**Note:**  
>_"Why use C++ when there is GDScript?"_ is a question some people might ask.
Although Godot wasn't primarily designed with C++ users in mind, there are still reasons why some developers might prefer to use C++ with Godot, albeit these reasons are likely to be personal in nature.
>
>In my case, when I approached my career change and started working as a C++ developer, using Godot with C++ gave me the opportunity to practice and greatly improve my skills in the language I was using within my job.
If you are a developer more than someone who just wants to make games, and instead want to build programming skills that could benefit your career, C# or C++ is probably a better way to go than GDScript.
>
>However, there are two main technical reasons for using C++ with Godot:
>- Writing high performance algorithms
>- Importing C++ libraries to benefit from functionalities that are not natively available in Godot.


## Using the Godot engine in C++ is quite similar to using the Qt framework's "Qt Widget" for C++
Similarly to _**Qt's QObject**_ (and derived) instances are non-copyable, _**Godot's Object**_ (and derived) instances cannot be copied. 

When using Qt Widget (not to be confused with QML), developers work with .ui files that describe the user interface at startup. They can get the pointer to any UI element when they need to manipulate it, and some commonly used element references can be stored in the class attributes. Also, new elements can be instantiated during the execution of the application and manipulated and/or added to the UI programmatically.

In Godot, the equivalent of .ui files are scene files (_**.tres**_), which describe the initial state of the scene tree and nodes. In C++, developers work with nodes by getting pointers to them, and frequently used node pointers can be stored in custom class attributes for ease of use (A few words about smart pointers can be found in the Memory management section).

Finally, both the Qt framework and the Godot engine encourage the use of the _**Observer pattern**_ (a software behavioral pattern) by allowing the developers to implement signals to be associated with specific functions ("slots" in the Qt framework, or in the case of Godot, any method explicitly exposed to it).

The Observer Pattern allows multiple objects to communicate with each other without the need for much coupling between them. It is particularly suited to the implementation of user interfaces, but is also widely used in video game development, where multiple actors can interact.

## There is always a Godot node class to inherent from
Godot offers a comprehensive set of node types, including pre-made nodes for characters, visual components, physics, animation, audio playback and mixing, pathfinding, as well as classes for threading, resource management, external multimedia file handling, saving current progress, and encryption.

Nearly every need of a game engine is covered by Godot, allowing developers to extend existing classes as required.

Although the official Godot documentation is aimed at GDScript users, it remains highly relevant for C++ development, as the functions generally share the same names. It is recommended to configure an IDE with autocompletion and code analysis to detect and report any inconsistencies with _**godot-cpp**_ without needing to compile.

## Memory management is handled by Godot and has specific rules and keywords
The only way to instantiate a new node in C++ is by dynamic allocation. However, Godot has its own _**memnew(**_NodeType_**)**_ macro for this purpose. The regular  _**new**_ keyword of C++ language won't work with Godot classes.

Any node added to the main tree will be properly freed by Godot after the game closes and using the regular _**delete**_ keyword of C++ language or freeing it while it is still under the main tree will cause a memory error. Any node tthat is instantiated but not part of the main tree will not be properly freed when the game closes. So the best practice is not to instantiate or keep any node unless it's going to be used with the main tree. 

I have made some handy functions that I recommend using, which are placed in the _**[awesome_node_toolbox.h](Cpp_Sources/Tools/awesome_node_toolbox.h)**_ header file. These functions take care of both instantiating and adding as a child to a parent node (to be used with the main tree), as well as detaching a node from its parent and freeing its allocated memory.

As seen above, using the Godot engine  with C++ has a lot to do with dynamic allocation and manipulation of pointers. In particular, Godot nodes can only be cast dynamically and Godot provides its own template function for this:
```cpp
ClassA* ptrObjectA = Object::cast_to<ClassA>(ptrObjectB)
```
Since Godot nodes are not copyable, there is no way to cast them statically.

>**Note:**  
>Storing raw C++ pointers in _**std::vector**_ objects (or similar containers) is generally ill-advised, as there is no built-in way to check whether a pointer is still valid, which can lead to dangling pointers. You may also notice that I have been discussing pointers without mentioning smart pointers.
>
>These concerns relate to ownership semantics in C++: Who is responsible for managing the object's lifetime? Should the pointer be responsible for cleaning up the data it points to?
>
>To answer these questions, it is important to understand that the Godot engine has its own highly efficient memory management system, which uses (as mentioned above) dedicated memory allocation functions for node classes and reference counting for resource objects. In other words, it is fully responsible for the lifecycle of its objects, and every attempt I made to instantiate or release any Godot object using standard C++ methods (such as the _**new**_ and _**delete**_ keywords) resulted in a build or memory error. This is due to Godot attempting to manage the objects I was manipulating. Therefore, I would not recommend using smart pointers, which introduce a notion of ownership and memory management, such as _**std::unique_ptr**_ or _**std::shared_ptr**_.
>
>Using _**std::weak_ptr**_ would be more appropriate, as they do not take ownership of the referenced data and provide the ability to check whether it has been deallocated.
>
>However, here is an area of thinking:
While traditional software might aim to recover from missing objects or errors, in game development, it can be appropriate to let the game crash when unexpected errors occur to avoid inconsistent or degraded gameplay states.
>
>For all of the reasons mentioned above, I consider the use of raw C++ pointers well-suited to the current needs.


## The usual C++ constructors and destructors are not convenient to use with Godot node classes
As seen above, each instantiated node should be intended to be part of the main tree. However, during the instantiation of a node, any logic in the constructor that depends on the scene tree or other nodes may result in an error because it has not yet been added to the tree.

The _**_ready()**_ method is the preferred place to initialize or interact with other nodes, as it's called once the node is part of the scene tree and fully initialized.

Due to the dynamic assignment done by Godot's _**memnew(**_ NodeType _**)**_ macro, which does not support constructor arguments, any non-default required attribute value must be set manually using setters immediately after instantiation.

Regarding deletion, Godot objects are deleted by the Godot engine itself (as mentioned above), so explicitly defining destructors may also be useless.

However, as an exception to the above, constructors and destructors are useful if the class makes use of non-Godot-related data that needs to be initialized and cleared.

## It is appropriate to use external libraries (e.g. Standard Template Library (STL))
The Godot engine defines its own types, in a similar way to the Qt framework, and does not rely on external libraries.

Developers should use _**Godot::String**_ instead of _**std::string**_ because it is a comprehensive class which is used by several Godot functions when it comes to text.

However, the _**Godot::Array**_ container is not convenient for a C++ developer to use. In fact, it was designed for GDScript, which is a dynamically typed language. When using it with C++, Godot's Arrays involve Godot's _**Variant**_ and require casting to the actual type of the contained element before use.  

Another point to note is that the Godot::Array type offers only one way of accessing its elements, which is non-constant.

All this makes iterating over the elements of a Godot::Array container particularly verbose and not in a modern style of C++.
```cpp
Array arrayChildNodes = ptrParentNode->get_children();
for (int i = 0; i < arrayChildNodes.size(); i++) {
    Polygon2D* ptrPolygon = Object::cast_to<Polygon2D>(arrayChildNodes[i]);
    if (ptrCurrentChildNode) // Needing to check if casting was successful
        UtilityFunctions::print(ptrPolygon->get_name());
}
```

For these reasons, I recommend using the _**std::vector**_ container instead, which is optimised, has const and non-const accessors, and most importantly allows the use of _**range-based loops**_, a very concise way of iterating introduced by C++11.
```cpp
// Using one of the template functions from "awesome_node_toolbox.h"
std::vector<Polygon2D*> vectorPtrPolygons = ant::get_children<Polygon2D>(ptrParentNode);
for (Polygon2D* ptrPolygon : vectorPtrPolygons) {
    UtilityFunctions::print(ptrPolygon->get_name());
}
```
However, as convenient as _**std::vector**_ is to use, sometimes there is no other way but to use Godot's containers, for example when passing data to existing Godot functions or when needing to interact with GDScript.

>**Note:**  
>GDExtensions are C++ shared librairies after all, so they make it possible to use any C++ library or framework. This is something to keep in mind when needing to use functionalities that are not provided by Godot.


## Classes, methods, attributes and signals defined in C++ won't be known by Godot's engine unless they are explicitly exposed to it.
The C++ classes developed for a GDExtension are compiled into a shared library that is loaded and used by the Godot engine.

Even when declared with the C++ **public** keyword, the classes, their internal methods and attributes are not (and are not required to be) known to the Godot engine. This means that even classes that are inherited from a Godot class may have complex handling that can manipulate data types from other libraries.

However, the developer can (and must) explicitly expose classes, methods, attributes (then considered as node properties) if it is necessary to interact with other Godot classes. Godot signals must also be exposed.

Methods connected to signals (which would be called "slots" in Qt) necessarily need to be explicitly exposed to Godot, as do methods and attributes/properties that need to be manipulated by an external Godot object such as an AnimationPlayer instance.

**Syntax for creating a new class and exposing it to Godot:**

```cpp

class myClass : public CharacterBody2D {
    GDCLASS(myClass, CharacterBody2D)
public:
    // ===== Required by Godot ======
    static void _bind_methods();
};
```

All custom classes deriving from Godot classes must also be declared in a function that will be called during the initialization of the Godot extension. For simplicity, I recommend consulting this example:

```cpp
ClassDB::register_class<Hero>();
```

For each class, its specific exposures must be done in a _**myClass::_bind_methods()**_ function.


**Syntax to expose a method to Godot:**
```cpp
ClassDB::bind_method(D_METHOD("myExposedFunctionA", "argument1", "argument2"), &myClass::myFunctionA);
ClassDB::bind_method(D_METHOD("myExposedFunctionB"), &myClass::myFunctionB); // This one doesn't have any argument
```

**Syntax to expose a property to Godot:**
```cpp
ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "myProperty"), "set_my_property", "get_my_property");
ClassDB::bind_method(D_METHOD("get_myProperty"), &myClass::get_myProperty);
ClassDB::bind_method(D_METHOD("set_myProperty", "newValue"), &myClass::set_myProperty);
```
When declaring a property, setter and getter functions must also be declared and exposed.
Note that the Godot Editor will call these methods when the user manipulates the property value of an instance through the HMI.

**Syntax to expose a signal to Godot:**
```cpp
ADD_SIGNAL(MethodInfo("mySignalA", PropertyInfo(Variant::VECTOR2, "globalPosition")));
ADD_SIGNAL(MethodInfo("mySignalB"); // This one doesn't have any argument
```

**Syntax for connecting an exposed signal to an exposed method:**
```cpp
m_ptrTimer->connect("timeout", Callable(this, "explode"), Node::ConnectFlags::CONNECT_PERSIST);
```
Connecting signals can be done from anywhere in the code.
When making a connection programmatically (through code and not through the Godot Editor), leaving the flags argument at its default value ("0") may not produce the expected result.

Refer to the [Godot::Object::ConnectFlags](https://docs.godotengine.org/en/stable/classes/class_object.html#enum-object-connectflags) enum, which flags that can be summed to achieve the desired behaviour, the enum values being the following ones 
- _CONNECT_DEFERRED_
- _CONNECT_PERSIST_
- _CONNECT_ONE_SHOT_
- _CONNECT_REFERENCE_COUNTED_

## Bonus: Restoring C++ style console prints
Printing to the console when using Godot is done by using the _**UtilityFunctions::print()**_, as also _**godot::String**_ number-to-string conversion methods.
```cpp
UtilityFunctions::print("Hello world");
UtilityFunctions::print("Here is a float number: " + String::num(3.14));
UtilityFunctions::print("Here is an integer number: " + String::num_int64(-10));
UtilityFunctions::print("Here is an unsigned integer number: " + String::num_uint64(15));
```
Unfortunaly, using _**std::iostream**_ does not work properly.
Instead, I recommend using the _**ConsoleLogger**_ class I implemented in the _**[console_logger.h](Cpp_Sources/Tools/console_logger.h)**_ header file.
  ```cpp
ConsoleLogger::logger() << "Hello world" << ConsoleLogger::endLine;
ConsoleLogger::logger() << "Here is a float number: " << 3.14 << ConsoleLogger::endLine;
ConsoleLogger::logger() << "Here is an integer number: " << -10 << ConsoleLogger::endLine;
ConsoleLogger::logger() << "Here is an unsigned integer number: " << 15 << ConsoleLogger::endLine;
```
## Conclusion
By adopting these practices, C++ developers should be able to create games using the Godot engine while maintaining clean and elegant code.
If you're interested in seeing these principles in action, be sure to take a look at my example game.
