## Introducation
This is Java Virtual Machine implemention completely developed in C language. It could be compiled with GCC compiler in both Linux and Mac OS X platform. Generally this virtual machine runtime implements the same technical level as the original SUN hotspot JVM implementation. It parses class file and interprets the byte code without any optimization technology like JIT.

## Background
Java language might be the most famous and successful modern computer language. As the fourth generation language, it is not like the former traditional languages (such as C, Pascal and C++ etc) which are commonly compiled into machine code running on the specific CPU instructions architecture. One of the most important design goals of Java language is its independency of specific platform, this feature makes Java source code be compiled into some kind of intermediate code running on some common runtime environment, rather than the machine code specific to some platform. As for Java language, the intermediate code is byte code and the runtime environment is the Java Virtual Machine, shortly called as JVM. About two years ago, in order to understand the internal workings of the JVM runtime deeply, I wrote some kind of JVM implementation totally in C language and I published all the source code in this github repository. Actually I have not completed all the JVM development work 100 percent, I only implemented the runtime facilities that allow the class file or jar file to run in the runtime. Generally speaking, this JVM implementation is the same with the initial Hot Spot JVM implementation published by former SUN Microsystem in technical level. Even so, it is still helpful for us to understand the internal workings and we can benefit much from this kind of the fourth generation language runtime. This document introduces the JVM implementation in detail, and I will introduce some of the key concepts later.

## High-Level Structure of JVM Runtime
The JVM structure could consist of two parts in logic. The first part is the parsing engine which is mainly responsible for loading and parsing class or jar files, and building the class definition map after defining the class instance in memory. The second part is the executing engine which is mainly responsible for fetching and executing the instructions in the form of byte code. Actually for the practical implementation of JVM runtime, the garbage collection mechanism is necessary and much important, it is even the key design component, because the performance and stability depends on the garbage collection mechanism largely.

![](https://github.com/shaojianqing/vmachine/blob/master/res/jvm_architecture.png)
<center>1.General Architecture of JVM Runtime</center>

## Binary Format of Byte Code Class File 
As the JVM ecology develops continuously, more and more other JVM-based languages are invented like Groovy, Scala and Kotlin etc, and the JVM runtime has become some kind of common runtime platform for all of that kinds of languages. Here in order to make all the JVM-based languages run on the JVM runtime, it is necessary for all of them to be compiled into the same byte code class file that follows the same binary format and technique specification. The byte code class file could be seen as the public binary interface between JVM-based languages and JVM runtime. The below part will introduce the internal data structure of the byte code class file in detail.

![](https://github.com/shaojianqing/vmachine/blob/master/res/class_definition.png)
<center>2.Class Definition Format in JVM</center>

|Seq|Item|Description|
|:-- |:--------------- |:-------------------------------------------|
|1|Magic Number|Magic number is the identifier of the binary byte code class file and it is fixed as CAFE which is the short form of CAFFEE. Since Java is named as some kind of coffee bean in Java island. So it is natural that the identifier is set to CAFE|
|2|Minor Version|Minor version of the JVM class file. JVM will check or validate the both the minor version and major version to confirm whether it can run the class file legally. Only if the JVM version is equal or higher than the both the minor version and major version, can it run the class file|
|3|Major Version|Major version of the JVM class file. Similar to the minor version, it is designed to sign the version of the class file for check and validation by the JVM runtime. So the old version JVM can not run the new version class file, but the new JVM can run the old version class file|
|4|Constant Pool Count|The count of the constant items in the constant pool. When resolving constant items, this count is used to loop the constants item|
|5|Constant Pool Item|The constant pool is much important for the class function. It mainly consists of two kinds of constants, the first is the literal value which could be like string value or numeric value in the program. The second is the symbol reference or index. In JVM byte code specification, the class, field, method and attribute are all defined as the symbol. For the reference between different symbols, like the situation that class references all its fields or methods, or method references all its attributes. As shown in the above diagram, the constant type is set by the one-byte constant tag field, and the constant value is in the body of the constant pool item|
|6|AccessFlag|Access flag defines the access permission of the class, it could be private, protected, default or public. As suggested in the object oriented principle, this access flag is design to control the access permission between different classes or objects|
|7|This Class Index|Since the full class name has been stored in the constant pool as some constant pool item, here the class index is a short number pointing to the corresponding constant pool item. So when resolving the class name, it is to query the constant item and get the class name|
|8|Super Class Index|Similar to the class index, the super class index is a short number pointing to the corresponding constant pool item too, and it is to query the constant item in the constant pool in the same way. When executing, the execution engine can get the super class definition by querying  the class definition map with the super class name and invoke the methods in the super class|
|9|Interface Count|The count of the interfaces that the class has implemented, so similar to the constant pool count, it is used to loop the interfaces indexes|
|10|Interface Index|The symbol reference index pointing to the constant pool item which contains the full name of the interface. When resolving interfaces, the constants pool items will be resolved and assigned to the class definition as an interfaces array|
|11|Field Count|The count of the fields that the class takes, so similar to the constant pool count, it is used to loop the field items|
|12|Field Info Item|The field definition set inside the class definition, the field item consists of serval parts in the green color as shown in the above diagram like access flag, name index, descriptor index and attribute set. So the resolving process is similar to the class definition here|
|13|Method Count|The count of the methods that the class takes, so similar to the constant pool count, it is used to loop the method items|
|14|Method Info Item|Method definition set inside the class definition. Similar to the field item, the method item consists of serval parts in the blue color as shown in the above diagram. Something most important about the method item is that all the byte code instruction is stored as the attribute in the method attribute set. This part will be introduced in the following part of execution engine in detail|
|15|Attribute Count|The count of the attributes that the class takes, so similar to the constant pool count, it is used to loop the attribute items|
|16|Attribute Info Item|Attribute item is some kind of common data structure in the class definition, so it is simpler than the field item and the method item there and could be used or referenced by them meanwhile. The attribute Item only consists of three parts: name index, attribute length and attribute data. So what the attribute data really represents depends on the item that references it rather than itself|

![](https://github.com/shaojianqing/vmachine/blob/master/res/class_definition_code.png)
<center>3.Data Structure for Class Definition in C language</center>

In the above code block shows the class definition which is actually defined as the traditional structure in C language. The class definition contains all the elements or parts mentioned in the above table including constants pool, field item list, method item list and so on. Since the class file is actually intended to be loaded into memory and in the form of finite byte array, the above class definition is generated and parsed from the byte array in memory loaded from the class file. The detailed steps and mechanism is described in the following section.

## Class Definition Parsing and Organization Mechanism of JVM Runtime
![](https://github.com/shaojianqing/vmachine/blob/master/res/class_definition_parsing.png)
<center>4.Class Definition Parsing Mechanism and Process</center>

Exactly as shown in the above diagram, initially both the class file or jar file is stored in the file system based on some kind of storage like hard disk or event network disk. Then they are intended to be loaded into the memory in the form of byte array. For jar file, it needs to be uncompressed and gets multiply class byte arrays, while for class file, it is loaded in the form of byte array directly. After getting the byte array of the class file in the memory, the parsing function starts to parse the byte array by sliding on the byte array and reading the data from the byte array. In practice, the slider could read short, integer or sub byte array like string type. As illustrated in the below code block, the defineClass function takes the byte array pointer as its input, and returns the Class definition pointer as its output.

In the parsing process, the input byte array pointer itself plays the role of slider. Since it is the pointer in C language, and as we can understand, the pointer itself could perform memory address operation of moving up or moving down on the memory. In this way, the data pointer slides on the memory address, reads values and assigns the values to the class definition sequentially. So we can naturally understand the whole process of parsing class file and constructing class definition precisely.

![](https://github.com/shaojianqing/vmachine/blob/master/res/class_definition_function.png)
<center>5.defineClass function in C language</center>

![](https://github.com/shaojianqing/vmachine/blob/master/res/class_parsing_function.png)
<center>6.resolveClassData function in C language</center>

![](https://github.com/shaojianqing/vmachine/blob/master/res/class_relationship.png)
<center>7.Class Definition Pointing Relationship</center>

Once constructing the class definition in the memory, the class definition will be put into the Class Definition Map with the class name as the key and the class definition itself as the value. In the above scene, Vehicle is the parent class of Car and Truck class, and both Car and Truck class consist of Engine Class. This scene is very simple and typical object oriented case. Actually as for the class definition and implementation, the superClassName property has the value of the full name of Vehicle, here it is the string of  "com/sys/data/Vehicle", this means the superClassName property points to the parent class Vehicle. Similarly, since both Car and Truck have the field of Engine, there must be the field definition in the field list. And the descriptor property in the field definition has the value of the full name of Engine, here it is the string of "com/sys/data/Engine".

Based on the above analysis, we can understand how JVM runtime organizes a lot of class definition in the memory and how multiply class definition connects each other to maintain the relationship. That mechanism is actually the meta space in modern JVM implementation like the Oracle JVM implementation.

## Instruction Execution Mechanism of JVM Runtime
Generally speaking, there exist two kinds of instruction execution mechanism: the register-based instruction and the stack-based instruction.Just as shown in the below two diagrams, the register-based instruction directly accesses the registers inside the CPU, and does the arithmetic or logic operation with the registers naturally. Based on its execution mechanism and the fact that the registers is the fastest and much faster than memory when reading or writing, register-based instruction generally could have much higher performance with less reading or writing operation on the memory. Actually for most of the compiling languages such as C, C++, Pascal and so on, they are commonly compiled into machine code which is actually the register-based instruction set. But the disadvantages is obvious meanwhile, since the register-based instruction is specific to the particular CPU instruction set like intel x86 instruction set, it is almost impossible to have cross-platform running features in the binary level. Commonly, this kind of execution mechanism plays the underlying execution role in the computer technology stack.

![](https://github.com/shaojianqing/vmachine/blob/master/res/register_based_inst.png)
<center>8.Exection Mechanism of Register-Based Instruction</center>

While it is hard for the register-based instruction to be independent of some specific CPU platform, actually it is easier for the stack-based instruction to be independent enough and have the cross-platform feature in the binary level. Since stack is some kind of data area in the memory, and memory structure almost keeps the same even in different CPU platform, the common implementation could introduce some kind of intermediate instruction set which could be interpreted by some runtime. In this design, the runtime could be specific to some particular CPU platform or Operating System, but the program based on the intermediate instruction set could be independent of any specific CPU platform or Operating System. This is the basic design philosophy for JVM runtime from the former SUN Microsystem, as well as the Common Language Runtime from Microsoft. Actually both JVM runtime and Common Language Runtime have become independent of platform and can support multiply languages until now, and the basis is due to the intermediate instruction set and the related runtime infrastructure which makes them a common platform for the Internet Application to run many kinds of programs.

![](https://github.com/shaojianqing/vmachine/blob/master/res/stack_based_inst.png)
<center>9.Exection Mechanism of Stack-Based Instruction</center>

As shown in the above diagram, for stack-based instruction, the main execution mechanism is based on the operations with the memory, particularly the memory structure stack. And the instruction set could be simple enough for execution, while some complex and platform related operations could be implemented with the native interface which is usually written with some native languages like C or C++. In JVM runtime, all the instructions are defined by only one byte, that is why the JVM instruction is called as byte code instruction here.

## Object Oriented Style In C Language
As we all know that C language is the procedure oriented language and not support the object oriented style natively. But actually we could design and develop with C language in object oriented style in practice and it sounds interesting, isn't it ? Since object oriented style can reduce the complexity of the system analysis and simplify the domain modeling process, I finally designed and developed my JVM implementation with C language in object oriented style. The following part will introduce this perspective and thinking style in detail.

![](https://github.com/shaojianqing/vmachine/blob/master/res/object_oriented_style.png)
<center>10.Object Oriented Programming Style in C language</center>

As shown in the above diagram, the object could be some kind of model mapping to the entity in the real world. So similarly the object has two parts which are properties and functions, and could be seen as the combination of that two parts. Properties mean the data or statement of the object, while functions mean the behavior of the object. Finally with these two parts, the object could work as a whole in itself scope, and multiply objects could work together to do some complex things further more. 

Based on the above analysis, we can conclude that if only some language has the feature that could organize the properties and functions, and combine them into some kind of unified data type or structure, it could be convenient to develop in the object oriented style with that language. As for C language, there exist two features that are the key factors for object oriented style. They are struct data type and function pointer. In the below part, they will be introduced in detail.

## Execution Engine Mechanism In JVM Runtime
As we all know that, the JVM runtime is designed to execute the byte code instructions, so the execution engine plays the most important role in the JVM architecture. Just as mentioned about the parsing engine, the class file is loaded from the storage and the class definition is parsed and maintained in the JVM memory. And something is much critical is that the byte code instructions are parsed as the attributes in the method. The parsing process is shown in the following code block:

![](https://github.com/shaojianqing/vmachine/blob/master/res/bytecode_parsing_code.png)
<center>11.Byte Code Instruction Parsing Mechanism</center>

In JVM specification, method contains multiply attributes, and there is one kind of attribute whose attribute name is "Code", actually this attribute is full of byte code instructions. So naturally the content of this attribute is parsed and copied to the codeData property and codeLength property to record the byte length of the codeData in method data structure. When executing the byte code instructions, the codeData will be passed to the ByteReader which provides a variety of function convenient for processing byte array efficiently.

![](https://github.com/shaojianqing/vmachine/blob/master/res/instruction_execution.png)
<center>12.Byte Code Instruction Execution Mechanism</center>

① The thread running loop fetches the operation code of the byte code instruction from the byte reader which maintains the instructions sequence coming from the method definition.

② Then gets the instruction from the Instruction Table. The instruction data structure contains five parts to support execution and will be described in detail in the below table.

③ The thread running loop calls the Fetcher function pointer to fetch the corresponding operands necessary for the instruction execution with some specific byte code instruction.

④ Finally the thread running loop calls the Processor function pointer to execute the instruction which may operate the operand stack or local variable table if necessary.

|Seq|Item|Description|
|:-- |:--------------- |:----------|:-------------------------------------------|
|1|Class Definition|Class definition is parsed in the parsing engine, mainly serval parts are parsed including field list, method list and attribute list|
|2|Method Definition|Method list is resolved and the most import part is the code data which is actually the byte code instructions inside the method here|
|3|Thread Running Loop|This is the while loop fetching operation code and operands from the byte reader and executing the instructions sequentially. So this loop is actually the thread running body in JVM runtime|
|4|ByteReader|ByteReader wraps the byte array and provides multiply access function to fetch and iterate byte data. In execution engine, ByteReader wraps the byte code instructions and the related operands, and in the Thread Running Loop, it provides the access functions for fetching byte code instructions and operands|
|5|Instruction Table|This is the static instruction table defining all the byte code instructions which mainly container the operation code, operation name, operand data, instruction processor function pointer and operand fetcher function pointer. The byte code instruction execution mechanism is mainly implemented by the instruction processor function pointer|
|6|Operate Code|This is a one-byte code defining the instruction. In JVM specification, all the instructions are defined by only one byte, and that is why the JVM instruction set is called as the byte code instruction|
|7|Operate Name|This is the operate name which is the literal name of the instruction like "iadd", "iload" or "istore". This field makes it more convenient for us to understand and organize the instructions in the instruction table|
|8|Operand Store|This is a union type that stores the operand temporarily. Usually this kind of operand comes from the fetcher function defined in the instruction data structure|
|9|Processor|This is the main part of the instruction, as this function pointer is what the instruction really does when executing. This Processor function pointer takes instruction itself and stack frame as the necessary facilities. The detailed information will be described in the below part|
|10|Fetcher|This is a utility function designed to fetch the operands from the byte reader and pass the operands to the operand store in the instruction temporarily. For most of the instructions, it is not necessary to fetch any operands, but for some specific instructions like xstore or xload, it is necessary to get the index as the operand|


From the above diagram, it is clear to understand the general process of instruction execution mechanism here. When executing, the thread running loop fetches the byte code instruction and operands sequentially and frequently, and then calls the corresponding  processor function according to the different byte code instructions. Since JVM instruction is based on the stack rather than the register, the processor will frequently access or operate the operand stack data structure in the most of the instructions, while for some specific instructions, it is necessary to store or load some parameters in the local variables data structure. The detailed implementation is shown in the below part.

As shown in the above code blocks, local variables is just some kind of array container that stores or loads data based on the array operation, so it is necessary to pass the index as the parameter. While operand stack is of course some kind of stack container that pushes or pops data always on the stack top, so it is totally not necessary to pass any kind of parameter to point out the position here. Actually, both local variables and operand stack are much important for instruction execution, they actually form the micro runtime environment which is called as stack frame in JVM runtime environment.

The load instructions actually load data values from local variables and push them into the operand stack. And correspondingly, the strore instructions pop the data values from the operand stack and store them into the local variables.

Almost all the arithmetic operation instructions in JVM is like the above code logic, they are all based on the operand stack to load and store data values.